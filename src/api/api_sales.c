#include "api_sales.h"

ApiResponse api_checkout(ShoppingCart *cart, char *member_id, int points_used, char *coupon_id) {
    ApiResponse res;
    Transaction *t = checkout(cart, member_id, points_used, coupon_id);
    if (t) {
        res.status = API_SUCCESS;
        res.data = t;
    } else {
        res.status = API_ERROR_INTERNAL;
        res.data = NULL;
    }
    return res;
}

ApiResponse api_process_return(char *transaction_id) {
    ApiResponse res;
    if (processReturn(transaction_id)) res.status = API_SUCCESS;
    else res.status = API_ERROR_NOT_FOUND;
    return res;
}
