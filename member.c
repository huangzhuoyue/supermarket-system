#include "member.h"

/**
 * 添加会员
 * @param member 会员信息
 * @return 成功返回1，失败返回0
 */
int addMember(Member member) {
    // 检查会员ID是否已存在
    if (isMemberIdExists(member.id)) {
        return 0;
    }
    
    // 检查手机号是否已存在
    if (isMemberPhoneExists(member.phone)) {
        return -1;
    }
    
    // 检查是否超出最大会员数
    if (member_count >= MAX_MEMBERS) {
        return -2;
    }
    
    // 添加会员
    strcpy(members[member_count].id, member.id);
    strcpy(members[member_count].name, member.name);
    strcpy(members[member_count].phone, member.phone);
    strcpy(members[member_count].reg_date, member.reg_date);
    members[member_count].total_amount = member.total_amount;
    members[member_count].points = member.points;
    members[member_count].level = member.level;
    
    member_count++;
    
    // 保存会员数据
    saveMembers();
    
    return 1;
}

/**
 * 更新会员信息
 * @param member 会员信息
 * @return 成功返回1，失败返回0
 */
int updateMember(Member member) {
    // 查找会员
    for (int i = 0; i < member_count; i++) {
        if (strcmp(members[i].id, member.id) == 0) {
            // 检查手机号是否已被其他会员使用
            if (strcmp(members[i].phone, member.phone) != 0) {
                for (int j = 0; j < member_count; j++) {
                    if (j != i && strcmp(members[j].phone, member.phone) == 0) {
                        return -1;  // 手机号已被其他会员使用
                    }
                }
            }
            
            // 更新会员信息
            strcpy(members[i].name, member.name);
            strcpy(members[i].phone, member.phone);
            // 注册日期不允许修改
            members[i].total_amount = member.total_amount;
            members[i].points = member.points;
            members[i].level = member.level;
            
            saveMembers();
            return 1;
        }
    }
    
    return 0;  // 会员不存在
}

/**
 * 根据ID查找会员
 * @param id 会员ID
 * @return 成功返回会员指针，失败返回NULL
 */
Member* findMemberById(char *id) {
    for (int i = 0; i < member_count; i++) {
        if (strcmp(members[i].id, id) == 0) {
            return &members[i];
        }
    }
    
    return NULL;
}

/**
 * 根据手机号查找会员
 * @param phone 手机号
 * @return 成功返回会员指针，失败返回NULL
 */
Member* findMemberByPhone(char *phone) {
    for (int i = 0; i < member_count; i++) {
        if (strcmp(members[i].phone, phone) == 0) {
            return &members[i];
        }
    }
    
    return NULL;
}

/**
 * 增加会员积分
 * @param member_id 会员ID
 * @param points 积分
 * @return 成功返回1，失败返回0
 */
int addPoints(char *member_id, int points) {
    if (points <= 0) {
        return 0;
    }
    
    // 查找会员
    for (int i = 0; i < member_count; i++) {
        if (strcmp(members[i].id, member_id) == 0) {
            // 增加积分
            members[i].points += points;
            
            saveMembers();
            return 1;
        }
    }
    
    return 0;  // 会员不存在
}

/**
 * 使用会员积分
 * @param member_id 会员ID
 * @param points 积分
 * @return 成功返回1，失败返回0
 */
int usePoints(char *member_id, int points) {
    if (points <= 0) {
        return 0;
    }
    
    // 查找会员
    for (int i = 0; i < member_count; i++) {
        if (strcmp(members[i].id, member_id) == 0) {
            // 检查积分是否足够
            if (members[i].points < points) {
                return -1;  // 积分不足
            }
            
            // 使用积分
            members[i].points -= points;
            
            saveMembers();
            return 1;
        }
    }
    
    return 0;  // 会员不存在
}

/**
 * 更新会员等级
 * @param member_id 会员ID
 * @return 成功返回1，失败返回0
 */
int updateMemberLevel(char *member_id) {
    // 查找会员
    for (int i = 0; i < member_count; i++) {
        if (strcmp(members[i].id, member_id) == 0) {
            // 根据累计消费金额更新会员等级
            int old_level = members[i].level;
            
            if (members[i].total_amount >= GOLD_THRESHOLD) {
                members[i].level = MEMBER_LEVEL_GOLD;
            } else if (members[i].total_amount >= SILVER_THRESHOLD) {
                members[i].level = MEMBER_LEVEL_SILVER;
            } else {
                members[i].level = MEMBER_LEVEL_NORMAL;
            }
            
            if (old_level != members[i].level) {
                saveMembers();
                return 1;
            }
            
            return 0;  // 等级未变化
        }
    }
    
    return -1;  // 会员不存在
}

/**
 * 创建优惠券
 * @param member_id 会员ID
 * @param amount 优惠券金额
 * @return 成功返回1，失败返回0
 */
int createCoupon(char *member_id, float amount) {
    if (amount <= 0) {
        return 0;
    }
    
    // 检查会员是否存在
    int member_exists = 0;
    for (int i = 0; i < member_count; i++) {
        if (strcmp(members[i].id, member_id) == 0) {
            member_exists = 1;
            break;
        }
    }
    
    if (!member_exists) {
        return -1;  // 会员不存在
    }
    
    // 检查是否超出最大优惠券数
    if (coupon_count >= MAX_COUPONS) {
        return -2;
    }
    
    // 生成优惠券ID
    sprintf(coupons[coupon_count].id, "C%03d", coupon_count + 1);
    
    // 设置优惠券信息
    strcpy(coupons[coupon_count].member_id, member_id);
    coupons[coupon_count].amount = amount;
    
    // 设置过期日期（当前日期+30天）
    char current_date[12];
    getCurrentDate(current_date);
    
    // 简化处理，直接加30天
    int year, month, day;
    sscanf(current_date, "%d-%d-%d", &year, &month, &day);
    
    day += 30;
    while (day > getDaysInMonth(year, month)) {
        day -= getDaysInMonth(year, month);
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }
    
    sprintf(coupons[coupon_count].expire_date, "%04d-%02d-%02d", year, month, day);
    coupons[coupon_count].is_used = 0;
    
    coupon_count++;
    
    // 保存优惠券数据
    saveCoupons();
    
    return 1;
}

/**
 * 使用优惠券
 * @param coupon_id 优惠券ID
 * @return 成功返回1，失败返回0
 */
int useCoupon(char *coupon_id) {
    // 查找优惠券
    for (int i = 0; i < coupon_count; i++) {
        if (strcmp(coupons[i].id, coupon_id) == 0) {
            // 检查优惠券是否已使用
            if (coupons[i].is_used) {
                return -1;  // 优惠券已使用
            }
            
            // 检查优惠券是否过期
            char current_date[12];
            getCurrentDate(current_date);
            if (strcmp(current_date, coupons[i].expire_date) > 0) {
                return -2;  // 优惠券已过期
            }
            
            // 使用优惠券
            coupons[i].is_used = 1;
            
            saveCoupons();
            return 1;
        }
    }
    
    return 0;  // 优惠券不存在
}

/**
 * 显示会员信息
 * @param member 会员信息
 */
void displayMember(Member member) {
    printf("会员ID: %s\n", member.id);
    printf("会员姓名: %s\n", member.name);
    printf("联系电话: %s\n", member.phone);
    printf("注册日期: %s\n", member.reg_date);
    printf("累计消费: %.2f\n", member.total_amount);
    printf("当前积分: %d\n", member.points);
    
    printf("会员等级: ");
    switch (member.level) {
        case MEMBER_LEVEL_NORMAL:
            printf("普通会员 (%.0f%%折扣)\n", DISCOUNT_NORMAL * 100);
            break;
        case MEMBER_LEVEL_SILVER:
            printf("银卡会员 (%.0f%%折扣)\n", DISCOUNT_SILVER * 100);
            break;
        case MEMBER_LEVEL_GOLD:
            printf("金卡会员 (%.0f%%折扣)\n", DISCOUNT_GOLD * 100);
            break;
        default:
            printf("未知\n");
    }
}

/**
 * 显示所有会员
 */
void displayAllMembers() {
    if (member_count == 0) {
        printf("没有会员记录！\n");
        return;
    }
    
    printf("%-10s %-20s %-15s %-12s %-10s %-10s %-10s\n", 
           "会员ID", "会员姓名", "联系电话", "注册日期", "累计消费", "当前积分", "会员等级");
    printf("---------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < member_count; i++) {
        char level_str[10];
        switch (members[i].level) {
            case MEMBER_LEVEL_NORMAL:
                strcpy(level_str, "普通");
                break;
            case MEMBER_LEVEL_SILVER:
                strcpy(level_str, "银卡");
                break;
            case MEMBER_LEVEL_GOLD:
                strcpy(level_str, "金卡");
                break;
            default:
                strcpy(level_str, "未知");
        }
        
        printf("%-10s %-20s %-15s %-12s %-10.2f %-10d %-10s\n", 
               members[i].id, 
               members[i].name, 
               members[i].phone, 
               members[i].reg_date, 
               members[i].total_amount, 
               members[i].points, 
               level_str);
    }
}

/**
 * 检查会员ID是否已存在
 * @param id 会员ID
 * @return 存在返回1，不存在返回0
 */
int isMemberIdExists(char *id) {
    for (int i = 0; i < member_count; i++) {
        if (strcmp(members[i].id, id) == 0) {
            return 1;
        }
    }
    
    return 0;
}

/**
 * 检查手机号是否已存在
 * @param phone 手机号
 * @return 存在返回1，不存在返回0
 */
int isMemberPhoneExists(char *phone) {
    for (int i = 0; i < member_count; i++) {
        if (strcmp(members[i].phone, phone) == 0) {
            return 1;
        }
    }
    
    return 0;
}
