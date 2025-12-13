#include <windows.h>
#ifdef byte
#undef byte
#endif
#include <sstream>
#include <regex>
#include <limits>
#include <cstddef>
#include "User.h"
#include "UI.h"

/*--------------------------------------------------------------
                    CONSTRUCTOR - DESTRUCTOR
---------------------------------------------------------------*/
User::User() :id(""), identityCard(""), password(""), email(""), fullName(""), userType(PATIENT) {}

User::User(string id, string identityCard,string password, UserType type)
    :id(id),identityCard(identityCard), password(password), fullName(""), userType(type) {}

User::User(string id, string identityCard, string password, string fullname, string dateofbirth, string gender, string email, string phoneNumber, string address, UserType type)
    :id(id),identityCard(identityCard),  password(password), fullName(fullname), dateOfBirth(dateofbirth), gender(gender), email(email), phoneNumber(phoneNumber),address(address), userType(type) {}

User::~User() {}

/*--------------------------------------------------------------
                        CÁC HÀM GET - SET
---------------------------------------------------------------*/
string User::getID() const {
    return id;
}

string User::getPassword() const {
    return password;
}

string User::getIdentityCard() const{
    return identityCard;
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

UserType User::getUserType() const {
    return userType;
}

string User::getUserTypeString() const {
    switch (userType) {
        case DOCTOR:return "Doctor";
        case PATIENT:return "Patient";
        default:return "Undefined";
    }
}

void User::setID(string id) {
    this->id = id;
}

void User::setPassword(string password) {
    this->password = password;
}

void User::setIdentityCard(string Identitycard){
    this->identityCard = Identitycard;
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

/*--------------------------------------------------------------
                    CẬP NHẬT THÔNG TIN CÁ NHÂN
---------------------------------------------------------------*/
bool User::updateProfile(User& user){
    cin >> user;
    return true;
}

bool User::isProfileComplete() const {
    return !fullName.empty();
}
void User::displayInfo() const {
    cout << "\t\t\t\t\tID:" << id << endl;
    cout << "\t\t\t\t\tIdentity card:" << identityCard << endl;
    cout << "\t\t\t\t\tPassword:" << password << endl;
    cout << "\t\t\t\t\tFull name:" << (fullName.empty() ? "[Not updated]" :fullName) << endl;
    cout << "\t\t\t\t\tEmail:" << (email.empty()?"[Not updated]" : email) << endl;
    cout << "\t\t\t\t\tDate of birth:" << (dateOfBirth.empty() ? "[Not updated]" : dateOfBirth) << endl;
    cout << "\t\t\t\t\tGender:" << (gender.empty() ? "[Not updated]" : gender) << endl;
    cout << "\t\t\t\t\tAddress:" << (address.empty() ? "[Not updated]" : address) << endl;
}

/*--------------------------------------------------------------
                    ĐỔI MẬT KHẨU TÀI KHOẢN
---------------------------------------------------------------*/
bool User::changePassword() {
    string oldPassword = User::getPassword();
    string oldPass, newPass, confirmNewPass;
    const int boxX = 55, boxY = 12, boxW = 40, boxH = 6;
    int pos = 0;
    drawBox(boxX,boxY,boxW,boxH);
    while(true){
        for (int i = 0; i < 3; i++){
            gotoXY(boxX + 2, boxY + 1 + i*2);
            cout << string(boxW - 4, ' ');
        }
        gotoXY(boxX + 2, boxY + 1);
        cout << "Old password: " << string(oldPass.length(), '*');
        gotoXY(boxX + 2, boxY + 3);
        cout << "New password: " << string(newPass.length(), '*');
        gotoXY(boxX + 2, boxY + 5);
        cout << "Confirm new password: " << string(confirmNewPass.length(), '*');
        if (pos == 0)
            gotoXY(boxX + 16 + oldPass.length(), boxY + 1);
        else if (pos == 1)
            gotoXY(boxX + 16 + newPass.length(),boxY + 3);
        else gotoXY(boxX + 24 + confirmNewPass.length(), boxY + 5);
        char key = _getch();
        if (key == 72 && pos > 0) pos--;
        else if (key == 80 && pos < 2) pos++;
        else if (key == 13){
            if (pos == 2 && !oldPass.empty() && !newPass.empty() && !confirmNewPass.empty()){
                if (newPass == confirmNewPass){
                    if (oldPass == oldPassword) {
                        this->setPassword(newPass);
                        return true;
                    }
                    else{
                        gotoXY(boxX + 2, boxY + 8);
                        SetColor(12);
                        cout << "\nWrong old password";
                        SetColor(7);
                        _getch();
                        gotoXY(boxX + 2, boxY + 8);
                        cout << string(boxW - 4, ' ');
                        oldPass = "";
                        pos = 0;
                    }
                }
                else{
                    gotoXY(boxX + 2, boxY + 8);
                    SetColor(12);
                    cout << "New passwords do not match! Please re-enter";
                    SetColor(7);
                    _getch();
                    gotoXY(boxX + 2, boxY + 8);
                    cout << string(boxW - 4, ' ');
                    newPass = confirmNewPass = "";
                    pos = 2;
                }
            }
        }
        else if (key == 8){
            if (pos == 0 && !oldPass.empty()) oldPass.pop_back();
            else if (pos == 1 && !newPass.empty()) newPass.pop_back();
            else if (pos == 2 && !confirmNewPass.empty()) confirmNewPass.pop_back();
        }
        else if (isprint(key)){
            if (pos == 0) oldPass += key;
            else if (pos == 1) newPass += key;
            else if (pos == 2) confirmNewPass += key;
        }
    }
    return true;
}

/*--------------------------------------------------------------
                    ĐA NĂNG HÓA TOÁN TỬ NHẬP - XUẤT
---------------------------------------------------------------*/
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
                else if (key == "Identity card") user.setIdentityCard(val);
                else if (key == "Password") user.setPassword(val);
                else if (key == "Full name") user.setFullName(val);
                else if (key == "Date of birth") user.setDateOfBirth(val);
                else if (key == "Gender") user.setGender(val);
                else if (key == "Email") user.setEmail(val);
                else if (key == "Phone number") user.setPhoneNumber(val);
                else if (key == "Address") user.setAddress(val);
                else break;
            }  
        }
        return in;
    }
    if (in.peek() == '\n') in.ignore();
    string temp_Name;
    SetColor(9);
    cout << "\t\t\t\t\tFull name:"; 
    SetColor(7);
    getline(in,temp_Name);
    if (!temp_Name.empty()) {
        user.setFullName(temp_Name);
    }
    if (isInteract){
        string temp_DOB;
        bool check = false;
        while (!check){
            SetColor(9);
            cout << "\t\t\t\t\tDate of birth (dd/mm/yyyy):"; 
            SetColor(7);
            getline(in,temp_DOB);
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
            if (!check) cout << "\t\t\t\t\tDate of birth is invalid! Please re-enter" << endl;
        }
        if (!temp_DOB.empty()) {
            user.setDateOfBirth(temp_DOB);
        }
    }
    if (isInteract){
        string temp_g;
        bool check_g = false;
        while (!check_g){
            SetColor(9);
            cout << "\t\t\t\t\tGender (Male/Female):"; 
            SetColor(7);
            getline(in,temp_g);
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
            if (!check_g) cout << "\t\t\t\t\tPlease re-enter according to the instructions" << endl;
        }
        if (!temp_g.empty()) {
            user.setGender(temp_g);
        }
    }
    if (isInteract){
        string temp_e;
        bool check_e = false;
        while(!check_e){
            SetColor(9);
            cout << "\t\t\t\t\tEmail(No need to enter '@gmail.com'):";
            SetColor(7);
            getline(in,temp_e);
            if (temp_e.empty()) {
                check_e = true;
                break;
            }
            regex local_part_e("^(?!.*\\.\\.)([A-Za-z0-9](?:[A-Za-z0-9._-]*[A-Za-z0-9])?)$");
            if (regex_match(temp_e, local_part_e)) 
                check_e = true;
            else check_e = false;
            if (!check_e) cout << "\t\t\t\t\tPlease re-enter" << endl;
        }
        if (!temp_e.empty()) {
            user.setEmail(temp_e + "@gmail.com");
        }
    }
    if (isInteract){
        string temp_sdt;
        bool check_sdt = false;
        while (!check_sdt){
            SetColor(9);
            cout << "\t\t\t\t\tPhone number:"; 
            SetColor(7);
            getline(in,temp_sdt);
            if (temp_sdt.empty()) {
                check_sdt = true;
                break;
            }
            regex sdt("^0[0-9]{9}$");
            if (regex_match(temp_sdt,sdt))
                check_sdt = true;
            else check_sdt = false;
            if (!check_sdt) cout << "\t\t\t\t\tPhone number is invalid! Please re-enter the phone number" << endl;
        }
        if (!temp_sdt.empty()) {
            user.setPhoneNumber(temp_sdt);
        }
    }
    string temp_Address;
    SetColor(9);
    cout << "\t\t\t\t\tAddress:"; 
    SetColor(7);
    getline(in,temp_Address);
    if (!temp_Address.empty()) {
        user.setAddress(temp_Address);
    }
    return in;
}

ostream& operator<<(ostream& out, const User& user){
    out << "ID:" << user.getID()
        << "\nIdentity card:" << user.getIdentityCard()
        << "\nPassword:" << user.getPassword()
        << "\nFull name:" << user.getFullName() 
        << "\nDate of birth:" << user.getDateOfBirth()
        << "\nGender:" << user.getGender()
        << "\nEmail:" << user.getEmail()
        << "\nPhone number:" << user.getPhoneNumber()
        << "\nAddress:" << user.getAddress() << endl;
        return out;
}

