#include "api_product.h"

ApiResponse api_add_product(Product product) {
    ApiResponse res;
    if (addProduct(product)) {
        res.status = API_SUCCESS;
        res.data = NULL;
    } else {
        res.status = API_ERROR_INTERNAL;
    }
    return res;
}

ApiResponse api_delete_product(char *id) {
    ApiResponse res;
    if (deleteProduct(id)) res.status = API_SUCCESS;
    else res.status = API_ERROR_NOT_FOUND;
    return res;
}

ApiResponse api_update_product(Product product) {
    ApiResponse res;
    if (updateProduct(product)) res.status = API_SUCCESS;
    else res.status = API_ERROR_NOT_FOUND;
    return res;
}

ApiResponse api_find_product_by_id(char *id) {
    ApiResponse res;
    Product *p = findProductById(id);
    if (p) {
        res.status = API_SUCCESS;
        res.data = p;
    } else {
        res.status = API_ERROR_NOT_FOUND;
        res.data = NULL;
    }
    return res;
}

ApiResponse api_find_product_by_name(char *name) {
    ApiResponse res;
    Product *p = findProductByName(name);
    if (p) {
        res.status = API_SUCCESS;
        res.data = p;
    } else {
        res.status = API_ERROR_NOT_FOUND;
        res.data = NULL;
    }
    return res;
}

ApiResponse api_find_products_by_category(char *category, int *count) {
    ApiResponse res;
    Product **arr = findProductsByCategory(category, count);
    if (arr) {
        res.status = API_SUCCESS;
        res.data = arr;
    } else {
        res.status = API_ERROR_NOT_FOUND;
        res.data = NULL;
    }
    return res;
}

ApiResponse api_find_products_by_supplier(char *supplier, int *count) {
    ApiResponse res;
    Product **arr = findProductsBySupplier(supplier, count);
    if (arr) {
        res.status = API_SUCCESS;
        res.data = arr;
    } else {
        res.status = API_ERROR_NOT_FOUND;
        res.data = NULL;
    }
    return res;
}
