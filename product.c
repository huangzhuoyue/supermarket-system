#include "product.h"

/**
 * 添加商品
 * @param product 商品信息
 * @return 成功返回1，失败返回0
 */
int addProduct(Product product) {
    // 检查商品ID是否已存在
    if (isProductIdExists(product.id)) {
        return 0;
    }
    
    // 检查是否超出最大商品数
    if (product_count >= MAX_PRODUCTS) {
        return 0;
    }
    
    // 添加商品
    strcpy(products[product_count].id, product.id);
    strcpy(products[product_count].name, product.name);
    strcpy(products[product_count].category, product.category);
    strcpy(products[product_count].supplier, product.supplier);
    products[product_count].price = product.price;
    products[product_count].stock = product.stock;
    strcpy(products[product_count].production_date, product.production_date);
    products[product_count].shelf_life = product.shelf_life;
    
    product_count++;
    
    // 保存商品数据
    saveProducts();
    
    return 1;
}

/**
 * 删除商品
 * @param id 商品ID
 * @return 成功返回1，失败返回0
 */
int deleteProduct(char *id) {
    // 查找商品
    int found = 0;
    for (int i = 0; i < product_count; i++) {
        if (found) {
            // 移动后续商品
            strcpy(products[i-1].id, products[i].id);
            strcpy(products[i-1].name, products[i].name);
            strcpy(products[i-1].category, products[i].category);
            strcpy(products[i-1].supplier, products[i].supplier);
            products[i-1].price = products[i].price;
            products[i-1].stock = products[i].stock;
            strcpy(products[i-1].production_date, products[i].production_date);
            products[i-1].shelf_life = products[i].shelf_life;
        } else if (strcmp(products[i].id, id) == 0) {
            found = 1;
        }
    }
    
    if (found) {
        product_count--;
        saveProducts();
        return 1;
    }
    
    return 0;
}

/**
 * 更新商品信息
 * @param product 商品信息
 * @return 成功返回1，失败返回0
 */
int updateProduct(Product product) {
    // 查找商品
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].id, product.id) == 0) {
            // 更新商品信息
            strcpy(products[i].name, product.name);
            strcpy(products[i].category, product.category);
            strcpy(products[i].supplier, product.supplier);
            products[i].price = product.price;
            products[i].stock = product.stock;
            strcpy(products[i].production_date, product.production_date);
            products[i].shelf_life = product.shelf_life;
            
            saveProducts();
            return 1;
        }
    }
    
    return 0;
}

/**
 * 根据ID查找商品
 * @param id 商品ID
 * @return 成功返回商品指针，失败返回NULL
 */
Product* findProductById(char *id) {
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].id, id) == 0) {
            return &products[i];
        }
    }
    
    return NULL;
}

/**
 * 根据名称查找商品
 * @param name 商品名称
 * @return 成功返回商品指针，失败返回NULL
 */
Product* findProductByName(char *name) {
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].name, name) == 0) {
            return &products[i];
        }
    }
    
    return NULL;
}

/**
 * 根据分类查找商品
 * @param category 商品分类
 * @param count 返回找到的商品数量
 * @return 成功返回商品指针数组，失败返回NULL
 */
Product** findProductsByCategory(char *category, int *count) {
    // 计算符合条件的商品数量
    *count = 0;
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].category, category) == 0) {
            (*count)++;
        }
    }
    
    if (*count == 0) {
        return NULL;
    }
    
    // 分配内存
    Product **result = (Product**)malloc((*count) * sizeof(Product*));
    if (!result) {
        *count = 0;
        return NULL;
    }
    
    // 填充结果
    int index = 0;
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].category, category) == 0) {
            result[index++] = &products[i];
        }
    }
    
    return result;
}

/**
 * 根据供应商查找商品
 * @param supplier 供应商
 * @param count 返回找到的商品数量
 * @return 成功返回商品指针数组，失败返回NULL
 */
Product** findProductsBySupplier(char *supplier, int *count) {
    // 计算符合条件的商品数量
    *count = 0;
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].supplier, supplier) == 0) {
            (*count)++;
        }
    }
    
    if (*count == 0) {
        return NULL;
    }
    
    // 分配内存
    Product **result = (Product**)malloc((*count) * sizeof(Product*));
    if (!result) {
        *count = 0;
        return NULL;
    }
    
    // 填充结果
    int index = 0;
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].supplier, supplier) == 0) {
            result[index++] = &products[i];
        }
    }
    
    return result;
}

/**
 * 显示单个商品信息
 * @param product 商品信息
 */
void displayProduct(Product product) {
    printf("商品ID: %s\n", product.id);
    printf("商品名称: %s\n", product.name);
    printf("商品分类: %s\n", product.category);
    printf("供应商: %s\n", product.supplier);
    printf("单价: %.2f\n", product.price);
    printf("库存数量: %d\n", product.stock);
    
    if (strlen(product.production_date) > 0) {
        printf("生产日期: %s\n", product.production_date);
    }
    
    if (product.shelf_life > 0) {
        printf("保质期: %d天\n", product.shelf_life);
    }
    
    // 库存预警
    if (product.stock < STOCK_WARNING_THRESHOLD) {
        printf("库存预警: 库存低于预警阈值 %d\n", STOCK_WARNING_THRESHOLD);
    }
}

/**
 * 显示商品列表
 * @param products 商品指针数组
 * @param count 商品数量
 */
void displayProductList(Product **product_list, int count) {
    printf("%-10s %-20s %-10s %-20s %-8s %-8s\n", 
           "商品ID", "商品名称", "分类", "供应商", "单价", "库存");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-10s %-20s %-10s %-20s %-8.2f %-8d\n", 
               product_list[i]->id, 
               product_list[i]->name, 
               product_list[i]->category, 
               product_list[i]->supplier, 
               product_list[i]->price, 
               product_list[i]->stock);
    }
}

/**
 * 显示所有商品
 */
void displayAllProducts() {
    if (product_count == 0) {
        printf("没有商品记录！\n");
        return;
    }
    
    // 创建商品指针数组
    Product *product_list[MAX_PRODUCTS];
    for (int i = 0; i < product_count; i++) {
        product_list[i] = &products[i];
    }
    
    displayProductList(product_list, product_count);
}

/**
 * 检查商品ID是否已存在
 * @param id 商品ID
 * @return 存在返回1，不存在返回0
 */
int isProductIdExists(char *id) {
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].id, id) == 0) {
            return 1;
        }
    }
    
    return 0;
}

/**
 * 更新库存
 * @param product_id 商品ID
 * @param change 库存变化量（正数增加，负数减少）
 * @return 成功返回1，失败返回0
 */
int updateStock(char *product_id, int change) {
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].id, product_id) == 0) {
            // 检查库存是否足够（减少库存时）
            if (change < 0 && products[i].stock + change < 0) {
                return 0;
            }
            
            // 更新库存
            products[i].stock += change;
            
            saveProducts();
            return 1;
        }
    }
    
    return 0;
}

/**
 * 检查库存不足的商品
 */
void checkLowStock() {
    int found = 0;
    
    printf("库存预警商品列表：\n");
    printf("%-10s %-20s %-10s %-8s\n", 
           "商品ID", "商品名称", "分类", "库存");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < product_count; i++) {
        if (products[i].stock < STOCK_WARNING_THRESHOLD) {
            printf("%-10s %-20s %-10s %-8d\n", 
                   products[i].id, 
                   products[i].name, 
                   products[i].category, 
                   products[i].stock);
            found = 1;
        }
    }
    
    if (!found) {
        printf("没有库存不足的商品！\n");
    }
}
