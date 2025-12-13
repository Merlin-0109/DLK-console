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
                    SetColor(5);
                    cout << setw(15) << "[BUSY]";
                    SetColor(7);
                    isBusy = true;
                    break;
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

bool Calendar::saveCalendarToFile(string doctorID, string date){
    DataStore dataStore;
    dataStore.saveBusyCalendarToFile(doctorID,date);
    return true;
}

void Calendar::printTicket(string appointmentId, string fullName, string doctor, 
                          string specialization, string clinic, string date, 
                          string time, string reason) {
    int startX = 40;
    int startY = 5;
    int width = 60;
    int totalHeight = 14; 
    
    drawBox(startX, startY, width, totalHeight);

    gotoXY(startX + 2, startY + 1); cout << string(width - 2, ' ');
    gotoXY(startX + (width - 18) / 2, startY + 2);
    cout << "MEDICAL APPOINTMENT";
    
    gotoXY(startX + 2, startY + 3); cout << string(width - 2, ' ');

    gotoXY(startX, startY + 4); cout << "├";
    for (int i = 1; i < width; i++) {
        gotoXY(startX + i, startY + 4); cout << "─";
    }
    gotoXY(startX + width, startY + 4); cout << "┤";

    int contentY = startY + 5;
    
    gotoXY(startX + 2, contentY); 
    cout << "   Appointment ID: " << appointmentId;
    gotoXY(startX + 2, contentY + 1); 
    cout << "   Patient: " << fullName;
    gotoXY(startX + 2, contentY + 2); 
    cout << "   Doctor: " << doctor;
    gotoXY(startX + 2, contentY + 3); 
    cout << "   Specialization: " << specialization;
    gotoXY(startX + 2, contentY + 4); 
    cout << "   Clinic: " << clinic;
    gotoXY(startX + 2, contentY + 5); 
    cout << "   Date: " << date << "  Time: " << time;
    gotoXY(startX + 2, contentY + 6); 
    cout << "   Reason: " << reason;

    gotoXY(startX, startY + 12); cout << "├";
    for (int i = 1; i < width; i++) {
        gotoXY(startX + i, startY + 12); cout << "─";
    }
    gotoXY(startX + width, startY + 12); cout << "┤";
    
    gotoXY(startX + 2, startY + 13);
    cout << "⚠ Please arrive on time and bring your Identity Card!";
    
    gotoXY(0, startY + totalHeight + 3);
}
