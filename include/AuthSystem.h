#ifndef AUTHSYSTEM_H
#define AUTHSYSTEM_H
#include <vector>
#include <fstream>
#include "User.h"
#include "Doctor.h"
#include "Patient.h"
#include "DataStore.h"
#include "HashTable.h"
class AuthSystem{
private:
    vector<User*> users;
    DataStore* dataStore;
    User* currentUser;
    HashTable<string, User*>* userByIdenticalCard;
    HashTable<string, User*>* userByID;
    void loadUsersFromDataStore();
    User* findUser(string username);
public:
    AuthSystem();
    ~AuthSystem();
    bool usernameExists(string username);
    void registerUser(UserType type);
    bool registerDoctor(string username, string password);
    bool registerPatient(string username, string password);
    User* handleLogin(AuthSystem& authSystem);
    User* login(string username, string password);
    void logout();
    bool updateUserProfile(User* user);
    bool saveUserData(User* user);
    User* getCurrentUser() const;
    DataStore* getDataStore() const;
};
#endif
