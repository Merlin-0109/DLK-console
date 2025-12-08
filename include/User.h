#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#ifdef byte
#undef byte
#endif



using namespace std;

// Enum để định nghĩa loại người dùng
enum UserType {
    DOCTOR,
    PATIENT
};

// Lớp cơ sở User
class User {
protected:
    string id;
    string password;
    string identityCard;

    string fullName;
    string dateOfBirth;
    string gender;
    string email;
    string phoneNumber;
    string address;

    UserType userType;

public:
    // Constructor
    User();
    User(string id,string identityCard, string password,  UserType type);
    User(string id,string identityCard, string password, string fullname, string dateofbirth, string gender, string email, string phoneNumber, string address , UserType type);
    
    // Destructor
    virtual ~User();
    
    // Getters
    string getID() const;
    string getIdentityCard() const;
    string getPassword() const;
    string getFullName() const;
    string getDateOfBirth() const;
    string getGender() const;
    string getEmail() const;
    string getPhoneNumber() const;
    string getAddress() const;

    UserType getUserType() const;
    virtual bool isProfileComplete() const;
    
    // Setters
    void setID(string id);
    void setIdentityCard(string cccd);
    void setPassword(string password);
    void setFullName(string fullName);
    void setDateOfBirth(string dateOfBirth);
    void setGender(string gender);
    void setEmail(string email);
    void setPhoneNumber(string phoneNumber);
    void setAddress(string address);

    void setUserType(UserType type);
    // Update profile
    virtual bool updateProfile(User& user);
    bool changePassword();
    
    // Virtual methods
    virtual void displayInfo() const;
    virtual string getUserTypeString() const;
    
    friend ostream& operator<<(ostream&, const User&);
    friend istream& operator>>(istream&, User&);
};

#endif 
