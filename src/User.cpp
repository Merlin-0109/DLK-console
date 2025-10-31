#include "User.h"
#include <sstream>

// Constructor mặc định
User::User() : id(""), username(""), password(""), email(""), fullName(""), userType(PATIENT) {}

// Constructor đơn giản cho đăng ký
User::User(string id, string username, string password, string email, UserType type)
    : id(id), username(username), password(password), email(email), fullName(""), userType(type) {}

// Constructor đầy đủ
User::User(string id, string username, string password, string email, string fullName, UserType type)
    : id(id), username(username), password(password), email(email), fullName(fullName), userType(type) {}

// Destructor
User::~User() {}

// Getters
string User::getID() const {
    return id;
}

string User::getUsername() const {
    return username;
}

string User::getPassword() const {
    return password;
}

string User::getEmail() const {
    return email;
}

string User::getFullName() const {
    return fullName;
}

bool User::isProfileComplete() const {
    return !fullName.empty();
}

UserType User::getUserType() const {
    return userType;
}

// Setters
void User::setID(string id) {
    this->id = id;
}

void User::setUsername(string username) {
    this->username = username;
}

void User::setPassword(string password) {
    this->password = password;
}

void User::setEmail(string email) {
    this->email = email;
}

void User::setFullName(string fullName) {
    this->fullName = fullName;
}

void User::setUserType(UserType type) {
    this->userType = type;
}

// Hiển thị thông tin
void User::displayInfo() const {
    cout << "==================================" << endl;
    cout << "Mã số: " << id << endl;
    cout << "Tên đăng nhập: " << username << endl;
    cout << "Email: " << email << endl;
    cout << "Họ và tên: " << (fullName.empty() ? "[Chưa cập nhật]" : fullName) << endl;
    cout << "Loại người dùng: " << getUserTypeString() << endl;
    cout << "==================================" << endl;
}

// Lấy chuỗi loại người dùng
string User::getUserTypeString() const {
    switch (userType) {
        case DOCTOR: return "Bác sĩ";
        case PATIENT: return "Bệnh nhân";
        default: return "Không xác định";
    }
}

// Serialize để lưu vào file
string User::serialize() const {
    stringstream ss;
    ss << userType << "|" << id << "|" << username << "|" << password << "|" << email << "|" << fullName;
    return ss.str();
}

// Deserialize từ chuỗi (không dùng nữa - chỉ để tương thích)
User* User::deserialize(const string& data) {
    // Hàm này không nên được gọi trực tiếp
    // Dữ liệu sẽ được load từ DataStore trong AuthSystem
    return nullptr;
}
