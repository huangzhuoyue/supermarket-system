#include "sales.h"
#include "product.h"
#include <time.h>

/**
 * 初始化购物车
 * @param cart 购物车指针
 */
void initShoppingCart(ShoppingCart *cart) {
    cart->item_count = 0;
    cart->total_amount = 0.0;
    cart->discount_amount = 0.0;
    cart->final_amount = 0.0;
    memset(cart->items, 0, sizeof(cart->items));
}

/**
 * 添加商品到购物车
 * @param cart 购物车指针
 * @param product_id 商品ID
 * @param quantity 数量
 * @return 成功返回1，失败返回0
 */
int addToCart(ShoppingCart *cart, char *product_id, int quantity) {
    if (quantity <= 0) {
        return 0;  // 数量必须为正数
    }
    
    // 查找商品
    Product *product = findProductById(product_id);
    if (!product) {
        return 0;  // 商品不存在
    }
    
    // 检查库存
    if (product->stock < quantity) {
        return -1;  // 库存不足
    }
    
    // 检查购物车中是否已有该商品
    for (int i = 0; i < cart->item_count; i++) {
        if (strcmp(cart->items[i].product_id, product_id) == 0) {
            // 更新数量
            if (product->stock < cart->items[i].quantity + quantity) {
                return -1;  // 库存不足
            }
            cart->items[i].quantity += quantity;
            cart->items[i].subtotal = cart->items[i].price * cart->items[i].quantity;
            
            // 更新购物车总金额
            cart->total_amount += product->price * quantity;
            cart->final_amount = cart->total_amount - cart->discount_amount;
            
            return 1;
        }
    }
    
    // 检查购物车是否已满
    if (cart->item_count >= MAX_CART_ITEMS) {
        return -2;  // 购物车已满
    }
    
    // 添加新商品到购物车
    strcpy(cart->items[cart->item_count].product_id, product->id);
    strcpy(cart->items[cart->item_count].product_name, product->name);
    cart->items[cart->item_count].price = product->price;
    cart->items[cart->item_count].quantity = quantity;
    cart->items[cart->item_count].subtotal = product->price * quantity;
    
    // 更新购物车总金额
    cart->total_amount += cart->items[cart->item_count].subtotal;
    cart->final_amount = cart->total_amount - cart->discount_amount;
    
    cart->item_count++;
    
    return 1;
}

/**
 * 更新购物车商品数量
 * @param cart 购物车指针
 * @param index 商品索引
 * @param quantity 新数量
 * @return 成功返回1，失败返回0
 */
int updateCartItemQuantity(ShoppingCart *cart, int index, int quantity) {
    if (index < 0 || index >= cart->item_count) {
        return 0;  // 索引无效
    }
    
    if (quantity <= 0) {
        // 如果数量为0或负数，则移除该商品
        return removeFromCart(cart, index);
    }
    
    // 查找商品
    Product *product = findProductById(cart->items[index].product_id);
    if (!product) {
        return 0;  // 商品不存在
    }
    
    // 检查库存
    if (product->stock < quantity) {
        return -1;  // 库存不足
    }
    
    // 更新购物车总金额
    cart->total_amount -= cart->items[index].subtotal;
    
    // 更新数量和小计
    cart->items[index].quantity = quantity;
    cart->items[index].subtotal = cart->items[index].price * quantity;
    
    // 更新购物车总金额
    cart->total_amount += cart->items[index].subtotal;
    cart->final_amount = cart->total_amount - cart->discount_amount;
    
    return 1;
}

/**
 * 从购物车中移除商品
 * @param cart 购物车指针
 * @param index 商品索引
 * @return 成功返回1，失败返回0
 */
int removeFromCart(ShoppingCart *cart, int index) {
    if (index < 0 || index >= cart->item_count) {
        return 0;  // 索引无效
    }
    
    // 更新购物车总金额
    cart->total_amount -= cart->items[index].subtotal;
    cart->final_amount = cart->total_amount - cart->discount_amount;
    
    // 移动后面的商品
    for (int i = index; i < cart->item_count - 1; i++) {
        strcpy(cart->items[i].product_id, cart->items[i+1].product_id);
        strcpy(cart->items[i].product_name, cart->items[i+1].product_name);
        cart->items[i].price = cart->items[i+1].price;
        cart->items[i].quantity = cart->items[i+1].quantity;
        cart->items[i].subtotal = cart->items[i+1].subtotal;
    }
    
    cart->item_count--;
    
    return 1;
}

/**
 * 清空购物车
 * @param cart 购物车指针
 */
void clearCart(ShoppingCart *cart) {
    cart->item_count = 0;
    cart->total_amount = 0.0;
    cart->discount_amount = 0.0;
    cart->final_amount = 0.0;
}

/**
 * 计算折扣
 * @param cart 购物车指针
 * @param member_id 会员ID
 * @return 折扣金额
 */
float calculateDiscount(ShoppingCart *cart, char *member_id) {
    float discount_rate = 1.0;  // 默认无折扣
    
    if (member_id != NULL && strlen(member_id) > 0 && strcmp(member_id, "NONE") != 0) {
        // 查找会员
        for (int i = 0; i < member_count; i++) {
            if (strcmp(members[i].id, member_id) == 0) {
                // 根据会员等级设置折扣率
                switch (members[i].level) {
                    case MEMBER_LEVEL_NORMAL:
                        discount_rate = DISCOUNT_NORMAL;
                        break;
                    case MEMBER_LEVEL_SILVER:
                        discount_rate = DISCOUNT_SILVER;
                        break;
                    case MEMBER_LEVEL_GOLD:
                        discount_rate = DISCOUNT_GOLD;
                        break;
                    default:
                        discount_rate = 1.0;
                }
                break;
            }
        }
    }
    
    // 计算折扣金额
    float discount_amount = cart->total_amount * (1.0 - discount_rate);
    
    // 更新购物车折扣金额和最终金额
    cart->discount_amount = discount_amount;
    cart->final_amount = cart->total_amount - discount_amount;
    
    return discount_amount;
}

/**
 * 结账
 * @param cart 购物车指针
 * @param member_id 会员ID
 * @param points_used 使用的积分
 * @param coupon_id 优惠券ID
 * @return 成功返回交易记录指针，失败返回NULL
 */
Transaction* checkout(ShoppingCart *cart, char *member_id, int points_used, char *coupon_id) {
    if (cart->item_count <= 0) {
        return NULL;  // 购物车为空
    }
    
    // 检查库存
    for (int i = 0; i < cart->item_count; i++) {
        Product *product = findProductById(cart->items[i].product_id);
        if (!product || product->stock < cart->items[i].quantity) {
            return NULL;  // 库存不足
        }
    }
    
    // 计算折扣
    calculateDiscount(cart, member_id);
    
    // 处理积分抵扣
    float points_discount = 0.0;
    if (points_used > 0 && member_id != NULL && strcmp(member_id, "NONE") != 0) {
        // 查找会员
        for (int i = 0; i < member_count; i++) {
            if (strcmp(members[i].id, member_id) == 0) {
                // 检查积分是否足够
                if (members[i].points >= points_used) {
                    // 计算积分抵扣金额
                    points_discount = (float)points_used / POINTS_FOR_COUPON * COUPON_VALUE;
                    
                    // 更新会员积分
                    members[i].points -= points_used;
                    
                    // 更新购物车最终金额
                    cart->final_amount -= points_discount;
                    if (cart->final_amount < 0) {
                        cart->final_amount = 0;
                    }
                }
                break;
            }
        }
    }
    
    // 处理优惠券
    float coupon_amount = 0.0;
    if (coupon_id != NULL && strlen(coupon_id) > 0) {
        // 查找优惠券
        for (int i = 0; i < coupon_count; i++) {
            if (strcmp(coupons[i].id, coupon_id) == 0 && 
                coupons[i].is_used == 0 && 
                (strcmp(coupons[i].member_id, member_id) == 0 || strcmp(member_id, "NONE") == 0)) {
                
                // 检查优惠券是否过期
                char current_date[12];
                getCurrentDate(current_date);
                if (strcmp(current_date, coupons[i].expire_date) <= 0) {
                    // 使用优惠券
                    coupon_amount = coupons[i].amount;
                    coupons[i].is_used = 1;
                    
                    // 更新购物车最终金额
                    cart->final_amount -= coupon_amount;
                    if (cart->final_amount < 0) {
                        cart->final_amount = 0;
                    }
                    
                    // 保存优惠券数据
                    saveCoupons();
                }
                break;
            }
        }
    }
    
    // 创建交易记录
    static Transaction transaction;
    memset(&transaction, 0, sizeof(Transaction));
    
    // 生成交易ID
    char datetime[20];
    getCurrentDateTime(datetime);
    sprintf(transaction.id, "%s-%04d", datetime, rand() % 10000);
    strcpy(transaction.date_time, datetime);
    
    // 设置会员ID
    if (member_id != NULL && strlen(member_id) > 0) {
        strcpy(transaction.member_id, member_id);
    } else {
        strcpy(transaction.member_id, "NONE");
    }
    
    // 设置交易信息
    transaction.item_count = cart->item_count;
    transaction.total_amount = cart->total_amount;
    transaction.discount_amount = cart->discount_amount;
    transaction.final_amount = cart->final_amount;
    transaction.points_earned = (int)(cart->final_amount * POINTS_PER_YUAN);
    transaction.points_used = points_used;
    transaction.coupon_used = (int)coupon_amount;
    
    // 创建交易明细
    TransactionDetail details[MAX_TRANSACTION_ITEMS];
    for (int i = 0; i < cart->item_count; i++) {
        strcpy(details[i].transaction_id, transaction.id);
        strcpy(details[i].product_id, cart->items[i].product_id);
        strcpy(details[i].product_name, cart->items[i].product_name);
        details[i].price = cart->items[i].price;
        details[i].quantity = cart->items[i].quantity;
        details[i].subtotal = cart->items[i].subtotal;
        
        // 更新库存
        updateStock(cart->items[i].product_id, -cart->items[i].quantity);
    }
    
    // 保存交易记录和明细
    saveTransaction(transaction);
    saveTransactionDetails(details, cart->item_count);
    
    // 更新会员信息
    if (strcmp(transaction.member_id, "NONE") != 0) {
        for (int i = 0; i < member_count; i++) {
            if (strcmp(members[i].id, transaction.member_id) == 0) {
                // 更新累计消费金额
                members[i].total_amount += transaction.final_amount;
                
                // 更新积分
                members[i].points += transaction.points_earned;
                
                // 更新会员等级
                if (members[i].total_amount >= GOLD_THRESHOLD && members[i].level < MEMBER_LEVEL_GOLD) {
                    members[i].level = MEMBER_LEVEL_GOLD;
                } else if (members[i].total_amount >= SILVER_THRESHOLD && members[i].level < MEMBER_LEVEL_SILVER) {
                    members[i].level = MEMBER_LEVEL_SILVER;
                }
                
                // 保存会员数据
                saveMembers();
                break;
            }
        }
    }
    
    // 清空购物车
    clearCart(cart);
    
    return &transaction;
}

/**
 * 处理退货
 * @param transaction_id 交易ID
 * @return 成功返回1，失败返回0
 */
int processReturn(char *transaction_id) {
    // 加载交易记录
    Transaction *transactions;
    int transaction_count;
    if (!loadTransactions(&transactions, &transaction_count)) {
        return 0;
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
        free(transactions);
        return 0;  // 交易记录不存在
    }
    
    // 获取交易明细
    TransactionDetail *details;
    int detail_count;
    if (!getTransactionDetails(transaction_id, &details, &detail_count)) {
        free(transactions);
        return 0;
    }
    
    // 恢复库存
    for (int i = 0; i < detail_count; i++) {
        updateStock(details[i].product_id, details[i].quantity);
    }
    
    // 处理会员积分和消费金额
    if (strcmp(transaction->member_id, "NONE") != 0) {
        for (int i = 0; i < member_count; i++) {
            if (strcmp(members[i].id, transaction->member_id) == 0) {
                // 更新累计消费金额
                members[i].total_amount -= transaction->final_amount;
                if (members[i].total_amount < 0) {
                    members[i].total_amount = 0;
                }
                
                // 更新积分
                members[i].points -= transaction->points_earned;
                if (members[i].points < 0) {
                    members[i].points = 0;
                }
                
                // 更新会员等级
                if (members[i].total_amount < SILVER_THRESHOLD) {
                    members[i].level = MEMBER_LEVEL_NORMAL;
                } else if (members[i].total_amount < GOLD_THRESHOLD) {
                    members[i].level = MEMBER_LEVEL_SILVER;
                }
                
                // 保存会员数据
                saveMembers();
                break;
            }
        }
    }
    
    // 释放内存
    free(transactions);
    free(details);
    
    return 1;
}

/**
 * 显示购物车
 * @param cart 购物车指针
 */
void displayCart(ShoppingCart *cart) {
    if (cart->item_count <= 0) {
        printf("购物车为空！\n");
        return;
    }
    
    printf("购物车内容：\n");
    printf("%-4s %-10s %-20s %-8s %-8s %-8s\n", 
           "序号", "商品ID", "商品名称", "单价", "数量", "小计");
    printf("------------------------------------------------------------------\n");
    
    for (int i = 0; i < cart->item_count; i++) {
        printf("%-4d %-10s %-20s %-8.2f %-8d %-8.2f\n", 
               i + 1,
               cart->items[i].product_id, 
               cart->items[i].product_name, 
               cart->items[i].price, 
               cart->items[i].quantity, 
               cart->items[i].subtotal);
    }
    
    printf("------------------------------------------------------------------\n");
    printf("总金额：%.2f\n", cart->total_amount);
    if (cart->discount_amount > 0) {
        printf("折扣金额：%.2f\n", cart->discount_amount);
        printf("应付金额：%.2f\n", cart->final_amount);
    }
}

/**
 * 显示交易记录
 * @param transaction 交易记录
 */
void displayTransaction(Transaction transaction) {
    printf("交易ID: %s\n", transaction.id);
    printf("交易时间: %s\n", transaction.date_time);
    printf("会员ID: %s\n", transaction.member_id);
    printf("商品种类数: %d\n", transaction.item_count);
    printf("总金额: %.2f\n", transaction.total_amount);
    
    if (transaction.discount_amount > 0) {
        printf("折扣金额: %.2f\n", transaction.discount_amount);
    }
    
    if (transaction.points_used > 0) {
        printf("使用积分: %d\n", transaction.points_used);
    }
    
    if (transaction.coupon_used > 0) {
        printf("使用优惠券: %.2f元\n", (float)transaction.coupon_used);
    }
    
    printf("最终金额: %.2f\n", transaction.final_amount);
    
    if (strcmp(transaction.member_id, "NONE") != 0) {
        printf("获得积分: %d\n", transaction.points_earned);
    }
    
    // 获取交易明细
    TransactionDetail *details;
    int detail_count;
    if (getTransactionDetails(transaction.id, &details, &detail_count)) {
        printf("\n商品明细：\n");
        printf("%-10s %-20s %-8s %-8s %-8s\n", 
               "商品ID", "商品名称", "单价", "数量", "小计");
        printf("----------------------------------------------------------\n");
        
        for (int i = 0; i < detail_count; i++) {
            printf("%-10s %-20s %-8.2f %-8d %-8.2f\n", 
                   details[i].product_id, 
                   details[i].product_name, 
                   details[i].price, 
                   details[i].quantity, 
                   details[i].subtotal);
        }
        
        free(details);
    }
}

/**
 * 获取交易明细
 * @param transaction_id 交易ID
 * @param details 交易明细数组指针的指针
 * @param count 交易明细数量指针
 * @return 成功返回1，失败返回0
 */
int getTransactionDetails(char *transaction_id, TransactionDetail **details, int *count) {
    FILE *file = fopen(TRANSACTION_DETAILS_FILE, "r");
    if (!file) {
        *count = 0;
        return 0;
    }
    
    // 计算符合条件的交易明细数量
    *count = 0;
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        char id[20];
        sscanf(line, "%[^,]", id);
        if (strcmp(id, transaction_id) == 0) {
            (*count)++;
        }
    }
    
    if (*count == 0) {
        fclose(file);
        return 0;
    }
    
    // 分配内存
    *details = (TransactionDetail*)malloc((*count) * sizeof(TransactionDetail));
    if (!(*details)) {
        fclose(file);
        *count = 0
(Content truncated due to size limit. Use line ranges to read in chunks)