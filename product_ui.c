#define _CRT_SECURE_NO_WARNINGS
#include "product.h"
#include "ui.h"

// 商品管理模块函数声明
void addProductUI();
void deleteProductUI();
void updateProductUI();
void findProductUI();
void findProductsByCategoryUI();
void findProductsBySupplierUI();

/**
 * 商品管理主函数
 */
void productManagement() {
    int choice;
    
    do {
        displayProductMenu();
        scanf("%d", &choice);
        
        // 清除输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        switch (choice) {
            case 1:
                addProductUI();
                break;
            case 2:
                deleteProductUI();
                break;
            case 3:
                updateProductUI();
                break;
            case 4:
                findProductUI();
                break;
            case 5:
                findProductsByCategoryUI();
                break;
            case 6:
                findProductsBySupplierUI();
                break;
            case 7:
                displayHeader("所有商品列表");
                displayAllProducts();
                pauseScreen();
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
 * 添加商品界面
 */
void addProductUI() {
    displayHeader("添加商品");
    
    Product product;
    
    printf("请输入商品信息：\n");
    
    printf("商品ID (如P001): ");
    scanf("%9s", product.id);
    
    // 检查ID是否已存在
    if (isProductIdExists(product.id)) {
        displayError("商品ID已存在！");
        pauseScreen();
        return;
    }
    
    printf("商品名称: ");
    scanf("%49s", product.name);
    
    printf("商品分类: ");
    scanf("%19s", product.category);
    
    printf("供应商: ");
    scanf("%49s", product.supplier);
    
    printf("单价: ");
    scanf("%f", &product.price);
    
    printf("库存数量: ");
    scanf("%d", &product.stock);
    
    printf("生产日期 (YYYY-MM-DD，可选): ");
    scanf("%11s", product.production_date);
    
    // 验证日期格式
    if (strlen(product.production_date) > 0 && !isValidDate(product.production_date)) {
        strcpy(product.production_date, "");
        displayWarning("日期格式不正确，已忽略生产日期！");
    }
    
    printf("保质期 (天数，可选): ");
    scanf("%d", &product.shelf_life);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // 添加商品
    if (addProduct(product)) {
        displaySuccess("商品添加成功！");
    } else {
        displayError("商品添加失败！");
    }
    
    pauseScreen();
}

/**
 * 删除商品界面
 */
void deleteProductUI() {
    displayHeader("删除商品");
    
    char id[10];
    
    printf("请输入要删除的商品ID: ");
    scanf("%9s", id);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // 查找商品
    Product *product = findProductById(id);
    if (!product) {
        displayError("商品不存在！");
        pauseScreen();
        return;
    }
    
    // 显示商品信息
    printf("\n要删除的商品信息：\n");
    displayProduct(*product);
    
    // 确认删除
    char confirm;
    printf("\n确认删除该商品？(y/n): ");
    scanf("%c", &confirm);
    
    // 清除输入缓冲区
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (confirm == 'y' || confirm == 'Y') {
        if (deleteProduct(id)) {
            displaySuccess("商品删除成功！");
        } else {
            displayError("商品删除失败！");
        }
    } else {
        printf("已取消删除操作！\n");
    }
    
    pauseScreen();
}

/**
 * 更新商品界面
 */
void updateProductUI() {
    displayHeader("修改商品");
    
    char id[10];
    
    printf("请输入要修改的商品ID: ");
    scanf("%9s", id);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // 查找商品
    Product *product = findProductById(id);
    if (!product) {
        displayError("商品不存在！");
        pauseScreen();
        return;
    }
    
    // 显示商品信息
    printf("\n当前商品信息：\n");
    displayProduct(*product);
    
    // 创建新的商品信息
    Product newProduct;
    strcpy(newProduct.id, id);
    
    printf("\n请输入新的商品信息（直接回车保持不变）：\n");
    
    char buffer[50];
    
    printf("商品名称 [%s]: ", product->name);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        strcpy(newProduct.name, buffer);
    } else {
        strcpy(newProduct.name, product->name);
    }
    
    printf("商品分类 [%s]: ", product->category);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        strcpy(newProduct.category, buffer);
    } else {
        strcpy(newProduct.category, product->category);
    }
    
    printf("供应商 [%s]: ", product->supplier);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        strcpy(newProduct.supplier, buffer);
    } else {
        strcpy(newProduct.supplier, product->supplier);
    }
    
    printf("单价 [%.2f]: ", product->price);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        newProduct.price = atof(buffer);
    } else {
        newProduct.price = product->price;
    }
    
    printf("库存数量 [%d]: ", product->stock);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        newProduct.stock = atoi(buffer);
    } else {
        newProduct.stock = product->stock;
    }
    
    printf("生产日期 [%s]: ", product->production_date);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        if (isValidDate(buffer)) {
            strcpy(newProduct.production_date, buffer);
        } else {
            strcpy(newProduct.production_date, product->production_date);
            displayWarning("日期格式不正确，保持原日期不变！");
        }
    } else {
        strcpy(newProduct.production_date, product->production_date);
    }
    
    printf("保质期 [%d]: ", product->shelf_life);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        newProduct.shelf_life = atoi(buffer);
    } else {
        newProduct.shelf_life = product->shelf_life;
    }
    
    // 更新商品
    if (updateProduct(newProduct)) {
        displaySuccess("商品修改成功！");
    } else {
        displayError("商品修改失败！");
    }
    
    pauseScreen();
}

/**
 * 查询商品界面
 */
void findProductUI() {
    displayHeader("查询商品");
    
    printf("1. 按ID查询\n");
    printf("2. 按名称查询\n");
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
            printf("\n请输入商品ID: ");
            scanf("%9s", id);
            
            // 清除输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            Product *product = findProductById(id);
            if (product) {
                printf("\n查询结果：\n");
                displayProduct(*product);
            } else {
                displayError("未找到该商品！");
            }
            break;
        }
        case 2: {
            char name[50];
            printf("\n请输入商品名称: ");
            scanf("%49s", name);
            
            // 清除输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            Product *product = findProductByName(name);
            if (product) {
                printf("\n查询结果：\n");
                displayProduct(*product);
            } else {
                displayError("未找到该商品！");
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
 * 按分类查询商品界面
 */
void findProductsByCategoryUI() {
    displayHeader("按分类查询商品");
    
    char category[20];
    printf("请输入商品分类: ");
    scanf("%19s", category);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    int count;
    Product **product_list = findProductsByCategory(category, &count);
    
    if (product_list) {
        printf("\n查询结果 (%d 个商品)：\n", count);
        displayProductList(product_list, count);
        free(product_list);
    } else {
        displayError("未找到该分类的商品！");
    }
    
    pauseScreen();
}

/**
 * 按供应商查询商品界面
 */
void findProductsBySupplierUI() {
    displayHeader("按供应商查询商品");
    
    char supplier[50];
    printf("请输入供应商: ");
    scanf("%49s", supplier);
    
    // 清除输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    int count;
    Product **product_list = findProductsBySupplier(supplier, &count);
    
    if (product_list) {
        printf("\n查询结果 (%d 个商品)：\n", count);
        displayProductList(product_list, count);
        free(product_list);
    } else {
        displayError("未找到该供应商的商品！");
    }
    
    pauseScreen();
}
