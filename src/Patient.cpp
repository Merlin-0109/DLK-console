#include "Patient.h"
#include "DataStore.h"
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
    cout << "==================================" << endl;
    cout << "THÔNG TIN BỆNH NHÂN" << endl;
    cout << "==================================" << endl;
    cout << "ID:" << id << endl;
    cout << "CCCD:" << username << endl;
    cout << "Email:" << email << endl;
    cout << "Họ và tên:" << (fullName.empty() ? "[Chưa cập nhật]" :fullName) << endl;
    cout << "Ngày sinh:" << (dateOfBirth.empty() ? "[Chưa cập nhật]" :dateOfBirth) << endl;
    cout << "Số điện thoại:" << (phoneNumber.empty() ? "[Chưa cập nhật]" :phoneNumber) << endl;
    cout << "Địa chỉ:" << (address.empty() ? "[Chưa cập nhật]" :address) << endl;
    cout << "==================================" << endl;
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
        cout << "\n⚠ Vui lòng cập nhật đầy đủ thông tin cá nhân trước khi đặt lịch khám!" << endl;
        return false;
    }
    
    // Validate input
    if (doctorId.empty()) {
        cout << "Lỗi: Mã bác sĩ không được để trống!" << endl;
        return false;
    }
    
    if (reason.empty()) {
        cout << "Lỗi: Vui lòng nhập lý do khám!" << endl;
        return false;
    }
    
    // Validate date format
    if (!validateDate(date)) {
        cout << "Lỗi: Định dạng ngày không hợp lệ! Vui lòng nhập theo định dạng DD/MM/YYYY" << endl;
        return false;
    }
    
    // Validate time format
    if (!validateTime(time)) {
        cout << "Lỗi: Định dạng giờ không hợp lệ! Vui lòng nhập theo định dạng HH:MM" << endl;
        return false;
    }
    
    // Check if date/time is in the future
    if (!isDateInFuture(date, time)) {
        cout << "Lỗi: Không thể đặt lịch cho thời gian trong quá khứ!" << endl;
        return false;
    }
    
    // Tạo appointment ID
    string appointmentId = DataStore::generateAppointmentID();
    
    // Tạo appointment details
    DataStore::AppointmentDetails details;
    details.appointmentId = appointmentId;
    details.patientId = this->id;
    details.doctorId = doctorId;
    details.date = date;
    details.time = time;
    details.reason = reason;
    details.bookStatus = "Booked";
    details.visitStatus = "Not Done";
    
    // Lưu appointment
    if (DataStore::writeAppointment(appointmentId, details)) {
        cout << "\n========================================" << endl;
        cout << "   ✓ ĐẶT LỊCH KHÁM THÀNH CÔNG!" << endl;
        cout << "========================================" << endl;
        cout << "Mã lịch khám: " << appointmentId << endl;
        cout << "Bệnh nhân: " << fullName << " (" << id << ")" << endl;
        cout << "Bác sĩ: " << getDoctorInfo(doctorId) << endl;
        cout << "Ngày: " << date << endl;
        cout << "Giờ: " << time << endl;
        cout << "Lý do: " << reason << endl;
        cout << "========================================" << endl;
        cout << "⚠ Vui lòng đến đúng giờ và mang theo CCCD/CMND!" << endl;
        cout << "========================================" << endl;
        return true;
    }
    
    cout << "Lỗi: Không thể đặt lịch khám!" << endl;
    return false;
}

// View my appointments
bool Patient::viewMyAppointments() const {
    vector<string> appointments = DataStore::getPatientAppointments(this->id);
    
    if (appointments.empty()) {
        cout << "\nBạn chưa có lịch khám nào." << endl;
        return false;
    }
    
    cout << "\n========================================" << endl;
    cout << "   DANH SÁCH LỊCH KHÁM CỦA BẠN" << endl;
    cout << "========================================" << endl;
    
    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        
        if (!details.appointmentId.empty()) {
            cout << "\nMã lịch khám: " << details.appointmentId << endl;
            cout << "Bác sĩ: " << getDoctorInfo(details.doctorId) << endl;
            cout << "Ngày: " << details.date << endl;
            cout << "Giờ: " << details.time << endl;
            cout << "Lý do: " << details.reason << endl;
            cout << "Trạng thái đặt lịch: " << details.bookStatus << endl;
            cout << "Trạng thái khám: " << details.visitStatus << endl;
            cout << "----------------------------------------" << endl;
        }
    }
    
    cout << "\nTổng số lịch khám: " << appointments.size() << endl;
    cout << "========================================" << endl;
    return true;
}

// Cancel appointment
bool Patient::cancelAppointment(const string& appointmentId) {
    // Kiểm tra appointment có tồn tại không
    if (!DataStore::appointmentExists(appointmentId)) {
        cout << "Lỗi: Không tìm thấy lịch khám với mã " << appointmentId << endl;
        return false;
    }
    
    // Đọc thông tin appointment
    DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
    
    // Kiểm tra appointment có phải của bệnh nhân này không
    if (details.patientId != this->id) {
        cout << "Lỗi: Lịch khám này không thuộc về bạn!" << endl;
        return false;
    }
    
    // Kiểm tra trạng thái hiện tại
    if (details.bookStatus == "Cancelled") {
        cout << "Lịch khám này đã được hủy trước đó." << endl;
        return false;
    }
    
    if (details.visitStatus == "Done") {
        cout << "Không thể hủy lịch khám đã hoàn thành!" << endl;
        return false;
    }
    
    // Cập nhật trạng thái
    if (DataStore::updateBookAppointmentStatus(appointmentId, "Cancelled")) {
        cout << "\n========================================" << endl;
        cout << "   HỦY LỊCH KHÁM THÀNH CÔNG!" << endl;
        cout << "========================================" << endl;
        cout << "Mã lịch khám: " << appointmentId << endl;
        cout << "========================================" << endl;
        return true;
    }
    
    cout << "Lỗi: Không thể hủy lịch khám!" << endl;
    return false;
}

// Reschedule appointment
bool Patient::rescheduleAppointment(const string& appointmentId, const string& newDate, const string& newTime) {
    // Kiểm tra appointment có tồn tại không
    if (!DataStore::appointmentExists(appointmentId)) {
        cout << "Lỗi: Không tìm thấy lịch khám với mã " << appointmentId << endl;
        return false;
    }
    
    // Đọc thông tin appointment
    DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
    
    // Kiểm tra appointment có phải của bệnh nhân này không
    if (details.patientId != this->id) {
        cout << "Lỗi: Lịch khám này không thuộc về bạn!" << endl;
        return false;
    }
    
    // Kiểm tra trạng thái
    if (details.bookStatus == "Cancelled") {
        cout << "Không thể đổi lịch cho cuộc hẹn đã bị hủy!" << endl;
        return false;
    }
    
    if (details.visitStatus == "Done") {
        cout << "Không thể đổi lịch cho cuộc hẹn đã hoàn thành!" << endl;
        return false;
    }
    
    // Validate ngày giờ mới
    if (!validateDate(newDate)) {
        cout << "Lỗi: Định dạng ngày không hợp lệ! Vui lòng nhập theo định dạng DD/MM/YYYY" << endl;
        return false;
    }
    
    if (!validateTime(newTime)) {
        cout << "Lỗi: Định dạng giờ không hợp lệ! Vui lòng nhập theo định dạng HH:MM" << endl;
        return false;
    }
    
    if (!isDateInFuture(newDate, newTime)) {
        cout << "Lỗi: Không thể đặt lịch cho thời gian trong quá khứ!" << endl;
        return false;
    }
    
    // Cập nhật thông tin
    details.date = newDate;
    details.time = newTime;
    
    // Lưu lại appointment
    if (DataStore::writeAppointment(appointmentId, details)) {
        cout << "\n========================================" << endl;
        cout << "   ĐỔI LỊCH KHÁM THÀNH CÔNG!" << endl;
        cout << "========================================" << endl;
        cout << "Mã lịch khám: " << appointmentId << endl;
        cout << "Ngày mới: " << newDate << endl;
        cout << "Giờ mới: " << newTime << endl;
        cout << "========================================" << endl;
        return true;
    }
    
    cout << "Lỗi: Không thể đổi lịch khám!" << endl;
    return false;
}

// View appointment history (all appointments including cancelled and completed)
bool Patient::viewAppointmentHistory() const {
    vector<string> appointments = DataStore::getPatientAppointments(this->id);
    
    if (appointments.empty()) {
        cout << "\nBạn chưa có lịch khám nào." << endl;
        return false;
    }
    
    cout << "\n========================================" << endl;
    cout << "   LỊCH SỬ KHÁM BỆNH" << endl;
    cout << "========================================" << endl;
    
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
    
    cout << "\n========================================" << endl;
    cout << "THỐNG KÊ:" << endl;
    cout << "- Tổng số lịch khám: " << appointments.size() << endl;
    cout << "- Đang chờ khám: " << activeCount << endl;
    cout << "- Đã khám xong: " << completedCount << endl;
    cout << "- Đã hủy: " << cancelledCount << endl;
    cout << "========================================" << endl;
    return true;
}

// View upcoming appointments (only active appointments)
bool Patient::viewUpcomingAppointments() const {
    vector<string> appointments = DataStore::getPatientAppointments(this->id);
    
    if (appointments.empty()) {
        cout << "\nBạn chưa có lịch khám nào." << endl;
        return false;
    }
    
    cout << "\n========================================" << endl;
    cout << "   LỊCH KHÁM SẮP TỚI" << endl;
    cout << "========================================" << endl;
    
    int upcomingCount = 0;
    
    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        
        if (!details.appointmentId.empty() && 
            details.bookStatus == "Booked" && 
            details.visitStatus == "Not Done") {
            
            displayAppointmentDetails(details);
            upcomingCount++;
        }
    }
    
    if (upcomingCount == 0) {
        cout << "\nBạn không có lịch khám nào sắp tới." << endl;
        cout << "========================================" << endl;
        return false;
    }
    
    cout << "\nTổng số lịch khám sắp tới: " << upcomingCount << endl;
    cout << "========================================" << endl;
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
    cout << "Mã lịch khám: " << details.appointmentId << endl;
    cout << "Bác sĩ: " << getDoctorInfo(details.doctorId) << endl;
    cout << "Ngày: " << details.date << endl;
    cout << "Giờ: " << details.time << endl;
    cout << "Lý do: " << details.reason << endl;
    cout << "Trạng thái đặt lịch: ";
    
    if (details.bookStatus == "Booked") {
        cout << "✓ Đã đặt" << endl;
    } else if (details.bookStatus == "Cancelled") {
        cout << "✗ Đã hủy" << endl;
    } else {
        cout << details.bookStatus << endl;
    }
    
    cout << "Trạng thái khám: ";
    if (details.visitStatus == "Done") {
        cout << "✓ Đã khám xong" << endl;
    } else if (details.visitStatus == "Not Done") {
        cout << "⧗ Chưa khám" << endl;
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
string Patient::getDoctorInfo(const string& doctorId) const {
    // Read doctor data from file directly
    string filepath = "data/doctors/" + doctorId + ".txt";
    ifstream file(filepath);
    
    if (!file.is_open()) {
        return doctorId + " [Không tìm thấy thông tin]";
    }
    
    // Parse doctor data to get name and specialization
    string line;
    string name = "";
    string specialization = "";
    
    while (getline(file, line)) {
        size_t pos = line.find(":");
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            
            if (key == "Họ và tên") {
                name = value;
            } else if (key == "Chuyên khoa") {
                specialization = value;
            }
        }
    }
    
    file.close();
    
    // Build display string
    stringstream result;
    result << doctorId;
    if (!name.empty()) {
        result << " - " << name;
    }
    if (!specialization.empty()) {
        result << " (" << specialization << ")";
    }
    
    return result.str();
}