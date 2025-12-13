#include <sstream>
#include <limits>
#include <iomanip>
#include <map>

#include "Doctor.h"
#include "Calendar.h"
#include "DataStore.h"
#include "UI.h"

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
Doctor::~Doctor() {}
string Doctor::getSpecialization() const {
    return specialization;
}
string Doctor::getDoctorRole() const{
    return doctorRole;
}
string Doctor::getClinic() const{
    return clinic;
}
void Doctor::setSpecialization(string specialization) {
    this->specialization = specialization;
}
void Doctor::setDoctorRole(string doctorRole){
    this->doctorRole = doctorRole;
}
void Doctor::setClinic(string clinic){
    this->clinic = clinic;
}
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
    SetColor(9);
    cout << "\t\t\t\t\tRole:";
    SetColor(7);
    string newRole;
    getline(cin, newRole);
    if (!newRole.empty()) {
        this->doctorRole = newRole;
    }
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
    // Get current time for date calculations
    time_t now = std::time(nullptr);
    
    // Get current busy days count for this month only
    int busyDaysCount = getBusyDaysInCurrentMonth();
    
    // Check if reached the limit of 3 busy days per month
    if (busyDaysCount >= 3) {
        SetColor(12);
        cout << "\n✘ You have reached the maximum limit of 3 busy days per month!" << endl;
        cout << "Busy days used this month: " << busyDaysCount << "/3" << endl;
        SetColor(7);
        system("pause");
        return false;
    }
    
    DataStore dataStore;
    vector<string> busyDate = dataStore.getBusyDate(this->id);
    
    Calendar calendar;
    calendar.showCalendar(this->id);

    cout << "\nBusy days used this month: " << busyDaysCount << "/3" << endl;
    cout << "Please choose your busy day" << endl;
    
    vector<string> dayChoice;
    vector<int> indexSlotDay;
    vector<tm> dates;
    
    for (int i = 0; i < 7; i++){
        time_t t = now + (i+1)*24*3600;
        tm tm_ptr = *localtime(&t);
        dates.push_back(tm_ptr);
    }

    for (int i = 0; i < 7; i++){
        char buf[6];
        strftime(buf,sizeof(buf),"%d/%m", &dates[i]);
        if (dates[i].tm_wday == 0) continue; // Bỏ qua Chủ nhật

        // Kiểm tra ngày này đã được đánh dấu bận chưa
        bool isAlreadyBusy = false;
        for (const string& date : busyDate){
            if (date == string(buf)){
                isAlreadyBusy = true;
                break;
            }
        }

        if (!isAlreadyBusy){
            string showDay = dateWeek[dates[i].tm_wday] + "(" + string(buf) + ")";
            dayChoice.push_back(showDay);
            indexSlotDay.push_back(i);
        }
    }

    if (dayChoice.empty()){
        SetColor(12);
        cout << "\n✘ All available days in the next 7 days are already marked as busy!" << endl;
        SetColor(7);
        system("pause");
        return false;
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
    SetColor(12);
    cout << "✔ Busy day: " << chosenDate << endl;
    SetColor(7);

    calendar.saveCalendarToFile(this->id, chosenDate);

    // Recalculate busy days count after adding new busy day
    busyDaysCount = getBusyDaysInCurrentMonth();
    
    SetColor(2);
    cout << "\n✔ Successfully marked " << chosenDate << " as busy day!" << endl;
    cout << "Remaining busy days this month: " << (3 - busyDaysCount) << "/3" << endl;
    SetColor(7);
    
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

// Helper method to count busy days in current month only
int Doctor::getBusyDaysInCurrentMonth() const {
    DataStore dataStore;
    vector<string> busyDates = dataStore.getBusyDate(this->id);
    
    // Get current month and year
    time_t now = std::time(nullptr);
    tm* currentTime = localtime(&now);
    int currentMonth = currentTime->tm_mon + 1; // tm_mon is 0-11
    int currentYear = currentTime->tm_year + 1900;
    
    int count = 0;
    for (const string& dateStr : busyDates) {
        // Parse date format "dd/mm"
        size_t slashPos = dateStr.find('/');
        if (slashPos != string::npos && slashPos > 0) {
            try {
                int day = stoi(dateStr.substr(0, slashPos));
                int month = stoi(dateStr.substr(slashPos + 1));
                
                // Only count if it's in current month
                if (month == currentMonth) {
                    count++;
                }
            } catch (...) {
                // Skip invalid date formats
                continue;
            }
        }
    }
    
    return count;
}
