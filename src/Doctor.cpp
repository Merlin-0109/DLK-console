#include "Doctor.h"
#include "DataStore.h"
#include <sstream>
#include <limits>
#include <iomanip>
#include "UI.h"

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
    cout << "PERSONAL INFORMATION" << endl;
    cout << "==================================" << endl;
    User::displayInfo();
    cout << "Specialization:" << (specialization.empty() ? "[Not updated]" :specialization) << endl;
    cout << "Role:" << (doctorRole.empty()?"[Not updated]" :doctorRole) << endl;
    cout << "==================================" << endl;
}

bool Doctor::updateProfile(Doctor& doctor){
    User::updateProfile(doctor);
    
    cout << "Specialization:";
    string newSpec;
    getline(cin, newSpec);
    if (!newSpec.empty()) {
        this->specialization = newSpec;
    }
    
    // Cập nhật Role
    cout << "Role:";
    string newRole;
    getline(cin, newRole);
    if (!newRole.empty()) {
        this->doctorRole = newRole;
    }
    return true;
}

ostream& operator<<(ostream& o, const Doctor& doctor){
    o << static_cast<const User&>(doctor);
    o << "Specialization:" << doctor.getSpecialization() 
        << "\nRole:" << doctor.getDoctorRole() << endl;
    return o;
}
istream& operator>>(istream& in, Doctor& doctor){
    // in >> static_cast<User&>(doctor);
    // if (in.peek() == '\n') in.ignore();
    // bool isInteract = (&in == &cin);
    // if (isInteract){
    //     cout << "Specialization:";
    //     string spe;
    //     getline(in,spe);
    //     doctor.setSpecialization(spe);

    //     cout << "Role:";
    //     string role;
    //     getline(in,role);
    //     doctor.setDoctorRole(role);
    // }
    // return in;
    in >> static_cast<User&>(doctor);
    if (in.peek() == '\n') in.ignore();
    bool isInteract = (&in == &cin);
    
    if (!isInteract) {
        // Đọc từ file - đọc specialization và doctorRole từ stream
        string line;
        while(getline(in, line)) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string val = line.substr(pos + 1);
                
                if (key == "Chuyên khoa") doctor.setSpecialization(val);
                else if (key == "Vai trò") doctor.setDoctorRole(val);
            }
        }
    } else {
        // Nhập từ bàn phím
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
        cout << "\nYou don't have any appointments yet" << endl;
        return false;
    }
    vector<int> widths = {22,23,18,14,21};
    vector<vector<string>> rows;

    rows.push_back({"Appointment ID", "Patient ID", "Date", "Time", "Reason"});
    for (string listID : appointments){
        DataStore::AppointmentDetails d = DataStore::readAppointment(listID);
        rows.push_back({d.appointmentId, d.patientId, d.date, d.time, d.reason});
    }
    drawTable(5,8,widths,rows);
    cout << "\nTotal appointments: " << appointments.size() << endl;
    return true;
}

// decline appointments from patients
bool Doctor::declineAppointment(){
    // Hiển thị danh sách lịch hẹn
    vector<string> appointments = DataStore::getDoctorAppointments(this->id);
    Doctor::viewAppointment();
    if (appointments.empty()) {
        cout << "\nYou have no appointment to decline" << endl;
        return false;
    }
    // Nhập mã lịch hẹn cần từ chối
    cout << "\nEnter the appointment code to decline (or 0 to cancel) ";
    string appointmentId;

    getline(cin, appointmentId);
    
    if (appointmentId == "0") {
        cout << "Operation has been canceled" << endl;
        return false;
    }
    
    // Kiểm tra appointment có tồn tại không
    if (!DataStore::appointmentExists(appointmentId)) {
        cout << "Not found appointment!" << endl;
        return false;
    }
    
    // Đọc thông tin appointment
    DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
    
    // Kiểm tra appointment có phải của bác sĩ này không
    if (details.doctorId != this->id) {
        cout << "Error: This appointment does not belong to you!" << endl;
        return false;
    }
    
    // Kiểm tra trạng thái
    if (details.bookStatus != "Booked") {
        cout << "Error: Cannot decline this appointment!" << endl;
        return false;
    }
    
    // Cập nhật trạng thái thành Cancelled
    if (DataStore::updateBookAppointmentStatus(appointmentId, "Cancelled")) {
        cout << "\n========================================" << endl;
        cout << "   ĐÃ TỪ CHỐI LỊCH HẸN THÀNH CÔNG!" << endl;
        cout << "========================================" << endl;
        cout << "Appointment ID: " << appointmentId << endl;
        cout << "========================================" << endl;
        return true;
    }
    
    cout << "Error: Cannot decline this appointment!" << endl;
    return false;
}