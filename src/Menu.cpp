#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <conio.h>
#include "UI.h"
#include "AuthSystem.h"
using namespace std;

/*--------------------------------------------------------------
                NỘI DUNG HIỂN THỊ TRÊN MENU
---------------------------------------------------------------*/
string menuMain[] = {
    "Register",
    "Log in",
    "Exit"
};

string menuLogIn_Out[] = {
    "Doctor",
    "Patient",
    "Exit"
};

string menuDoctor[] = {
    "View appointment",
    "Mark as busy",
    "Update visit status of appointment",
    "View personal information",
    "Update personal information",
    "Change password",
    "Log out"
};

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

/*--------------------------------------------------------------
                        MENU CHÍNH
---------------------------------------------------------------*/
void mainMenu(){
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
                clearInputBuffer();
                User* user = authSystem.handleLogin(authSystem);
                if (user != nullptr) {
                    handleUserSession(authSystem, user);
                }
                break;
            }
            case 2:{ // đăng nhập
                system("cls");
                User* user = authSystem.handleLogin(authSystem);
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
}

/*--------------------------------------------------------------
                    HIỂN THỊ CÁC MENU CHÍNH
---------------------------------------------------------------*/
int displayMainMenu() {
    return runMenu(menuMain,3);
}

int displayRegisterMenu() {
    return runMenu(menuLogIn_Out,3);
}

int displayDoctorChoice(){
    return runMenu(menuDoctor,7);
}

int displayPatientChoice(){
    return runMenu(menuPatient,8);
}

/*--------------------------------------------------------------
            HIỂN THỊ MENU ĐĂNG KÝ VAI TRÒ CỦA TÀI KHOẢN
---------------------------------------------------------------*/
void handleRegistration(AuthSystem& authSystem) {
    int choice;
        SetColor(2);
        cout << "\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\tWHICH ROLE ARE YOU REGISTERING FOR?" << endl;
        SetColor(7);
        choice = displayRegisterMenu();
        switch (choice) {
            case 1:{
                system("cls");
                SetColor(14);
                showTitle("title.txt");
                SetColor(7);
                authSystem.registerUser(DOCTOR);
                break;
            }
            case 2:{
                system("cls");
                SetColor(14);
                showTitle("title.txt");
                SetColor(7);
                authSystem.registerUser(PATIENT);
                break;
            }
        }
}

/*--------------------------------------------------------------
        HIỂN THỊ MENU CHỨC NĂNG CỦA BÁC SĨ - BỆNH NHÂN
---------------------------------------------------------------*/
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
                    cout << "\n\n\n\t\t\t\t\t\t\tREMARK AS BUSY" << endl;
                    SetColor(7);
                    
                    doctor->remarkAsBusy();
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

                    if (authSystem.updateUserProfile(doctor)){
                            gotoXY(50,20);
                            SetColor(10*16+6);
                            cout << "Information saved successfully!" << endl;
                            SetColor(7);
                    }
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
                            gotoXY(55,20);
                            SetColor(10*16+6);
                            cout << "Updated password successfully!" << endl;
                            SetColor(7);
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
                    vector<string> doctorIDs = authSystem.getDataStore()->getAllDoctorIDs();
                    if (doctorIDs.empty()) {
                        cout << "Currently, there are no doctors in the system" << endl;
                        break;
                    }
                    
                    vector<int> widths = {20,30,15,25,20,15};
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
                    patient->viewUpcomingAppointments();
                    // cin.ignore();
                    cout << "\nEnter the appointment code to cancel:";
                    getline(cin, appointmentId);
                    patient->cancelAppointment(appointmentId);
                    wayBackMenu();
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
                    wayBackMenu();
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
                            gotoXY(55,20);
                            SetColor(10*16+6);
                            cout << "Updated password successfully!" << endl;
                            SetColor(7);
                        } else {
                            cout << "Updated password unsuccessfully!" << endl;
                        }
                    }
                    wayBackMenu();
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

/*--------------------------------------------------------------
                    CÁC HÀM HỖ TRỢ KHÁC
---------------------------------------------------------------*/
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void wayBackMenu(){
    cout << "Please press any key to way back main menu ..." << endl;
    _getch();
}