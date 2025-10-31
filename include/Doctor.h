#ifndef DOCTOR_H
#define DOCTOR_H

#include "User.h"
#include <vector>

class Doctor : public User {
private:
    string specialization;
    string doctorRole;

public:
    // Constructor
    Doctor();
    Doctor(string id, string username, string password, string email);
    Doctor(string id, string username, string password, string email, string fullName,
           string specialization);
    
    // Destructor
    ~Doctor();
    
    // Getters
    string getSpecialization() const;
    
    // Setters
    void setSpecialization(string specialization);
    
    // Override methods
    void displayInfo() const override;
    string serialize() const override;
    bool updateProfile() override;

    void updateInformation(const string& name, const string& sdt, 
            const string& email, const string& address, const string& dob,
            const string& gender, const string& cccd, const string& specialization);

        friend ostream& operator<<(ostream&, const Doctor&);
        friend istream& operator>>(istream&, Doctor&);
        
        bool viewAppointment();
        bool rejectAppointment(); 

        // Doctor service
        vector<string> getDoctorIDs();
        bool updateAppointmentStatus(const string& appointmentID, const string& newStatus);
};

#endif
