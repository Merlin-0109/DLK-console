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
void User::setIdenticalCard(string Identitycard){
    this->identicalCard = Identitycard;
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
        cout << "The old password is incorrect!" << endl;
        return false;
    }
    if (newPassword.empty()) {
        cout << "The new password cannot be empty!" << endl;
        return false;
    }
    password = newPassword;
    cout << "Change password successfully" << endl;
    return true;
}

// Hiển thị thông tin
void User::displayInfo() const {
    cout << "ID:" << id << endl;
    cout << "Identity card:" << identicalCard << endl;
    cout << "Password:" << password << endl;
    cout << "Full name:" << (fullName.empty() ? "[Not updated]" :fullName) << endl;
    cout << "Email:" << (email.empty()?"[Not updated]" : email) << endl;
    cout << "Date of birth:" << (dateOfBirth.empty() ? "[Not updated]" : dateOfBirth) << endl;
    cout << "Gender:" << (gender.empty() ? "[Not updated]" : gender) << endl;
    cout << "Address:" << (address.empty() ? "[Not updated]" "\nPlease update the information completely": address) << endl;
}

bool User::updateProfile(User& user){
    cin >> user;
    return true;
}

// Lấy chuỗi loại người dùng, chuyển mô tả enum thành string cụ thể khi in ra màn hình
string User::getUserTypeString() const {
    switch (userType) {
        case DOCTOR:return "Doctor";
        case PATIENT:return "Patient";
        default:return "Undefined";
    }
}

ostream& operator<<(ostream& out, const User& user){
    out << "ID:" << user.getID()
        << "\nIdentity card:" << user.getIdenticalCard()
        << "\nPassword:" << user.getPassword()
        << "\nFull name:" << user.getFullName() 
        << "\nDate of birth:" << user.getDateOfBirth()
        << "\nGender:" << user.getGender()
        << "\nEmail:" << user.getEmail()
        << "\nPhone number:" << user.getPhoneNumber()
        << "\nAddress:" << user.getAddress() << endl;

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
                else if (key == "Identity card") user.setIdenticalCard(val);
                else if (key == "Password") user.setPassword(val);
                else if (key == "Full name") user.setFullName(val);
                else if (key == "Date of birth") user.setDateOfBirth(val);
                else if (key == "Gender") user.setGender(val);
                else if (key == "Email") user.setEmail(val);
                else if (key == "Phone number") user.setPhoneNumber(val);
                else if (key == "Address") user.setAddress(val);
            }  
        }
        return in;
    }

    if (in.peek() == '\n') in.ignore();
    
    string temp_Name;
    cout << "Full name:"; 
    getline(in,temp_Name);
    if (!temp_Name.empty()) {
        user.setFullName(temp_Name);
    }

    // Đảm bảo sự đúng đắn khi nhập Date of birth
    if (isInteract){
        string temp_DOB;
        bool check = false;
   
        while (!check){
            cout << "Date of birth (dd/mm/yyyy):"; 
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
            if (!check) cout << "Date of birth is invalid! Please re-enter" << endl;
        }
        if (!temp_DOB.empty()) {
            user.setDateOfBirth(temp_DOB);
        }
    }

    // Gender
    if (isInteract){
        string temp_g;
        bool check_g = false;
   
        while (!check_g){
            cout << "Gender (Male/Female):"; 
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
            if (temp1 != "male" && temp1 != "female")
                check_g = false;
            if (temp1 == "male") temp_g = "Male";
            else if (temp1 == "female") temp_g = "Female";
            if (!check_g) cout << "Please re-enter according to the instructions" << endl;
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
            cout << "Email(No need to enter '@gmail.com'):";
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
            if (!check_e) cout << "Please re-enter" << endl;
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
            cout << "Phone number:"; 
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
            
            if (!check_sdt) cout << "Phone number is invalid! Please re-enter the phone number" << endl;
        }
        if (!temp_sdt.empty()) {
            user.setPhoneNumber(temp_sdt);
        }
    }

    // Address
    string temp_Address;
    cout << "Address:"; 
    getline(in,temp_Address);
    if (!temp_Address.empty()) {
        user.setAddress(temp_Address);
    }

    return in;
}
