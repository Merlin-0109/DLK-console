#ifndef PATIENT_H
#define PATIENT_H

#include <iomanip>
#include <ctime>
#include <vector>
#include <set>
#include <limits>
#include <map>

#include "User.h"
#include "DataStore.h"
class Patient :public User {
public:
    Patient();
    Patient(string id, string identityCard, string password);
    Patient(string id, string identityCard, string password,string fullName,string dateofbirth, string gender, string email, string phoneNumber, string address);
    
    // Destructor
    ~Patient();
    friend ostream& operator<<(ostream&, const Patient&);
    friend istream& operator>>(istream&, Patient&);
    void displayInfo() const override;
    bool isProfileComplete() const override;
    bool bookAppointment(const string& doctorId, const string& date, const string& time, const string& reason);
    bool cancelAppointment(const string& appointmentId);
    bool viewAppointmentHistory() const;
    bool viewUpcomingAppointments() const;
    void displayAppointmentDetails(const DataStore::AppointmentDetails& details) const;
    int countActiveAppointments() const;
    vector<string> getDoctorInfo(const string& doctorId) const;
};
#endif
