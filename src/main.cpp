#include <iostream>
#include <string>
#include <limits>
#include <conio.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <cstddef>
#include <windows.h>
#ifdef byte
#undef byte
#endif
#include <sstream>
#include <fstream>
#include <map>
#include "AuthSystem.h"
#include "Doctor.h"
#include "Patient.h"
#include <regex>
#include "UI.H"

using namespace std;

// Khi mới vô hệ thống
string menuMain[] = {
    "Register",
    "Log in",
    "Exit"
};

// chọn loại tài khoản để đăng ký
string menuLogIn_Out[] = {
    "Doctor",
    "Patient",
    "Exit"
};

// chức năng dành cho bác sĩ
string menuDoctor[] = {
    "View appointment",
    "Decline appointment",
    "Update visit status of appointment",
    "View personal information",
    "Update personal information",
    "Change password",
    "Log out"
};

// chức năng dành cho bệnh nhân
string menuPatient[] = {
    "Book appointment",
    "View upcomming appointment",
    "View medical examination history",
    "Cancel the appointment",
    "View personal information",
    "Update personal information",
    "Change password",
    "Log out"
};

void showTitle(string fileName){
    ostringstream oss;
    ifstream file(fileName);
    string line;
    while(getline(file,line)){
        oss << line << endl;
    }
    file.close();
    
    SetColor(14);
    cout << oss.str() << flush;
    SetColor(7);
}

// Hàm xóa buffer input
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Hàm hiển thị menu chính
int displayMainMenu() {
    return runMenu(menuMain,3);
}

// Hàm hiển thị menu đăng ký
int displayRegisterMenu() {
    return runMenu(menuLogIn_Out,3);
}

int displayDoctorChoice(){
    return runMenu(menuDoctor,7);
}
int displayPatientChoice(){
    return runMenu(menuPatient,8);
}

void wayBackMenu(){
    cout << "Please press any key to way back main menu ..." << endl;
    _getch();
}

void registerUser(AuthSystem& authSystem, UserType type) {
    const string temp = (type == DOCTOR) ? "DOCTOR" : "PATIENT";
    const int boxX = 55, boxY = 15;
    const int boxW = 40, boxH = 6;

    string username = "", password1 = "", password2 = "";
    int position = 0; // 0 = CCCD, 1 = pass1, 2 = pass2

    // VẼ PHẦN TĨNH 1 LẦN DUY NHẤT
    system("cls");
    showTitle("title.txt");
    SetColor(14);
    cout << "\n\n\n\n\t\t\t\t\t\t\t===== REGISTER " + temp + " ACCOUNT =====\n\n";
    SetColor(7);
    drawBox(boxX, boxY, boxW, boxH);
    while(true) {
        // CHỈ XÓA VÀ CẬP NHẬT PHẦN ĐỘNG
        // XÓA DÒNG CŨ
        for (int i = 0; i < 3; ++i) {
            gotoXY(boxX + 2, boxY + 1 + i * 2);
            cout << string(boxW - 4, ' ');
        }

        // Viết lại nhãn + dữ liệu
        gotoXY(boxX + 2, boxY + 1);
        cout << "Identity card: " << username;
        gotoXY(boxX + 2, boxY + 3);
        cout << "Password: " << string(password1.length(), '*');
        gotoXY(boxX + 2, boxY + 5);
        cout << "Confirm password: " << string(password2.length(), '*');
        // Đặt con trỏ đúng vị trí
        if (position == 0)
            gotoXY(boxX + 17 + username.length(), boxY + 1); 
        else if (position == 1)
            gotoXY(boxX + 12 + password1.length(), boxY + 3);
        else gotoXY(boxX + 20 + password2.length(), boxY + 5);

        char key = _getch();

        // DI CHUYỂN
        if (key == 72 && position > 0) position--;        // Lên
        else if (key == 80 && position < 2) position++;   // Xuống

        // ENTER
        else if (key == 13) {
            if (position == 2 && !username.empty() && !password1.empty() && !password2.empty()) {
                if (password1 == password2) {
                    // KIỂM TRA CCCD
                    regex cccd("^0[0-9]{11}$");
                    if (regex_match(username, cccd)) {
                        break; // THÀNH CÔNG
                    } else {
                        gotoXY(boxX + 2, boxY + 8);
                        SetColor(12);
                        cout << "\nInvalid identity card! Must be 12 digits, start with 0";
                        SetColor(7);
                        _getch();
                        // XÓA THÔNG BÁO LỖI
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
                    // XÓA THÔNG BÁO LỖI
                    gotoXY(boxX + 2, boxY + 8);
                    cout << string(boxW - 4, ' ');
                    password1 = password2 = "";
                    position = 1;
                }
            }
        }

        // BACKSPACE
        else if (key == 8) {
            if (position == 0 && !username.empty()) username.pop_back();
            else if (position == 1 && !password1.empty()) password1.pop_back();
            else if (position == 2 && !password2.empty()) password2.pop_back();
        }

        // NHẬP KÝ TỰ
        else if (isprint(key)) {
            if (position == 0 && username.length() < 12) username += key;
            else if (position == 1) password1 += key;
            else if (position == 2) password2 += key;
        }
    }
    (type == DOCTOR) ? authSystem.registerDoctor(username, password1): authSystem.registerPatient(username,password1);
}

// Hàm xử lý đăng ký
void handleRegistration(AuthSystem& authSystem) {
    int choice;
        SetColor(2);
        cout << "\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\tWHICH ROLE ARE YOU REGISTERING FOR?" << endl;
        SetColor(7);

        choice = displayRegisterMenu();
        
        switch (choice) {
            case 1:{
                system("cls");
                SetColor(14);
                showTitle("title.txt");
                SetColor(7);
                registerUser(authSystem,DOCTOR);
                break;
            }
            case 2:{
                system("cls");
                SetColor(14);
                showTitle("title.txt");
                SetColor(7);
                registerUser(authSystem,PATIENT);
                break;
            }
        }
}

User* handleLogin(AuthSystem& authSystem) {
    const int boxX = 70, boxY = 15;
    const int boxW = 40, boxH = 4;

    string username = "", password = "";
    int position = 0; // 0 = CCCD, 1 = password

    // VẼ PHẦN TĨNH 1 LẦN DUY NHẤT
    system("cls");
    showTitle("title.txt");
    drawBox(boxX, boxY, boxW, boxH);

    while (true) {
        // CHỈ CẬP NHẬT PHẦN ĐỘNG
        gotoXY(boxX + 2, boxY + 1);
        cout << string(boxW - 4, ' ');  // xóa dòng Identity card
        gotoXY(boxX + 2, boxY + 3);
        cout << string(boxW - 4, ' ');  // xóa dòng Password

        // Viết lại nhãn + dữ liệu
        gotoXY(boxX + 2, boxY + 1);
        cout << "Identity card: " << username;

        gotoXY(boxX + 2, boxY + 3);
        cout << "Password: " << string(password.length(), '*');
        
        // Đặt con trỏ đúng vị trí
        if (position == 0)
            gotoXY(boxX + 17 + username.length(), boxY + 1); 
        else
            gotoXY(boxX + 12 + password.length(), boxY + 3); 

        char key = _getch();
        if (key == 72) position = 0; // mũi tên lên
        else if (key == 80) position = 1; // mũi tên xuống
        else if (key == 8) { // backspace
            if (position == 0 && !username.empty()) username.pop_back();
            if (position == 1 && !password.empty()) password.pop_back();
        }
        else if (isprint(key)) { // chỉ thêm ký tự in được
            if (position == 0) username += key;
            else password += key;
        } 
        else if (key == 13) {
            if (!username.empty() && !password.empty()){
                // enter khi đầy đủ
                User* user = authSystem.login(username,password);
                if (user){
                    system("pause");
                    return user;
                }
            }
        }
    }
}

// Hàm xử lý menu sau khi đăng nhập
void handleUserSession(AuthSystem& authSystem, User* user) {
    int choice;
    bool logout = false;
    
    while (!logout) {     
        if (user->getUserType() == DOCTOR){
            system("cls");
            showTitle("titleDoctor.txt");
            cout << "\nWelcome, Dr.  " << user->getFullName() << "!" << endl;
            Doctor* doctor = dynamic_cast<Doctor*>(user);
            choice = displayDoctorChoice();
            switch (choice){
                case 1:{ // Xem lịch khám
                        system("cls");
                        showTitle("titleDoctor.txt");

                        SetColor(2);
                        cout << "\n\n\n\t\t\t\t\tLIST OF APPOINTMENT" << endl;
                        SetColor(7);

                        doctor->viewAppointment();

                        wayBackMenu();
                        break;
                    }
                case 2: {// Từ chối lịch khám
                    system("cls");
                    showTitle("titleDoctor.txt");

                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\tDECLINE APPOINTMENT" << endl;
                    SetColor(7);
                    
                    doctor->declineAppointment();
                    wayBackMenu();
                    break;
                }
                case 3: { // cap nhat trang thai tham kham
                    system("cls");
                    showTitle("titleDoctor.txt");

                    SetColor(2);
                    cout << "\n\n\n\t\t\t\tUPDATE VISIT STATUS OF APPOINTMENT" << endl;
                    SetColor(7);

                    doctor->updateAppointmentStatus();
                    wayBackMenu();
                    break;
                }
                case 4:{ // Xem thông tin cá nhân
                    system("cls");
                    showTitle("titleDoctor.txt");

                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\tPERSONAL INFORMATION" << endl;
                    SetColor(7);

                    doctor->displayInfo();
                    wayBackMenu();
                    break;
                }
                case 5:{ // Cập nhật thông tin cá nhân
                    system("cls");
                    showTitle("titleDoctor.txt");

                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\tUPDATE PERSONAL INFORMATION" << endl;
                    SetColor(7);

                    if (authSystem.updateUserProfile(doctor)) 
                            cout << "Information saved successfully!" << endl;
                    else
                        cout << "Error saving information!" << endl;
                    wayBackMenu();
                    break;
                }
                case 6:{ // Đổi Password
                    system("cls");
                    showTitle("titleDoctor.txt");
                    string oldPassword, newPassword, confirmPassword;
                    
                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\t\t\t\tCHANGE PASSWORD" << endl;
                    SetColor(7);
                    
                    if (user->changePassword()) {
                        if (authSystem.saveUserData(user)) {
                            cout << "Updated password successfully!" << endl;
                        } else {
                            cout << "Updated password unsuccessfully!" << endl;
                        }
                    }
                    wayBackMenu();
                    break;
                }
                case 7:
                    system("cls");
                    authSystem.logout();
                    logout = true;
                    break;
            }
        }
        else if (user->getUserType() == PATIENT){
            Patient* patient = dynamic_cast<Patient*>(user);
            system("cls");
            showTitle("titlePatient.txt");
            cout << "\nWelcome, " << user->getFullName() << endl;
            
            // Hiển thị số lịch khám đang chờ
            int activeCount = patient->countActiveAppointments();
            if (activeCount > 0) {
                SetColor(4*16+15);
                cout << "\nNotification: You're having " << activeCount << " appointments" << endl;
                SetColor(7);
            }
            
            choice = displayPatientChoice();
            
            switch (choice){
                case 1:{ // Đặt lịch khám mới
                    system("cls");
                    showTitle("titlePatient.txt");

                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\t\t\t📋BOOK APPOINTMENT" << endl;
                    SetColor(7);

                    string doctorId, date, time, reason;
                    
                    // Hiển thị danh sách bác sĩ
                    vector<string> doctorIDs = authSystem.getDataStore()->getAllDoctorIDs();
                    if (doctorIDs.empty()) {
                        cout << "Currently, there are no doctors in the system" << endl;
                        break;
                    }
                    
                    vector<int> widths = {20,30,15,25,15,15};
                    vector<vector<string>> rows;
                    rows.push_back({"Doctor ID", "Full name", "Gender","Specialization","Role", "Clinic room"});
                    for (const string& dId :doctorIDs) {
                        string data = authSystem.getDataStore()->loadDoctorData(dId);
                        if (!data.empty()) {
                            stringstream ss(data);
                            string line, id, fullName, gender, spec, role, clinic;
                            while(getline(ss,line)){
                                size_t pos = line.find(":");
                                string key = line.substr(0,pos);
                                string value = line.substr(pos+1);
                                if (key == "ID") id = value;
                                else if (key == "Full name") fullName = value;
                                else if (key == "Gender") gender = value;
                                else if (key == "Specialization") spec = value;
                                else if (key == "Role") role = value; 
                                else if (key == "Clinic") clinic = value;
                            }
                            
                            rows.push_back({id,fullName,gender,spec,role,clinic});
                        }
                    }
                    drawTable(15,10,widths,rows);
                    
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n\n\n\t\t\t\t\t📝 ENTER BOOKING INFORMATION" << endl;
                    cout << "\t\t\t\t\tDoctor ID:";
                    getline(cin, doctorId);

                    system("cls");
                    SetColor(4);
                    showTitle("titlePatient.txt");
                    SetColor(7);

                    // Let Patient::bookAppointment handle date/slot/reason selection interactively
                    patient->bookAppointment(doctorId, "", "", "");

                    break;
                }
                case 2: {// Xem lịch khám sắp tới
                    system("cls");
                    showTitle("titlePatient.txt");

                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\tUPCOMMING APPOINTMENT" << endl;
                    SetColor(7);

                    patient->viewUpcomingAppointments();
                    break;
                }
                case 3: {// Xem lịch sử khám bệnh
                    system("cls");
                    showTitle("titlePatient.txt");

                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\tMEDICAL EXAMINATION HISTORY" << endl;
                    SetColor(7);

                    patient->viewAppointmentHistory();
                    break;
                }
                case 4:{ // Hủy lịch khám
                    system("cls");
                    showTitle("titlePatient.txt");
                    string appointmentId;

                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\tCANCEL THE APPOINTMENT" << endl;
                    SetColor(7);
                    
                    // Hiển thị lịch khám hiện tại
                    patient->viewUpcomingAppointments();
                    
                    cin.ignore();
                    cout << "\nEnter the appointment code to cancel:";
                    getline(cin, appointmentId);
                    patient->cancelAppointment(appointmentId);
                    break;
                }
                case 5:{// Xem thông tin cá nhân
                    system("cls");
                    showTitle("titlePatient.txt");

                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\tPERSONAL INFORMATION" << endl;
                    SetColor(7);

                    patient->displayInfo();
                    wayBackMenu();
                    break;
                }
                case 6:{ // Cập nhật thông tin cá nhân
                    system("cls");
                    showTitle("titlePatient.txt");

                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\tUPDATE PERSONAL INFORMATION" << endl;
                    SetColor(7);

                    if (authSystem.updateUserProfile(patient)){
                        SetColor(10*16+6);
                        cout << "\t\t\t\t\t✓ Information saved successfully!" << endl;
                        SetColor(7);
                    }
                    else{
                        SetColor(4*16+6);
                        cout << "\t\t\t\t\t✗ Information saved unsuccessfully!" << endl;
                        SetColor(7);
                    }
                    break;
                }
                case 7:{ // Đổi Password
                    system("cls");
                    showTitle("titlePatient.txt");
                    string oldPassword, newPassword, confirmPassword;
                    
                    SetColor(2);
                    cout << "\n\n\n\t\t\t\t\t\t\t\tCHANGE PASSWORD" << endl;
                    SetColor(7);

                    if (patient->changePassword()) {
                        if (authSystem.saveUserData(patient)) {
                            cout << "Updated password successfully!" << endl;
                        } else {
                            cout << "Updated password unsuccessfully!" << endl;
                        }
                    }
                    break;
                }
                case 8:// Đăng xuất
                    cout << "\n👋 Log out successfully! See you later" << endl;
                    system("cls");
                    authSystem.logout();
                    logout = true;
                    break;
            }
        }   
    }
}

// Hàm main
int main() {
    // Thiết lập UTF-8 cho console
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    system("cls");

    const int boxX = 50, boxY = 10, boxW = 80, boxH = 12;
    SetColor(2);
    drawBox(boxX,boxY,boxW,boxH);
    SetColor(7);
    
    gotoXY(boxX + 30, boxY + 1);
    SetColor(6);
    cout << "PBL2: DO AN LAP TRINH CO SO";
    gotoXY(boxX + 20, boxY + 2);
    cout << "DE TAI: XAY DUNG UNG DUNG DAT LICH KHAM BENH";
    SetColor(7);
    gotoXY(boxX + 20, boxY + 5);
    cout << "LOP: 24T_KHDL";
    gotoXY(boxX + 20, boxY + 6);
    cout << "GVHG: ThS. Tran Ho Thuy Tien";
    gotoXY(boxX + 20, boxY + 9);
    cout << "MSSV: 102240262 - Phan Vu Long";
    gotoXY(boxX + 20, boxY + 10);
    cout << "MSSV: 102240282 - Tran Thi Tuyet Trinh\n\n\n\n\n\n";
    system("pause");

    AuthSystem authSystem;
    bool exit = false;
    system("cls");
    while (!exit) {
        showTitle("title.txt");
        int choice = displayMainMenu();

        switch (choice) {
                case 1:{ // đăng ký
                system("cls");
                showTitle("title.txt");
                handleRegistration(authSystem);
                // Clear any leftover input before prompting login
                clearInputBuffer();
                User* user = handleLogin(authSystem);
                if (user != nullptr) {
                    handleUserSession(authSystem, user);
                }
                break;
            }
            case 2:{ // đăng nhập
                system("cls");
                User* user = handleLogin(authSystem);
                if (user != nullptr) {
                    handleUserSession(authSystem, user);
                }
                break;
            }
            case 3:{ // thoát
                showTitle("title.txt");
                system("cls");
                cout << "\nThank you for using the system! Goodbye" << endl;
                exit = true;
                break;
            }
        }
    }
    return 0;
}
