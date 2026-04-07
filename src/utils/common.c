#define _CRT_SECURE_NO_WARNINGS
#include "common.h"

// 全局变量定义
Product products[MAX_PRODUCTS];
int product_count = 0;
Member members[MAX_MEMBERS];
int member_count = 0;
User users[MAX_USERS];
int user_count = 0;
Coupon coupons[MAX_COUPONS];
int coupon_count = 0;
int current_user_role = 0;

/**
 * 清屏函数
 */
void clearScreen() {
    system("cls || clear");
}

/**
 * 暂停屏幕显示
 */
void pauseScreen() {
    printf("\n按回车键继续...");
    getchar();
    // 处理可能的多余输入
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * 获取当前日期
 * @param date_str 用于存储日期的字符串，格式为YYYY-MM-DD
 * @return 日期字符串
 */
char* getCurrentDate(char* date_str) {
    time_t now;
    struct tm *local;
    
    time(&now);
    local = localtime(&now);
    
    sprintf(date_str, "%04d-%02d-%02d", 
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    
    return date_str;
}

/**
 * 获取当前日期和时间
 * @param datetime_str 用于存储日期时间的字符串，格式为YYYY-MM-DD HH:MM:SS
 * @return 日期时间字符串
 */
char* getCurrentDateTime(char* datetime_str) {
    time_t now;
    struct tm *local;
    
    time(&now);
    local = localtime(&now);
    
    sprintf(datetime_str, "%04d-%02d-%02d %02d:%02d:%02d", 
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
            local->tm_hour, local->tm_min, local->tm_sec);
    
    return datetime_str;
}

/**
 * 生成交易ID
 * @param id_str 用于存储交易ID的字符串，格式为YYYYMMDD-NNNN
 * @return 成功返回1，失败返回0
 */
int generateTransactionId(char* id_str) {
    time_t now;
    struct tm *local;
    static int counter = 1;
    
    time(&now);
    local = localtime(&now);
    
    sprintf(id_str, "%04d%02d%02d-%04d", 
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, counter++);
    
    return 1;
}

/**
 * 判断是否为闰年
 * @param year 年份
 * @return 是闰年返回1，否则返回0
 */
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * 获取指定年月的天数
 * @param year 年份
 * @param month 月份
 * @return 该月的天数
 */
int getDaysInMonth(int year, int month) {
    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    
    return days[month];
}

/**
 * 验证日期格式是否正确
 * @param date_str 日期字符串，格式为YYYY-MM-DD
 * @return 格式正确返回1，否则返回0
 */
int isValidDate(const char* date_str) {
    int year, month, day;
    
    if (sscanf(date_str, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0;
    }
    
    if (year < 1900 || year > 2100) {
        return 0;
    }
    
    if (month < 1 || month > 12) {
        return 0;
    }
    
    if (day < 1 || day > getDaysInMonth(year, month)) {
        return 0;
    }
    
    return 1;
}

/**
 * 计算两个日期之间的天数差
 * @param date1 第一个日期，格式为YYYY-MM-DD
 * @param date2 第二个日期，格式为YYYY-MM-DD
 * @return 天数差，如果日期格式错误返回-1
 */
int dateDiff(const char* date1, const char* date2) {
    // 简化实现，实际应该使用更复杂的算法
    // 这里只是一个示例，不考虑闰年等因素
    int year1, month1, day1;
    int year2, month2, day2;
    
    if (sscanf(date1, "%d-%d-%d", &year1, &month1, &day1) != 3 ||
        sscanf(date2, "%d-%d-%d", &year2, &month2, &day2) != 3) {
        return -1;
    }
    
    // 简化计算，假设每月30天
    int days1 = year1 * 365 + month1 * 30 + day1;
    int days2 = year2 * 365 + month2 * 30 + day2;
    
    return days2 - days1;
}

/**
 * 去除字符串首尾的空白字符
 * @param str 需要处理的字符串
 */
void trim(char* str) {
    if (str == NULL) {
        return;
    }
    
    // 去除尾部空白
    int len = strlen(str);
    while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\t' || 
           str[len-1] == '\n' || str[len-1] == '\r')) {
        str[--len] = '\0';
    }
    
    // 去除首部空白
    char* p = str;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
        p++;
    }
    
    if (p != str) {
        memmove(str, p, len - (p - str) + 1);
    }
}
