#ifndef AUTH_H
#define AUTH_H

#include "common.h"

// 用户认证函数声明
int userLogin(char *username, char *password);
void changePassword(char *username, char *newPassword);
int validateUser(char *username, char *password);
int addUser(User user);
int deleteUser(char *username);

#endif // AUTH_H
