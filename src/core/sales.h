#ifndef SALES_H
#define SALES_H

#include "common.h"

// 销售管理函数声明
void initShoppingCart(ShoppingCart *cart);
int addToCart(ShoppingCart *cart, char *product_id, int quantity);
int updateCartItemQuantity(ShoppingCart *cart, int index, int quantity);
int removeFromCart(ShoppingCart *cart, int index);
void clearCart(ShoppingCart *cart);
float calculateDiscount(ShoppingCart *cart, char *member_id);
Transaction* checkout(ShoppingCart *cart, char *member_id, int points_used, char *coupon_id);
int processReturn(char *transaction_id);
void displayCart(ShoppingCart *cart);
void displayTransaction(Transaction transaction);
int getTransactionDetails(char *transaction_id, TransactionDetail **details, int *count);

#endif // SALES_H
