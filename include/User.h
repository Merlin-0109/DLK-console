#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

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
    string username;
    string password;
    string identicalCard;

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
    User(string id,string identicalCard, string password,  UserType type);
    User(string id,string identicalCard, string password, string fullname, string dateofbirth, string gender, string email, string phoneNumber, string address , UserType type);
    
    // Destructor
    virtual ~User();
    
    // Getters
    string getID() const;
    string getIdenticalCard() const;
    string getPassword() const;
    string getFullName() const;
    string getDateOfBirth() const;
    string getGender() const;
    string getEmail() const;
    string getPhoneNumber() const;
    string getAddress() const;

    UserType getUserType() const;
    bool isProfileComplete() const;
    
    // Setters
    void setID(string id);
    void setIdenticalCard(string cccd);
    void setPassword(string password);
    void setFullName(string fullName);
    void setDateOfBirth(string dateOfBirth);
    void setGender(string gender);
    void setEmail(string email);
    void setPhoneNumber(string phoneNumber);
    void setAddress(string address);

    void setUserType(UserType type);
    // Update profile
    vbool updateProfile(User& user);
    
    // Virtual methods
    virtual void displayInfo() const;
    virtual string getUserTypeString() const;
    
    friend ostream& operator<<(ostream&, const User&);
    friend istream& operator>>(istream&, User&);
};

#endif 
