#ifndef DOCTOR_H
#define DOCTOR_H
#include <vector>
#include "User.h"
class Doctor :public User {
private:
    string specialization;
    string doctorRole;
    string clinic;
    int getBusyDaysInCurrentMonth() const;
public:
    Doctor();
    Doctor(string id, string identityCard, string password);
    Doctor(string id, string identityCard, string password,string fullName,string dateofbirth, string gender, string email, string phoneNumber, string address,
           string specialization, string doctorRole, string clinic = "");
    ~Doctor();
    string getSpecialization() const;
    string getDoctorRole() const;
    string getClinic() const;
    void setSpecialization(string specialization);
    void setDoctorRole(string doctorRole);
    void setClinic(string clinic);
    void displayInfo() const override;
    bool isProfileComplete() const override;
    bool updateProfile(Doctor& doctor);
    friend ostream& operator<<(ostream&, const Doctor&);
    friend istream& operator>>(istream&, Doctor&);
    bool viewAppointment();
    bool remarkAsBusy();
    bool updateAppointmentStatus();
};
#endif
