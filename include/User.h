#ifndef USER_H
#define USER_H
#include <string>
#include <iostream>
using namespace std;
enum UserType {
    DOCTOR,
    PATIENT
};
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
    User();
    User(string id,string identicalCard, string password,  UserType type);
    User(string id,string identicalCard, string password, string fullname, string dateofbirth, string gender, string email, string phoneNumber, string address , UserType type);
    virtual ~User();
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
    virtual bool isProfileComplete() const;
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
    virtual bool updateProfile(User& user);
    bool changePassword();
    virtual void displayInfo() const;
    virtual string getUserTypeString() const;
    friend ostream& operator<<(ostream&, const User&);
    friend istream& operator>>(istream&, User&);
};
#endif 
