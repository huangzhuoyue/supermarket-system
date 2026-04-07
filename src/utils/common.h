#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 定义常量
#define MAX_PRODUCTS 1000
#define MAX_MEMBERS 500
#define MAX_CART_ITEMS 100
#define MAX_TRANSACTION_ITEMS 100
#define MAX_USERS 20
#define MAX_COUPONS 200

// 定义会员等级
#define MEMBER_LEVEL_NORMAL 1  // 普通会员
#define MEMBER_LEVEL_SILVER 2  // 银卡会员
#define MEMBER_LEVEL_GOLD 3    // 金卡会员

// 定义折扣率
#define DISCOUNT_NORMAL 0.98   // 普通会员98折
#define DISCOUNT_SILVER 0.95   // 银卡会员95折
#define DISCOUNT_GOLD 0.90     // 金卡会员9折

// 定义积分兑换比例
#define POINTS_PER_YUAN 1      // 每消费1元获得1积分
#define POINTS_FOR_COUPON 100  // 100积分兑换10元优惠券
#define COUPON_VALUE 10        // 优惠券面值10元

// 定义会员等级门槛
#define SILVER_THRESHOLD 1000  // 累计消费1000元升级为银卡会员
#define GOLD_THRESHOLD 5000    // 累计消费5000元升级为金卡会员

// 定义库存预警阈值
#define STOCK_WARNING_THRESHOLD 10  // 库存低于10个触发预警

// 定义文件路径
#define USERS_FILE "users.txt"
#define PRODUCTS_FILE "products.txt"
#define MEMBERS_FILE "members.txt"
#define TRANSACTIONS_FILE "transactions.txt"
#define TRANSACTION_DETAILS_FILE "transaction_details.txt"
#define COUPONS_FILE "coupons.txt"

// 商品结构体
typedef struct {
    char id[10];           // 商品ID
    char name[50];         // 商品名称
    char category[20];     // 商品分类
    char supplier[50];     // 供应商
    float price;           // 单价
    int stock;             // 库存数量
    char production_date[12]; // 生产日期 (YYYY-MM-DD)
    int shelf_life;        // 保质期(天)
} Product;

// 会员结构体
typedef struct {
    char id[10];           // 会员ID
    char name[50];         // 会员姓名
    char phone[15];        // 联系电话
    char reg_date[12];     // 注册日期 (YYYY-MM-DD)
    float total_amount;    // 累计消费金额
    int points;            // 当前积分
    int level;             // 会员等级 (1-普通, 2-银卡, 3-金卡)
} Member;

// 购物车项结构体
typedef struct {
    char product_id[10];   // 商品ID
    char product_name[50]; // 商品名称
    float price;           // 单价
    int quantity;          // 数量
    float subtotal;        // 小计金额
} CartItem;

// 购物车结构体
typedef struct {
    CartItem items[MAX_CART_ITEMS];   // 购物车项数组
    int item_count;        // 购物车中的商品数量
    float total_amount;    // 总金额
    float discount_amount; // 折扣金额
    float final_amount;    // 最终金额
} ShoppingCart;

// 交易记录结构体
typedef struct {
    char id[20];           // 交易ID (格式: YYYYMMDD-NNNN)
    char date_time[20];    // 交易日期时间
    char member_id[10];    // 会员ID (非会员为"NONE")
    int item_count;        // 商品种类数量
    float total_amount;    // 总金额
    float discount_amount; // 折扣金额
    float final_amount;    // 最终金额
    int points_earned;     // 获得积分
    int points_used;       // 使用积分
    int coupon_used;       // 使用优惠券金额
} Transaction;

// 交易明细结构体
typedef struct {
    char transaction_id[20]; // 交易ID
    char product_id[10];     // 商品ID
    char product_name[50];   // 商品名称
    float price;             // 单价
    int quantity;            // 数量
    float subtotal;          // 小计金额
} TransactionDetail;

// 优惠券结构体
typedef struct {
    char id[10];           // 优惠券ID
    char member_id[10];    // 会员ID
    float amount;          // 优惠金额
    char expire_date[12];  // 过期日期 (YYYY-MM-DD)
    int is_used;           // 是否已使用 (0-未使用, 1-已使用)
} Coupon;

// 用户结构体
typedef struct {
    char username[20];     // 用户名
    char password[20];     // 密码
    int role;              // 角色 (1-管理员, 2-普通用户)
} User;

// 全局变量声明
extern Product products[MAX_PRODUCTS];
extern int product_count;
extern Member members[MAX_MEMBERS];
extern int member_count;
extern User users[MAX_USERS];
extern int user_count;
extern Coupon coupons[MAX_COUPONS];
extern int coupon_count;
extern int current_user_role;

// 通用函数声明
void clearScreen();
void pauseScreen();
char* getCurrentDate(char* date_str);
char* getCurrentDateTime(char* datetime_str);
int generateTransactionId(char* id_str);
int isLeapYear(int year);
int getDaysInMonth(int year, int month);
int isValidDate(const char* date_str);
int dateDiff(const char* date1, const char* date2);
void trim(char* str);

#endif // COMMON_H
