#include "UI.h"
#include <iostream>
#include <vector>

using namespace std;

void gotoXY(int x, int y) {
    COORD c{ (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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
    // Chiều rộng cho nội dung (cộng thêm 2 khoảng trắng lề)
    const int contentWidth = maxLen + 4; 
    // Chiều rộng tổng = nội dung + 2 viền dọc (cột)
    const int menuWidth = contentWidth + 2; 
    // Chiều cao = Số hàng * 2 + 1 (mỗi hàng 1 dòng data + 1 dòng chia, + 1 dòng cuối)
    const int menuHeight = count * 2; 
    
    // Ẩn con trỏ
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    while (true) {
        // Lấy kích thước console và tính startX, startY
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
        int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
        int startX = (consoleWidth - menuWidth) / 2;
        int startY = (consoleHeight - menuHeight) / 2;
        
        // VẼ KHUNG NGOÀI (Sử dụng drawBox)
        SetColor(11); // Xanh ngọc
        drawBox(startX, startY, menuWidth, menuHeight);
        
        // Vòng lặp vẽ nội dung và đường chia
        for (int i = 0; i < count; i++) {
            
            // Vị trí y của dòng dữ liệu (data row)
            int itemY = startY + 1 + i * 2; 

            // SET MÀU CHO HÀNG DỮ LIỆU VÀ IN CHỮ
            if (i + 1 == choice) {
                // Highlight: Nền Đỏ (4) + Chữ Trắng Sáng (15)
                SetColor(4 * 16 + 15); 
            } else {
                // Mặc định: Nền Đen (0) + Chữ Trắng (7)
                SetColor(0 * 16 + 7);
            }
            
            // Vị trí x của text (cách viền 1 đơn vị)
            int itemX = startX + 1;
            gotoXY(itemX, itemY);
            
            // In nội dung (chiếm toàn bộ chiều rộng nội dung)
            // Lấy độ dài để căn giữa text trong vùng (menuWidth - 2)
            // cout << centerText(items[i], menuWidth - 2); 

            int textLength = items[i].length();
            int requiredPadding = (menuWidth - 4) - textLength; // Chiều rộng nội dung - độ dài chuỗi
            
            // Tạo chuỗi căn lề trái: items[i] + khoảng trắng đệm
            string leftAlignedText = "   " + items[i] + string(requiredPadding, ' ');
            
            // In chuỗi đã căn lề trái (để tô đầy đủ màu nền)
            cout << leftAlignedText;
            
            // VẼ ĐƯỜNG PHÂN CÁCH (trừ hàng cuối)
            if (i < count - 1) {
                SetColor(11); // Viền xanh ngọc
                int sepY = startY + 2 + i * 2;
                gotoXY(startX, sepY);
                
                // VẼ ĐƯỜNG NGANG CHIA DÒNG:
                // Bắt đầu bằng '├' (góc trái dưới của ô trên)
                cout << "├";
                // Vẽ đường ngang '─'
                for (int j = 0; j < menuWidth - 1; ++j) {
                    cout << "─";
                }
                // Kết thúc bằng '┤' (góc phải dưới của ô trên)
                cout << "┤";
            }
        }
        
        // Đặt lại màu chữ mặc định sau khi vẽ xong
        SetColor(7);

        key = _getch();
        if (key == 72 && choice > 1) choice--;          // UP
        else if (key == 80 && choice < count) choice++; // DOWN
        else if (key == 13) break;                      // ENTER
    }

    // Hiển thị lại con trỏ
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    return choice;
}