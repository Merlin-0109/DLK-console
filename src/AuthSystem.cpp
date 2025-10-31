#include "AuthSystem.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

// Constructor
AuthSystem::AuthSystem() : currentUser(nullptr) {
    dataStore = new DataStore();
    loadUsersFromDataStore();
}

// Destructor
AuthSystem::~AuthSystem() {
    for (User* user : users) {
        delete user;
    }
    users.clear();
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
            string type, userId, username, password, email, fullName, dob, phone, address;
            
            getline(ss, type);       // userType
            getline(ss, userId);     // id
            getline(ss, username);   // username
            getline(ss, password);   // password
            getline(ss, email);      // email
            getline(ss, fullName);   // fullName
            getline(ss, dob);        // dateOfBirth
            getline(ss, phone);      // phoneNumber
            getline(ss, address);    // address
            
            Patient* patient = new Patient(userId, username, password, email, fullName, dob, phone, address);
            users.push_back(patient);
        }
    }
    
    // Load Doctors
    vector<string> doctorIDs = dataStore->getAllDoctorIDs();
    for (const string& id : doctorIDs) {
        string data = dataStore->loadDoctorData(id);
        if (!data.empty()) {
            stringstream ss(data);
            string type, userId, username, password, email, fullName, specialization;
            
            getline(ss, type);              // userType
            getline(ss, userId);            // id
            getline(ss, username);          // username
            getline(ss, password);          // password
            getline(ss, email);             // email
            getline(ss, fullName);          // fullName
            getline(ss, specialization);    // specialization
            
            Doctor* doctor = new Doctor(userId, username, password, email, fullName, specialization);
            users.push_back(doctor);
        }
    }
}

// Tìm user theo username
User* AuthSystem::findUser(string username) {
    for (User* user : users) {
        if (user->getUsername() == username) {
            return user;
        }
    }
    return nullptr;
}

// Tìm user theo ID
User* AuthSystem::findUserByID(string id) {
    for (User* user : users) {
        if (user->getID() == id) {
            return user;
        }
    }
    return nullptr;
}

// Kiểm tra username đã tồn tại
bool AuthSystem::usernameExists(string username) {
    return findUser(username) != nullptr;
}

// Đăng ký Doctor
bool AuthSystem::registerDoctor(string username, string password, string email) {
    if (usernameExists(username)) {
        cout << "Lỗi: Tên đăng nhập đã tồn tại!" << endl;
        return false;
    }
    
    // Generate ID
    string id = dataStore->generateDoctorID();
    
    // Tạo đối tượng Doctor
    Doctor* doctor = new Doctor(id, username, password, email);
    
    // Lưu vào DataStore
    if (dataStore->saveDoctorData(id, doctor->serialize())) {
        users.push_back(doctor);
        cout << "Đăng ký thành công! Mã số của bạn là: " << id << endl;
        cout << "Vui lòng đăng nhập để cập nhật thông tin cá nhân." << endl;
        return true;
    }
    
    delete doctor;
    return false;
}

// Đăng ký Patient
bool AuthSystem::registerPatient(string username, string password, string email) {
    if (usernameExists(username)) {
        cout << "Lỗi: Tên đăng nhập đã tồn tại!" << endl;
        return false;
    }
    
    // Generate ID
    string id = dataStore->generatePatientID();
    
    // Tạo đối tượng Patient
    Patient* patient = new Patient(id, username, password, email);
    
    // Lưu vào DataStore
    if (dataStore->savePatientData(id, patient->serialize())) {
        users.push_back(patient);
        cout << "Đăng ký thành công! Mã số của bạn là: " << id << endl;
        cout << "Vui lòng cập nhật thông tin cá nhân sau khi đăng nhập." << endl;
        return true;
    }
    
    delete patient;
    return false;
}

// Đăng nhập
User* AuthSystem::login(string username, string password) {
    User* user = findUser(username);
    
    if (user == nullptr) {
        cout << "Lỗi: Người dùng không tồn tại!" << endl;
        return nullptr;
    }
    
    if (user->getPassword() != password) {
        cout << "Lỗi: Mật khẩu không chính xác!" << endl;
        return nullptr;
    }
    
    currentUser = user;
    string name = user->getFullName().empty() ? user->getUsername() : user->getFullName();
    cout << "Đăng nhập thành công! Chào mừng, " << name << "!" << endl;
    
    // Kiểm tra profile cho Doctor và Patient
    if (!user->isProfileComplete()) {
        cout << "⚠ Bạn chưa cập nhật đầy đủ thông tin cá nhân. Vui lòng cập nhật!" << endl;
    }
    
    return user;
}

// Đăng xuất
void AuthSystem::logout() {
    if (currentUser != nullptr) {
        string name = currentUser->getFullName().empty() ? currentUser->getUsername() : currentUser->getFullName();
        cout << "Tạm biệt, " << name << "!" << endl;
        currentUser = nullptr;
    }
}

// Cập nhật thông tin người dùng
bool AuthSystem::updateUserProfile(User* user) {
    if (user == nullptr) return false;
    
    if (user->updateProfile()) {
        return saveUserData(user);
    }
    return false;
}

// Lưu dữ liệu người dùng
bool AuthSystem::saveUserData(User* user) {
    if (user == nullptr) return false;
    
    string id = user->getID();
    string data = user->serialize();
    
    switch (user->getUserType()) {
        case DOCTOR:
            return dataStore->saveDoctorData(id, data);
        case PATIENT:
            return dataStore->savePatientData(id, data);
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
        cout << "Không có người dùng nào trong hệ thống." << endl;
        return;
    }
    
    cout << "\n========== TẤT CẢ NGƯỜI DÙNG ==========" << endl;
    for (const User* user : users) {
        user->displayInfo();
        cout << endl;
    }
}
