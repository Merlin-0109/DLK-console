#include <iostream>
#include <ctime>
#include <vector>
#include <iomanip>

using namespace std;

inline vector<string> dateWeek= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
inline vector<string> timeSlot = {"07:00","08:00","09:00","10:00","13:30","14:30","15:30"};

class Calendar{
    public:
        Calendar();
        ~Calendar();
        
        void showCalendar(string doctorID);
        bool saveCalendarToFile(string doctorID, string date);
        void printTicket(string appointmentId, string fullName, string doctor, 
                          string specialization, string clinic, string date, 
                          string time, string reason);
};
