#ifndef AUTHSYSTEM_H
#define AUTHSYSTEM_H

#include "User.h"
#include "Doctor.h"
#include "Patient.h"
#include "DataStore.h"
#include <vector>
#include <fstream>

class AuthSystem{
private:
    vector<User*> users;
    DataStore* dataStore;
    User* currentUser;

    // Private helper methods
    void loadUsersFromDataStore();
    User* findUser(string username);
    User* findUserByID(string id);
    bool usernameExists(string username);

public:
    // Constructor & Destructor
    AuthSystem();
    ~AuthSystem();
    
    // Authentication methods
    bool registerDoctor(string username, string password);
    bool registerPatient(string username, string password);
    
    User* login(string username, string password);
    void logout();
    
    // Update user data
    bool updateUserProfile(User* user);
    bool saveUserData(User* user);
    
    // Getters
    User* getCurrentUser() const;
    vector<User*> getAllUsers() const;
    DataStore* getDataStore() const;
    
    // Display methods
    void displayAllUsers() const;
};

#endif
