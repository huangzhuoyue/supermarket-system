#ifndef API_SALES_H
#define API_SALES_H

#include "api_types.h"
#include "sales.h"

ApiResponse api_checkout(ShoppingCart *cart, char *member_id, int points_used, char *coupon_id);
ApiResponse api_process_return(char *transaction_id);

#endif
