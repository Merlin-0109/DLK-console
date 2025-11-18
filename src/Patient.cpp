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
    // Nếu date rỗng -> gọi tương tác để chọn ngày và khung giờ trong 1 tuần
    string chosenDate = date;
    string chosenTime = time;
    string reasonInput = reason;

    if (doctorId.empty()) {
        cout << "Error: Doctor code cannot be empty!" << endl;
        return false;
    }
    string dayWeek[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    string dayWeek_runMenu[8];
    string slots[] = {"07:00","08:00","09:00","10:00","13:30","14:30","15:30"};
    vector<tm> dates;
    
    if (chosenDate.empty()) {
        // Hiển thị 7 ngày gần nhất (từ hôm nay)
        
        time_t now = std::time(nullptr);
        // chi dat trong vong 7 ngay, khong ke ngay hien tai va chu nhat
        for (int i = 0; i < 7; ++i) {
            time_t t = now + (i+1) * 24 * 60 * 60;
            tm tm_ptr = *localtime(&t);
            dates.push_back(tm_ptr);
        }

        cout << "\n\n\t\t\t\t\t==========BOOKING CALENDAR==========" << endl;
        cout << setw(15) << " ";
        for (int i = 0; i < 7; i++){
            tm d = dates[i];
            if (d.tm_wday == 0) SetColor(12);
            else SetColor(15);
            // format day dd/mm
            char buf[6];
            strftime(buf,sizeof(buf),"%d/%m",&dates[i]);

            cout << setw(15) << dayWeek[d.tm_wday] + "(" + buf +")";
            dayWeek_runMenu[i] = dayWeek[d.tm_wday];
        }
        dayWeek_runMenu[7] = dayWeek[dates[6].tm_wday];
        SetColor(7);
    }
    cout << "\n\n";
    string booked[7][8];
    vector<string> appointments = DataStore::getDoctorAppointments(doctorId);
    for (string listID : appointments){
        DataStore::AppointmentDetails details = DataStore::readAppointment(listID);
        size_t pos1 = details.date.find(":");
        string key1 = details.date.substr(pos1 + 1);
        size_t pos2 = details.time.find(":");
        string key2 = details.time.substr(pos2 + 1);
        
    }
    for (int row = 0; row < 7; row++){
        cout << setw(15) << slots[row];

        for (int col = 0; col < 7; col++){
            tm d = dates[col];
            char buf[6];
            strftime(buf, sizeof(buf),"%d/%m", &dates[col]);
            string key = buf + slots[row];

            if (d.tm_wday == 0){
                SetColor(12);
                cout << setw(15) << "[OFF]";
                SetColor(7);
            }
            else if(booked[row][col] != "null")
                cout << setw(15) << "[✓]";
            else cout << setw(15) << "[ ]";
        }
        cout << endl;
    }
    cout << "Note: " << endl;
    cout << "- [OFF]: Unable to schedule (Sunday)" << endl;
    cout << "- [✓]: Booked" << endl;
    cout << "- [ ]: Empty slot" << endl;
    system("pause");
    // chon ngay
    cout << "\n\n\t\t\t\t\tPlease choose an appointment date!" << endl;
    int choiceDate = runMenu(dayWeek_runMenu,7);
    switch(choiceDate){
        case 1:{
            tm d = dates[0];
            if (d.tm_wday == 0) SetColor(12);
            else SetColor(15);
            // format day dd/mm
            char buf[6];
            strftime(buf,sizeof(buf),"%d/%m",&dates[0]);
            chosenDate = string(buf);
            break;
        }
        case 2:{
            tm d = dates[1];
            if (d.tm_wday == 0) SetColor(12);
            else SetColor(15);

            char buf[6];
            strftime(buf,sizeof(buf),"%d/%m",&dates[1]);
            chosenDate = string(buf);
            break;
        }
        case 3:{
            tm d = dates[2];
            if (d.tm_wday == 0) SetColor(12);
            else SetColor(15);

            char buf[6];
            strftime(buf,sizeof(buf),"%d/%m",&dates[2]);
            chosenDate = string(buf);
            break;
        }
        case 4:{
            tm d = dates[3];
            if (d.tm_wday == 0) SetColor(12);
            else SetColor(15);
            
            char buf[6];
            strftime(buf,sizeof(buf),"%d/%m",&dates[3]);
            chosenDate = string(buf);
            break;
        }
        case 5:{
            tm d = dates[4];
            if (d.tm_wday == 0) SetColor(12);
            else SetColor(15);
            
            char buf[6];
            strftime(buf,sizeof(buf),"%d/%m",&dates[4]);
            chosenDate = string(buf);
            break;
        }
        case 6:{
            tm d = dates[5];
            if (d.tm_wday == 0) SetColor(12);
            else SetColor(15);
            
            char buf[6];
            strftime(buf,sizeof(buf),"%d/%m",&dates[5]);
            chosenDate = string(buf);
            break;
        }
        case 7:{
            tm d = dates[6];
            if (d.tm_wday == 0) SetColor(12);
            else SetColor(15);

            char buf[6];
            strftime(buf,sizeof(buf),"%d/%m",&dates[6]);
            chosenDate = string(buf);
            break;
        }
    }

    cout << "\n\n\t\t\t\t\tPlease choose an appointment time!" << endl;
    // chon khung gio
    int choiceTime = runMenu(slots,7);
    switch(choiceTime){
        case 1: // 07:00
            chosenTime = "07:00";
            break;
        case 2: // 08:00
            chosenTime = "08:00";
            break;
        case 3: // 09:00
            chosenTime = "09:00";
            break;
        case 4: // 10:00
            chosenTime = "10:00";
            break;
        case 5: // 13:30
            chosenTime = "13:30";
            break;
        case 6: // 14:30
            chosenTime = "14:30";
            break;
        case 7: // 15:30
            chosenTime = "15:30";
            break;
    }
    

    // Nhập lý do nếu chưa có
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

    // Tạo appointment ID
    string appointmentId = DataStore::generateAppointmentID();

    // Tạo appointment details
    DataStore::AppointmentDetails details;
    details.appointmentId = appointmentId;
    details.patientId = this->id;
    details.doctorId = doctorId;
    details.date = chosenDate;
    details.time = chosenTime;
    details.reason = reasonInput;
    details.bookStatus = "Booked";
    details.visitStatus = "Not Done";

    // Lưu appointment
    if (DataStore::writeAppointment(appointmentId, details)) {
        ostringstream oss;
        oss << "\n\t\t\t\t\t========================================" << endl;
        SetColor(14);
        oss << "   ✓ BOOKED APPOINTMENT SUCCESSFULL!" << endl;
        cout << oss.str() << flush;
        SetColor(7);
        oss.str("");
        oss.clear();

        vector<string>infor = getDoctorInfo(doctorId);
        oss << "\t\t\t\t\t========================================" << endl;
        oss << "\t\t\t\t\tAppointment ID: " << appointmentId << endl;
        oss << "\t\t\t\t\tPatient: " << fullName << " (" << id << ")" << endl;
        oss << "\t\t\t\t\tDoctor: " << infor[0] << endl;
        oss << "\t\t\t\t\tDate: " << chosenDate << endl;
        oss << "\t\t\t\t\tTime: " << chosenTime << endl;
        oss << "\t\t\t\t\tReason: " << reasonInput << endl;
        oss << "\t\t\t\t\t========================================" << endl;
        oss << "\t\t\t\t\t⚠ Please arrive on time and bring your Identity Card!" << endl;
        cout << oss.str() << flush;
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
    
    vector<int> widths = {22,23,18,14,21};
    vector<vector<string>> rows;

    rows.push_back({"Appointment ID", "Doctor", "Date", "Time", "Reason"});
    
    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        vector<string> infor = getDoctorInfo(details.doctorId);
        rows.push_back({details.appointmentId, infor[0],details.date, details.time});
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
            displayAppointmentDetails(details);
            
            if (details.visitStatus == "Done") {
                completedCount++;
            } else if (details.bookStatus == "Cancelled") {
                cancelledCount++;
            } else {
                activeCount++;
            }
        }
    }
    
    ostringstream os;
    os << "\n========================================" << endl;
    os << "Statistics:" << endl;
    os << "- Total appointments: " << appointments.size() << endl;
    os << "- Waiting for examination: " << activeCount << endl;
    os << "- Examination completed: " << completedCount << endl;
    os << "- Cancelled: " << cancelledCount << endl;
    os << "========================================" << endl;
    cout << os.str() << flush;
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
    
    vector<int> widths = {30,15,30,30,20,15,20};
    vector<vector<string>> rows;
    rows.push_back({"Appointment ID","Doctor ID","Doctor's full name","Specialization","Date","Time","Reason"});

    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        
        if (!details.appointmentId.empty() && 
            details.bookStatus == "Booked" && 
            details.visitStatus == "Not Done") {
            vector<string> infor = getDoctorInfo(details.doctorId);
            // displayAppointmentDetails(details);
            rows.push_back({details.appointmentId,details.doctorId, infor[0],infor[1], details.date,details.time,details.reason});

            upcomingCount++;
        }
    }
    drawTable(15,10,widths,rows);
    if (upcomingCount == 0) {
        cout << "\n\t\t\t\t\tYou don't have any upcoming appointments" << endl;
        cout << "\t\t\t\t\t========================================" << endl;
        return false;
    }
    
    cout << "\n\t\t\t\t\tTotal number of upcoming appointments: " << upcomingCount << endl;
    cout << "\t\t\t\t\t========================================" << endl;
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

// Display appointment details (helper function)
void Patient::displayAppointmentDetails(const DataStore::AppointmentDetails& details) const {
    cout << "\n----------------------------------------" << endl;
    cout << "Appointment ID: " << details.appointmentId << endl;
    cout << "Doctor: " << getDoctorInfo(details.doctorId)[0] << endl;
    cout << "Date: " << details.date << endl;
    cout << "Time: " << details.time << endl;
    cout << "Reason: " << details.reason << endl;
    cout << "Book status: ";
    
    if (details.bookStatus == "Booked") {
        cout << "✓ Booked" << endl;
    } else if (details.bookStatus == "Cancelled") {
        cout << "✗ Cancelled" << endl;
    } else {
        cout << details.bookStatus << endl;
    }
    
    cout << "Visit status: ";
    if (details.visitStatus == "Done") {
        cout << "✓ Done" << endl;
    } else if (details.visitStatus == "Not Done") {
        cout << "⧗ Not done" << endl;
    } else {
        cout << details.visitStatus << endl;
    }
    cout << "----------------------------------------" << endl;
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
    
    return infor;
}