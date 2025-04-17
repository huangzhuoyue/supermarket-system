#ifndef UI_H
#define UI_H

#include "common.h"

// 用户界面函数声明
void displayCover();
void displayMainMenu();
void displayProductMenu();
void displaySalesMenu();
void displayMemberMenu();
void displayHelpDoc();
void displayAdminMenu();
void displayAbout();
void displayHeader(const char* title);
void displayFooter();
void displayError(const char* message);
void displaySuccess(const char* message);
void displayWarning(const char* message);

#endif // UI_H
