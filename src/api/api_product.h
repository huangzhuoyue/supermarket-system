#ifndef API_PRODUCT_H
#define API_PRODUCT_H

#include "api_types.h"
#include "product.h"

// Product API wraps core functions and returns ApiResponse
ApiResponse api_add_product(Product product);
ApiResponse api_delete_product(char *id);
ApiResponse api_update_product(Product product);
ApiResponse api_find_product_by_id(char *id);
ApiResponse api_find_product_by_name(char *name);
ApiResponse api_find_products_by_category(char *category, int *count);
ApiResponse api_find_products_by_supplier(char *supplier, int *count);

#endif
