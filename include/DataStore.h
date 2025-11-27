#ifndef DATASTORE_H
#define DATASTORE_H
#include <string>
#include <vector>
#include <fstream>
#include <direct.h>  // For _mkdir on Windows
#include <sys/stat.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <cstddef>
#include <windows.h>  // For directory scanning on Windows
#ifdef byte
#undef byte
#endif
using namespace std;
class DataStore {
    public:
        struct AppointmentDetails {
            string appointmentId;
            string patientId;
            string doctorId;
            string date;
            string time;
            string reason;
            string clinic; // phong kham
            string bookStatus; // booked/cancelled
            string visitStatus; // done; not_done
        };
    private:
        string dataFolderPath;
        string patientsFolder;
        string doctorsFolder;
        string patientIDsFile;
        string doctorIDsFile;
        void createDirectoryIfNotExists(const string& path);
        void initializeDirectories();
        int getLastPatientNumber();
        int getLastDoctorNumber();
        vector<string> loadIDsFromFile(const string& filename);
        void saveIDToFile(const string& filename, const string& id);
    public:
        DataStore();
        DataStore(const string& basePath);
        string generatePatientID();
        string generateDoctorID();
        bool patientIDExists(const string& id);
        bool doctorIDExists(const string& id);
        bool savePatientData(const string& id, const string& data);
        bool saveDoctorData(const string& id, const string& data);
        string loadPatientData(const string& id);
        string loadDoctorData(const string& id);
        vector<string> getAllPatientIDs();
        vector<string> getAllDoctorIDs();
        string getPatientFilePath(const string& id);
        string getDoctorFilePath(const string& id);
        static bool writeAppointment(const  string& appointmentId, const AppointmentDetails& details);
        static AppointmentDetails readAppointment(const  string& appointmentId);
        static bool updateBookAppointmentStatus(const  string& appointmentId, const  string& newStatus); // cap nhat tu benh nhan, neu bn dat xong thi "Booked" neu huy thi "Cancelled"
        static bool updateVisitAppointmentStatus(const  string& appointmentId, const  string& newvisitStatus); // cap nhat tu bac si, neu kham xong thi "Done"
        static string generateAppointmentID(); // Tao ID cho appointment
        static vector<string> getPatientAppointments(const string& patientId); // Lay danh sach appointments cua benh nhan
        static vector<string> getDoctorAppointments(const string& doctorId); // Lay danh sach appointments cua bac si
        static vector<string> getDoctorAppointmentsForDateSlot(const string& doctorId, const string& date, const string& timeSlot); // Lay appointments theo bac si, ngay, khung gio
        static bool appointmentExists(const string& appointmentId); // Kiem tra appointment co ton tai
};
#endif
