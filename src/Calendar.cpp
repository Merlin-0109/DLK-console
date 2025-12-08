#include <map>

#include "Calendar.h"
#include "DataStore.h"
#include "UI.h"

Calendar::Calendar(){

}
Calendar::~Calendar(){

}
void Calendar::showCalendar(string doctorID){
    DataStore dataStore;
    vector<string> busyDate = dataStore.getBusyDate(doctorID);
    map<string,vector<string>> busyTime;
    vector<string> oneDate;
    for (const string& date : busyDate){
        oneDate = dataStore.getBusyTime(doctorID,date); // Lấy ra các khung giờ bận trong một ngày
        busyTime[date].insert(busyTime[date].end(),oneDate.begin(),oneDate.end()); // tổng hợp khung giờ bận 
    }

    auto getSlotCount = [&](const string &date, const string& time){
        return DataStore::getDoctorAppointmentsForDateSlot(doctorID, date, time).size();
    };
   

    vector<tm> dates;
    time_t now = std::time(nullptr);
    for (int i = 0; i < 7; i++){
        time_t t = now + (i+1)*24*3600;
        tm tm_ptr = *localtime(&t);
        dates.push_back(tm_ptr);
    }

    cout << setw(15) << endl << " ";
    for (int col = 0; col < 7; col++){
        char buf[6];
        strftime(buf,sizeof(buf),"%d/%m", &dates[col]);
        if (dates[col].tm_wday == 0)
            SetColor(12);
        else SetColor(15);
        cout << setw(15) << dateWeek[dates[col].tm_wday] + "(" + string(buf) + ")";
    }
    cout << "\n";

    for (int row = 0; row < 7; row++){
        cout << setw(15) << timeSlot[row];
        for (int col = 0; col < 7; col++){
            char buf[6];
            strftime(buf,sizeof(buf),"%d/%m",&dates[col]);

            if (dates[col].tm_wday == 0){
                SetColor(12);
                cout << setw(15) << "[OFF]";
                SetColor(7);
                continue;
            }

            bool isBusy = false;
            for (const string& date : busyDate){
                if (buf == date){
                    for (const string& time : busyTime[date]){
                        if (time == "AllDay" || time == timeSlot[row]){
                            SetColor(5);
                            cout << setw(15) << "[BUSY]";
                            SetColor(7);
                            isBusy = true;
                            break;
                        }
                    }
                    if (isBusy) break;
                }
            } 

            if (isBusy) continue;
            
            int count = getSlotCount(string(buf),timeSlot[row]);
            if (count == 0){
                SetColor(6);
                cout << setw(15) << "[ ]";
            }
            else if (count >= 5){
                SetColor(12);
                cout << setw(15) << "[FULL]";
            }
            else{
                SetColor(10);
                cout << setw(15) << "[" + to_string(count) + "/5" + "]"; 
            }
            SetColor(7);
        } 
        cout << "\n";
    }
}

bool Calendar::saveCalendarToFile(string doctorID, string date, string time){
    DataStore dataStore;
    dataStore.saveBusyCalendarToFile(doctorID,date,time);
    return true;
}
