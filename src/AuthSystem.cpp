#include "AuthSystem.h"
#include "UI.h"
#include "Doctor.h"
#include "Patient.h"
#include <sstream>
#include <algorithm> 
#include <iostream>
#include <iomanip>

// Constructor
AuthSystem::AuthSystem() : currentUser(nullptr) {
    dataStore = new DataStore();
    
    // Khởi tạo HashTables với kích thước hợp lý
    userByIdenticalCard = new HashTable<string, User*>(1009);  // Số nguyên tố cho phân bổ tốt
    userByID = new HashTable<string, User*>(1009);
    
    loadUsersFromDataStore();
}

// Destructor
AuthSystem::~AuthSystem() {
    for (User* user : users) {
        delete user;
    }
    users.clear();
    
    // Giải phóng HashTables
    delete userByIdenticalCard;
    delete userByID;
    delete dataStore;
}

// Load dữ liệu từ DataStore
void AuthSystem::loadUsersFromDataStore() {
    // Load Patients
    vector<string> patientIDs = dataStore->getAllPatientIDs();
    for (const string& id : patientIDs) {
        string data = dataStore->loadPatientData(id);
        if (!data.empty()) {
            stringstream ss(data);
            Patient* patient = new Patient;
            ss >> *patient;
            users.push_back(patient);
            
            // Thêm vào HashTables để tìm kiếm O(1)
            userByIdenticalCard->insert(patient->getIdenticalCard(), patient);
            userByID->insert(patient->getID(), patient);
        }
    }
    
    // Load Doctors
    vector<string> doctorIDs = dataStore->getAllDoctorIDs();
    for (const string& id : doctorIDs) {
        string data = dataStore->loadDoctorData(id);
        if (!data.empty()) {
            stringstream ss(data); // stringstream tao ra mot luong input ao - luc nay data chua co gi ne
            Doctor* doctor = new Doctor;
            ss >> *doctor;
            users.push_back(doctor);
            
            // Thêm vào HashTables để tìm kiếm O(1)
            userByIdenticalCard->insert(doctor->getIdenticalCard(), doctor);
            userByID->insert(doctor->getID(), doctor);
        }
    }
}

// Tìm user theo username - SỬ DỤNG HASHTABLE O(1)
User* AuthSystem::findUser(string identicalCard) {
    User* result = nullptr;
    if (userByIdenticalCard->find(identicalCard, result)) {
        return result;
    }
    return nullptr;
}

// Tìm user theo ID - SỬ DỤNG HASHTABLE O(1)
User* AuthSystem::findUserByID(string id) {
    User* result = nullptr;
    if (userByID->find(id, result)) {
        return result;
    }
    return nullptr;
}

// Kiểm tra số CCCD đã tồn tại
bool AuthSystem::usernameExists(string identicalCard) {
    return findUser(identicalCard) != nullptr;
}

// Đăng ký Doctor
bool AuthSystem::registerDoctor(string identicalCard, string password) {
    if (usernameExists(identicalCard)) {
        cout << "Error: The identity card number already exists!" << endl;
        return false;
    }

    if (identicalCard.length() != 12){
        cout << "Error: The identity card number must be exactly 12 digits long" << endl;
        return false;
    }

    string id = dataStore->generateDoctorID();
    Doctor* doctor = new Doctor(id, identicalCard, password);
    ostringstream oss;
    oss << *doctor;
    // Lưu vào DataStore
    if (dataStore->saveDoctorData(id,oss.str())) {
        users.push_back(doctor);
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

// Đăng ký Patient
bool AuthSystem::registerPatient(string identicalCard, string password) {
    if (usernameExists(identicalCard)) {
        cout << "Error: The identity card number already exists!" << endl;
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
    // Lưu vào DataStore
    if (dataStore->savePatientData(id,ss.str())) {
        users.push_back(patient);
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

// Đăng nhập
User* AuthSystem::login(string identicalCard, string password) {
    User* user = findUser(identicalCard);
    if (user->getPassword() != password)
        return nullptr; 
    if (!user) return nullptr;
    return user;
}

// Đăng xuất
void AuthSystem::logout() {
    if (currentUser != nullptr) {
        string name = currentUser->getFullName().empty() ? currentUser->getIdenticalCard() : currentUser->getFullName();
        cout << "See you later, " << name << "!" << endl;
        currentUser = nullptr;
    }
}

// Cập nhật thông tin người dùng
bool AuthSystem::updateUserProfile(User* user) {
    if (user == nullptr) return false;
    
    bool updated = false;
    
    // Kiểm tra kiểu người dùng và gọi hàm updateProfile tương ứng
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

// Lưu dữ liệu người dùng
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

// Lấy user hiện tại
User* AuthSystem::getCurrentUser() const {
    return currentUser;
}

// Lấy tất cả users
vector<User*> AuthSystem::getAllUsers() const {
    return users;
}

// Lấy DataStore
DataStore* AuthSystem::getDataStore() const {
    return dataStore;
}

// Hiển thị tất cả users
void AuthSystem::displayAllUsers() const {
    if (users.empty()) {
        cout << "There is no user in the system" << endl;
        return;
    }
    
    ostringstream oss;
    oss << "\n========== ALL USER ==========" << endl;
    cout << oss.str() << flush;
    
    for (const User* user : users) {
        user->displayInfo();
        cout << endl;
    }
}
