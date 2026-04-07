#define _CRT_SECURE_NO_WARNINGS
#include "file_utils.h"

/**
 * 检查文件是否存在
 * @param filename 文件名
 * @return 存在返回1，不存在返回0
 */
int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

/**
 * 创建空文件
 * @param filename 文件名
 * @return 成功返回1，失败返回0
 */
int createEmptyFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

/**
 * 初始化数据文件
 * 如果文件不存在则创建
 * @return 成功返回1，失败返回0
 */
int initializeDataFiles() {
    int success = 1;
    
    // 检查并创建用户文件
    if (!fileExists(USERS_FILE)) {
        FILE *file = fopen(USERS_FILE, "w");
        if (file) {
            // 创建默认管理员账号
            fprintf(file, "admin,admin123,1\n");
            fclose(file);
        } else {
            success = 0;
        }
    }
    
    // 检查并创建其他数据文件
    if (!fileExists(PRODUCTS_FILE)) {
        success &= createEmptyFile(PRODUCTS_FILE);
    }
    
    if (!fileExists(MEMBERS_FILE)) {
        success &= createEmptyFile(MEMBERS_FILE);
    }
    
    if (!fileExists(TRANSACTIONS_FILE)) {
        success &= createEmptyFile(TRANSACTIONS_FILE);
    }
    
    if (!fileExists(TRANSACTION_DETAILS_FILE)) {
        success &= createEmptyFile(TRANSACTION_DETAILS_FILE);
    }
    
    if (!fileExists(COUPONS_FILE)) {
        success &= createEmptyFile(COUPONS_FILE);
    }
    
    return success;
}

/**
 * 加载用户数据
 * @return 成功返回1，失败返回0
 */
int loadUsers() {
    FILE *file = fopen(USERS_FILE, "r");
    if (!file) {
        return 0;
    }
    
    user_count = 0;
    char line[100];
    
    while (fgets(line, sizeof(line), file) && user_count < MAX_USERS) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;
        
        // 解析用户数据
        char *username = strtok(line, ",");
        char *password = strtok(NULL, ",");
        char *role_str = strtok(NULL, ",");
        
        if (username && password && role_str) {
            strcpy(users[user_count].username, username);
            strcpy(users[user_count].password, password);
            users[user_count].role = atoi(role_str);
            user_count++;
        }
    }
    
    fclose(file);
    return 1;
}

/**
 * 保存用户数据
 * @return 成功返回1，失败返回0
 */
int saveUsers() {
    FILE *file = fopen(USERS_FILE, "w");
    if (!file) {
        return 0;
    }
    
    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s,%s,%d\n", 
                users[i].username, 
                users[i].password, 
                users[i].role);
    }
    
    fclose(file);
    return 1;
}

/**
 * 加载商品数据
 * @return 成功返回1，失败返回0
 */
int loadProducts() {
    FILE *file = fopen(PRODUCTS_FILE, "r");
    if (!file) {
        return 0;
    }
    
    product_count = 0;
    char line[200];
    
    while (fgets(line, sizeof(line), file) && product_count < MAX_PRODUCTS) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;
        
        // 解析商品数据
        char *id = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *category = strtok(NULL, ",");
        char *supplier = strtok(NULL, ",");
        char *price_str = strtok(NULL, ",");
        char *stock_str = strtok(NULL, ",");
        char *production_date = strtok(NULL, ",");
        char *shelf_life_str = strtok(NULL, ",");
        
        if (id && name && category && supplier && price_str && stock_str) {
            strcpy(products[product_count].id, id);
            strcpy(products[product_count].name, name);
            strcpy(products[product_count].category, category);
            strcpy(products[product_count].supplier, supplier);
            products[product_count].price = atof(price_str);
            products[product_count].stock = atoi(stock_str);
            
            if (production_date) {
                strcpy(products[product_count].production_date, production_date);
            } else {
                strcpy(products[product_count].production_date, "");
            }
            
            if (shelf_life_str) {
                products[product_count].shelf_life = atoi(shelf_life_str);
            } else {
                products[product_count].shelf_life = 0;
            }
            
            product_count++;
        }
    }
    
    fclose(file);
    return 1;
}

/**
 * 保存商品数据
 * @return 成功返回1，失败返回0
 */
int saveProducts() {
    FILE *file = fopen(PRODUCTS_FILE, "w");
    if (!file) {
        return 0;
    }
    
    for (int i = 0; i < product_count; i++) {
        fprintf(file, "%s,%s,%s,%s,%.2f,%d,%s,%d\n", 
                products[i].id, 
                products[i].name, 
                products[i].category, 
                products[i].supplier, 
                products[i].price, 
                products[i].stock, 
                products[i].production_date, 
                products[i].shelf_life);
    }
    
    fclose(file);
    return 1;
}

/**
 * 加载会员数据
 * @return 成功返回1，失败返回0
 */
int loadMembers() {
    FILE *file = fopen(MEMBERS_FILE, "r");
    if (!file) {
        return 0;
    }
    
    member_count = 0;
    char line[200];
    
    while (fgets(line, sizeof(line), file) && member_count < MAX_MEMBERS) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;
        
        // 解析会员数据
        char *id = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *phone = strtok(NULL, ",");
        char *reg_date = strtok(NULL, ",");
        char *total_amount_str = strtok(NULL, ",");
        char *points_str = strtok(NULL, ",");
        char *level_str = strtok(NULL, ",");
        
        if (id && name && phone && reg_date && total_amount_str && points_str && level_str) {
            strcpy(members[member_count].id, id);
            strcpy(members[member_count].name, name);
            strcpy(members[member_count].phone, phone);
            strcpy(members[member_count].reg_date, reg_date);
            members[member_count].total_amount = atof(total_amount_str);
            members[member_count].points = atoi(points_str);
            members[member_count].level = atoi(level_str);
            member_count++;
        }
    }
    
    fclose(file);
    return 1;
}

/**
 * 保存会员数据
 * @return 成功返回1，失败返回0
 */
int saveMembers() {
    FILE *file = fopen(MEMBERS_FILE, "w");
    if (!file) {
        return 0;
    }
    
    for (int i = 0; i < member_count; i++) {
        fprintf(file, "%s,%s,%s,%s,%.2f,%d,%d\n", 
                members[i].id, 
                members[i].name, 
                members[i].phone, 
                members[i].reg_date, 
                members[i].total_amount, 
                members[i].points, 
                members[i].level);
    }
    
    fclose(file);
    return 1;
}

/**
 * 加载交易记录
 * @param transactions 交易记录数组指针
 * @param count 交易记录数量指针
 * @return 成功返回1，失败返回0
 */
int loadTransactions(Transaction **transactions, int *count) {
    FILE *file = fopen(TRANSACTIONS_FILE, "r");
    if (!file) {
        *count = 0;
        return 0;
    }
    
    // 计算交易记录数量
    int transaction_count = 0;
    char line[200];
    
    while (fgets(line, sizeof(line), file)) {
        transaction_count++;
    }
    
    // 重置文件指针
    rewind(file);
    
    // 分配内存
    *transactions = (Transaction*)malloc(transaction_count * sizeof(Transaction));
    if (!(*transactions)) {
        fclose(file);
        *count = 0;
        return 0;
    }
    
    // 读取交易记录
    *count = 0;
    while (fgets(line, sizeof(line), file) && (*count) < transaction_count) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;
        
        // 解析交易记录
        char *id = strtok(line, ",");
        char *date_time = strtok(NULL, ",");
        char *member_id = strtok(NULL, ",");
        char *item_count_str = strtok(NULL, ",");
        char *total_amount_str = strtok(NULL, ",");
        char *discount_amount_str = strtok(NULL, ",");
        char *final_amount_str = strtok(NULL, ",");
        char *points_earned_str = strtok(NULL, ",");
        char *points_used_str = strtok(NULL, ",");
        char *coupon_used_str = strtok(NULL, ",");
        
        if (id && date_time && member_id && item_count_str && 
            total_amount_str && discount_amount_str && final_amount_str && 
            points_earned_str && points_used_str && coupon_used_str) {
            
            strcpy((*transactions)[*count].id, id);
            strcpy((*transactions)[*count].date_time, date_time);
            strcpy((*transactions)[*count].member_id, member_id);
            (*transactions)[*count].item_count = atoi(item_count_str);
            (*transactions)[*count].total_amount = atof(total_amount_str);
            (*transactions)[*count].discount_amount = atof(discount_amount_str);
            (*transactions)[*count].final_amount = atof(final_amount_str);
            (*transactions)[*count].points_earned = atoi(points_earned_str);
            (*transactions)[*count].points_used = atoi(points_used_str);
            (*transactions)[*count].coupon_used = atoi(coupon_used_str);
            
            (*count)++;
        }
    }
    
    fclose(file);
    return 1;
}

/**
 * 保存交易记录
 * @param transaction 交易记录
 * @return 成功返回1，失败返回0
 */
int saveTransaction(Transaction transaction) {
    FILE *file = fopen(TRANSACTIONS_FILE, "a");
    if (!file) {
        return 0;
    }
    
    fprintf(file, "%s,%s,%s,%d,%.2f,%.2f,%.2f,%d,%d,%d\n", 
            transaction.id, 
            transaction.date_time, 
            transaction.member_id, 
            transaction.item_count, 
            transaction.total_amount, 
            transaction.discount_amount, 
            transaction.final_amount, 
            transaction.points_earned, 
            transaction.points_used, 
            transaction.coupon_used);
    
    fclose(file);
    return 1;
}

/**
 * 保存交易明细
 * @param details 交易明细数组
 * @param count 交易明细数量
 * @return 成功返回1，失败返回0
 */
int saveTransactionDetails(TransactionDetail *details, int count) {
    FILE *file = fopen(TRANSACTION_DETAILS_FILE, "a");
    if (!file) {
        return 0;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%.2f,%d,%.2f\n", 
                details[i].transaction_id, 
                details[i].product_id, 
                details[i].product_name, 
                details[i].price, 
                details[i].quantity, 
                details[i].subtotal);
    }
    
    fclose(file);
    return 1;
}

/**
 * 加载优惠券数据
 * @return 成功返回1，失败返回0
 */
int loadCoupons() {
    FILE *file = fopen(COUPONS_FILE, "r");
    if (!file) {
        return 0;
    }
    
    coupon_count = 0;
    char line[100];
    
    while (fgets(line, sizeof(line), file) && coupon_count < MAX_COUPONS) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;
        
        // 解析优惠券数据
        char *id = strtok(line, ",");
        char *member_id = strtok(NULL, ",");
        char *amount_str = strtok(NULL, ",");
        char *expire_date = strtok(NULL, ",");
        char *is_used_str = strtok(NULL, ",");
        
        if (id && member_id && amount_str && expire_date && is_used_str) {
            strcpy(coupons[coupon_count].id, id);
            strcpy(coupons[coupon_count].member_id, member_id);
            coupons[coupon_count].amount = atof(amount_str);
            strcpy(coupons[coupon_count].expire_date, expire_date);
            coupons[coupon_count].is_used = atoi(is_used_str);
            coupon_count++;
        }
    }
    
    fclose(file);
    return 1;
}

/**
 * 保存优惠券数据
 * @return 成功返回1，失败返回0
 */
int saveCoupons() {
    FILE *file = fopen(COUPONS_FILE, "w");
    if (!file) {
        return 0;
    }
    
    for (int i = 0; i < coupon_count; i++) {
        fprintf(file, "%s,%s,%.2f,%s,%d\n", 
                coupons[i].id, 
                coupons[i].member_id, 
                coupons[i].amount, 
                coupons[i].expire_date, 
                coupons[i].is_used);
    }
    
    fclose(file);
    return 1;
}
