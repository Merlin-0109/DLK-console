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
    clinic = "";
    userType = DOCTOR;
}

// Constructor đơn giản cho đăng ký
Doctor::Doctor(string id, string identicalCard, string password)
    :User(id,identicalCard , password, DOCTOR), specialization(""), doctorRole(""), clinic(""){}

// Constructor đầy đủ
Doctor::Doctor(string id, string identicalCard, string password,string fullName,string dateofbirth, string gender, string email, string phoneNumber, string address ,string specialization, string doctorRole, string clinic)
    :User(id,identicalCard,password,fullName,dateofbirth,gender,email,phoneNumber,address, DOCTOR),
    specialization(specialization), doctorRole(doctorRole), clinic(clinic) {}

// Destructor
Doctor::~Doctor() {}

// Getters
string Doctor::getSpecialization() const {
    return specialization;
}
string Doctor::getDoctorRole() const{
    return doctorRole;
}
string Doctor::getClinic() const{
    return clinic;
}

// Setters
void Doctor::setSpecialization(string specialization) {
    this->specialization = specialization;
}
void Doctor::setDoctorRole(string doctorRole){
    this->doctorRole = doctorRole;
}
void Doctor::setClinic(string clinic){
    this->clinic = clinic;
}
// Hiển thị thông tin
void Doctor::displayInfo() const {
    cout << "\t\t\t\t\t==================================" << endl;
    User::displayInfo();
    cout << "\t\t\t\t\tSpecialization:" << (specialization.empty() ? "[Not updated]" :specialization) << endl;
    cout << "\t\t\t\t\tRole:" << (doctorRole.empty()?"[Not updated]" :doctorRole) << endl;
    cout << "\t\t\t\t\tClinic:" << (clinic.empty()?"[Not updated]" :clinic) << endl;
    cout << "\t\t\t\t\t==================================" << endl;
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
    
    // Cập nhật Clinic
    cout << "Clinic:";
    string newClinic;
    getline(cin, newClinic);
    if (!newClinic.empty()) {
        this->clinic = newClinic;
    }
    return true;
}

ostream& operator<<(ostream& o, const Doctor& doctor){
    o << static_cast<const User&>(doctor);
    o << "Specialization:" << doctor.getSpecialization()
        << "\nRole:" << doctor.getDoctorRole()
        << "\nClinic:" << doctor.getClinic() << endl;
    return o;
}

istream& operator>>(istream& in, Doctor& doctor){
    bool isInteract = (&in == &cin);
    
    if (!isInteract) {
        // Đọc từ file - đọc toàn bộ và parse
        string line;
        while(getline(in, line)) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string val = line.substr(pos + 1);
                
                if (key == "ID") doctor.setID(val);
                else if (key == "Identity card") doctor.setIdenticalCard(val);
                else if (key == "Password") doctor.setPassword(val);
                else if (key == "Full name") doctor.setFullName(val);
                else if (key == "Date of birth") doctor.setDateOfBirth(val);
                else if (key == "Gender") doctor.setGender(val);
                else if (key == "Email") doctor.setEmail(val);
                else if (key == "Phone number") doctor.setPhoneNumber(val);
                else if (key == "Address") doctor.setAddress(val);
                else if (key == "Specialization") doctor.setSpecialization(val);
                else if (key == "Role") doctor.setDoctorRole(val);
                else if (key == "Clinic") doctor.setClinic(val);
            }
        }
    } else {
        in >> static_cast<User&>(doctor);
        if (in.peek() == '\n') in.ignore();
        // Nhập từ bàn phím
        cout << "Specialization:";
        string spe;
        getline(in,spe);
        doctor.setSpecialization(spe);

        cout << "Role:";
        string role;
        getline(in,role);
        doctor.setDoctorRole(role);
        
        cout << "Clinic:";
        string clinic;
        getline(in,clinic);
        doctor.setClinic(clinic);
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
    vector<int> widths = {22,23,15,18,14,21};
    vector<vector<string>> rows;

    rows.push_back({"Appointment ID", "Patient ID","Clinic room", "Date", "Time", "Reason"});
    for (string listID : appointments){
        DataStore::AppointmentDetails d = DataStore::readAppointment(listID);
        if (d.bookStatus == "Booked" && d.visitStatus != "Done")
            rows.push_back({d.appointmentId, d.patientId, d.clinic, d.date, d.time, d.reason});
    }
    drawTable(5,8,widths,rows);
    cout << "\nTotal appointments: " << rows.size() - 1 << endl;
    return true;
}

// decline appointments from patients
bool Doctor::declineAppointment(){
    // Hiển thị danh sách lịch hẹn
    vector<string> appointments = DataStore::getDoctorAppointments(this->id);
    Doctor::viewAppointment();
    if (appointments.empty()) {
        cout << "\nYou don't have appointment to decline" << endl;
        return false;
    }
    // Nhập mã lịch hẹn cần từ chối
    cout << "\nEnter the appointment code to decline (or 0 to cancel) ";
    string appointmentId;

    getline(cin, appointmentId);
    
    if (appointmentId == "0") {
        cout << "Operation has been cancelled" << endl;
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
        SetColor(2);
        cout << "DECLINED APPOINTMENT SUCCESSFULLY!" << endl;
        SetColor(7);
        cout << "Declined appointment ID: " << appointmentId << endl;
        return true;
    }
    
    cout << "Error: Cannot decline this appointment!" << endl;
    return false;
}

bool Doctor::updateAppointmentStatus(){
    vector<string> appointments = DataStore::getDoctorAppointments(this->id);
    Doctor::viewAppointment();
    if (appointments.empty()){
        cout << "\nYou don't have appointment" << endl;
        return false;
    }
    cout << "Enter the appointment code to update visist status (or 0 to cancel): ";
    string appointmentId;
    getline(cin,appointmentId);

    if (appointmentId == "0"){
        cout << "Operation has been cancelled" << endl;
        return false;
    }

    if (!DataStore::appointmentExists(appointmentId)){
        cout << "Not found appointment!" << endl;
        return false;
    }

    DataStore::AppointmentDetails details = DataStore::readAppointment(appointmentId);
    if (details.doctorId != this->id){
        cout << "Error: This appointment does not belong to you!" << endl;
        return false;
    }

    if (details.bookStatus != "Booked"){
        cout << "Error: You can't update the visit status of this appointment!" << endl;
        return false;
    }
    else DataStore::updateVisitAppointmentStatus(appointmentId,"Done");
    return false;
}