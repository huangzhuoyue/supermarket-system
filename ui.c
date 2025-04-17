#include "ui.h"

/**
 * 显示系统封面
 */
void displayCover() {
    clearScreen();
    printf("\n\n");
    printf("*******************************************************\n");
    printf("*                                                     *\n");
    printf("*                超市管理系统 v1.0                    *\n");
    printf("*                                                     *\n");
    printf("*                 C语言课程设计                       *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*                                                     *\n");
    printf("*******************************************************\n");
    printf("\n\n                按任意键继续...\n");
    getchar();
}

/**
 * 显示页面头部
 * @param title 页面标题
 */
void displayHeader(const char* title) {
    clearScreen();
    printf("=======================================================\n");
    printf("                    %s                    \n", title);
    printf("=======================================================\n\n");
}

/**
 * 显示页面底部
 */
void displayFooter() {
    printf("\n=======================================================\n");
}

/**
 * 显示错误信息
 * @param message 错误信息
 */
void displayError(const char* message) {
    printf("\n[错误] %s\n", message);
}

/**
 * 显示成功信息
 * @param message 成功信息
 */
void displaySuccess(const char* message) {
    printf("\n[成功] %s\n", message);
}

/**
 * 显示警告信息
 * @param message 警告信息
 */
void displayWarning(const char* message) {
    printf("\n[警告] %s\n", message);
}

/**
 * 显示主菜单
 */
void displayMainMenu() {
    displayHeader("主菜单");
    printf("1. 商品管理\n");
    printf("2. 销售管理\n");
    printf("3. 会员管理\n");
    printf("4. 系统管理\n");
    printf("5. 帮助文档\n");
    printf("6. 关于系统\n");
    printf("0. 退出系统\n");
    printf("\n请选择功能 (0-6): ");
}

/**
 * 显示商品管理菜单
 */
void displayProductMenu() {
    displayHeader("商品管理");
    printf("1. 添加商品\n");
    printf("2. 删除商品\n");
    printf("3. 修改商品\n");
    printf("4. 查询商品\n");
    printf("5. 按分类查询\n");
    printf("6. 按供应商查询\n");
    printf("7. 显示所有商品\n");
    printf("0. 返回主菜单\n");
    printf("\n请选择功能 (0-7): ");
}

/**
 * 显示销售管理菜单
 */
void displaySalesMenu() {
    displayHeader("销售管理");
    printf("1. 新建购物车\n");
    printf("2. 查看购物车\n");
    printf("3. 结算\n");
    printf("4. 退货处理\n");
    printf("5. 查询交易记录\n");
    printf("0. 返回主菜单\n");
    printf("\n请选择功能 (0-5): ");
}

/**
 * 显示会员管理菜单
 */
void displayMemberMenu() {
    displayHeader("会员管理");
    printf("1. 添加会员\n");
    printf("2. 查询会员\n");
    printf("3. 修改会员信息\n");
    printf("4. 积分兑换优惠券\n");
    printf("5. 显示所有会员\n");
    printf("0. 返回主菜单\n");
    printf("\n请选择功能 (0-5): ");
}

/**
 * 显示系统管理菜单
 */
void displayAdminMenu() {
    displayHeader("系统管理");
    printf("1. 修改密码\n");
    printf("2. 添加用户\n");
    printf("3. 删除用户\n");
    printf("4. 数据备份\n");
    printf("5. 数据恢复\n");
    printf("0. 返回主菜单\n");
    printf("\n请选择功能 (0-5): ");
}

/**
 * 显示帮助文档
 */
void displayHelpDoc() {
    displayHeader("帮助文档");
    
    printf("【系统概述】\n");
    printf("本系统是一个基于C语言开发的超市管理系统，主要包含商品管理、销售管理和会员管理三个核心模块。\n\n");
    
    printf("【功能说明】\n");
    printf("1. 商品管理：\n");
    printf("   - 支持商品信息的添加、删除、修改和查询\n");
    printf("   - 支持按分类或供应商筛选商品\n\n");
    
    printf("2. 销售管理：\n");
    printf("   - 支持购物车功能，可添加多种商品\n");
    printf("   - 支持会员折扣和积分累计\n");
    printf("   - 支持交易记录保存和查询\n");
    printf("   - 支持退货处理\n");
    printf("   - 销售后自动更新库存\n\n");
    
    printf("3. 会员管理：\n");
    printf("   - 支持会员信息的添加、查询和修改\n");
    printf("   - 支持积分累计和兑换优惠券\n");
    printf("   - 支持会员等级划分，不同等级享受不同折扣\n\n");
    
    printf("【操作指南】\n");
    printf("- 在各级菜单中，按数字键选择相应功能\n");
    printf("- 按0返回上一级菜单\n");
    printf("- 按提示输入相应信息\n");
    printf("- 在输入错误时，系统会给出相应提示\n\n");
    
    printf("【注意事项】\n");
    printf("- 请妥善保管管理员账号和密码\n");
    printf("- 定期备份数据，避免数据丢失\n");
    printf("- 退出系统前请确保所有操作已完成\n\n");
    
    printf("\n按回车键返回主菜单...");
    getchar();
}

/**
 * 显示关于系统
 */
void displayAbout() {
    displayHeader("关于系统");
    
    printf("【系统名称】超市管理系统 v1.0\n\n");
    
    printf("【开发环境】C语言\n\n");
    
    printf("【系统功能】\n");
    printf("- 商品管理\n");
    printf("- 销售管理\n");
    printf("- 会员管理\n\n");
    
    printf("【开发日期】2025年4月\n\n");
    
    printf("【版权声明】本系统仅用于C语言课程设计，不得用于商业用途\n\n");
    
    printf("\n按回车键返回主菜单...");
    getchar();
}
