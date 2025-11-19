#include "Patient.h"
#include "DataStore.h"
#include "AuthSystem.h"
#include "UI.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <set>
#include <limits>

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

    // Input cua menuHorizontal
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

    // int choiceDate = runMenuHorizontal(dayChoice.data(), (int)dayChoice.size());
    // int realChoiceDate = indexSlotDay[choiceDate-1];
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

    // --- Chọn giờ ---
    cout << "\nPlease choose an appointment time!" << endl;
    // int choiceTime = runMenuHorizontal(timeChoice.data(), (int)timeChoice.size());
    // chosenTime = timeChoice[choiceTime-1];
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

    // --- nhập lý do nếu chưa có ---
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
        cout << "\n========================================" << endl;
        SetColor(14);
        cout << "   ✓ BOOKED APPOINTMENT SUCCESSFULL!" << endl;
        SetColor(7);
        cout << "========================================" << endl;
        cout << "Appointment ID: " << appointmentId << endl;
        cout << "Patient: " << fullName << " (" << id << ")" << endl;
        cout << "Doctor: " << doctorInfo[0] << endl;
        cout << "Clinic: " << clinicName << endl;
        cout << "Date: " << chosenDate << endl;
        cout << "Time: " << chosenTime << endl;
        cout << "Reason: " << reasonInput << endl;
        cout << "========================================" << endl;
        cout << "⚠ Please arrive on time and bring your Identity Card!" << endl;
        return true;
    }

    cout << "Error: Unable to schedule an appointment!" << endl;
    return false;
}


// View my appointments
bool Patient::viewMyAppointments() const {
    vector<string> appointments = DataStore::getPatientAppointments(this->id);
    
    if (appointments.empty()) {
        cout << "\nYou don't have any appointments yet" << endl;
        return false;
    }
    
    vector<int> widths = {22,23,18,15,14,21};
    vector<vector<string>> rows;

    rows.push_back({"Appointment ID", "Doctor","Clinic room", "Date", "Time", "Reason"});
    
    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        vector<string> infor = getDoctorInfo(details.doctorId);
        rows.push_back({details.appointmentId, infor[0], details.clinic,details.date, details.time});
    }
    drawTable(5,8,widths, rows);
    cout << "\nTotal appointments: " << appointments.size() << endl;
    return true;
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

// Reschedule appointment
bool Patient::rescheduleAppointment(const string& appointmentId, const string& newDate, const string& newTime) {
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
    
    // Kiểm tra trạng thái
    if (details.bookStatus == "Cancelled") {
        cout << "Cannot reschedule an appointment that has been canceled!" << endl;
        return false;
    }
    
    if (details.visitStatus == "Done") {
        cout << "Cannot reschedule an appointment that has been completed!" << endl;
        return false;
    }
    
    // Validate ngày giờ mới
    if (!validateDate(newDate)) {
        cout << "Error: Invalid date format! Please enter in the format DD/MM/YYYY" << endl;
        return false;
    }
    
    if (!validateTime(newTime)) {
        cout << "Error: Invalid time format! Please enter in HH:MM format" << endl;
        return false;
    }
    
    if (!isDateInFuture(newDate, newTime)) {
        cout << "Error: Cannot schedule for a time in the past!" << endl;
        return false;
    }
    
    // Cập nhật thông tin
    details.date = newDate;
    details.time = newTime;
    
    // Lưu lại appointment
    if (DataStore::writeAppointment(appointmentId, details)) {
        ostringstream oss;
        oss << "\n========================================" << endl;
        oss << "   RESCHEDULED APPOINTMENT SUCCESSFULLY!" << endl;
        oss << "========================================" << endl;
        oss << "Appointment ID: " << appointmentId << endl;
        oss << "Date: " << newDate << endl;
        oss << "Time: " << newTime << endl;
        oss << "========================================" << endl;
        cout << oss.str() << flush;
        return true;
    }
    
    cout << "Error: Unable to reschedule the appointment!" << endl;
    return false;
}

// View appointment history (all appointments including cancelled and completed)
bool Patient::viewAppointmentHistory() const {
    vector<string> appointments = DataStore::getPatientAppointments(this->id);
    
    if (appointments.empty()) {
        cout << "\nYou do not have any medical appointments" << endl;
        return false;
    }
    
    ostringstream oss;
    oss << "\n========================================" << endl;
    oss << "   MEDICAL EXAMINATION HISTORY" << endl;
    oss << "========================================" << endl;
    cout << oss.str() << flush;
    
    int completedCount = 0;
    int cancelledCount = 0;
    int activeCount = 0;
    
    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        
        if (!details.appointmentId.empty()) {
            if (details.bookStatus == "Cancelled" || details.visitStatus == "Done"){
                cout << "\n----------------------------------------" << endl;
                cout << "Appointment ID: " << details.appointmentId << endl;
                cout << "Doctor: " << getDoctorInfo(details.doctorId)[0] << endl;
                cout << "Clinic: " << (details.clinic.empty() ? "Not updated" : details.clinic) << endl;
                cout << "Date: " << details.date << endl;
                cout << "Time: " << details.time << endl;
                cout << "Reason: " << details.reason << endl;
                cout << "Book status: ";

                if (details.bookStatus == "Cancelled"){
                    cout << "✗ Cancelled" << endl;
                    cout << "Visit Status: " << "✗" << details.visitStatus << endl;
                    cancelledCount++;
                }
                else {
                    cout << "✓ Booked" << endl;
                    cout << "Visit status: ✓ Done" << endl;
                    completedCount++;
                }
            }
        }
    }
    
    ostringstream os;
    os << "\n========================================" << endl;
    os << "Statistics:" << endl;
    os << "- Total appointments: " << appointments.size() << endl;
    os << "- Examination completed: " << completedCount << endl;
    os << "- Cancelled: " << cancelledCount << endl;
    os << "========================================" << endl;
    cout << os.str() << flush;
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

// Validate date format DD/MM/YYYY
bool Patient::validateDate(const string& date) const {
    if (date.length() != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;
    
    // Kiểm tra các ký tự là số
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return false;
    }
    
    // Kiểm tra giá trị ngày, tháng, năm
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if (year < 2024 || year > 2100) return false;
    
    // Kiểm tra số ngày trong tháng
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Năm nhuận
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[1] = 29;
    }
    
    if (day > daysInMonth[month - 1]) return false;
    
    return true;
}

// Validate time format HH:MM
bool Patient::validateTime(const string& time) const {
    if (time.length() != 5) return false;
    if (time[2] != ':') return false;
    
    // Kiểm tra các ký tự là số
    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (!isdigit(time[i])) return false;
    }
    
    // Kiểm tra giá trị giờ và phút
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));
    
    if (hour < 0 || hour > 23) return false;
    if (minute < 0 || minute > 59) return false;
    
    return true;
}

// Check if date and time is in the future
bool Patient::isDateInFuture(const string& date, const string& time) const {
    if (!validateDate(date) || !validateTime(time)) return false;
    
    // Lấy thời gian hiện tại
    time_t now = std::time(0);
    tm* currentTime = localtime(&now);
    
    // Parse ngày giờ đặt lịch
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));
    
    // So sánh năm
    if (year > currentTime->tm_year + 1900) return true;
    if (year < currentTime->tm_year + 1900) return false;
    
    // So sánh tháng
    if (month > currentTime->tm_mon + 1) return true;
    if (month < currentTime->tm_mon + 1) return false;
    
    // So sánh ngày
    if (day > currentTime->tm_mday) return true;
    if (day < currentTime->tm_mday) return false;
    
    // So sánh giờ
    if (hour > currentTime->tm_hour) return true;
    if (hour < currentTime->tm_hour) return false;
    
    // So sánh phút
    if (minute > currentTime->tm_min) return true;
    
    return false;
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