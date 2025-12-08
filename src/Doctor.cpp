#include <sstream>
#include <limits>
#include <iomanip>
#include <map>

#include "Doctor.h"
#include "Calendar.h"
#include "DataStore.h"
#include "UI.h"

// Constructor mặc định
Doctor::Doctor() :User(){
    specialization = "";
    doctorRole = "";
    clinic = "";
    userType = DOCTOR;
}

// Constructor đơn giản cho đăng ký
Doctor::Doctor(string id, string identityCard, string password)
    :User(id,identityCard , password, DOCTOR), specialization(""), doctorRole(""), clinic(""){}

// Constructor đầy đủ
Doctor::Doctor(string id, string identityCard, string password,string fullName,string dateofbirth, string gender, string email, string phoneNumber, string address ,string specialization, string doctorRole, string clinic)
    :User(id,identityCard,password,fullName,dateofbirth,gender,email,phoneNumber,address, DOCTOR),
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
    SetColor(9);
    cout << "\t\t\t\t\tSpecialization:";
    SetColor(7);
    string newSpec;
    getline(cin, newSpec);
    if (!newSpec.empty()) {
        this->specialization = newSpec;
    }
    
    // Cập nhật Role
    SetColor(9);
    cout << "\t\t\t\t\tRole:";
    SetColor(7);
    string newRole;
    getline(cin, newRole);
    if (!newRole.empty()) {
        this->doctorRole = newRole;
    }
    
    // Cập nhật Clinic
    SetColor(9);
    cout << "\t\t\t\t\tClinic:";
    SetColor(7);
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
                else if (key == "Identity card") doctor.setIdentityCard(val);
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
           !doctorRole.empty()&&
           !clinic.empty();
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
    drawTable(20,8,widths,rows);
    cout << "\nTotal appointments: " << rows.size() - 1 << endl;
    return true;
}

bool Doctor::remarkAsBusy(){
    DataStore dataStore;

    vector<string> busyDate = dataStore.getBusyDate(this->id);
    vector<string> oneDate;
    map<string,vector<string>> busyTime;
    for (const string& date : busyDate){
        oneDate = dataStore.getBusyTime(this->id,date);
        busyTime[date].insert(busyTime[date].end(), oneDate.begin(),oneDate.end());
    }

    
    Calendar calendar;
    calendar.showCalendar(this->id);

    cout << "Please choose your busy day" << endl;
    vector<string> dayChoice;
    vector<int> indexSlotDay;
    vector<tm> dates;
    time_t now = std::time(nullptr);
    for (int i = 0; i < 7; i++){
        time_t t = now + (i+1)*24*3600;
        tm tm_ptr = *localtime(&t);
        dates.push_back(tm_ptr);
    }

    for (int i = 0; i < 7; i++){
        char buf[6];
        strftime(buf,sizeof(buf),"%d/%m", &dates[i]);
        if (dates[i].tm_wday == 0) continue;

        bool isAllDay = false;
        if (busyTime.find(string(buf)) != busyTime.end()){
            for (const string& time : busyTime[string(buf)]){
                if (time == "AllDay"){
                    isAllDay = true;
                    break;
                }
            }
        }

        if (!isAllDay){
            string showDay = dateWeek[dates[i].tm_wday] + "(" + string(buf) + ")";
            dayChoice.push_back(showDay);
            indexSlotDay.push_back(i);
        }
    }

    int realChoiceDate = -1;
    while(true){
        int choice = runMenuHorizontal(dayChoice.data(), (int)dayChoice.size());
        if (choice >= 1 &&  choice <= (int)dayChoice.size()){
            realChoiceDate = indexSlotDay[choice-1];
            break;
        }
    }
    char buf[6];
    strftime(buf, sizeof(buf), "%d/%m", &dates[realChoiceDate]);
    string chosenDate = string(buf);
    SetColor(2);
    cout << "✔ Busy day: " << chosenDate << endl;
    SetColor(7);

    cout << "\nPlease choose your busy time" << endl;
    vector<string> timeChoice;
    vector<int> indexSlotTime;
    
    bool isBusyTime[7] = {false};
    for (const string& time : busyTime[chosenDate]){
        if (time == "07:00")
            isBusyTime[0] = true;
        if (time == "08:00")
            isBusyTime[1] = true;
        if (time == "09:00")
            isBusyTime[2] = true;
        if (time == "10:00")
            isBusyTime[3] = true;
        if (time == "13:30")
            isBusyTime[4] = true;
        if (time == "14:30")
            isBusyTime[5] = true;
        if (time == "15:30")
            isBusyTime[6] = true;
    }

    int index = 0;
    for (int i = 0; i < 7; i++){
        if (isBusyTime[i] == false){
            timeChoice.push_back(timeSlot[i]);
            indexSlotTime.push_back(i);
            ++index;
        }
    }
    timeChoice.push_back("AllDay");
    indexSlotTime.push_back(timeChoice.size());

    int realChoiceTime = -1;
    while(true){
        int choice = runMenuHorizontal(timeChoice.data(), (int)timeChoice.size());
        if (choice >= 1 && choice <= (int)timeChoice.size()){
            realChoiceTime = indexSlotTime[choice - 1];
            break;
        }
    }
    string chosenTime;
    if (realChoiceTime == timeChoice.size()){
        chosenTime = "AllDay";
    }
    else chosenTime = timeSlot[realChoiceTime];
    SetColor(2);
    cout << "✔ Busy time: " << chosenTime << endl;
    SetColor(7);

    calendar.saveCalendarToFile(this->id, chosenDate, chosenTime);
    
    return true;
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