#include "product.h"
#include "file_utils.h"

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

