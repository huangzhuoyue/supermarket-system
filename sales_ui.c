#define _CRT_SECURE_NO_WARNINGS
#include "sales_ui.h"
#include "sales.h"
#include "product.h"
#include "ui.h"

// 全局购物车
static ShoppingCart current_cart;

// 销售管理模块函数声明
void newCartUI();
void viewCartUI();
void checkoutUI();
void processReturnUI();
void queryTransactionUI();

/**
 * 销售管理主函数
 */
void salesManagement() {
    int choice;
    
    do {
        displaySalesMenu();
        scanf("%d", &choice);
        
        // 清除输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        switch (choice) {
            case 1:
                newCartUI();
                break;
            case 2:
                viewCartUI();
                break;
            case 3:
                checkoutUI();
                break;
            case 4:
                processReturnUI();
                break;
            case 5:
                queryTransactionUI();
                break;
            case 0:
                break;
            default:
                displayError("无效的选择，请重新输入！");
                pauseScreen();
        }
    } while (choice != 0);
}

/**
 * 新建购物车界面
 */
void newCartUI() {
    displayHeader("新建购物车");
    
    // 确认是否清空当前购物车
    if (current_cart.item_count > 0) {
        printf("当前购物车中有 %d 种商品，是否清空？(y/n): ", current_cart.item_count);
        char confirm;
        scanf("%c", &confirm);
        
        // 清除输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        if (confirm != 'y' && confirm != 'Y') {
            printf("已取消操作！\n");
            pauseScreen();
            return;
        }
    }
    
    // 初始化购物车
    initShoppingCart(&current_cart);
    displaySuccess("购物车已初始化！");
    
    // 添加商品到购物车
    char continue_add = 'y';
    while (continue_add == 'y' || continue_add == 'Y') {
        // 显示所有商品
        displayHeader("添加商品到购物车");
        displayAllProducts();
        
        // 输入商品ID
        char product_id[10];
        printf("\n请输入要添加的商品ID (输入0返回): ");
        scanf("%9s", product_id);
        
        if (strcmp(product_id, "0") == 0) {
            break;
        }
        
        // 查找商品
        Product *product = findProductById(product_id);
        if (!product) {
            displayError("商品不存在！");
            pauseScreen();
            continue;
        }
        
        // 显示商品信息
        printf("\n商品信息：\n");
        displayProduct(*product);
        
        // 输入数量
        int quantity;
        printf("\n请输入购买数量: ");
        scanf("%d", &quantity);
        
        // 清除输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // 添加到购物车
        int result = addToCart(&current_cart, product_id, quantity);
        if (result == 1) {
            displaySuccess("商品已添加到购物车！");
        } else if (result == -1) {
            displayError("库存不足！");
        } else if (result == -2) {
            displayError("购物车已满！");
        } else {
            displayError("添加失败！");
        }
        
        // 显示购物车
        printf("\n当前购物车：\n");
        displayCart(&current_cart);
        
        // 询问是否继续添加
        printf("\n是否继续添加商品？(y/n): ");
        scanf("%c", &continue_add);
        
        // 清除输入缓冲区
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    pauseScreen();
}

/**
 * 查看购物车界面
 */
void viewCartUI() {
    displayHeader("查看购物车");
    
    if (current_cart.item_count <= 0) {
        displayError("购物车为空！");
        pauseScreen();
        return;
    }
    
    displayCart(&current_cart);
    
    printf("\n操作选项：\n");
    printf("1. 修改商品数量\n");
    printf("2. 移除商品\n");
    printf("3. 清空购物车\n");
    printf("0. 返回\n");
    
    int choice;
    printf("\n请选择操作 (0-3): ");
    scanf("%d", &choice);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    switch (choice) {
        case 1: {
            int index;
            printf("请输入要修改的商品序号: ");
            scanf("%d", &index);
            
            // 清除输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            if (index < 1 || index > current_cart.item_count) {
                displayError("无效的商品序号！");
                break;
            }
            
            int quantity;
            printf("请输入新的数量: ");
            scanf("%d", &quantity);
            
            // 清除输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            int result = updateCartItemQuantity(&current_cart, index - 1, quantity);
            if (result == 1) {
                displaySuccess("商品数量已更新！");
            } else if (result == -1) {
                displayError("库存不足！");
            } else {
                displayError("更新失败！");
            }
            break;
        }
        case 2: {
            int index;
            printf("请输入要移除的商品序号: ");
            scanf("%d", &index);
            
            // 清除输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            if (index < 1 || index > current_cart.item_count) {
                displayError("无效的商品序号！");
                break;
            }
            
            if (removeFromCart(&current_cart, index - 1)) {
                displaySuccess("商品已从购物车移除！");
            } else {
                displayError("移除失败！");
            }
            break;
        }
        case 3:
            clearCart(&current_cart);
            displaySuccess("购物车已清空！");
            break;
        case 0:
            return;
        default:
            displayError("无效的选择！");
    }
    
    pauseScreen();
}

/**
 * 结账界面
 */
void checkoutUI() {
    displayHeader("结账");
    
    if (current_cart.item_count <= 0) {
        displayError("购物车为空，无法结账！");
        pauseScreen();
        return;
    }
    
    // 显示购物车
    displayCart(&current_cart);
    
    // 询问是否是会员
    char is_member;
    printf("\n是否是会员？(y/n): ");
    scanf("%c", &is_member);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    char member_id[10] = "NONE";
    int points_used = 0;
    char coupon_id[10] = "";
    
    if (is_member == 'y' || is_member == 'Y') {
        printf("请输入会员ID: ");
        scanf("%9s", member_id);
        
        // 清除输入缓冲区
        while ((c = getchar()) != '\n' && c != EOF);
        
        // 查找会员
        int found = 0;
        for (int i = 0; i < member_count; i++) {
            if (strcmp(members[i].id, member_id) == 0) {
                found = 1;
                printf("\n会员信息：\n");
                printf("会员ID: %s\n", members[i].id);
                printf("会员姓名: %s\n", members[i].name);
                printf("会员等级: %d\n", members[i].level);
                printf("当前积分: %d\n", members[i].points);
                
                // 计算折扣
                calculateDiscount(&current_cart, member_id);
                printf("\n折扣后金额: %.2f\n", current_cart.final_amount);
                
                // 询问是否使用积分
                char use_points;
                printf("\n是否使用积分？(y/n): ");
                scanf("%c", &use_points);
                
                // 清除输入缓冲区
                while ((c = getchar()) != '\n' && c != EOF);
                
                if (use_points == 'y' || use_points == 'Y') {
                    printf("请输入要使用的积分 (当前积分: %d): ", members[i].points);
                    scanf("%d", &points_used);
                    
                    // 清除输入缓冲区
                    while ((c = getchar()) != '\n' && c != EOF);
                    
                    if (points_used > members[i].points) {
                        displayError("积分不足！");
                        points_used = 0;
                    } else {
                        float points_discount = (float)points_used / POINTS_FOR_COUPON * COUPON_VALUE;
                        printf("使用 %d 积分可抵扣 %.2f 元\n", points_used, points_discount);
                    }
                }
                
                // 询问是否使用优惠券
                char use_coupon;
                printf("\n是否使用优惠券？(y/n): ");
                scanf("%c", &use_coupon);
                
                // 清除输入缓冲区
                while ((c = getchar()) != '\n' && c != EOF);
                
                if (use_coupon == 'y' || use_coupon == 'Y') {
                    // 显示可用优惠券
                    printf("\n可用优惠券：\n");
                    printf("%-10s %-10s %-8s %-12s\n", "优惠券ID", "会员ID", "金额", "过期日期");
                    printf("------------------------------------------\n");
                    
                    char current_date[12];
                    getCurrentDate(current_date);
                    int has_coupon = 0;
                    
                    for (int j = 0; j < coupon_count; j++) {
                        if (strcmp(coupons[j].member_id, member_id) == 0 && 
                            coupons[j].is_used == 0 && 
                            strcmp(current_date, coupons[j].expire_date) <= 0) {
                            
                            printf("%-10s %-10s %-8.2f %-12s\n", 
                                   coupons[j].id, 
                                   coupons[j].member_id, 
                                   coupons[j].amount, 
                                   coupons[j].expire_date);
                            has_coupon = 1;
                        }
                    }
                    
                    if (has_coupon) {
                        printf("\n请输入要使用的优惠券ID: ");
                        scanf("%9s", coupon_id);
                        
                        // 清除输入缓冲区
                        while ((c = getchar()) != '\n' && c != EOF);
                    } else {
                        printf("没有可用的优惠券！\n");
                    }
                }
                
                break;
            }
        }
        
        if (!found) {
            displayError("会员不存在！");
            strcpy(member_id, "NONE");
        }
    }
    
    // 确认结账
    printf("\n确认结账？(y/n): ");
    char confirm;
    scanf("%c", &confirm);
    
    // 清除输入缓冲区
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (confirm != 'y' && confirm != 'Y') {
        printf("已取消结账！\n");
        pauseScreen();
        return;
    }
    
    // 执行结账
    Transaction *transaction = checkout(&current_cart, member_id, points_used, coupon_id);
    if (transaction) {
        displaySuccess("结账成功！");
        printf("\n交易信息：\n");
        displayTransaction(*transaction);
    } else {
        displayError("结账失败！");
    }
    
    pauseScreen();
}

/**
 * 退货处理界面
 */
void processReturnUI() {
    displayHeader("退货处理");
    
    char transaction_id[20];
    printf("请输入交易ID: ");
    scanf("%19s", transaction_id);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // 加载交易记录
    Transaction *transactions;
    int transaction_count;
    if (!loadTransactions(&transactions, &transaction_count)) {
        displayError("加载交易记录失败！");
        pauseScreen();
        return;
    }
    
    // 查找交易记录
    Transaction *transaction = NULL;
    for (int i = 0; i < transaction_count; i++) {
        if (strcmp(transactions[i].id, transaction_id) == 0) {
            transaction = &transactions[i];
            break;
        }
    }
    
    if (!transaction) {
        displayError("交易记录不存在！");
        free(transactions);
        pauseScreen();
        return;
    }
    
    // 显示交易信息
    printf("\n交易信息：\n");
    displayTransaction(*transaction);
    
    // 确认退货
    printf("\n确认退货？(y/n): ");
    char confirm;
    scanf("%c", &confirm);
    
    // 清除输入缓冲区
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (confirm != 'y' && confirm != 'Y') {
        printf("已取消退货！\n");
        free(transactions);
        pauseScreen();
        return;
    }
    
    // 执行退货
    if (processReturn(transaction_id)) {
        displaySuccess("退货成功！");
    } else {
        displayError("退货失败！");
    }
    
    free(transactions);
    pauseScreen();
}

/**
 * 查询交易记录界面
 */
void queryTransactionUI() {
    displayHeader("查询交易记录");
    
    printf("1. 按交易ID查询\n");
    printf("2. 按会员ID查询\n");
    printf("3. 显示所有交易\n");
    printf("0. 返回\n");
    
    int choice;
    printf("\n请选择查询方式 (0-3): ");
    scanf("%d", &choice);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // 加载交易记录
    Transaction *transactions;
    int transaction_count;
    if (!loadTransactions(&transactions, &transaction_count)) {
        displayError("加载交易记录失败！");
        pauseScreen();
        return;
    }
    
    switch (choice) {
        case 1: {
            char transaction_id[20];
            printf("\n请输入交易ID: ");
            scanf("%19s", transaction_id);
            
            // 清除输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            // 查找交易记录
            int found = 0;
            for (int i = 0; i < transaction_count; i++) {
                if (strcmp(transactions[i].id, transaction_id) == 0) {
                    printf("\n查询结果：\n");
                    displayTransaction(transactions[i]);
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                displayError("未找到该交易记录！");
            }
            break;
        }
        case 2: {
            char member_id[10];
            printf("\n请输入会员ID: ");
            scanf("%9s", member_id);
            
            // 清除输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            // 查找交易记录
            int found = 0;
            printf("\n查询结果：\n");
            printf("%-20s %-20s %-10s %-10s %-10s\n", 
                   "交易ID", "交易时间", "商品数", "总金额", "最终金额");
            printf("----------------------------------------------------------------------\n");
            
            for (int i = 0; i < transaction_count; i++) {
                if (strcmp(transactions[i].member_id, member_id) == 0) {
                    printf("%-20s %-20s %-10d %-10.2f %-10.2f\n", 
                           transactions[i].id, 
                           transactions[i].date_time, 
                           transactions[i].item_count, 
                           transactions[i].total_amount, 
                           transactions[i].final_amount);
                    found = 1;
                }
            }
            
            if (!found) {
                displayError("未找到该会员的交易记录！");
            }
            break;
        }
        case 3: {
            printf("\n所有交易记录：\n");
            printf("%-20s %-20s %-10s %-10s %-10s\n", 
                   "交易ID", "交易时间", "会员ID", "商品数", "最终金额");
            printf("----------------------------------------------------------------------\n");
            
            for (int i = 0; i < transaction_count; i++) {
                printf("%-20s %-20s %-10s %-10d %-10.2f\n", 
                       transactions[i].id, 
                       transactions[i].date_time, 
                       transactions[i].member_id, 
                       transactions[i].item_count, 
                       transactions[i].final_amount);
            }
            break;
        }
        case 0:
            free(transactions);
            return;
        default:
            displayError("无效的选择！");
    }
    
    free(transactions);
    pauseScreen();
}
