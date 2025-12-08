#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "UI.h"
using namespace std;
void gotoXY(int x, int y) {
    COORD c{ (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void clearScreen() {
    cout << "\033[2J\033[H" << flush;
}
void drawBox(int x, int y, int w, int h) {
    gotoXY(x, y); cout << "┌";
    gotoXY(x + w, y); cout << "┐";
    gotoXY(x, y + h); cout << "└";
    gotoXY(x + w, y + h); cout << "┘";
    for (int i = 1; i < w; i++) {
        gotoXY(x + i, y); cout << "─";
        gotoXY(x + i, y + h); cout << "─";
    }
    for (int i = 1; i < h; i++) {
        gotoXY(x, y + i); cout << "│";
        gotoXY(x + w, y + i); cout << "│";
    }
}
string centerText(const string& text, int width){
    int len = text.length();
    if (len >= width)
        return text.substr(0,width);
    int left = (width - len)/2;
    int right = width - len - left;
    return string(left,' ') + text + string(right, ' ');
}
void drawTable(int x, int y, vector<int> widths, vector<vector<string>> rows){
    int cols = widths.size();
    gotoXY(x,y);
    cout << "┌";
    for (int i = 0; i < cols; i++){
        for (int j = 0; j < widths[i]; ++j) {
            cout << "─"; 
        } 
        if (i == cols - 1) cout << "┐";
        else cout << "┬";
    }
    for (int r = 0; r < rows.size(); r++){
        gotoXY(x, y + 1 + r * 2);
        cout << "│";
        for (int c = 0; c < cols; c++){
            cout << centerText(rows[r][c], widths[c]);
            cout << "│";
        }
        gotoXY(x, y + 2 + r * 2);
        if (r == rows.size() - 1){
            cout << "└";
            for (int i = 0; i < cols; i++){
                for (int j = 0; j < widths[i]; ++j) {
                    cout << "─"; 
                } 
                if (i == cols - 1) cout << "┘";
                else cout << "┴";
            }
        }
        else {
            cout << "├"; 
            for (int i = 0; i < cols; i++){
                for (int j = 0; j < widths[i]; ++j) {
                    cout << "─"; 
                } 
                if (i == cols - 1) cout << "┤"; // Kết thúc phân cách
                else cout << "┼"; // Ngã tư
            }
        }
    }
}
int runMenu(string items[], int count) {
    int choice = 1; 
    char key;
    int maxLen = 0;
    for (int i = 0; i < count; ++i) {
        if (items[i].length() > maxLen) {
            maxLen = items[i].length();
        }
    }
    const int contentWidth = maxLen + 4; 
    const int menuWidth = contentWidth + 2; 
    const int menuHeight = count * 2; 
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
    int startX = (consoleWidth - menuWidth) / 2;
    int startY = (consoleHeight - menuHeight)/2;
    SetColor(11);
    drawBox(startX, startY, menuWidth, menuHeight);
    for (int i = 0; i < count - 1; i++) {
        SetColor(11);
        int sepY = startY + 2 + i * 2;
        gotoXY(startX, sepY);
        cout << "├";
        for (int j = 0; j < menuWidth - 1; ++j) {
            cout << "─";
        }
        cout << "┤";
    }
    for (int i = 0; i < count; i++) {
        int itemY = startY + 1 + i * 2; 
        int itemX = startX + 1;
        if (i + 1 == choice) {
            SetColor(4 * 16 + 15); // Highlight item đầu tiên
        } else {
            SetColor(0 * 16 + 7); // Mặc định
        }
        gotoXY(itemX, itemY);
        int textLength = items[i].length();
        int requiredPadding = (menuWidth - 4) - textLength;
        string leftAlignedText = "   " + items[i] + string(requiredPadding, ' ');
        cout << leftAlignedText;
    }
    SetColor(7);
    int oldChoice = choice; // Để track item cũ cần unhighlight
    while (true) {
        key = _getch();
        if (key == 72 && choice > 1) choice--;          // UP
        else if (key == 80 && choice < count) choice++; // DOWN
        else if (key == 13) break;                      // ENTER
        if (oldChoice != choice) {
            for (int i = 0; i < count; i++) {
                if (i + 1 == choice || i + 1 == oldChoice) {
                    int itemY = startY + 1 + i * 2; 
                    int itemX = startX + 1;
                    if (i + 1 == choice) {
                        SetColor(4 * 16 + 15); // Highlight
                    } else {
                        SetColor(0 * 16 + 7); // Mặc định
                    }
                    gotoXY(itemX, itemY);
                    int textLength = items[i].length();
                    int requiredPadding = (menuWidth - 4) - textLength;
                    string leftAlignedText = "   " + items[i] + string(requiredPadding, ' ');
                    cout << leftAlignedText;
                }
            }
            SetColor(7);
            oldChoice = choice;
        }
    }
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    return choice;
}
int runMenuHorizontal(string items[], int count){
    int choice = 1;
    char key;
    if (count == 0) return 0;
    int totalWidth = 15*count;
    const int menuWidth = totalWidth + count + 1; // count + 1 la so phan cach |
    const int menuHeight = 2;
    CONSOLE_CURSOR_INFO cursorInfor;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfor);
    cursorInfor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfor);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int startX = (consoleWidth - menuWidth)/2;
    int startY = csbi.dwCursorPosition.Y;
    int cursorReturnY = startY;
    SetColor(11);
    gotoXY(startX, startY);
    cout << "┌";
    for (int i = 0; i < count; ++i) {
        int w = 15;
        for (int j = 0; j < w; ++j) {
            cout << "─";
        }
        if (i == count - 1) {
            cout << "┐"; 
        } else {
            cout << "┬"; 
        }
    }
    gotoXY(startX, startY + 1);
    cout << "│";
    int currentX = startX + 1;
    for (int i = 0; i < count; ++i) {
        int w = 15;
        if (i + 1 == choice) {
            SetColor(4 * 16 + 15); 
        } else {
            SetColor(0 * 16 + 7); 
        }
        string alignedText = centerText(items[i], w);
        cout << alignedText;
        SetColor(11);
        cout << "│";
        currentX += w + 1;
    }
    gotoXY(startX, startY + 2);
    cout << "└";
    for (int i = 0; i < count; ++i) {
        int w = 15;
        for (int j = 0; j < w; ++j) {
            cout << "─";
        }
        if (i == count - 1) {
            cout << "┘"; 
        } else {
            cout << "┴"; 
        }
    }
    SetColor(7);
    int oldChoice = choice;
    while (true){
        key = _getch();
        if (key == 0 || key == -32){
            key = _getch();
            if (key == 75 && choice > 1) choice--;
            else if (key == 77 && choice < count) choice++;
        }
        else if (key == 13) break;
        if (oldChoice != choice){
            int x_old = startX + 1;
            for(int k=0; k < oldChoice - 1; ++k) x_old += 15 + 1; 
            SetColor(0 * 16 + 7); 
            gotoXY(x_old, startY + 1);
            string alignedText_old = centerText(items[oldChoice - 1], 15);
            cout << alignedText_old;
            int x_new = startX + 1;
            for(int k=0; k < choice - 1; ++k) 
                x_new += 15 + 1; 
            SetColor(4 * 16 + 15);
            gotoXY(x_new, startY + 1);
            string alignedText_new = centerText(items[choice - 1], 15);
            cout << alignedText_new;
            SetColor(7);
            oldChoice = choice;
        }
    }
    CONSOLE_CURSOR_INFO tempCursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &tempCursorInfo);
    tempCursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &tempCursorInfo);
    for (int y = startY; y < startY + menuHeight + 1; ++y){
        gotoXY(startX, y);
        cout << string(menuWidth + 1, ' '); 
    }
    gotoXY(startX, cursorReturnY); 
    return choice;
}
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
