#ifndef Menu_H
#define Menu_H

#include <iostream>

#include "User.h"
#include "AuthSystem.h"

using namespace std;

void clearInputBuffer();
int displayMainMenu();
int displayRegisterMenu();
int displayDoctorChoice();
int displayPatientChoice();
void wayBackMenu();

void mainMenu();
void handleRegistration(AuthSystem& authSystem);
void handleUserSession(AuthSystem& authSystem, User* user);

#endif 