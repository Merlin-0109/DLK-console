#include "DataStore.h"
#include "User.h"
#include "Doctor.h"
#include "Patient.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>

// Constructor
DataStore::DataStore() :dataFolderPath("data") {
    patientsFolder = dataFolderPath + "/patients";
    doctorsFolder = dataFolderPath + "/doctors";
    
    patientIDsFile = dataFolderPath + "/patient_ids.txt";
    doctorIDsFile = dataFolderPath + "/doctor_ids.txt";
    
    initializeDirectories();
    // initializeDefaultAdmins();
}

DataStore::DataStore(const string& basePath) :dataFolderPath(basePath) {
    patientsFolder = dataFolderPath + "/patients";
    doctorsFolder = dataFolderPath + "/doctors";
    
    patientIDsFile = dataFolderPath + "/patient_ids.txt";
    doctorIDsFile = dataFolderPath + "/doctor_ids.txt";
    
    initializeDirectories();
    // initializeDefaultAdmins();
}

// Create directory if it doesn't exist
void DataStore::createDirectoryIfNotExists(const string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        _mkdir(path.c_str());
    }
}

// Initialize all directories
void DataStore::initializeDirectories() {
    createDirectoryIfNotExists(dataFolderPath);
    createDirectoryIfNotExists(patientsFolder);
    createDirectoryIfNotExists(doctorsFolder);
    createDirectoryIfNotExists(dataFolderPath + "/appointments");
}

string DataStore::generatePatientID() {
    int lastNumber = getLastPatientNumber();
    int newNumber = lastNumber + 1;
    
    stringstream ss;
    ss << "01" << setfill('0') << setw(3) << newNumber;
    string newID = ss.str();
    
    saveIDToFile(patientIDsFile, newID);
    return newID;
}

// Generate Doctor ID
string DataStore::generateDoctorID() {
    int lastNumber = getLastDoctorNumber();
    int newNumber = lastNumber + 1;
    
    stringstream ss;
    ss << "02" << setfill('0') << setw(3) << newNumber;
    string newID = ss.str();
    
    saveIDToFile(doctorIDsFile, newID);
    return newID;
}

// Get last patient number
int DataStore::getLastPatientNumber() {
    vector<string> ids = loadIDsFromFile(patientIDsFile);
    if (ids.empty()) return 0;
    
    string lastID = ids.back();
    return stoi(lastID.substr(2));  // Bỏ "01" ở đầu
}

// Get last doctor number
int DataStore::getLastDoctorNumber() {
    vector<string> ids = loadIDsFromFile(doctorIDsFile);
    if (ids.empty()) return 0;
    
    string lastID = ids.back();
    return stoi(lastID.substr(2));  // Bỏ "02" ở đầu
}

// Load IDs from file
vector<string> DataStore::loadIDsFromFile(const string& filename) {
    vector<string> ids;
    ifstream file(filename);
    
    if (file.is_open()) {
        string id;
        while (getline(file, id)) {
            if (!id.empty()) {
                ids.push_back(id);
            }
        }
        file.close();
    }
    
    return ids;
}

// Save ID to file
void DataStore::saveIDToFile(const string& filename, const string& id) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << id << endl;
        file.close();
    }
}

// Check if patient ID exists
bool DataStore::patientIDExists(const string& id) {
    vector<string> ids = getAllPatientIDs();
    return find(ids.begin(), ids.end(), id) != ids.end();
}

// Check if doctor ID exists
bool DataStore::doctorIDExists(const string& id) {
    vector<string> ids = getAllDoctorIDs();
    return find(ids.begin(), ids.end(), id) != ids.end();
}

// Save patient data
bool DataStore::savePatientData(const string& id, const string& data) {
    string filepath = getPatientFilePath(id);
    ofstream file(filepath);
    
    if (file.is_open()) {
        file << data;
        file.close();
        return true;
    }
    
    return false;
}

// Save doctor data
bool DataStore::saveDoctorData(const string& id, const string& data) {
    string filepath = getDoctorFilePath(id);
    ofstream file(filepath);
    
    if (file.is_open()) {
        file << data;
        file.close();
        return true;
    }
    
    return false;
}

// Load patient data
string DataStore::loadPatientData(const string& id) {
    string filepath = getPatientFilePath(id);
    ifstream file(filepath);
    
    if (file.is_open()) {
        stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }
    
    return "";
}

// Load doctor data
string DataStore::loadDoctorData(const string& id) {
    string filepath = getDoctorFilePath(id);
    ifstream file(filepath);
    
    if (file.is_open()) {
        stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }
    
    return "";
}

// Get all patient IDs
vector<string> DataStore::getAllPatientIDs() {
    vector<string> ids;
    
    // Đọc từ các file trong thư mục patients
    // Giả sử file có tên dạng:01001.txt, 01002.txt, ...
    for (int i = 1; i <= 999; i++) {
        stringstream ss;
        ss << "01" << setfill('0') << setw(3) << i;
        string id = ss.str();
        string filepath = getPatientFilePath(id);
        
        ifstream file(filepath);
        if (file.good()) {
            ids.push_back(id);
        }
        file.close();
    }
    
    return ids;
}

// Get all doctor IDs
vector<string> DataStore::getAllDoctorIDs() {
    vector<string> ids;
    
    // Đọc từ các file trong thư mục doctors
    for (int i = 1; i <= 999; i++) {
        stringstream ss;
        ss << "02" << setfill('0') << setw(3) << i;
        string id = ss.str();
        string filepath = getDoctorFilePath(id);
        
        ifstream file(filepath);
        if (file.good()) {
            ids.push_back(id);
        }
        file.close();
    }
    
    return ids;
}

// Delete patient data
bool DataStore::deletePatientData(const string& id) {
    string filepath = getPatientFilePath(id);
    return remove(filepath.c_str()) == 0;
}

// Delete doctor data
bool DataStore::deleteDoctorData(const string& id) {
    string filepath = getDoctorFilePath(id);
    return remove(filepath.c_str()) == 0;
}

// Get patient file path
string DataStore::getPatientFilePath(const string& id) {
    return patientsFolder + "/" + id + ".txt";
}

// Get doctor file path
string DataStore::getDoctorFilePath(const string& id) {
    return doctorsFolder + "/" + id + ".txt";
}
// loi appoint
bool  DataStore::writeAppointment(const  string& appointmentId, const AppointmentDetails& details){
    string filepath = "data/appointments/" + appointmentId + ".txt";
    ofstream file(filepath);

    if (!file.is_open()){
        cout << "Khong mo duoc file appointments de ghi lai cuoc hen voi id " + appointmentId << endl;
        return false;
    }
    file << "appointmentId:" << details.appointmentId << endl;
    file << "patientId:" << details.patientId << endl;
    file << "doctorId:" << details.doctorId << endl;
    file << "date:" << details.date << endl;
    file << "time:" << details.time << endl;
    file << "reason:" << details.reason << endl;
    file << "bookStatus:" << details.bookStatus << endl; // booked/cancelled
    file << "visitStatus:" << details.visitStatus << endl; // done/not done

    file.close();

    // string patientListFile = "data/Patient/" + details.patientId + "_appointments.txt";
    return true;
}
DataStore::AppointmentDetails DataStore::readAppointment(const  string& appointmentId){
    AppointmentDetails details;
    string filepath = "data/appointments/" + appointmentId + ".txt";
    ifstream file(filepath);

    if (!file.is_open()){
        cout << "Khong the mo file cuoc hen voi id " + appointmentId << endl;
        return details;
    }

    string line;
    while(getline(file,line)){
        size_t pos = line.find(":");
        if (pos != string::npos){ // npos:khong tim thay gi
            string key = line.substr(0,pos); 
            string value = line.substr(pos+1);

            if (key == "appointmentId") details.appointmentId = value;
            else if (key == "patientId") details.patientId = value;
            else if (key == "doctorId") details.doctorId = value;
            else if (key == "date") details.date = value;
            else if (key == "time") details.time = value;
            else if (key == "reason") details.reason = value;
            else if (key == "bookStatus") details.bookStatus = value;
            else if (key == "visitStatus") details.visitStatus = value;
        }
    }
    file.close();
    return details;
}

// cap nhat trang thai cuoc hen tu benh nhan
bool DataStore::updateBookAppointmentStatus(const  string& appointmentId, const  string& newStatus){
    string filepath = "data/appointments/" + appointmentId + ".txt";
    fstream file(filepath);

    if (!file.is_open()){
        cout << "Khong the mo file cuoc hen voi id " + appointmentId + " de cap nhat trang thai";
        return false;
    }

    vector<string> lines;
    string line;
    while(getline(file,line)){
        size_t pos = line.find(":");
        string key = line.substr(0,pos);
        if (pos != string::npos){
            if (key == "bookStatus"){
                line = "bookStatus:" + newStatus;
                lines.push_back(line);
            }
        }
    }
    file.close();
     ofstream fileWrite(filepath);
    if (!fileWrite.is_open()){
        cout << "Khong the mo file de cap nhat trang thai tham dat lich" << endl;
        return false;
    }

    for (auto& line :lines){
        fileWrite << line << endl; 
    }

    fileWrite.close();

    return true;
}
// cap nhap trang thai tham kham tu bac si
bool DataStore::updateVisitAppointmentStatus(const  string& appointmentId,const  string& newvisitStatus){
    string filepath = "data/appointments/" + appointmentId + ".txt";
    ifstream file(filepath);

    if (!file.is_open()){
        cout << "Khong the mo file cuoc hen voi id " + appointmentId;
        return false;
    }

    vector<string> lines;
    string line;
    while(getline(file,line)){
        size_t pos = line.find(":");
        string key = line.substr(0,pos);
        if (pos != string::npos){
            if (key == "visitStatus"){
                line = "visitStatus:" + newvisitStatus;
                lines.push_back(line);
            } 
        }
    }
    file.close();

    ofstream fileWrite(filepath);
    if (!fileWrite.is_open()){
        cout << "Khong the mo file de cap nhat trang thai tham kham" << endl;
        return false;
    }

    for (auto& line :lines){
        fileWrite << line << endl; 
    }

    fileWrite.close();
    return true;
}
// Generate Appointment ID
string DataStore::generateAppointmentID() {
    static int counter = 0;
    
    // Lay thoi gian hien tai
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    stringstream ss;
    ss << "APT" 
       << setfill('0') << setw(4) << (1900 + ltm->tm_year)
       << setw(2) << (1 + ltm->tm_mon)
       << setw(2) << ltm->tm_mday
       << setw(2) << ltm->tm_hour
       << setw(2) << ltm->tm_min
       << setw(2) << ltm->tm_sec
       << setw(3) << (counter++ % 1000);
    
    return ss.str();
}

// Get patient appointments
vector<string> DataStore::getPatientAppointments(const string& patientId) {
    vector<string> appointments;
    string appointmentsFolder = "data/appointments";
    
    // Scan thư mục appointments để lấy tất cả các file
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((appointmentsFolder + "\\*.txt").c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string filename = findData.cFileName;
            // Loại bỏ extension .txt
            if (filename.length() > 4) {
                string appointmentId = filename.substr(0, filename.length() - 4);
                AppointmentDetails details = readAppointment(appointmentId);
                
                if (!details.appointmentId.empty() && details.patientId == patientId) {
                    appointments.push_back(details.appointmentId);
                }
            }
        } while (FindNextFileA(hFind, &findData) != 0);
        FindClose(hFind);
    }
    
    return appointments;
}

// Get doctor appointments
vector<string> DataStore::getDoctorAppointments(const string& doctorId) {
    vector<string> appointments;
    string appointmentsFolder = "data/appointments";
    
    // Scan thư mục appointments để lấy tất cả các file
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((appointmentsFolder + "\\*.txt").c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string filename = findData.cFileName;
            // Loại bỏ extension .txt
            if (filename.length() > 4) {
                string appointmentId = filename.substr(0, filename.length() - 4);
                AppointmentDetails details = readAppointment(appointmentId);
                
                if (!details.appointmentId.empty() && details.doctorId == doctorId) {
                    appointments.push_back(details.appointmentId);
                }
            }
        } while (FindNextFileA(hFind, &findData) != 0);
        FindClose(hFind);
    }
    
    return appointments;
}

// Check if appointment exists
bool DataStore::appointmentExists(const string& appointmentId) {
    string filepath = "data/appointments/" + appointmentId + ".txt";
    ifstream file(filepath);
    bool exists = file.good();
    file.close();
    return exists;
}
