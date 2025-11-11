#ifndef DOCTOR_H
#define DOCTOR_H

#include "User.h"
#include <vector>

class Doctor :public User {
private:
    string specialization;
    string doctorRole;

public:
    // Constructor
    Doctor();
    Doctor(string id, string identicalCard, string password);
    Doctor(string id, string identicalCard, string password,string fullName,string dateofbirth, string gender, string email, string phoneNumber, string address,
           string specialization, string doctorRole);
    
    // Destructor
    ~Doctor();
    
    // Getters
    string getSpecialization() const;
    string getDoctorRole() const;
    
    // Setters
    void setSpecialization(string specialization);
    void setDoctorRole(string doctorRole);
    
    // Override methods
    void displayInfo() const override;
    bool isProfileComplete() const override;

    bool updateProfile(Doctor& doctor);
    friend ostream& operator<<(ostream&, const Doctor&);
    friend istream& operator>>(istream&, Doctor&);
    
    bool viewAppointment();
    bool declineAppointment();
    bool doneAppointment();

    // Doctor service
    vector<string> getDoctorIDs();
    bool updateAppointmentStatus(const string& appointmentID, const string& newStatus);
};

#endif
