#include "User.h"
#include <sstream>
#include <regex>
#include <limits>

User::User() :id(""), username(""), password(""), email(""), fullName(""), userType(PATIENT) {}

// construct cho đăng ký
User::User(string id, string identicalCard,string password, UserType type)
    :id(id),identicalCard(identicalCard), password(password), fullName(""), userType(type) {}

// constructor đầy đủ
User::User(string id, string identicalCard, string password, string fullname, string dateofbirth, string gender, string email, string phoneNumber, string address, UserType type)
    :id(id),identicalCard(identicalCard),  password(password), fullName(fullname), dateOfBirth(dateofbirth), gender(gender), email(email), phoneNumber(phoneNumber),address(address), userType(type) {}

// Destructor
User::~User() {}

// Getters
string User::getID() const {
    return id;
}
string User::getPassword() const {
    return password;
}
string User::getIdenticalCard() const{
    return identicalCard;
}
string User::getFullName() const {
    return fullName;
}
string User::getDateOfBirth() const {
    return dateOfBirth;
}
string User::getGender() const{
    return gender;
}
string User::getEmail() const {
    return email;
}

string User::getPhoneNumber() const {
    return phoneNumber;
}
string User::getAddress() const {
    return address;
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
void User::setPassword(string password) {
    this->password = password;
}
void User::setIdenticalCard(string cccd){
    this->identicalCard = cccd;
}
void User::setFullName(string fullName) {
    this->fullName = fullName;
}
void User::setDateOfBirth(string dateOfBirth) {
    this->dateOfBirth = dateOfBirth;
}
void User::setGender(string gender){
    this->gender = gender;
}
void User::setEmail(string email) {
    this->email = email;
}
void User::setPhoneNumber(string phoneNumber) {
    this->phoneNumber = phoneNumber;
}
void User::setAddress(string address) {
    this->address = address;
}
void User::setUserType(UserType type) {
    this->userType = type;
}

bool User::changePassword(const string& oldPassword, const string& newPassword) {
    if (password != oldPassword) {
        cout << "Mật khẩu cũ không đúng!" << endl;
        return false;
    }
    if (newPassword.empty()) {
        cout << "Mật khẩu mới không được để trống!" << endl;
        return false;
    }
    password = newPassword;
    cout << "Đổi mật khẩu thành công!" << endl;
    return true;
}

// Hiển thị thông tin
void User::displayInfo() const {
    cout << "ID:" << id << endl;
    cout << "CCCD:" << identicalCard << endl;
    cout << "Mật khẩu:" << password << endl;
    cout << "Họ và tên:" << (fullName.empty() ? "[Chưa cập nhật]" :fullName) << endl;
    cout << "Email:" << (email.empty()?"[Chưa cập nhật]" : email) << endl;
    cout << "Ngày sinh:" << (dateOfBirth.empty() ? "[Chưa cập nhật]" : dateOfBirth) << endl;
    cout << "Giới tính:" << (gender.empty() ? "[Chưa cập nhật]" : gender) << endl;
    cout << "Địa chỉ:" << (address.empty() ? "[Chưa cập nhật]" "\nVui lòng cập nhật đầy đủ thông tin": address) << endl;
}

bool User::updateProfile(User& user){
    cin >> user;
    return true;
}

// Lấy chuỗi loại người dùng, chuyển mô tả enum thành string cụ thể khi in ra màn hình
string User::getUserTypeString() const {
    switch (userType) {
        case DOCTOR:return "Bác sĩ";
        case PATIENT:return "Bệnh nhân";
        default:return "Không xác định";
    }
}

ostream& operator<<(ostream& out, const User& user){
    out << "ID:" << user.getID()
        << "\nCCCD:" << user.getIdenticalCard()
        << "\nMật khẩu:" << user.getPassword()
        << "\nHọ và tên:" << user.getFullName() 
        << "\nNgày sinh:" << user.getDateOfBirth()
        << "\nGiới tính:" << user.getGender()
        << "\nEmail:" << user.getEmail()
        << "\nSố điện thoại:" << user.getPhoneNumber()
        << "\nĐịa chỉ:" << user.getAddress() << endl;

        return out;
}

istream& operator>>(istream& in, User& user){
    bool isInteract = (&in == &cin);
    if (!isInteract){
        string line;
        while(getline(in,line)){
            size_t pos = line.find(":");
            if (pos != string::npos){
                string key = line.substr(0,pos);
                string val = line.substr(pos+1);

                if (key == "ID") user.setID(val);
                else if (key == "CCCD") user.setIdenticalCard(val);
                else if (key == "Mật khẩu") user.setPassword(val);
            }  
        }
        return in;
    }

    if (in.peek() == '\n') in.ignore();
    
    string temp_Name;
    cout << "Họ và tên:"; 
    getline(in,temp_Name);
    if (!temp_Name.empty()) {
        user.setFullName(temp_Name);
    }

    // Đảm bảo sự đúng đắn khi nhập ngày sinh
    if (isInteract){
        string temp_DOB;
        bool check = false;
   
        while (!check){
            cout << "Ngày sinh (dd/mm/yyyy):"; 
            getline(in,temp_DOB);
            
            // Nếu nhấn Enter, giữ nguyên giá trị cũ
            if (temp_DOB.empty()) {
                check = true;
                break;
            }

            regex dob("^(0[1-9]|1[0-9]|2[0-9]|3[0-1])/(0[1-9]|1[0-2])/(19[0-9]{2}|20(0[0-9]|1[0-9]|2[0-5]))$");
            if (regex_match(temp_DOB,dob)){ // đoạn ni kiểm tra định dạng 
                int dd = stoi(temp_DOB.substr(0,2));
                int mm = stoi(temp_DOB.substr(3,2));
                if (mm == 2 && dd > 29) // kiểm tra tháng 2
                    check = false;
                else check = true;
            }
            else check = false;
            if (!check) cout << "Ngày sinh không hợp lệ! Vui lòng nhập lại" << endl;
        }
        if (!temp_DOB.empty()) {
            user.setDateOfBirth(temp_DOB);
        }
    }

    // giới tính
    if (isInteract){
        string temp_g;
        bool check_g = false;
   
        while (!check_g){
            cout << "Giới tính (Nam/Nữ):"; 
            getline(in,temp_g);
            
            // Nếu nhấn Enter, giữ nguyên giá trị cũ
            if (temp_g.empty()) {
                check_g = true;
                break;
            }

            check_g = true;
            string temp1 = temp_g;
            for (char &c :temp1){
                c = tolower(c);
            }
            if (temp1 != "nam" && temp1 != "nữ")
                check_g = false;
            if (temp1 == "nam") temp_g = "Nam";
            else if (temp1 == "nữ") temp_g = "Nữ";
            if (!check_g) cout << "Vui lòng nhập lại theo hướng dẫn" << endl;
        }
        if (!temp_g.empty()) {
            user.setGender(temp_g);
        }
    }
    
    // Email
    if (isInteract){
        string temp_e;
        bool check_e = false;
    
        while(!check_e){
            cout << "Email(Không cần nhập '@gmail.com'):";
            getline(in,temp_e);
            
            // Nếu nhấn Enter, giữ nguyên giá trị cũ
            if (temp_e.empty()) {
                check_e = true;
                break;
            }
            
            regex local_part_e("^(?!.*\\.\\.)([A-Za-z0-9](?:[A-Za-z0-9._-]*[A-Za-z0-9])?)$");
            if (regex_match(temp_e, local_part_e)) 
                check_e = true;
            else check_e = false;
            if (!check_e) cout << "Vui lòng nhập lại" << endl;
        }
        if (!temp_e.empty()) {
            user.setEmail(temp_e + "@gmail.com");
        }
    }
    

    // sđt
    if (isInteract){
        string temp_sdt;
        bool check_sdt = false;
        while (!check_sdt){
            cout << "Số điện thoại:"; 
            getline(in,temp_sdt);
            
            // Nếu nhấn Enter, giữ nguyên giá trị cũ
            if (temp_sdt.empty()) {
                check_sdt = true;
                break;
            }
            
            regex sdt("^0[0-9]{9}$");
            if (regex_match(temp_sdt,sdt))
                check_sdt = true;
            else check_sdt = false;
            
            if (!check_sdt) cout << "Số điện thoại không hợp lệ! Vui lòng nhập lại số điện thoại" << endl;
        }
        if (!temp_sdt.empty()) {
            user.setPhoneNumber(temp_sdt);
        }
    }

    // địa chỉ
    string temp_Address;
    cout << "Địa chỉ:"; 
    getline(in,temp_Address);
    if (!temp_Address.empty()) {
        user.setAddress(temp_Address);
    }

    return in;
}
