#include <sstream>
#include <algorithm> 
#include <iostream>
#include <iomanip>
#include <regex>
#include "AuthSystem.h"
#include "Doctor.h"
#include "Patient.h"
#include "UI.h"
#include "Menu.h"
AuthSystem::AuthSystem() : currentUser(nullptr) {
    dataStore = new DataStore();
    userByIdenticalCard = new HashTable<string, User*>(1009);
    userByID = new HashTable<string, User*>(1009);
    loadUsersFromDataStore();
}
AuthSystem::~AuthSystem() {
    for (User* user : users) {
        delete user;
    }
    users.clear();
    delete userByIdenticalCard;
    delete userByID;
    delete dataStore;
}
void AuthSystem::loadUsersFromDataStore() {
    vector<string> patientIDs = dataStore->getAllPatientIDs();
    for (const string& id : patientIDs) {
        string data = dataStore->loadPatientData(id);
        if (!data.empty()) {
            stringstream ss(data);
            Patient* patient = new Patient;
            ss >> *patient;
            users.push_back(patient);
            userByIdenticalCard->insert(patient->getIdenticalCard(), patient);
            userByID->insert(patient->getID(), patient);
        }
    }
    vector<string> doctorIDs = dataStore->getAllDoctorIDs();
    for (const string& id : doctorIDs) {
        string data = dataStore->loadDoctorData(id);
        if (!data.empty()) {
            stringstream ss(data);
            Doctor* doctor = new Doctor;
            ss >> *doctor;
            users.push_back(doctor);
            userByIdenticalCard->insert(doctor->getIdenticalCard(), doctor);
            userByID->insert(doctor->getID(), doctor);
        }
    }
}
User* AuthSystem::findUser(string identicalCard) {
    User* result = nullptr;
    if (userByIdenticalCard->find(identicalCard, result)) {
        return result;
    }
    return nullptr;
}
bool AuthSystem::usernameExists(string identicalCard) {
    return findUser(identicalCard) != nullptr;
}
void AuthSystem::registerUser(UserType type) {
    const string temp = (type == DOCTOR) ? "DOCTOR" : "PATIENT";
    const int boxX = 55, boxY = 15;
    const int boxW = 40, boxH = 6;
    string username = "", password1 = "", password2 = "";
    int position = 0;
    system("cls");
    showTitle("title.txt");
    SetColor(14);
    cout << "\n\n\n\n\t\t\t\t\t\t\t===== REGISTER " + temp + " ACCOUNT =====\n\n";
    SetColor(7);
    drawBox(boxX, boxY, boxW, boxH);
    while(true) {
        for (int i = 0; i < 3; ++i) {
            gotoXY(boxX + 2, boxY + 1 + i * 2);
            cout << string(boxW - 4, ' ');
        }
        gotoXY(boxX + 2, boxY + 1);
        cout << "Identity card: " << username;
        gotoXY(boxX + 2, boxY + 3);
        cout << "Password: " << string(password1.length(), '*');
        gotoXY(boxX + 2, boxY + 5);
        cout << "Confirm password: " << string(password2.length(), '*');
        if (position == 0)
            gotoXY(boxX + 17 + username.length(), boxY + 1); 
        else if (position == 1)
            gotoXY(boxX + 12 + password1.length(), boxY + 3);
        else gotoXY(boxX + 20 + password2.length(), boxY + 5);
        char key = _getch();
        if (key == 72 && position > 0) position--;
        else if (key == 80 && position < 2) position++;
        else if (key == 13) {
            if (position == 2 && !username.empty() && !password1.empty() && !password2.empty()) {
                if (password1 == password2) {
                    regex cccd("^0[0-9]{11}$");
                    if (regex_match(username, cccd)) {
                        break;
                    } else {
                        gotoXY(boxX + 2, boxY + 8);
                        SetColor(12);
                        cout << "\nInvalid identity card! Must be 12 digits, start with 0";
                        SetColor(7);
                        _getch();
                        gotoXY(boxX + 2, boxY + 8);
                        cout << string(boxW - 4, ' ');
                        username = "";
                        position = 0;
                    }
                } else {
                    gotoXY(boxX + 2, boxY + 8);
                    SetColor(12);
                    cout << "\nPasswords do not match! Please try again.";
                    SetColor(7);
                    _getch();
                    gotoXY(boxX + 2, boxY + 8);
                    cout << string(boxW - 4, ' ');
                    password1 = password2 = "";
                    position = 1;
                }
            }
        }
        else if (key == 8) {
            if (position == 0 && !username.empty()) username.pop_back();
            else if (position == 1 && !password1.empty()) password1.pop_back();
            else if (position == 2 && !password2.empty()) password2.pop_back();
        }
        else if (isprint(key)) {
            if (position == 0 && username.length() < 12) username += key;
            else if (position == 1) password1 += key;
            else if (position == 2) password2 += key;
        }
    }
    (type == DOCTOR) ? registerDoctor(username, password1): registerPatient(username,password1);
}
bool AuthSystem::registerDoctor(string identicalCard, string password) {
    if (usernameExists(identicalCard)) {
        cout << "\n\n\t\t\t\t\t\tError: The identity card number already exists!" << endl;
        return false;
    }
    if (identicalCard.length() != 12){
        cout << "\n\nError: The identity card number must be exactly 12 digits long" << endl;
        return false;
    }
    SetColor(7);
    string id = dataStore->generateDoctorID();
    Doctor* doctor = new Doctor(id, identicalCard, password);
    ostringstream oss;
    oss << *doctor;
    if (dataStore->saveDoctorData(id,oss.str())) {
        users.push_back(doctor);
        userByIdenticalCard->insert(identicalCard, doctor);
        userByID->insert(id, doctor);
        SetColor(2);
        cout << "\n\nRegistration successful! Your ID is:" << id << endl;
        cout << "Please log in to update your personal information" << endl;
        SetColor(7);
        system("pause");
        return true;
    }
    delete doctor;
    return false;
}
bool AuthSystem::registerPatient(string identicalCard, string password) {
    if (usernameExists(identicalCard)) {
        cout << "\n\n\t\t\t\t\tError: The identity card number already exists!" << endl;
        return false;
    }
    if (identicalCard.length() != 12){
        cout << "Error: The identity card number must be exactly 12 digits long" << endl;
        return false;
    }
    string id = dataStore->generatePatientID();
    Patient* patient = new Patient(id, identicalCard, password);
    ostringstream ss;
    ss << *patient;
    if (dataStore->savePatientData(id,ss.str())) {
        users.push_back(patient);
        userByIdenticalCard->insert(identicalCard, patient);
        userByID->insert(id, patient);
        SetColor(2);
        cout << "\n\nRegistration successful! Your ID is " << id << endl;
        cout << "Please log in to update your personal information" << endl;
        SetColor(7);
        system("pause");
        return true;
    }
    delete patient;
    return false;
}
User* AuthSystem::handleLogin(AuthSystem& authSystem) {
    const int boxX = 70, boxY = 15;
    const int boxW = 40, boxH = 4;
    string username = "", password = "";
    int position = 0;
    system("cls");
    showTitle("title.txt");
    drawBox(boxX, boxY, boxW, boxH);
    while (true) {
        gotoXY(boxX + 2, boxY + 1);
        cout << string(boxW - 4, ' ');
        gotoXY(boxX + 2, boxY + 3);
        cout << string(boxW - 4, ' ');
        gotoXY(boxX + 2, boxY + 1);
        cout << "Identity card: " << username;
        gotoXY(boxX + 2, boxY + 3);
        cout << "Password: " << string(password.length(), '*');
        if (position == 0)
            gotoXY(boxX + 17 + username.length(), boxY + 1); 
        else
            gotoXY(boxX + 12 + password.length(), boxY + 3); 
        char key = _getch();
        if (key == 72) position = 0;
        else if (key == 80) position = 1;
        else if (key == 8) {
            if (position == 0 && !username.empty()) username.pop_back();
            if (position == 1 && !password.empty()) password.pop_back();
        }
        else if (isprint(key)) {
            if (position == 0) username += key;
            else password += key;
        } 
        else if (key == 13) {
            if (!username.empty() && !password.empty()){
                User* user = authSystem.login(username,password);
                if (user){
                    system("pause");
                    return user;
                }
            }
        }
    }
}
User* AuthSystem::login(string identicalCard, string password) {
    User* user = findUser(identicalCard);
    if (user == nullptr) {
        cout << "\t\t\t\t\t\t\n\nLogin failed: account not found." << endl;
        return nullptr;
    }
    if (user->getPassword() != password) {
        gotoXY(70,21);
        SetColor(4*16+6);
        cout << "Login failed: incorrect password." << endl;
        SetColor(7);
        return nullptr;
    }
    gotoXY(70,21);
    cout << string(50,' ');
    currentUser = user;
    gotoXY(70,21);
    SetColor(10*16+6);
    cout << "Login successful. Welcome " << (user->getFullName().empty() ? user->getIdenticalCard() : user->getFullName()) << "!" << endl;
    SetColor(7);
    if (!user->isProfileComplete()) {
        cout << "⚠ You have not fully updated your personal information. Please update it!" << endl;
    }
    return user;
}
void AuthSystem::logout() {
    if (currentUser != nullptr) {
        string name = currentUser->getFullName().empty() ? currentUser->getIdenticalCard() : currentUser->getFullName();
        cout << "See you later, " << name << "!" << endl;
        currentUser = nullptr;
    }
}
bool AuthSystem::updateUserProfile(User* user) {
    if (user == nullptr) return false;
    bool updated = false;
    if (user->getUserType() == DOCTOR) {
        Doctor* doctor = dynamic_cast<Doctor*>(user);
        updated = doctor->updateProfile(*doctor);
    } else if (user->getUserType() == PATIENT) {
        Patient* patient = dynamic_cast<Patient*>(user);
        updated = patient->updateProfile(*patient);
    }
    if (updated) {
        return saveUserData(user);
    }
    return false;
}
bool AuthSystem::saveUserData(User* user) {
    if (user == nullptr) return false;
    ostringstream oss;
    string id = user->getID();
    switch (user->getUserType()) {
        case DOCTOR: {
            Doctor* doctor = dynamic_cast<Doctor*>(user);
            if (doctor) {
                oss << *doctor;
                string data = oss.str();
                return dataStore->saveDoctorData(id, data);
            }
            return false;
        }
        case PATIENT: {
            Patient* patient = dynamic_cast<Patient*>(user);
            if (patient) {
                oss << *patient;
                string data = oss.str();
                return dataStore->savePatientData(id, data);
            }
            return false;
        }
        default:
            return false;
    }
}
DataStore* AuthSystem::getDataStore() const {
    return dataStore;
}
