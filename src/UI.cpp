#include "UI.h"
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

void gotoXY(int x, int y) {
    COORD c{ (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    // ANSI escape code để xóa màn hình và đưa con trỏ về (0,0)
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

    // Dòng trên cùng
    cout << "┌";
    for (int i = 0; i < cols; i++){
        for (int j = 0; j < widths[i]; ++j) {
            cout << "─"; 
        } 
        
        if (i == cols - 1) cout << "┐";
        else cout << "┬";
    }

    for (int r = 0; r < rows.size(); r++){
        // Dòng dữ liệu
        gotoXY(x, y + 1 + r * 2);
        cout << "│";
        for (int c = 0; c < cols; c++){
            cout << centerText(rows[r][c], widths[c]);
            cout << "│";
        }

        gotoXY(x, y + 2 + r * 2);
        
        // Dòng phân cách hoặc cuối cùng
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

    // 1. TÌM CHIỀU DÀI CỦA MỤC DÀI NHẤT
    int maxLen = 0;
    for (int i = 0; i < count; ++i) {
        if (items[i].length() > maxLen) {
            maxLen = items[i].length();
        }
    }
    
    // 2. TÍNH TOÁN KÍCH THƯỚC KHUNG
    const int contentWidth = maxLen + 4; 
    const int menuWidth = contentWidth + 2; 
    const int menuHeight = count * 2; 
    
    // Ẩn con trỏ
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    // Lấy kích thước console và tính startX, startY
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
    int startX = (consoleWidth - menuWidth) / 2;
    int startY = (consoleHeight - menuHeight) / 2;
    
    // VẼ KHUNG NGOÀI 1 LẦN DUY NHẤT
    SetColor(11);
    drawBox(startX, startY, menuWidth, menuHeight);
    
    // VẼ CÁC ĐƯỜNG PHÂN CÁCH 1 LẦN
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
    
    // VẼ TẤT CẢ CÁC ITEMS LẦN ĐẦU TIÊN
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
        
        // Xử lý phím
        if (key == 72 && choice > 1) choice--;          // UP
        else if (key == 80 && choice < count) choice++; // DOWN
        else if (key == 13) break;                      // ENTER
        
        // CHỈ VẼ LẠI 2 ITEMS BỊ THAY ĐỔI (nếu có thay đổi)
        if (oldChoice != choice) {
            for (int i = 0; i < count; i++) {
                // Chỉ vẽ lại nếu là item mới được chọn HOẶC item cũ bị bỏ chọn
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

    // Hiển thị lại con trỏ
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    return choice;
}