#define _CRT_SECURE_NO_WARNINGS
#include "auth.h"

/**
 * 验证用户登录
 * @param username 用户名
 * @param password 密码
 * @return 成功返回用户角色(1-管理员, 2-普通用户)，失败返回0
 */
int userLogin(char *username, char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0) {
            current_user_role = users[i].role;
            return users[i].role;
        }
    }
    return 0;
}

/**
 * 修改用户密码
 * @param username 用户名
 * @param newPassword 新密码
 * @return 成功返回1，失败返回0
 */
void changePassword(char *username, char *newPassword) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            strcpy(users[i].password, newPassword);
            saveUsers();
            return;
        }
    }
}

/**
 * 验证用户名和密码
 * @param username 用户名
 * @param password 密码
 * @return 成功返回1，失败返回0
 */
int validateUser(char *username, char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * 添加用户
 * @param user 用户信息
 * @return 成功返回1，失败返回0
 */
int addUser(User user) {
    // 检查用户名是否已存在
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, user.username) == 0) {
            return 0;
        }
    }
    
    // 检查是否超出最大用户数
    if (user_count >= MAX_USERS) {
        return 0;
    }
    
    // 添加用户
    strcpy(users[user_count].username, user.username);
    strcpy(users[user_count].password, user.password);
    users[user_count].role = user.role;
    user_count++;
    
    // 保存用户数据
    saveUsers();
    
    return 1;
}

/**
 * 删除用户
 * @param username 用户名
 * @return 成功返回1，失败返回0
 */
int deleteUser(char *username) {
    // 不允许删除admin用户
    if (strcmp(username, "admin") == 0) {
        return 0;
    }
    
    // 查找用户
    int found = 0;
    for (int i = 0; i < user_count; i++) {
        if (found) {
            // 移动后续用户
            strcpy(users[i-1].username, users[i].username);
            strcpy(users[i-1].password, users[i].password);
            users[i-1].role = users[i].role;
        } else if (strcmp(users[i].username, username) == 0) {
            found = 1;
        }
    }
    
    if (found) {
        user_count--;
        saveUsers();
        return 1;
    }
    
    return 0;
}
