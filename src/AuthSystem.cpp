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
            Patient* patient = new Patient;
            ss >> *patient;
            users.push_back(patient);
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
        }
    }
}

// Tìm user theo username
User* AuthSystem::findUser(string identicalCard) {
    for (User* user : users) {
        if (user->getIdenticalCard() == identicalCard) {
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

// Kiểm tra số CCCD đã tồn tại
bool AuthSystem::usernameExists(string identicalCard) {
    return findUser(identicalCard) != nullptr;
}

// Đăng ký Doctor
bool AuthSystem::registerDoctor(string identicalCard, string password) {
    if (usernameExists(identicalCard)) {
        cout << "Lỗi: Số căn cước công dân đăng ký đã tồn tại!" << endl;
        return false;
    }

    if (identicalCard.length() != 12){
        cout << "Lỗi: Số căn cước công dân phải đủ 12 chữ số!" << endl;
        return false;
    }

    string id = dataStore->generateDoctorID();
    Doctor* doctor = new Doctor(id, identicalCard, password);
    // cin >> *doctor;
    ostringstream oss;
    oss << *doctor;
    // Lưu vào DataStore
    if (dataStore->saveDoctorData(id,oss.str())) {
        users.push_back(doctor);
        cout << "Đăng ký thành công! ID của bạn là: " << id << endl;
        cout << "Vui lòng đăng nhập để cập nhật thông tin cá nhân." << endl;
        return true;
    }
    delete doctor;
    return false;
}

// Đăng ký Patient
bool AuthSystem::registerPatient(string identicalCard, string password) {
    if (usernameExists(identicalCard)) {
        cout << "Lỗi: CCCD đã tồn tại!" << endl;
        return false;
    }
    
    if (identicalCard.length() != 12){
        cout << "Lỗi: Số căn cước công dân phải đủ 12 chữ số!" << endl;
        return false;
    }

    string id = dataStore->generatePatientID();
    Patient* patient = new Patient(id, identicalCard, password);
    ostringstream ss;
    ss << *patient;
    // Lưu vào DataStore
    if (dataStore->savePatientData(id,ss.str())) {
        users.push_back(patient);
        cout << "Đăng ký thành công! ID của bạn là: " << id << endl;
        cout << "Vui lòng cập nhật thông tin cá nhân sau khi đăng nhập." << endl;
        return true;
    }
    
    delete patient;
    return false;
}

// Đăng nhập
User* AuthSystem::login(string identicalCard, string password) {
    User* user = findUser(identicalCard);
    
    if (user == nullptr) {
        cout << "Lỗi: Người dùng không tồn tại!" << endl;
        return nullptr;
    }
    
    if (user->getPassword() != password) {
        cout << "Lỗi: Mật khẩu không chính xác!" << endl;
        return nullptr;
    }
    
    currentUser = user;
    string name = user->getFullName().empty() ? user->getIdenticalCard() : user->getFullName();
    cout << "Đăng nhập thành công! Chào mừng, " << name << "!" << endl;
    
    // Chỉ kiểm tra profile cho Doctor và Patient, không kiểm tra Admin
    if (!user->isProfileComplete()) {
        cout << "⚠ Bạn chưa cập nhật đầy đủ thông tin cá nhân. Vui lòng cập nhật!" << endl;
    }
    
    return user;
}

// Đăng xuất
void AuthSystem::logout() {
    if (currentUser != nullptr) {
        string name = currentUser->getFullName().empty() ? currentUser->getIdenticalCard() : currentUser->getFullName();
        cout << "Tạm biệt, " << name << "!" << endl;
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
        if (doctor) {
            updated = doctor->updateProfile(*doctor);
        }
    } else if (user->getUserType() == PATIENT) {
        Patient* patient = dynamic_cast<Patient*>(user);
        if (patient) {
            updated = patient->updateProfile(*patient);
        }
    } else {
        updated = user->updateProfile(*user);
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
    string id,data;
    
    switch (user->getUserType()) {
        case DOCTOR:{
            Doctor* doctor = dynamic_cast<Doctor*> (user);
            oss << *doctor;
            id = user->getID();
            data = oss.str();
            return dataStore->saveDoctorData(id,data);
        }
        case PATIENT:
            return dataStore->savePatientData(id,data);
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
