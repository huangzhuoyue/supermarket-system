#ifndef PRODUCT_H
#define PRODUCT_H

#include "common.h"

// 商品管理函数声明
int addProduct(Product product);
int deleteProduct(char *id);
int updateProduct(Product product);
Product* findProductById(char *id);
Product* findProductByName(char *name);
Product** findProductsByCategory(char *category, int *count);
Product** findProductsBySupplier(char *supplier, int *count);
void displayProduct(Product product);
void displayProductList(Product **products, int count);
void displayAllProducts();
int isProductIdExists(char *id);
int updateStock(char *product_id, int change);
void checkLowStock();

#endif // PRODUCT_H
