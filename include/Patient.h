#ifndef PATIENT_H
#define PATIENT_H

#include "User.h"
#include "DataStore.h"

class Patient :public User {
public:
    // Constructor
    Patient();
    Patient(string id, string identicalCard, string password);
    Patient(string id, string identicalCard, string password,string fullName,string dateofbirth, string gender, string email, string phoneNumber, string address);
    
    // Destructor
    ~Patient();
    
    friend ostream& operator<<(ostream&, const Patient&);
    friend istream& operator>>(istream&, Patient&);
    
    // Override methods
    void displayInfo() const override;
    bool isProfileComplete() const override;

    // Appointment methods
    bool bookAppointment(const string& doctorId, const string& date, const string& time, const string& reason);
    bool viewMyAppointments() const;
    bool cancelAppointment(const string& appointmentId);
    bool viewAppointmentHistory() const;
    bool viewUpcomingAppointments() const;
    
    // Helper methods
    void displayAppointmentDetails(const DataStore::AppointmentDetails& details) const;
    int countActiveAppointments() const;
    vector<string> getDoctorInfo(const string& doctorId) const;
};

#endif
