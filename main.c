#define _CRT_SECURE_NO_WARNINGS
#include "common.h"
#include "file_utils.h"
#include "ui.h"
#include "auth.h"
#include "product_ui.h"
#include "sales_ui.h"
#include "member_ui.h"

// 前向声明
void mainMenuLoop();
void adminMenuLoop();

/**
 * 主函数
 */
int main() {
    // 初始化数据文件
    if (!initializeDataFiles()) {
        printf("初始化数据文件失败，请检查文件权限！\n");
        return 1;
    }
    
    // 加载数据
    loadUsers();
    loadProducts();
    loadMembers();
    loadCoupons();
    
    // 显示系统封面
    displayCover();
    
    // 用户登录
    char username[20];
    char password[20];
    int loginSuccess = 0;
    int attempts = 0;
    
    while (!loginSuccess && attempts < 3) {
        displayHeader("用户登录");
        
        printf("用户名: ");
        scanf("%19s", username);
        
        printf("密  码: ");
        scanf("%19s", password);
        
        // 清除输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        int role = userLogin(username, password);
        if (role > 0) {
            loginSuccess = 1;
            printf("\n登录成功！欢迎 %s\n", username);
            pauseScreen();
        } else {
            attempts++;
            printf("\n用户名或密码错误，请重试！（还剩 %d 次机会）\n", 3 - attempts);
            pauseScreen();
        }
    }
    
    if (!loginSuccess) {
        printf("\n登录失败次数过多，系统将退出！\n");
        return 1;
    }
    
    // 进入主菜单循环
    mainMenuLoop();
    
    return 0;
}

/**
 * 主菜单循环
 */
void mainMenuLoop() {
    int choice;
    
    do {
        displayMainMenu();
        scanf("%d", &choice);
        
        // 清除输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        switch (choice) {
            case 1:
                productManagement();
                break;
            case 2:
                salesManagement();
                break;
            case 3:
                memberManagement();
                break;
            case 4:
                if (current_user_role == 1) { // 只有管理员可以进入系统管理
                    adminMenuLoop();
                } else {
                    displayError("您没有权限访问系统管理功能！");
                    pauseScreen();
                }
                break;
            case 5:
                displayHelpDoc();
                break;
            case 6:
                displayAbout();
                break;
            case 0:
                displayHeader("退出系统");
                printf("感谢使用超市管理系统，再见！\n");
                pauseScreen();
                break;
            default:
                displayError("无效的选择，请重新输入！");
                pauseScreen();
        }
    } while (choice != 0);
}







/**
 * 系统管理菜单循环
 * 注意：此处只是框架，具体功能将在后续实现
 */
void adminMenuLoop() {
    int choice;
    
    do {
        displayAdminMenu();
        scanf("%d", &choice);
        
        // 清除输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        switch (choice) {
            case 1:
                displayHeader("修改密码");
                printf("此功能将在系统管理模块中实现\n");
                pauseScreen();
                break;
            case 2:
                displayHeader("添加用户");
                printf("此功能将在系统管理模块中实现\n");
                pauseScreen();
                break;
            case 3:
                displayHeader("删除用户");
                printf("此功能将在系统管理模块中实现\n");
                pauseScreen();
                break;
            case 4:
                displayHeader("数据备份");
                printf("此功能将在系统管理模块中实现\n");
                pauseScreen();
                break;
            case 5:
                displayHeader("数据恢复");
                printf("此功能将在系统管理模块中实现\n");
                pauseScreen();
                break;
            case 0:
                break;
            default:
                displayError("无效的选择，请重新输入！");
                pauseScreen();
        }
    } while (choice != 0);
}
