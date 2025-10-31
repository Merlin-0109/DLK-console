#include "Doctor.h"
#include "DataStore.h"
#include <sstream>
#include <limits>

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
    cout << "ID:" << id << endl;
    cout << "CCCD:" << username << endl;
    cout << "Email:" << email << endl;
    cout << "Họ và tên:" << (fullName.empty() ? "[Chưa cập nhật]" :fullName) << endl;
    cout << "Chuyên khoa:" << (specialization.empty() ? "[Chưa cập nhật]" :specialization) << endl;
    cout << "Vai trò:" << (doctorRole.empty()?"[Chưa cập nhật]" :doctorRole) << endl;
    cout << "==================================" << endl;
}

bool Doctor::updateProfile(Doctor& doctor){
    User::updateProfile(doctor);
    
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

// View Appointments
bool Doctor::viewAppointment(){
    return true;
}

// Reject appointments from patients
bool Doctor::rejectAppointment(){
    return true;
}