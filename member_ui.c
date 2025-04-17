#define _CRT_SECURE_NO_WARNINGS
#include "member_ui.h"
#include "member.h"
#include "ui.h"

// 会员管理模块函数声明
void addMemberUI();
void queryMemberUI();
void updateMemberUI();
void exchangePointsUI();
void displayAllMembersUI();

/**
 * 会员管理主函数
 */
void memberManagement() {
    int choice;
    
    do {
        displayMemberMenu();
        scanf("%d", &choice);
        
        // 清除输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        switch (choice) {
            case 1:
                addMemberUI();
                break;
            case 2:
                queryMemberUI();
                break;
            case 3:
                updateMemberUI();
                break;
            case 4:
                exchangePointsUI();
                break;
            case 5:
                displayAllMembersUI();
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
 * 添加会员界面
 */
void addMemberUI() {
    displayHeader("添加会员");
    
    Member member;
    
    printf("请输入会员信息：\n");
    
    printf("会员ID (如M001): ");
    scanf("%9s", member.id);
    
    // 检查ID是否已存在
    if (isMemberIdExists(member.id)) {
        displayError("会员ID已存在！");
        pauseScreen();
        return;
    }
    
    printf("会员姓名: ");
    scanf("%49s", member.name);
    
    printf("联系电话: ");
    scanf("%14s", member.phone);
    
    // 检查手机号是否已存在
    if (isMemberPhoneExists(member.phone)) {
        displayError("该手机号已被注册！");
        pauseScreen();
        return;
    }
    
    // 设置注册日期为当前日期
    char date[12];
    getCurrentDate(date);
    strcpy(member.reg_date, date);
    
    // 初始化其他字段
    member.total_amount = 0.0;
    member.points = 0;
    member.level = MEMBER_LEVEL_NORMAL;
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // 添加会员
    int result = addMember(member);
    if (result == 1) {
        displaySuccess("会员添加成功！");
        printf("\n会员信息：\n");
        displayMember(member);
    } else if (result == -1) {
        displayError("该手机号已被注册！");
    } else if (result == -2) {
        displayError("会员数量已达上限！");
    } else {
        displayError("会员添加失败！");
    }
    
    pauseScreen();
}

/**
 * 查询会员界面
 */
void queryMemberUI() {
    displayHeader("查询会员");
    
    printf("1. 按ID查询\n");
    printf("2. 按手机号查询\n");
    printf("0. 返回\n");
    
    int choice;
    printf("\n请选择查询方式 (0-2): ");
    scanf("%d", &choice);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    switch (choice) {
        case 1: {
            char id[10];
            printf("\n请输入会员ID: ");
            scanf("%9s", id);
            
            // 清除输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            Member *member = findMemberById(id);
            if (member) {
                printf("\n查询结果：\n");
                displayMember(*member);
            } else {
                displayError("未找到该会员！");
            }
            break;
        }
        case 2: {
            char phone[15];
            printf("\n请输入手机号: ");
            scanf("%14s", phone);
            
            // 清除输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            Member *member = findMemberByPhone(phone);
            if (member) {
                printf("\n查询结果：\n");
                displayMember(*member);
            } else {
                displayError("未找到该会员！");
            }
            break;
        }
        case 0:
            return;
        default:
            displayError("无效的选择！");
    }
    
    pauseScreen();
}

/**
 * 更新会员信息界面
 */
void updateMemberUI() {
    displayHeader("修改会员信息");
    
    char id[10];
    printf("请输入要修改的会员ID: ");
    scanf("%9s", id);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // 查找会员
    Member *member = findMemberById(id);
    if (!member) {
        displayError("会员不存在！");
        pauseScreen();
        return;
    }
    
    // 显示会员信息
    printf("\n当前会员信息：\n");
    displayMember(*member);
    
    // 创建新的会员信息
    Member newMember;
    strcpy(newMember.id, id);
    strcpy(newMember.reg_date, member->reg_date);
    
    printf("\n请输入新的会员信息（直接回车保持不变）：\n");
    
    char buffer[50];
    
    printf("会员姓名 [%s]: ", member->name);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        strcpy(newMember.name, buffer);
    } else {
        strcpy(newMember.name, member->name);
    }
    
    printf("联系电话 [%s]: ", member->phone);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        strcpy(newMember.phone, buffer);
    } else {
        strcpy(newMember.phone, member->phone);
    }
    
    // 累计消费金额、积分和等级通常不直接修改，而是通过消费和积分兑换自动更新
    newMember.total_amount = member->total_amount;
    newMember.points = member->points;
    newMember.level = member->level;
    
    // 更新会员信息
    int result = updateMember(newMember);
    if (result == 1) {
        displaySuccess("会员信息修改成功！");
    } else if (result == -1) {
        displayError("该手机号已被其他会员使用！");
    } else {
        displayError("会员信息修改失败！");
    }
    
    pauseScreen();
}

/**
 * 积分兑换优惠券界面
 */
void exchangePointsUI() {
    displayHeader("积分兑换优惠券");
    
    char id[10];
    printf("请输入会员ID: ");
    scanf("%9s", id);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // 查找会员
    Member *member = findMemberById(id);
    if (!member) {
        displayError("会员不存在！");
        pauseScreen();
        return;
    }
    
    // 显示会员积分
    printf("\n会员信息：\n");
    printf("会员ID: %s\n", member->id);
    printf("会员姓名: %s\n", member->name);
    printf("当前积分: %d\n", member->points);
    
    // 检查积分是否足够
    if (member->points < POINTS_FOR_COUPON) {
        displayError("积分不足！兑换优惠券需要至少 %d 积分", POINTS_FOR_COUPON);
        pauseScreen();
        return;
    }
    
    // 计算可兑换的优惠券数量
    int max_coupons = member->points / POINTS_FOR_COUPON;
    printf("\n您最多可以兑换 %d 张 %d元 优惠券\n", max_coupons, COUPON_VALUE);
    
    // 输入兑换数量
    int count;
    printf("请输入要兑换的数量: ");
    scanf("%d", &count);
    
    // 清除输入缓冲区
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (count <= 0) {
        displayError("兑换数量必须大于0！");
        pauseScreen();
        return;
    }
    
    if (count > max_coupons) {
        displayError("积分不足！");
        pauseScreen();
        return;
    }
    
    // 确认兑换
    printf("\n将使用 %d 积分兑换 %d 张 %d元 优惠券，确认？(y/n): ", 
           count * POINTS_FOR_COUPON, count, COUPON_VALUE);
    char confirm;
    scanf("%c", &confirm);
    
    // 清除输入缓冲区
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (confirm != 'y' && confirm != 'Y') {
        printf("已取消兑换！\n");
        pauseScreen();
        return;
    }
    
    // 扣除积分
    int result = usePoints(id, count * POINTS_FOR_COUPON);
    if (result != 1) {
        displayError("积分扣除失败！");
        pauseScreen();
        return;
    }
    
    // 创建优惠券
    printf("\n创建的优惠券：\n");
    printf("%-10s %-10s %-8s %-12s\n", "优惠券ID", "会员ID", "金额", "过期日期");
    printf("------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        int coupon_result = createCoupon(id, COUPON_VALUE);
        if (coupon_result == 1) {
            // 显示最新创建的优惠券
            printf("%-10s %-10s %-8.2f %-12s\n", 
                   coupons[coupon_count-1].id, 
                   coupons[coupon_count-1].member_id, 
                   coupons[coupon_count-1].amount, 
                   coupons[coupon_count-1].expire_date);
        } else {
            displayError("优惠券创建失败！");
            break;
        }
    }
    
    displaySuccess("积分兑换成功！");
    pauseScreen();
}

/**
 * 显示所有会员界面
 */
void displayAllMembersUI() {
    displayHeader("所有会员列表");
    
    displayAllMembers();
    
    pauseScreen();
}
