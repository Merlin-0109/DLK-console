#include <iomanip>
#include <ctime>
#include <vector>
#include <set>
#include <limits>

#include "Patient.h"
#include "DataStore.h"
#include "AuthSystem.h"
#include "UI.h"

// Constructor mặc định
Patient::Patient() :User() {
    userType = PATIENT;
}

// Constructor đơn giản cho đăng ký
Patient::Patient(string id, string identicalCard, string password)
    :User(id, identicalCard, password, PATIENT) {}

// Constructor đầy đủ
Patient::Patient(string id, string identicalCard, string password,string fullName,string dateofbirth, string gender, string email, string phoneNumber, string address)
    :User(id,identicalCard,password,fullName,dateofbirth,gender,email,phoneNumber,address,PATIENT) {}

// Destructor
Patient::~Patient() {}

// Hiển thị thông tin
void Patient::displayInfo() const {
    cout << "\t\t\t\t\t==================================" << endl;
    User::displayInfo();
    cout << "\t\t\t\t\t==================================" << endl;
}

ostream& operator<<(ostream& o, const Patient& patient){
    o << static_cast<const User&>(patient);
    return o;
}

// Check if profile is complete (Patient needs more info than base User)
bool Patient::isProfileComplete() const {
    return !fullName.empty() && 
           !dateOfBirth.empty() && 
           !gender.empty() && 
           !phoneNumber.empty() && 
           !address.empty();
}
istream& operator>>(istream& in, Patient& patient){
    in >> static_cast<User&>(patient);
    // in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (in.peek() == '\n') in.ignore();

    return in;
}
// Book appointment
bool Patient::bookAppointment(const string& doctorId, const string& date, const string& time, const string& reason) {
    // Kiểm tra thông tin cá nhân đã đầy đủ chưa
    if (!isProfileComplete()) {
        cout << "\n⚠ Please complete your personal information before scheduling an appointment!" << endl;
        return false;
    }

    string chosenDate = date;
    string chosenTime = time;
    string reasonInput = reason;

    string dayWeek[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    string slots[] = {"07:00","08:00","09:00","10:00","13:30","14:30","15:30"};
    vector<tm> dates;
    const int MAX_SLOT = 5;

    time_t now = std::time(nullptr);
    for (int i = 0; i < 7; ++i) {
        time_t t = now + (i+1) * 24*60*60;
        tm tm_ptr = *localtime(&t);
        dates.push_back(tm_ptr);
    }

    // lay ra nhung ngay benh nhan da dat => khong the dat 2 lich kham trong cung mot ngay...
    auto patientApp = DataStore::getPatientAppointments(this->id);
    set<string> bookDates;
    for (auto &appID : patientApp){
        DataStore::AppointmentDetails details = DataStore::readAppointment(appID);
        if (details.bookStatus == "Booked")
            bookDates.insert(details.date);
    }

    auto getSlotCount = [&](const string& d, const string& slot){
        return DataStore::getDoctorAppointmentsForDateSlot(doctorId,d,slot).size();
    };

    // kiem tra ngay nao FULL thi ko hien thi tren menu de chon nua
    bool isDateFull[7] = {false};
    for (int i = 0; i < 7; i++){
        char buf[16];
        strftime(buf, sizeof(buf), "%d/%m", &dates[i]);
        string day = buf;

        int total = 0;
        for (int s = 0; s < 7; s++){
            // Đếm số appointment ở mỗi slot
            total += (int)DataStore::getDoctorAppointmentsForDateSlot(doctorId, day, slots[s]).size();
        }
        if (total >= 35) isDateFull[i] = true;
    }

    cout << "\n\n\t\t\t\t\t==================== BOOKING CALENDAR ====================" << "\n\n";
    cout << setw(15) << " ";
    for (int col = 0; col < 7; ++col) {
        char buf[6];
        strftime(buf, sizeof(buf), "%d/%m", &dates[col]);
        if (dates[col].tm_wday == 0) SetColor(12);
        else SetColor(15);
        cout << setw(15) << dayWeek[dates[col].tm_wday] + "(" + string(buf) + ")";
        SetColor(7);
    }
    cout << "\n";

    for (int row = 0; row < 7; ++row) {
        cout << setw(15) << slots[row];
        for (int col = 0; col < 7; ++col) {
            char buf[6];
            strftime(buf, sizeof(buf), "%d/%m", &dates[col]);
            string dayStr(buf);

            if (dates[col].tm_wday == 0) {
                SetColor(12);
                cout << setw(15) << "[OFF]";
                SetColor(7);
                continue;
            }

            int count = getSlotCount(dayStr, slots[row]);
            if (count == 0) {
                SetColor(6);
                cout << setw(15) << "[ ]";
            } else if (count >= MAX_SLOT) {
                SetColor(12);
                cout << setw(15) << "[FULL]";
            } else {
                SetColor(10);
                string currentSlot = "[" + to_string(count) + "/" + to_string(MAX_SLOT) + "]";
                cout << setw(15) << currentSlot;
            }
            SetColor(7);
        }
        cout << endl;
    }

    SetColor(6*16 + 4);
    cout << "Note:";
    SetColor(7);
    cout << "\n- [OFF]: Unable to schedule (Sunday)" << endl;
    cout << "- [FULL]: Slot fully booked" << endl;
    cout << "- [n/5]: n patients booked, still available" << endl;
    cout << "- [ ]: Empty slot" << endl;

    vector<string> dayChoice;
    vector<int> indexSlotDay;
    for (int i = 0; i < 7; i++){
        char buf[16];
        strftime(buf,sizeof(buf),"%d/%m",&dates[i]);
        if (dates[i].tm_wday == 0) continue;
        if (isDateFull[i]) continue;
        if (bookDates.count(string(buf))) continue; // = 1(true) => co trong danh sach da dat thi bo qua

        string showDay = dayWeek[dates[i].tm_wday] + "(" + string(buf) + ")";
        dayChoice.push_back(showDay);
        indexSlotDay.push_back(i);
    }

   
    // kiem tra gio FULL 
    vector<string> timeChoice;
    vector<int> indexSlotTime;
    for (int i = 0; i < 7; i++){
        int count = (int)getSlotCount(chosenDate,slots[i]);
        if (count < 7){
            timeChoice.push_back(slots[i]);
            indexSlotTime.push_back(i);
        }
    }
    cout << "\nPlease choose an appointment date!" << endl;

    int realChoiceDate = -1;
    while (true) {
        int choiceDate = runMenuHorizontal(dayChoice.data(), (int)dayChoice.size());
        if (choiceDate >= 1 && choiceDate <= (int)indexSlotDay.size()) {
            realChoiceDate = indexSlotDay[choiceDate - 1];
            break; // chọn hợp lệ, thoát vòng lặp
        }
    }

    char buf[6];
    strftime(buf, sizeof(buf), "%d/%m", &dates[realChoiceDate]);
    chosenDate = string(buf);
    SetColor(2);
    cout << "✔ You chose the appointment date: " << chosenDate << endl;
    SetColor(7);

    cout << "\nPlease choose an appointment time!" << endl;
    int realChoiceTime = -1;
    while (true) {
        int choiceTime = runMenuHorizontal(timeChoice.data(), (int)timeChoice.size());
        if (choiceTime >= 1 && choiceTime <= (int)timeChoice.size()) {
            realChoiceTime = choiceTime - 1;
            break; // chọn hợp lệ
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
            // return false;
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
    system("cls");

    if(DataStore::writeAppointment(appointmentId, details)) {
        cout << "\n========================================" << endl;
        SetColor(14);
        cout << "   ✓ BOOKED APPOINTMENT SUCCESSFULL!" << endl;
        SetColor(7);
        cout << "========================================" << endl;
        cout << "Appointment ID: " << appointmentId << endl;
        cout << "Patient: " << fullName << " (" << id << ")" << endl;
        cout << "Doctor: " << doctorInfo[0] << endl;
        cout << "Specialization: " << doctorInfo[1] << endl;
        cout << "Clinic: " << clinicName << endl;
        cout << "Date: " << chosenDate << endl;
        cout << "Time: " << chosenTime << endl;
        cout << "Reason: " << reasonInput << endl;
        cout << "========================================" << endl;
        cout << "⚠ Please arrive on time and bring your Identity Card!" << endl;
        system("pause");
        return true;
    }
    cout << "Error: Unable to schedule an appointment!" << endl;
    return false;
}

// Cancel appointment
bool Patient::cancelAppointment(const string& appointmentId) {
    // Kiểm tra appointment có tồn tại không
    if (!DataStore::appointmentExists(appointmentId)) {
        cout << "Error: Not found appointment " << appointmentId << endl;
        return false;
    }
    
    // Đọc thông tin appointment
    DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
    
    // Kiểm tra appointment có phải của Patient này không
    if (details.patientId != this->id) {
        cout << "Error: This appointment does not belong to you!" << endl;
        return false;
    }
    
    // Kiểm tra trạng thái hiện tại
    if (details.bookStatus == "Cancelled") {
        cout << "This appointment has been canceled previously." << endl;
        return false;
    }
    
    if (details.visitStatus == "Done") {
        cout << "Cannot cancel an appointment that has been completed!" << endl;
        return false;
    }
    
    // Cập nhật trạng thái
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

// View appointment history (all appointments including cancelled and completed)
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

// View upcoming appointments (only active appointments)
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
        
        if (!details.appointmentId.empty() && 
            details.bookStatus == "Booked" && 
            details.visitStatus == "Not Done") {
            vector<string> infor = getDoctorInfo(details.doctorId);
            string clinicName = details.clinic.empty() ? "Not updated" : details.clinic;
            rows.push_back({details.appointmentId,details.doctorId, infor[0],infor[1], clinicName, details.date,details.time,details.reason});

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

// Count active appointments
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

// Get doctor information for display
vector<string> Patient::getDoctorInfo(const string& doctorId) const {
    // Read doctor data from file directly
    string filepath = "data/doctors/" + doctorId + ".txt";
    ifstream file(filepath);
    
    if (!file.is_open()) {
        return { doctorId + " [Not found information]"};
    }
    
    // Parse doctor data to get name and specialization
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