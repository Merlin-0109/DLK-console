#include "Doctor.h"
#include <sstream>

// Constructor mặc định
Doctor::Doctor() : User(), specialization("") {
    userType = DOCTOR;
}

// Constructor đơn giản cho đăng ký
Doctor::Doctor(string id, string username, string password, string email)
    : User(id, username, password, email, DOCTOR),
      specialization("") {}

// Constructor đầy đủ
Doctor::Doctor(string id, string username, string password, string email, string fullName,
               string specialization)
    : User(id, username, password, email, fullName, DOCTOR),
      specialization(specialization) {}

// Destructor
Doctor::~Doctor() {}

// Getters
string Doctor::getSpecialization() const {
    return specialization;
}

// Setters
void Doctor::setSpecialization(string specialization) {
    this->specialization = specialization;
}

// Hiển thị thông tin
void Doctor::displayInfo() const {
    cout << "==================================" << endl;
    cout << "THÔNG TIN BÁC SĨ" << endl;
    cout << "==================================" << endl;
    cout << "Mã số: " << id << endl;
    cout << "Tên đăng nhập: " << username << endl;
    cout << "Email: " << email << endl;
    cout << "Họ và tên: " << (fullName.empty() ? "[Chưa cập nhật]" : fullName) << endl;
    cout << "Chuyên khoa: " << (specialization.empty() ? "[Chưa cập nhật]" : specialization) << endl;
    cout << "==================================" << endl;
}

// Serialize
string Doctor::serialize() const {
    stringstream ss;
    ss << userType << "\n"
       << id << "\n"
       << username << "\n"
       << password << "\n"
       << email << "\n"
       << fullName << "\n"
       << specialization;
    return ss.str();
}

// Update profile
bool Doctor::updateProfile() {
    cout << "\n=== CẬP NHẬT THÔNG TIN BÁC SĨ ===" << endl;
    
    cin.ignore();
    cout << "Họ và tên: ";
    getline(cin, fullName);
    
    cout << "Chuyên khoa: ";
    getline(cin, specialization);
    
    cout << "Cập nhật thông tin thành công!" << endl;
    return true;
}

// View Appointments
bool Doctor::viewAppointment(){

}

// Reject appointments from patients
bool Doctor::rejectAppointment(){

}