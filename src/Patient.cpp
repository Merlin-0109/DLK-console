#include <iomanip>
#include <ctime>
#include <vector>
#include <set>
#include <limits>
#include <map>

#include "Patient.h"
#include "DataStore.h"
#include "AuthSystem.h"
#include "UI.h"
#include "Calendar.h"

/*--------------------------------------------------------------
                    CONSTRUCTOR - DESTRUCTOR
---------------------------------------------------------------*/
Patient::Patient() :User() {
    userType = PATIENT;
}

Patient::Patient(string id, string identicalCard, string password)
    :User(id, identicalCard, password, PATIENT) {}

Patient::Patient(string id, string identicalCard, string password,string fullName,string dateofbirth, string gender, string email, string phoneNumber, string address)
    :User(id,identicalCard,password,fullName,dateofbirth,gender,email,phoneNumber,address,PATIENT) {}

Patient::~Patient() {}

/*--------------------------------------------------------------
                    HIỂN THỊ THÔNG TIN CÁ NHÂN
---------------------------------------------------------------*/
void Patient::displayInfo() const {
    cout << "\t\t\t\t\t==================================" << endl;
    User::displayInfo();
    cout << "\t\t\t\t\t==================================" << endl;
}

bool Patient::isProfileComplete() const {
    return !fullName.empty() && 
           !dateOfBirth.empty() && 
           !gender.empty() && 
           !phoneNumber.empty() && 
           !address.empty();
}

/*--------------------------------------------------------------
                ĐA NĂNG HÓA TOÁN TỬ NHẬP - XUẤT
---------------------------------------------------------------*/
ostream& operator<<(ostream& o, const Patient& patient){
    o << static_cast<const User&>(patient);
    return o;
}

istream& operator>>(istream& in, Patient& patient){
    in >> static_cast<User&>(patient);
    if (in.peek() == '\n') in.ignore();
    return in;
}

/*--------------------------------------------------------------
                CÁC CHỨC NĂNG DÀNH CHO BỆNH NHÂN
---------------------------------------------------------------*/
bool Patient::bookAppointment(const string& doctorId, const string& date, const string& time, const string& reason) {
    if (!isProfileComplete()) {
        cout << "\n⚠ Please complete your personal information before scheduling an appointment!" << endl;
        return false;
    }
    string chosenDate = date;
    string chosenTime = time;
    string reasonInput = reason;

    DataStore dataStore;

    vector<string> busyDate = dataStore.getBusyDate(doctorId);

    vector<tm> dates;
    time_t now = std::time(nullptr);
    for (int i = 0; i < 7; i++){
        time_t t = now + (i+1)*24*3600;
        tm tm_ptr = *localtime(&t);
        dates.push_back(tm_ptr);
    }
    auto patientApp = DataStore::getPatientAppointments(this->id);
    vector<pair<string, set<string>>> bookSlots; // pair: date + set (time)
    for (auto &appID : patientApp){
        DataStore::AppointmentDetails details = DataStore::readAppointment(appID);
        if (details.bookStatus == "Booked" && details.visitStatus == "Not Done"){
            bookSlots.emplace_back(details.date,set<string>{details.time});
        }
    }
    auto getSlotCount = [&](const string& d, const string& slot){
        return DataStore::getDoctorAppointmentsForDateSlot(doctorId,d,slot).size();
    };
    bool isDateFull[7] = {false};
    for (int i = 0; i < 7; i++){
        int total = 0;
        for (int s = 0; s < 7; s++){
            // Đếm số appointment ở mỗi slot ngayf
            total += (int)DataStore::getDoctorAppointmentsForDateSlot(doctorId, dateWeek[i], timeSlot[s]).size();
        }
        if (total >= 35) isDateFull[i] = true;
    }
    cout << "\n\n\t\t\t\t\t==================== BOOKING CALENDAR ====================" << "\n\n";

    Calendar calendar;
    calendar.showCalendar(doctorId);

    SetColor(6*16 + 4);
    cout << "Note:";
    SetColor(7);
    cout << "\n- [OFF]: Unable to schedule (Sunday)" << endl;
    cout << "- [BUSY]: Unable to schedule because doctor is busy" << endl;
    cout << "- [FULL]: Slot fully booked" << endl;
    cout << "- [n/5]: n patients booked, still available" << endl;
    cout << "- [ ]: Empty slot" << endl;

    // Kiem tra ngay book lich hop le (khong off)
    vector<string> dayChoice;
    vector<int> indexSlotDay;
    for (int i = 0; i < 7; i++){
        char buf[16];
        strftime(buf,sizeof(buf),"%d/%m",&dates[i]); //*******
        if (dates[i].tm_wday == 0) continue;
        if (isDateFull[i]) continue;

        bool isBusyDay = false;
        for (const string& date : busyDate){
            if (buf == date){
                isBusyDay = true;
                break;
            }
        }
        if (isBusyDay) continue;
        string showDay = dateWeek[dates[i].tm_wday] + "(" + string(buf) + ")"; // khi show menu
        dayChoice.push_back(showDay);
        indexSlotDay.push_back(i);
    }

    cout << "\nPlease choose an appointment date!" << endl;
    int realChoiceDate = -1;
    while (true) {
        int choiceDate = runMenuHorizontal(dayChoice.data(), (int)dayChoice.size());
        if (choiceDate >= 1 && choiceDate <= (int)indexSlotDay.size()) {
            realChoiceDate = indexSlotDay[choiceDate - 1];
            break;
        }
    }
    char buf[6];
    strftime(buf, sizeof(buf), "%d/%m", &dates[realChoiceDate]);
    chosenDate = string(buf);
    SetColor(2);
    cout << "✔ You chose the appointment date: " << chosenDate << endl;
    SetColor(7);

    // kiem tra gio FULL va gio hop le (bac si khong off) 
    vector<string> timeChoice;
    vector<int> indexSlotTime;
    for (int i = 0; i < 7; i++){
        int count = (int)getSlotCount(chosenDate,timeSlot[i]);
        if (count >= 7) continue;

        bool checkTime = false;
        for (auto & booked : bookSlots){
            if (booked.first == chosenDate && booked.second.count(timeSlot[i])){
                checkTime = true;
                break;
            }
        }
        if (checkTime) continue;
        
        timeChoice.push_back(timeSlot[i]);
        indexSlotTime.push_back(i);
    }

    cout << "\nPlease choose an appointment time!" << endl;
    int realChoiceTime = -1;
    while (true) {
        int choiceTime = runMenuHorizontal(timeChoice.data(), (int)timeChoice.size());
        if (choiceTime >= 1 && choiceTime <= (int)timeChoice.size()) {
            realChoiceTime = choiceTime - 1;
            break; 
        }
    }
    chosenTime = timeChoice[realChoiceTime];
    SetColor(2);
    cout << "✔ You chose the appointment time: " << chosenTime << endl;
    SetColor(7);
    if (reasonInput.empty()) {
        cout << "Reason: ";
        string r; 
        getline(cin, r);
        if (r.empty()) {
            cout << "Error: Please enter the reason for the consultation!" << endl;
            return false;
        }
        reasonInput = r;
    }

    // --- tạo appointment ---
    string appointmentId = DataStore::generateAppointmentID();
    vector<string> doctorInfo = getDoctorInfo(doctorId);
    string clinicName = (doctorInfo.size()>=3)? doctorInfo[2]:"Not updated";
    DataStore::AppointmentDetails details;
    details.appointmentId = appointmentId;
    details.patientId = this->id;
    details.doctorId = doctorId;
    details.date = chosenDate;
    details.time = chosenTime;
    details.reason = reasonInput;
    details.clinic = clinicName;
    details.bookStatus = "Booked";
    details.visitStatus = "Not Done";
    if(DataStore::writeAppointment(appointmentId, details)) {
        system("cls");
        calendar.printTicket(appointmentId,fullName,doctorInfo[0],doctorInfo[1],clinicName,chosenDate,chosenTime,reasonInput);
        system("pause");
        return true;
    }
    cout << "Error: Unable to schedule an appointment!" << endl;
    return false;
}
 
bool Patient::viewUpcomingAppointments() const {
    vector<string> appointments = DataStore::getPatientAppointments(this->id);
    
    if (appointments.empty()) {
        cout << "\n\t\t\t\t\tYou do not have any medical appointments." << endl;
        system("pause");
        return false;
    }
    
    int upcomingCount = 0;
    
    vector<int> widths = {30,15,25,20,15,15,15,20};
    vector<vector<string>> rows;
    rows.push_back({"Appointment ID","Doctor ID","Doctor's full name","Specialization","Clinic","Date","Time","Reason"});

    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        
        if (!details.appointmentId.empty() && details.bookStatus == "Booked" && details.visitStatus == "Not Done"){
            string clinicName = details.clinic.empty() ? "Not updated" : details.clinic;
            vector<string> infor = getDoctorInfo(details.doctorId);
            rows.push_back({details.appointmentId,details.doctorId,infor[0],infor[1],clinicName, details.date,details.time,details.reason});

            upcomingCount++;
        }
    }
    drawTable(15,10,widths,rows);
    if (upcomingCount == 0) {
        cout << "\n\t\t\t\t\tYou don't have any upcoming appointments" << endl;
        cout << "\t\t\t\t\t========================================" << endl;
        system("pause");
        return false;
    }
    
    cout << "\n\t\t\t\t\tTotal number of upcoming appointments: " << upcomingCount << endl;
    cout << "\t\t\t\t\t========================================" << endl;
    system("pause");
    return true;
}

bool Patient::cancelAppointment(const string& appointmentId) {
    if (!DataStore::appointmentExists(appointmentId)) {
        cout << "Error: Not found appointment " << appointmentId << endl;
        return false;
    }
    DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
    if (details.patientId != this->id) {
        cout << "Error: This appointment does not belong to you!" << endl;
        return false;
    }
    if (details.bookStatus == "Cancelled") {
        cout << "This appointment has been canceled previously." << endl;
        return false;
    }
    if (details.visitStatus == "Done") {
        cout << "Cannot cancel an appointment that has been completed!" << endl;
        return false;
    }
    if (DataStore::updateBookAppointmentStatus(appointmentId, "Cancelled")) {
        ostringstream oss;
        oss << "\n========================================" << endl;
        oss << "   CANCELLED APPOINTMENT SUCCESSFULLY!" << endl;
        oss << "========================================" << endl;
        oss << "Appointment ID: " << appointmentId << endl;
        oss << "========================================" << endl;
        cout << oss.str() << flush;
        return true;
    }
    cout << "Error: Cannot cancel the appointment!" << endl;
    return false;
}

bool Patient::viewAppointmentHistory() const {
    vector<string> appointments = DataStore::getPatientAppointments(this->id);
    if (appointments.empty()) {
        cout << "\nYou do not have any medical appointments" << endl;
        return false;
    }
    int completedCount = 0;
    int cancelledCount = 0;
    int activeCount = 0;
    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        if (!details.appointmentId.empty()) {
            if (details.bookStatus == "Cancelled" || details.visitStatus == "Done"){
                cout << "\n\t\t\t\t\t----------------------------------------" << endl;
                cout << "\t\t\t\t\tAppointment ID: " << details.appointmentId << endl;
                cout << "\t\t\t\t\tDoctor: " << getDoctorInfo(details.doctorId)[0] << endl;
                cout << "\t\t\t\t\tClinic: " << (details.clinic.empty() ? "Not updated" : details.clinic) << endl;
                cout << "\t\t\t\t\tDate: " << details.date << endl;
                cout << "\t\t\t\t\tTime: " << details.time << endl;
                cout << "\t\t\t\t\tReason: " << details.reason << endl;
                cout << "\t\t\t\t\tBook status: ";
                if (details.bookStatus == "Cancelled"){
                    cout << "✗ Cancelled" << endl;
                    cout << "\t\t\t\t\tVisit Status: " << "✗" << details.visitStatus << endl;
                    cancelledCount++;
                }
                else {
                    cout << "✓ Booked" << endl;
                    cout << "\t\t\t\t\tVisit status: ✓ Done" << endl;
                    completedCount++;
                }
            }
        }
    }
    ostringstream os;
    os << "\n\t\t\t\t\t========================================" << endl;
    os << "\t\t\t\t\tStatistics:" << endl;
    os << "\t\t\t\t\t- Total appointments: " << appointments.size() << endl;
    os << "\t\t\t\t\t- Examination completed: " << completedCount << endl;
    os << "\t\t\t\t\t- Cancelled: " << cancelledCount << endl;
    os << "\t\t\t\t\t========================================" << endl;
    SetColor(14);
    cout << os.str() << flush;
    SetColor(7);
    system("pause");
    return true;
}

/*--------------------------------------------------------------
            ĐẾM SỐ LỊCH KHÁM SẮP TỚI CỦA BỆNH NHÂN
---------------------------------------------------------------*/
int Patient::countActiveAppointments() const {
    vector<string> appointments = DataStore::getPatientAppointments(this->id);
    int count = 0;
    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        if (!details.appointmentId.empty() && 
            details.bookStatus == "Booked" && 
            details.visitStatus == "Not Done") {
            count++;
        }
    }
    return count;
}

/*--------------------------------------------------------------
                LẤY THÔNG TIN CƠ BẢN CỦA BÁC SĨ
---------------------------------------------------------------*/
vector<string> Patient::getDoctorInfo(const string& doctorId) const {
    string filepath = "data/doctors/" + doctorId + ".txt";
    ifstream file(filepath);
    if (!file.is_open()) {
        return { doctorId + " [Not found information]"};
    }
    string line;
    string name = "";
    string specialization = "";
    string clinic = "";
    vector<string> infor;
    while (getline(file, line)) {
        size_t pos = line.find(":");
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            if (key == "Full name") {
                name = value;
            } else if (key == "Specialization") {
                specialization = value;
            } else if (key == "Clinic") {
                clinic = value;
            }
        }
    }
    file.close();

    if (!name.empty()) {
        infor.push_back(name);
    }
    if (!specialization.empty()) {
        infor.push_back(specialization);
    }
    if (!clinic.empty()) {
        infor.push_back(clinic);
    }
    return infor;
}
