#ifndef PATIENT_H
#define PATIENT_H

#include "User.h"
#include "DataStore.h"
#include <vector>

class Patient : public User {
private:
    string dateOfBirth;
    string phoneNumber;
    string address;

public:
    // Constructor
    Patient();
    Patient(string id, string username, string password, string email);
    Patient(string id, string username, string password, string email, string fullName,
            string dateOfBirth, string phoneNumber, string address);
    
    // Destructor
    ~Patient();
    
    // Getters
    string getDateOfBirth() const;
    string getPhoneNumber() const;
    string getAddress() const;
    
    // Setters
    void setDateOfBirth(string dateOfBirth);
    void setPhoneNumber(string phoneNumber);
    void setAddress(string address);
    
    // Override methods
    void displayInfo() const override;
    string serialize() const override;
    bool updateProfile() override;
    
    // Appointment methods
    bool bookAppointment(const string& doctorId, const string& date, const string& time, const string& reason);
    bool viewMyAppointments() const;
    bool cancelAppointment(const string& appointmentId);
    bool rescheduleAppointment(const string& appointmentId, const string& newDate, const string& newTime);
    bool viewAppointmentHistory() const;
    bool viewUpcomingAppointments() const;
    
    // Helper methods
    bool validateDate(const string& date) const;
    bool validateTime(const string& time) const;
    bool isDateInFuture(const string& date, const string& time) const;
    void displayAppointmentDetails(const DataStore::AppointmentDetails& details) const;
    int countActiveAppointments() const;
};

#endif
