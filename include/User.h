#ifndef USER_H
#define USER_H
#include <string>
#include <iostream>
#ifdef byte
#undef byte
#endif
using namespace std;

enum UserType {
    DOCTOR,
    PATIENT
};
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
        User();
        User(string id,string identityCard, string password,  UserType type);
        User(string id,string identityCard, string password, string fullname, string dateofbirth, string gender, string email, string phoneNumber, string address , UserType type);
        virtual ~User();

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
        virtual string getUserTypeString() const;
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
        
        virtual bool updateProfile(User& user);
        virtual bool isProfileComplete() const;
        virtual void displayInfo() const;
        bool changePassword();

        friend istream& operator>>(istream&, User&);
        friend ostream& operator<<(ostream&, const User&);
};
#endif 
