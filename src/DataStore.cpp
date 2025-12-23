#include <iostream>
#include <cctype>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "DataStore.h"
#include "User.h"
#include "Doctor.h"
#include "Patient.h"

/*--------------------------------------------------------------
                        CONSTRUCTOR
---------------------------------------------------------------*/
DataStore::DataStore() :dataFolderPath("data") {
    patientsFolder = dataFolderPath + "/patients";
    doctorsFolder = dataFolderPath + "/doctors";
    busyFolder = dataFolderPath + "/busy";
    appointmentsFolder = dataFolderPath + "/appointments";
    
    patientIDsFile = dataFolderPath + "/patient_ids.txt";
    doctorIDsFile = dataFolderPath + "/doctor_ids.txt";
    initializeDirectories();
}

DataStore::DataStore(const string& basePath) :dataFolderPath(basePath) {
    patientsFolder = dataFolderPath + "/patients";
    doctorsFolder = dataFolderPath + "/doctors";
    busyFolder = dataFolderPath + "/busy";
    appointmentsFolder = dataFolderPath + "/appointments";
    
    patientIDsFile = dataFolderPath + "/patient_ids.txt";
    doctorIDsFile = dataFolderPath + "/doctor_ids.txt";
    initializeDirectories();
}

/*--------------------------------------------------------------
                    TẠO FOLDER - FILE
---------------------------------------------------------------*/
void DataStore::createDirectoryIfNotExists(const string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        _mkdir(path.c_str());
    }
}

void DataStore::initializeDirectories() {
    createDirectoryIfNotExists(dataFolderPath);
    createDirectoryIfNotExists(patientsFolder);
    createDirectoryIfNotExists(doctorsFolder);
    createDirectoryIfNotExists(appointmentsFolder);
    createDirectoryIfNotExists(busyFolder);
}

string DataStore::getPatientFilePath(const string& id) {
    return patientsFolder + "/" + id + ".txt";
}

string DataStore::getDoctorFilePath(const string& id) {
    return doctorsFolder + "/" + id + ".txt";
}

string DataStore::getBusyFilePath(const string& doctorID){
    return busyFolder + "/" + doctorID + ".txt";
}

/*--------------------------------------------------------------
                    TẠO ID BỆNH NHÂN - BÁC SĨ
---------------------------------------------------------------*/
int DataStore::getLastPatientNumber() {
    vector<string> ids = loadIDsFromFile(patientIDsFile);
    if (ids.empty()) return 0;
    string lastID = ids.back();
    return stoi(lastID.substr(2));  // Bỏ "01" ở đầu
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
int DataStore::getLastDoctorNumber() {
    vector<string> ids = loadIDsFromFile(doctorIDsFile);
    if (ids.empty()) return 0;
    string lastID = ids.back();
    return stoi(lastID.substr(2));  // Bỏ "02" ở đầu
}

string DataStore::generateDoctorID() {
    int lastNumber = getLastDoctorNumber();
    int newNumber = lastNumber + 1;
    stringstream ss;
    ss << "02" << setfill('0') << setw(3) << newNumber;
    string newID = ss.str();
    saveIDToFile(doctorIDsFile, newID);
    return newID;
}

/*--------------------------------------------------------------
                    TẢI & LƯU TRỮ ID 
---------------------------------------------------------------*/
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
void DataStore::saveIDToFile(const string& filename, const string& id) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << id << endl;
        file.close();
    }
}

/*--------------------------------------------------------------
                KIỂM TRA ID BỆNH NHÂN - BÁC SĨ
---------------------------------------------------------------*/
vector<string> DataStore::getAllPatientIDs() {
    vector<string> ids;
    
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

vector<string> DataStore::getAllDoctorIDs() {
    vector<string> ids;
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

bool DataStore::patientIDExists(const string& id) {
    vector<string> ids = getAllPatientIDs();
    return find(ids.begin(), ids.end(), id) != ids.end();
}

bool DataStore::doctorIDExists(const string& id) {
    vector<string> ids = getAllDoctorIDs();
    return find(ids.begin(), ids.end(), id) != ids.end();
}

/*--------------------------------------------------------------
                    TẢI DỮ LIỆU TỪ FILE
---------------------------------------------------------------*/
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

/*--------------------------------------------------------------
                    LƯU DỮ LIỆU VÀO FILE
---------------------------------------------------------------*/
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
bool DataStore::saveDoctorData(const string& id, const string& data) {
    string filepath = getDoctorFilePath(id);
    ofstream file(filepath);
    if (file.is_open()) {
        file << data;
        file.close();
        
        string busyFilepath = getBusyFilePath(id);
        ofstream busyFile(busyFilepath, ios::app);
        busyFile.close();
        
        return true;
    }
    return false;
}

/*--------------------------------------------------------------
                    LƯU VÀ LẤY RA LỊCH BẬN
---------------------------------------------------------------*/
bool DataStore::saveBusyCalendarToFile(const string& doctorID, string date){
    string filePath = DataStore::getBusyFilePath(doctorID);
    ofstream createFile(filePath, ios::app);

    ifstream file(filePath);
    if (!file.is_open()){
        cout << "Can not open the busy file! _ save" << endl;
        return false;
    }

    vector<string> lines;
    string line;
    while(getline(file,line)){
        lines.push_back(line);
    }
    file.close();

    time_t now = time(0);
    tm* ltm = localtime(&now);
    char monthYearBuf[8];
    strftime(monthYearBuf, sizeof(monthYearBuf), "%m/%Y", ltm);
    string currentMonthYear = string(monthYearBuf);

    auto isMonthYear = [](const string& s)->bool{
        if (s.size() != 7) return false;
        if (!isdigit(s[0]) || !isdigit(s[1]) || s[2] != '/' ||
            !isdigit(s[3]) || !isdigit(s[4]) || !isdigit(s[5]) || !isdigit(s[6])) return false;
        int mm = stoi(s.substr(0,2));
        int yyyy = stoi(s.substr(3));
        return mm >= 1 && mm <= 12 && yyyy >= 1900;
    };

    bool foundBlock = false;
    int blockStart = -1;
    vector<string> blockDates;
    for (int i = 0; i < (int)lines.size(); ){ 
        if (lines[i].empty()) { i++; continue; }
        if (isMonthYear(lines[i])){
            int j = i + 1;
            vector<string> dates;
            for (int c = 0; c < 3 && j < (int)lines.size(); c++, j++){
                if (lines[j].empty()) break;
                dates.push_back(lines[j]);
            }
            if (lines[i] == currentMonthYear){
                foundBlock = true;
                blockStart = i;
                blockDates = dates;
                break;
            }
            i = j;
        } else {
            i++;
        }
    }

    if (foundBlock){
        if (find(blockDates.begin(), blockDates.end(), date) != blockDates.end()){
        } else if ((int)blockDates.size() >= 3){
            cout << "You have reached the maximum of 3 busy days for this month" << endl;
        } else {
            int insertPos = blockStart + 1 + (int)blockDates.size();
            lines.insert(lines.begin() + insertPos, date);
            if ((int)blockDates.size() + 1 == 3){
                int blankPos = blockStart + 4; // month/year + 3 dates
                if (blankPos >= (int)lines.size() || !lines[blankPos].empty()){
                    lines.insert(lines.begin() + blankPos, "");
                }
            }
        }
    } else {
        lines.push_back(currentMonthYear);
        lines.push_back(date);
    }

    ofstream out(filePath);
    if (!out.is_open()){
        cout << "Can not open the busy file!" << endl;
        return false;
    }
    for (const auto& l : lines){
        out << l << endl;
    }
    out.close();
    return true;
}

vector<string> DataStore::getBusyDate(const string& doctorID){
    string filePath = DataStore::getBusyFilePath(doctorID);

    ifstream file(filePath);
    if (!file.is_open()){
        cout << "Can not open the busy file! _ getDate" << endl;
        return {};
    }

    vector<string> allLines;
    string line;
    while(getline(file,line)){
        allLines.push_back(line);
    }
    file.close();

    time_t now = time(0);
    tm* ltm = localtime(&now);
    char monthYearBuf[8];
    strftime(monthYearBuf, sizeof(monthYearBuf), "%m/%Y", ltm);
    string currentMonthYear = string(monthYearBuf);

    auto isMonthYear = [](const string& s)->bool{
        if (s.size() != 7) return false;
        if (!isdigit(s[0]) || !isdigit(s[1]) || s[2] != '/' ||
            !isdigit(s[3]) || !isdigit(s[4]) || !isdigit(s[5]) || !isdigit(s[6])) return false;
        int mm = stoi(s.substr(0,2));
        int yyyy = stoi(s.substr(3));
        return mm >= 1 && mm <= 12 && yyyy >= 1900;
    };

    vector<string> dateBusy;

    bool hasMonthHeaders = false;
    for (int i = 0; i < (int)allLines.size();){
        if (allLines[i].empty()){ i++; continue; }
        if (isMonthYear(allLines[i])){
            hasMonthHeaders = true;
            string monthHeader = allLines[i];
            int j = i + 1;
            vector<string> dates;
            for (int c = 0; c < 3 && j < (int)allLines.size(); c++, j++){
                if (allLines[j].empty()) break;
                dates.push_back(allLines[j]);
            }
            if (monthHeader == currentMonthYear){
                dateBusy = dates;
                break;
            }
            i = j;
        } else {
            i++;
        }
    }

    if (!hasMonthHeaders){
        for (const auto& l : allLines){
            if (l.empty()) continue;
            size_t slashPos = l.find('/');
            if (slashPos == string::npos) continue;
            try{
                int month = stoi(l.substr(slashPos + 1));
                int currentMonth = ltm->tm_mon + 1;
                if (month == currentMonth){
                    dateBusy.push_back(l);
                }
            }catch(...){
                continue;
            }
        }
    }

    return dateBusy;
}

/*--------------------------------------------------------------
                    KIỂM TRA & TẠO ID CUỘC HẸN
---------------------------------------------------------------*/
bool DataStore::appointmentExists(const string& appointmentId) {
    string filepath = "data/appointments/" + appointmentId + ".txt";
    ifstream file(filepath);
    bool exists = file.good();
    file.close();
    return exists;
}

string DataStore::generateAppointmentID() {
    static int counter = 0;
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

/*--------------------------------------------------------------
                    LƯU & ĐỌC CUỘC HẸN
---------------------------------------------------------------*/
bool  DataStore::writeAppointment(const  string& appointmentId, const AppointmentDetails& details){
    string filepath = "data/appointments/" + appointmentId + ".txt";
    ofstream file(filepath);
    if (!file.is_open()){
        cout << "Can not open file " + appointmentId << endl;
        return false;
    }
    file << "appointmentId:" << details.appointmentId << endl;
    file << "patientId:" << details.patientId << endl;
    file << "doctorId:" << details.doctorId << endl;
    file << "date:" << details.date << endl;
    file << "time:" << details.time << endl;
    file << "reason:" << details.reason << endl;
    file << "clinic:" << details.clinic << endl;
    file << "bookStatus:" << details.bookStatus << endl; // booked/cancelled
    file << "visitStatus:" << details.visitStatus << endl; // done/not done
    file.close();
    return true;
}

DataStore::AppointmentDetails DataStore::readAppointment(const  string& appointmentId){
    AppointmentDetails details;
    string filepath = "data/appointments/" + appointmentId + ".txt";
    ifstream file(filepath);
    if (!file.is_open()){
        cout << "Can not open file " + appointmentId << endl;
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
            else if (key == "clinic") details.clinic = value;
            else if (key == "bookStatus") details.bookStatus = value;
            else if (key == "visitStatus") details.visitStatus = value;
        }
    }
    file.close();
    return details;
}

/*--------------------------------------------------------------
            CẬP NHẬT TRẠNG THÁI ĐẶT LỊCH VÀ THĂM KHÁM
---------------------------------------------------------------*/
bool DataStore::updateBookAppointmentStatus(const  string& appointmentId, const  string& newStatus){
    string filepath = "data/appointments/" + appointmentId + ".txt";
    fstream file(filepath);
    if (!file.is_open()){
        cout << "Can not open file " + appointmentId + " to update the bookstatus";
        return false;
    }
    vector<string> lines;
    string line;
    while(getline(file,line)){
        size_t pos = line.find(":");
        string key = line.substr(0,pos);
        if (pos != string::npos){
            if (key == "appointmentId") lines.push_back(line);
            else if (key == "patientId") lines.push_back(line);
            else if (key == "doctorId") lines.push_back(line);
            else if (key == "date") lines.push_back(line);
            else if (key == "time") lines.push_back(line);
            else if (key == "reason") lines.push_back(line);
            else if (key == "bookStatus"){
                line = "bookStatus:" + newStatus;
                lines.push_back(line);
            }
            else if (key == "visitStatus") lines.push_back(line);
        }
    }
    file.close();
    ofstream fileWrite(filepath);
    if (!fileWrite.is_open()){
        cout << "Can not open file to update the book status" << endl;
        return false;
    }
    for (auto& line :lines){
        fileWrite << line << endl; 
    }
    fileWrite.close();
    return true;
}

bool DataStore::updateVisitAppointmentStatus(const  string& appointmentId,const  string& newvisitStatus){
    string filepath = "data/appointments/" + appointmentId + ".txt";
    ifstream file(filepath);
    if (!file.is_open()){
        cout << "Can not open file " + appointmentId;
        return false;
    }
    vector<string> lines;
    string line;
    while(getline(file,line)){
        size_t pos = line.find(":");
        string key = line.substr(0,pos);
        if (pos != string::npos){
            if (key == "appointmentId") lines.push_back(line);
            else if (key == "patientId") lines.push_back(line);
            else if (key == "doctorId") lines.push_back(line);
            else if (key == "date") lines.push_back(line);
            else if (key == "time") lines.push_back(line);
            else if (key == "reason") lines.push_back(line);
            else if (key == "bookStatus") lines.push_back(line);
            else if (key == "visitStatus"){
                line = "visitStatus:" + newvisitStatus;
                lines.push_back(line);
            }
        }
    }
    file.close();
    ofstream fileWrite(filepath);
    if (!fileWrite.is_open()){
        cout << "Can not open file to update the visit status" << endl;
        return false;
    }
    for (auto& line :lines){
        fileWrite << line << endl; 
    }
    fileWrite.close();
    return true;
}

/*--------------------------------------------------------------
        TỔNG HỢP CÁC LỊCH KHÁM CỦA BỆNH NHÂN & BÁC SĨ
---------------------------------------------------------------*/
vector<string> DataStore::getPatientAppointments(const string& patientId) {
    vector<string> appointments;
    string appointmentsFolder = "data/appointments";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((appointmentsFolder + "\\*.txt").c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string filename = findData.cFileName;
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
vector<string> DataStore::getDoctorAppointments(const string& doctorId) {
    vector<string> appointments;
    string appointmentsFolder = "data/appointments";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((appointmentsFolder + "\\*.txt").c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string filename = findData.cFileName;
            if (filename.length() > 4) {
                string appointmentId = filename.substr(0, filename.length() - 4);
                AppointmentDetails details = readAppointment(appointmentId);
                if (!details.appointmentId.empty() && details.doctorId == doctorId && details.bookStatus != "Cancelled") {
                    appointments.push_back(details.appointmentId);
                }
            }
        } while (FindNextFileA(hFind, &findData) != 0);
        FindClose(hFind);
    }
    return appointments;
}

vector<string> DataStore::getDoctorAppointmentsForDateSlot(const string& doctorId, const string& date, const string& timeSlot) {
    vector<string> appointments;
    string appointmentsFolder = "data/appointments";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((appointmentsFolder + "\\*.txt").c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string filename = findData.cFileName;
            if (filename.length() > 4) {
                string appointmentId = filename.substr(0, filename.length() - 4);
                AppointmentDetails details = readAppointment(appointmentId);
                if (!details.appointmentId.empty() && details.doctorId == doctorId && details.date == date && details.time == timeSlot && details.bookStatus != "Cancelled") {
                    appointments.push_back(details.appointmentId);
                }
            }
        } while (FindNextFileA(hFind, &findData) != 0);
        FindClose(hFind);
    }
    sort(appointments.begin(), appointments.end());
    return appointments;
}
