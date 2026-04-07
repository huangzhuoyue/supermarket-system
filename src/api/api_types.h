#ifndef API_TYPES_H
#define API_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

// API响应状态码
typedef enum {
    API_SUCCESS = 0,
    API_ERROR_NOT_FOUND = -1,
    API_ERROR_INVALID_PARAM = -2,
    API_ERROR_UNAUTHORIZED = -3,
    API_ERROR_INTERNAL = -4,
    API_ERROR_INSUFFICIENT_STOCK = -5,
    API_ERROR_ALREADY_EXISTS = -6
} ApiStatus;

// 统一API响应结构体
typedef struct {
    ApiStatus status;
    char message[256];
    void *data;
} ApiResponse;

#ifdef __cplusplus
}
#endif

#endif // API_TYPES_H
