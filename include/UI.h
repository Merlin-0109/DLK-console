#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <conio.h>

void gotoXY(int x, int y);
void SetColor(int color);
void clearScreen();
void drawBox(int x, int y, int w, int h);
std::string centerText(const std::string& text, int width);
void drawTable(int x, int y, std::vector<int> widths, std::vector<std::vector<std::string>> rows);
int runMenu(std::string items[], int count);

#endif // UI_H