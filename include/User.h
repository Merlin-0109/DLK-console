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
    string email;
    string fullName;
    UserType userType;

public:
    // Constructor
    User();
    User(string id, string username, string password, string email, UserType type);
    User(string id, string username, string password, string email, string fullName, UserType type);
    
    // Destructor
    virtual ~User();
    
    // Getters
    string getID() const;
    string getUsername() const;
    string getPassword() const;
    string getEmail() const;
    string getFullName() const;
    UserType getUserType() const;
    bool isProfileComplete() const;
    
    // Setters
    void setID(string id);
    void setUsername(string username);
    void setPassword(string password);
    void setEmail(string email);
    void setFullName(string fullName);
    void setUserType(UserType type);
    
    // Update profile
    virtual bool updateProfile() = 0;
    
    // Virtual methods
    virtual void displayInfo() const;
    virtual string getUserTypeString() const;
    
    // Serialization for file storage
    virtual string serialize() const;
    static User* deserialize(const string& data);
};

#endif
