#include "Doctor.h"
#include "DataStore.h"
#include <sstream>
#include <limits>
#include <iomanip>

// Constructor mặc định
Doctor::Doctor() :User(){
    specialization = "";
    doctorRole = "";
    userType = DOCTOR;
}

// Constructor đơn giản cho đăng ký
Doctor::Doctor(string id, string identicalCard, string password)
    :User(id,identicalCard , password, DOCTOR), specialization(""), doctorRole(""){}

// Constructor đầy đủ
Doctor::Doctor(string id, string identicalCard, string password,string fullName,string dateofbirth, string gender, string email, string phoneNumber, string address ,string specialization, string doctorRole)
    :User(id,identicalCard,password,fullName,dateofbirth,gender,email,phoneNumber,address, DOCTOR),
    specialization(specialization), doctorRole(doctorRole) {}

// Destructor
Doctor::~Doctor() {}

// Getters
string Doctor::getSpecialization() const {
    return specialization;
}
string Doctor::getDoctorRole() const{
    return doctorRole;
}

// Setters
void Doctor::setSpecialization(string specialization) {
    this->specialization = specialization;
}
void Doctor::setDoctorRole(string doctorRole){
    this->doctorRole = doctorRole;
}

// Hiển thị thông tin
void Doctor::displayInfo() const {
    cout << "==================================" << endl;
    cout << "THÔNG TIN BÁC SĨ" << endl;
    cout << "==================================" << endl;
    User::displayInfo();
    cout << "Chuyên khoa:" << (specialization.empty() ? "[Chưa cập nhật]" :specialization) << endl;
    cout << "Vai trò:" << (doctorRole.empty()?"[Chưa cập nhật]" :doctorRole) << endl;
    cout << "==================================" << endl;
}

bool Doctor::updateProfile(Doctor& doctor){
    User::updateProfile(doctor);
    
    // Cập nhật Chuyên khoa
    cout << "Chuyên khoa:";
    string newSpec;
    getline(cin, newSpec);
    if (!newSpec.empty()) {
        this->specialization = newSpec;
    }
    
    // Cập nhật Vai trò
    cout << "Vai trò:";
    string newRole;
    getline(cin, newRole);
    if (!newRole.empty()) {
        this->doctorRole = newRole;
    }
    
    return true;
}

ostream& operator<<(ostream& o, const Doctor& doctor){
    o << static_cast<const User&>(doctor);
    o << "Chuyên khoa:" << doctor.getSpecialization() 
        << "\nVai trò:" << doctor.getDoctorRole() << endl;
    return o;
}
istream& operator>>(istream& in, Doctor& doctor){
    in >> static_cast<User&>(doctor);
    if (in.peek() == '\n') in.ignore();
    bool isInteract = (&in == &cin);
    if (isInteract){
        cout << "Chuyên khoa:";
        string spe;
        getline(in,spe);
        doctor.setSpecialization(spe);

        cout << "Vai trò:";
        string role;
        getline(in,role);
        doctor.setDoctorRole(role);
    }
    return in;
}

// Check if profile is complete (Doctor needs specialization and role)
bool Doctor::isProfileComplete() const {
    return !fullName.empty() && 
           !dateOfBirth.empty() && 
           !gender.empty() && 
           !phoneNumber.empty() && 
           !address.empty() &&
           !specialization.empty() &&
           !doctorRole.empty();
}

// View Appointments
bool Doctor::viewAppointment(){
    vector<string> appointments = DataStore::getDoctorAppointments(this->id);
    
    if (appointments.empty()) {
        cout << "\nKhông có lịch hẹn nào." << endl;
        return false;
    }
    
    cout << "\n========================================" << endl;
    cout << "   DANH SÁCH LỊCH HẸN CỦA BẠN" << endl;
    cout << "========================================" << endl;
    
    // cout << "-------------"
    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        
        if (!details.appointmentId.empty()) {
            cout << "┌──────────────────────┬───────────────────────┬──────────────────┬──────────────┐" << endl;
            cout << "│      Mã cuộc hẹn     │       ID bệnh nhân    │       Ngày       │     Giờ      │" << endl;
            cout << "└──────────────────────┴───────────────────────┴──────────────────┴──────────────┘" << endl;
            cout << left << setw(15) << appointmentId << setw(15) << details.patientId << setw(10) << details.date << setw(10) << details.time << endl;
            cout << appointmentId << " ---" << endl;
            cout << "Bệnh nhân ID: " << details.patientId << endl;
            cout << "Ngày: " << details.date << endl;
            cout << "Giờ: " << details.time << endl;
            // cout << "Lý do: " << details.reason << endl;
            cout << "Trạng thái đặt: " << details.bookStatus << endl;
            cout << "Trạng thái khám: " << details.visitStatus << endl;
             cout << "------------------------------------------------------------------------------------------------" << endl;
        }
    }
    
    cout << "\nTổng số lịch hẹn: " << appointments.size() << endl;
    cout << "========================================" << endl;
    return true;
}

// Reject appointments from patients
bool Doctor::rejectAppointment(){
    // Hiển thị danh sách lịch hẹn
    vector<string> appointments = DataStore::getDoctorAppointments(this->id);
    
    if (appointments.empty()) {
        cout << "\nKhông có lịch hẹn nào để từ chối." << endl;
        return false;
    }
    
    cout << "\n========================================" << endl;
    cout << "   TỪ CHỐI LỊCH HẸN" << endl;
    cout << "========================================" << endl;
    
    // Hiển thị các lịch hẹn có thể từ chối (Booked, Not Done)
    int count = 0;
    for (const string& appointmentId : appointments) {
        DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
        
        if (!details.appointmentId.empty() && 
            details.bookStatus == "Booked" && 
            details.visitStatus == "Not Done") {
            cout << "\nMã lịch hẹn: " << details.appointmentId << endl;
            cout << "Bệnh nhân: " << details.patientId << endl;
            cout << "Ngày: " << details.date << " | Giờ: " << details.time << endl;
            cout << "Lý do: " << details.reason << endl;
            cout << "----------------------------------------" << endl;
            count++;
        }
    }
    
    if (count == 0) {
        cout << "\nKhông có lịch hẹn nào có thể từ chối." << endl;
        return false;
    }
    
    // Nhập mã lịch hẹn cần từ chối
    cout << "\nNhập mã lịch hẹn cần từ chối (hoặc 0 để hủy): ";
    string appointmentId;
    cin.ignore();
    getline(cin, appointmentId);
    
    if (appointmentId == "0") {
        cout << "Đã hủy thao tác." << endl;
        return false;
    }
    
    // Kiểm tra appointment có tồn tại không
    if (!DataStore::appointmentExists(appointmentId)) {
        cout << "Lỗi: Không tìm thấy lịch hẹn!" << endl;
        return false;
    }
    
    // Đọc thông tin appointment
    DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
    
    // Kiểm tra appointment có phải của bác sĩ này không
    if (details.doctorId != this->id) {
        cout << "Lỗi: Lịch hẹn này không thuộc về bạn!" << endl;
        return false;
    }
    
    // Kiểm tra trạng thái
    if (details.bookStatus != "Booked") {
        cout << "Lỗi: Không thể từ chối lịch hẹn này!" << endl;
        return false;
    }
    
    // Cập nhật trạng thái thành Cancelled
    if (DataStore::updateBookAppointmentStatus(appointmentId, "Cancelled")) {
        cout << "\n========================================" << endl;
        cout << "   ĐÃ TỪ CHỐI LỊCH HẸN THÀNH CÔNG!" << endl;
        cout << "========================================" << endl;
        cout << "Mã lịch hẹn: " << appointmentId << endl;
        cout << "========================================" << endl;
        return true;
    }
    
    cout << "Lỗi: Không thể từ chối lịch hẹn!" << endl;
    return false;
}