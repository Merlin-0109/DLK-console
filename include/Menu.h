#ifndef Menu_H
#define Menu_H
#include <iostream>

#include "User.h"
#include "AuthSystem.h"
using namespace std;
void mainMenu();
int displayRegisterMenu();
int displayDoctorChoice();
int displayPatientChoice();
void handleRegistration(AuthSystem& authSystem);
void handleUserSession(AuthSystem& authSystem, User* user);
int displayMainMenu();
void clearInputBuffer();
void wayBackMenu();
#endif 
