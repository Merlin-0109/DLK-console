#ifndef AUTHSYSTEM_H
#define AUTHSYSTEM_H

#include "User.h"
#include "Doctor.h"
#include "Patient.h"
#include "DataStore.h"
#include "HashTable.h"
#include <vector>
#include <fstream>

class AuthSystem{
private:
    vector<User*> users;
    DataStore* dataStore;
    User* currentUser;
    
    // HashTables for O(1) lookup
    HashTable<string, User*>* userByIdenticalCard;  // Tìm theo CCCD - O(1)
    HashTable<string, User*>* userByID;             // Tìm theo ID - O(1)

    // Private helper methods
    void loadUsersFromDataStore();
    User* findUser(string username);
    User* findUserByID(string id);
    
public:
    // Constructor & Destructor
    AuthSystem();
    ~AuthSystem();
    
    bool usernameExists(string username);

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
    DataStore* getDataStore() const;
};

#endif
