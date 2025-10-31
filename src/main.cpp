#include <iostream>
#include <string>
#include <limits>
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
#include "AuthSystem.h"
#include "Doctor.h"
#include "Patient.h"

using namespace std;

// Hàm xóa buffer input
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Hàm hiển thị menu chính
void displayMainMenu() {
    cout << "\n========================================" << endl;
    cout << "   HỆ THỐNG QUẢN LÝ BỆNH VIỆN" << endl;
    cout << "========================================" << endl;
    cout << "1. Đăng ký" << endl;
    cout << "2. Đăng nhập" << endl;
    cout << "3. Thoát" << endl;
    cout << "========================================" << endl;
    cout << "Nhập lựa chọn của bạn:";
}

// Hàm hiển thị menu đăng ký
void displayRegisterMenu() {
    cout << "\n========================================" << endl;
    cout << "           ĐĂNG KÝ TÀI KHOẢN" << endl;
    cout << "========================================" << endl;
    cout << "Chọn loại tài khoản:" << endl;
    cout << "1. Bác sĩ" << endl;
    cout << "2. Bệnh nhân" << endl;
    cout << "3. Thoát" << endl;
    cout << "========================================" << endl;
    cout << "Nhập lựa chọn của bạn:";
}

// Hàm hiển thị menu sau khi đăng nhập
void displayUserMenu(User* user) {
    cout << "\n========================================" << endl;
    cout << "   MENU NGƯỜI DÙNG - " << user->getUserTypeString() << endl;
    cout << "========================================" << endl;
    cout << "1. Xem thông tin cá nhân" << endl;
    
    // Chỉ hiển thị tùy chọn cập nhật cho Doctor và Patient
    if (user->getUserType() != ADMIN) {
        cout << "2. Cập nhật thông tin cá nhân" << endl;
    } else {
        cout << "2. Đăng xuất" << endl;
    }
    cout << "3. Đăng xuất" << endl;
    cout << "========================================" << endl;
    cout << "Nhập lựa chọn của bạn:";
}

void displayDoctorChoice(){
    cout << "\n==========MENU BÁC SĨ==========" << endl;
    cout << "1. Xem lịch khám" << endl;
    cout << "2. Từ chối lịch khám" << endl;
    cout << "3. Xem thông tin cá nhân" << endl;
    cout << "4. Cập nhật thông tin cá nhân" << endl;
    cout << "5. Đăng xuất" << endl;
    cout << "Nhập lựa chọn của bạn:" ;
}
void displayPatientChoice(){
    cout << "\n========================================" << endl;
    cout << "           MENU BENH NHAN" << endl;
    cout << "========================================" << endl;
    cout << "1. Dat lich kham moi" << endl;
    cout << "2. Xem lich kham sap toi" << endl;
    cout << "3. Xem lich su kham benh" << endl;
    cout << "4. Doi lich kham" << endl;
    cout << "5. Huy lich kham" << endl;
    cout << "6. Xem thong tin ca nhan" << endl;
    cout << "7. Cap nhat thong tin ca nhan" << endl;
    cout << "8. Dang xuat" << endl;
    cout << "========================================" << endl;
    cout << "Nhap lua chon cua ban: ";
}

// Hàm đăng ký Doctor
void registerDoctor(AuthSystem& authSystem) {
    string username, password;
    
    cout << "\n--- Đăng ký tài khoản Bác sĩ ---" << endl;
    cout << "CCCD: ";
    cin >> username;
    cout << "Mật khẩu:";
    cin >> password;
    clearInputBuffer();
    authSystem.registerDoctor(username, password);
}

// Hàm đăng ký Patient
void registerPatient(AuthSystem& authSystem) {
    string username, password;
    
    cout << "\n--- Đăng ký Bệnh nhân ---" << endl;
    cout << "CCCD: ";
    cin >> username;
    cout << "Mật khẩu:";
    cin >> password;
    clearInputBuffer();
    authSystem.registerPatient(username, password);
}

// Hàm xử lý đăng ký
void handleRegistration(AuthSystem& authSystem) {
    int choice;
        displayRegisterMenu();
        cin >> choice;
        
        if (cin.fail()) {
            clearInputBuffer();
            cout << "Đầu vào không hợp lệ! Vui lòng nhập một số." << endl;
            return;
        }
        
        switch (choice) {
            case 1:
                registerDoctor(authSystem);
                break;
            case 2:
                registerPatient(authSystem);
                break;
            default:
                cout << "Lựa chọn không hợp lệ! Vui lòng thử lại." << endl;
                break;
        }
}

// Hàm xử lý đăng nhập
User* handleLogin(AuthSystem& authSystem) {
    string username, password;
    
    cout << "\n--- Đăng nhập ---" << endl;
    cout << "CCCD: ";
    cin >> username;
    cout << "Mật khẩu:";
    cin >> password;
    
    return authSystem.login(username, password);
}

// Hàm xử lý menu sau khi đăng nhập
void handleUserSession(AuthSystem& authSystem, User* user) {
    int choice;
    bool logout = false;
    
    while (!logout) {
        if (user->getUserType() == ADMIN) {
            displayUserMenu(user);
            cin >> choice;
            // Menu cho Admin (chỉ có 2 lựa chọn)
            switch (choice) {
                case 1:
                    cout << endl;
                    user->displayInfo();
                    break;
                case 2:
                    authSystem.logout();
                    logout = true;
                    break;
                default:
                    cout << "Lựa chọn không hợp lệ! Vui lòng thử lại." << endl;
            }
        } 
        else if (user->getUserType() == DOCTOR){
            displayDoctorChoice();
            cin >> choice;
            switch (choice){
                case 1:// Xem lịch khám
                    break;
                case 2:// Từ chối lịch khám
                    break;
                case 3:// Xem thông tin cá nhân
                    user->displayInfo();
                    break;
                case 4:// Cập nhật thông tin cá nhân
                    if (authSystem.updateUserProfile(user)) 
                            cout << "Đã lưu thông tin thành công!" << endl;
                    else
                        cout << "Lỗi khi lưu thông tin!" << endl;
                    break;
                case 5:
                    authSystem.logout();
                    logout = true;
                    break;
                default:
                    cout << "Lựa chọn không hợp lệ vui lòng chọn lại" << endl;
            }
        }
        else if (user->getUserType() == PATIENT){
            Patient* patient = dynamic_cast<Patient*>(user);
            
            // Hiển thị số lịch khám đang chờ
            int activeCount = patient->countActiveAppointments();
            if (activeCount > 0) {
                cout << "\n📋 Bạn có " << activeCount << " lịch khám đang chờ." << endl;
            }
            
            displayPatientChoice();
            cin >> choice;
            
            switch (choice){
                case 1: { // Đặt lịch khám mới
                    string doctorId, date, time, reason;
                    
                    cout << "\n╔══════════════════════════════════════╗" << endl;
                    cout << "║       ĐẶT LỊCH KHÁM MỚI              ║" << endl;
                    cout << "╚══════════════════════════════════════╝" << endl;
                    
                    // Hiển thị danh sách bác sĩ
                    vector<string> doctorIDs = authSystem.getDataStore()->getAllDoctorIDs();
                    if (doctorIDs.empty()) {
                        cout << "Hiện tại chưa có bác sĩ nào trong hệ thống." << endl;
                        break;
                    }
                    
                    cout << "\n📋 DANH SÁCH BÁC SĨ:" << endl;
                    cout << "----------------------------------------" << endl;
                    for (const string& dId : doctorIDs) {
                        string data = authSystem.getDataStore()->loadDoctorData(dId);
                        if (!data.empty()) {
                            stringstream ss(data);
                            string type, id, username, password, email, fullName, spec;
                            getline(ss, type);
                            getline(ss, id);
                            getline(ss, username);
                            getline(ss, password);
                            getline(ss, email);
                            getline(ss, fullName);
                            getline(ss, spec);
                            
                            cout << "👨‍⚕️ Mã BS: " << id;
                            cout << " | Tên: " << (fullName.empty() ? username : fullName);
                            if (!spec.empty()) cout << " | Chuyên khoa: " << spec;
                            cout << endl;
                        }
                    }
                    cout << "----------------------------------------" << endl;
                    
                    cin.ignore();
                    cout << "\n📝 Nhập thông tin đặt lịch:" << endl;
                    cout << "Mã bác sĩ: ";
                    getline(cin, doctorId);
                    
                    cout << "Ngày khám (DD/MM/YYYY): ";
                    getline(cin, date);
                    
                    cout << "Giờ khám (HH:MM): ";
                    getline(cin, time);
                    
                    cout << "Lý do khám: ";
                    getline(cin, reason);
                    
                    patient->bookAppointment(doctorId, date, time, reason);
                    break;
                }
                case 2: // Xem lịch khám sắp tới
                    patient->viewUpcomingAppointments();
                    break;
                case 3: // Xem lịch sử khám bệnh
                    patient->viewAppointmentHistory();
                    break;
                case 4: { // Đổi lịch khám
                    string appointmentId, newDate, newTime;
                    cout << "\n╔══════════════════════════════════════╗" << endl;
                    cout << "║       ĐỔI LỊCH KHÁM                  ║" << endl;
                    cout << "╚══════════════════════════════════════╝" << endl;
                    
                    // Hiển thị lịch khám hiện tại
                    patient->viewUpcomingAppointments();
                    
                    cin.ignore();
                    cout << "\nNhập mã lịch khám cần đổi: ";
                    getline(cin, appointmentId);
                    
                    cout << "Ngày khám mới (DD/MM/YYYY): ";
                    getline(cin, newDate);
                    
                    cout << "Giờ khám mới (HH:MM): ";
                    getline(cin, newTime);
                    
                    patient->rescheduleAppointment(appointmentId, newDate, newTime);
                    break;
                }
                case 5: { // Hủy lịch khám
                    string appointmentId;
                    cout << "\n╔══════════════════════════════════════╗" << endl;
                    cout << "║       HỦY LỊCH KHÁM                  ║" << endl;
                    cout << "╚══════════════════════════════════════╝" << endl;
                    
                    // Hiển thị lịch khám hiện tại
                    patient->viewUpcomingAppointments();
                    
                    cin.ignore();
                    cout << "\nNhập mã lịch khám cần hủy: ";
                    getline(cin, appointmentId);
                    patient->cancelAppointment(appointmentId);
                    break;
                }
                case 6: // Xem thông tin cá nhân
                    patient->displayInfo();
                    break;
                case 7: // Cập nhật thông tin cá nhân
                    if (authSystem.updateUserProfile(patient)) 
                        cout << "✓ Đã lưu thông tin thành công!" << endl;
                    else
                        cout << "✗ Lỗi khi lưu thông tin!" << endl;
                    break;
                case 8: // Đăng xuất
                    cout << "\n👋 Đăng xuất thành công. Hẹn gặp lại!" << endl;
                    authSystem.logout();
                    logout = true;
                    break;
                default:
                    cout << "❌ Lựa chọn không hợp lệ! Vui lòng chọn từ 1-8" << endl;
            }
        }   
    }
}

// Hàm main
int main() {
    // Thiết lập UTF-8 cho console
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    AuthSystem authSystem;
    int choice;
    bool exit = false;
    
    cout << "========================================" << endl;
    cout << "   Chào mừng đến Hệ thống Bệnh viện!" << endl;
    cout << "========================================" << endl;
    
    while (!exit) {
        displayMainMenu();
        cin >> choice;
        
        if (cin.fail()) {
            clearInputBuffer();
            cout << "Đầu vào không hợp lệ! Vui lòng nhập một số" << endl;
            continue;
        } 

        switch (choice) {
            case 1:{
                handleRegistration(authSystem);
                User* user = handleLogin(authSystem);
                if (user != nullptr) {
                    handleUserSession(authSystem, user);
                }
                break;
            }
            case 2:{
                User* user = handleLogin(authSystem);
                if (user != nullptr) {
                    handleUserSession(authSystem, user);
                }
                break;
            }
            case 3:
                cout << "\nCảm ơn bạn đã sử dụng hệ thống. Tạm biệt!" << endl;
                exit = true;
                break;
            default:
                cout << "Lựa chọn không hợp lệ! Vui lòng thử lại." << endl;
                break;
        }
    }
    return 0;
}
