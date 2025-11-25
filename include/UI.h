#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <conio.h>
using namespace std;

void gotoXY(int x, int y);
void SetColor(int color);
void clearScreen();
void drawBox(int x, int y, int w, int h);
string centerText(const string& text, int width);
void drawTable(int x, int y, vector<int> widths, vector<vector<string>> rows);
int runMenu(string items[], int count);
int runMenuHorizontal(string items[], int count);
void showTitle(string fileName);

#endif // UI_H