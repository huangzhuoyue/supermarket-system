#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "common.h"

// 文件操作函数声明
int loadUsers();
int saveUsers();
int loadProducts();
int saveProducts();
int loadMembers();
int saveMembers();
int loadTransactions(Transaction **transactions, int *count);
int saveTransaction(Transaction transaction);
int saveTransactionDetails(TransactionDetail *details, int count);
int loadCoupons();
int saveCoupons();
int fileExists(const char *filename);
int createEmptyFile(const char *filename);
int initializeDataFiles();

#endif // FILE_UTILS_H
