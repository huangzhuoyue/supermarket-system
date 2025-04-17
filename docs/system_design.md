# C语言超市管理系统架构设计

## 1. 系统总体架构

系统采用模块化设计，主要分为以下几个模块：
- 用户界面模块：负责与用户交互，显示菜单和信息
- 用户认证模块：负责用户登录验证
- 商品管理模块：负责商品的增删改查和分类查询
- 销售管理模块：负责购物车、交易记录和库存同步
- 会员管理模块：负责会员信息、积分和等级管理
- 文件操作模块：负责数据的读写和持久化存储
- 辅助功能模块：包括帮助文档、系统设置等

## 2. 数据结构设计

### 2.1 商品结构体
```c
typedef struct {
    char id[10];           // 商品ID
    char name[50];         // 商品名称
    char category[20];     // 商品分类
    char supplier[50];     // 供应商
    float price;           // 单价
    int stock;             // 库存数量
    char production_date[12]; // 生产日期 (YYYY-MM-DD)
    int shelf_life;        // 保质期(天)
} Product;
```

### 2.2 会员结构体
```c
typedef struct {
    char id[10];           // 会员ID
    char name[50];         // 会员姓名
    char phone[15];        // 联系电话
    char reg_date[12];     // 注册日期 (YYYY-MM-DD)
    float total_amount;    // 累计消费金额
    int points;            // 当前积分
    int level;             // 会员等级 (1-普通, 2-银卡, 3-金卡)
} Member;
```

### 2.3 购物车项结构体
```c
typedef struct {
    char product_id[10];   // 商品ID
    char product_name[50]; // 商品名称
    float price;           // 单价
    int quantity;          // 数量
    float subtotal;        // 小计金额
} CartItem;
```

### 2.4 购物车结构体
```c
typedef struct {
    CartItem items[100];   // 购物车项数组
    int item_count;        // 购物车中的商品数量
    float total_amount;    // 总金额
    float discount_amount; // 折扣金额
    float final_amount;    // 最终金额
} ShoppingCart;
```

### 2.5 交易记录结构体
```c
typedef struct {
    char id[20];           // 交易ID (格式: YYYYMMDD-NNNN)
    char date_time[20];    // 交易日期时间
    char member_id[10];    // 会员ID (非会员为"NONE")
    int item_count;        // 商品种类数量
    float total_amount;    // 总金额
    float discount_amount; // 折扣金额
    float final_amount;    // 最终金额
    int points_earned;     // 获得积分
    int points_used;       // 使用积分
    int coupon_used;       // 使用优惠券金额
} Transaction;
```

### 2.6 交易明细结构体
```c
typedef struct {
    char transaction_id[20]; // 交易ID
    char product_id[10];     // 商品ID
    char product_name[50];   // 商品名称
    float price;             // 单价
    int quantity;            // 数量
    float subtotal;          // 小计金额
} TransactionDetail;
```

### 2.7 优惠券结构体
```c
typedef struct {
    char id[10];           // 优惠券ID
    char member_id[10];    // 会员ID
    float amount;          // 优惠金额
    char expire_date[12];  // 过期日期 (YYYY-MM-DD)
    int is_used;           // 是否已使用 (0-未使用, 1-已使用)
} Coupon;
```

### 2.8 用户结构体
```c
typedef struct {
    char username[20];     // 用户名
    char password[20];     // 密码
    int role;              // 角色 (1-管理员, 2-普通用户)
} User;
```

## 3. 文件存储格式设计

所有数据文件采用文本格式(.txt)存储，各字段之间使用逗号(,)分隔。

### 3.1 users.txt
存储系统用户信息，格式：
```
username,password,role
admin,admin123,1
cashier1,cash123,2
```

### 3.2 products.txt
存储商品信息，格式：
```
id,name,category,supplier,price,stock,production_date,shelf_life
P001,可口可乐,饮料,可口可乐公司,3.50,100,2025-03-01,180
P002,康师傅方便面,食品,康师傅,4.50,80,2025-02-15,365
```

### 3.3 members.txt
存储会员信息，格式：
```
id,name,phone,reg_date,total_amount,points,level
M001,张三,13812345678,2024-01-15,1580.50,1580,2
M002,李四,13987654321,2024-02-20,350.00,350,1
```

### 3.4 transactions.txt
存储交易记录，格式：
```
id,date_time,member_id,item_count,total_amount,discount_amount,final_amount,points_earned,points_used,coupon_used
20250415-0001,2025-04-15 10:30:25,M001,3,56.50,5.65,50.85,51,0,0
20250415-0002,2025-04-15 14:22:10,NONE,2,18.00,0.00,18.00,0,0,0
```

### 3.5 transaction_details.txt
存储交易明细，格式：
```
transaction_id,product_id,product_name,price,quantity,subtotal
20250415-0001,P001,可口可乐,3.50,2,7.00
20250415-0001,P002,康师傅方便面,4.50,1,4.50
```

### 3.6 coupons.txt
存储优惠券信息，格式：
```
id,member_id,amount,expire_date,is_used
C001,M001,10.00,2025-05-15,0
C002,M002,5.00,2025-04-30,1
```

## 4. 模块接口设计

### 4.1 用户界面模块
- `void displayCover()` - 显示系统封面
- `void displayMainMenu()` - 显示主菜单
- `void displayProductMenu()` - 显示商品管理菜单
- `void displaySalesMenu()` - 显示销售管理菜单
- `void displayMemberMenu()` - 显示会员管理菜单
- `void displayHelpDoc()` - 显示帮助文档

### 4.2 用户认证模块
- `int userLogin(char *username, char *password)` - 用户登录验证
- `void changePassword(char *username, char *newPassword)` - 修改密码

### 4.3 商品管理模块
- `int addProduct(Product product)` - 添加商品
- `int deleteProduct(char *id)` - 删除商品
- `int updateProduct(Product product)` - 更新商品信息
- `Product* findProductById(char *id)` - 按ID查询商品
- `Product* findProductByName(char *name)` - 按名称查询商品
- `Product** findProductsByCategory(char *category, int *count)` - 按分类查询商品
- `Product** findProductsBySupplier(char *supplier, int *count)` - 按供应商查询商品

### 4.4 销售管理模块
- `int addToCart(ShoppingCart *cart, char *product_id, int quantity)` - 添加商品到购物车
- `int updateCartItemQuantity(ShoppingCart *cart, int index, int quantity)` - 更新购物车商品数量
- `int removeFromCart(ShoppingCart *cart, int index)` - 从购物车移除商品
- `void clearCart(ShoppingCart *cart)` - 清空购物车
- `float calculateDiscount(ShoppingCart *cart, Member *member)` - 计算折扣
- `Transaction* checkout(ShoppingCart *cart, char *member_id)` - 结账并生成交易记录
- `int processReturn(char *transaction_id)` - 处理退货
- `int updateStock(char *product_id, int change)` - 更新库存
- `void checkLowStock()` - 检查库存不足商品

### 4.5 会员管理模块
- `int addMember(Member member)` - 添加会员
- `Member* findMemberById(char *id)` - 按ID查询会员
- `int updateMember(Member member)` - 更新会员信息
- `int addPoints(char *member_id, int points)` - 增加会员积分
- `int usePoints(char *member_id, int points)` - 使用会员积分
- `Coupon* createCoupon(char *member_id, float amount)` - 创建优惠券
- `int useCoupon(char *coupon_id)` - 使用优惠券
- `int updateMemberLevel(char *member_id)` - 更新会员等级

### 4.6 文件操作模块
- `int loadProducts(Product **products, int *count)` - 加载商品数据
- `int saveProducts(Product *products, int count)` - 保存商品数据
- `int loadMembers(Member **members, int *count)` - 加载会员数据
- `int saveMembers(Member *members, int count)` - 保存会员数据
- `int loadTransactions(Transaction **transactions, int *count)` - 加载交易记录
- `int saveTransaction(Transaction transaction)` - 保存交易记录
- `int saveTransactionDetails(TransactionDetail *details, int count)` - 保存交易明细
- `int loadCoupons(Coupon **coupons, int *count)` - 加载优惠券数据
- `int saveCoupons(Coupon *coupons, int count)` - 保存优惠券数据
- `int loadUsers(User **users, int *count)` - 加载用户数据

## 5. 程序流程设计

### 5.1 系统启动流程
1. 显示系统封面
2. 提示用户登录
3. 验证用户名和密码
4. 登录成功后显示主菜单

### 5.2 商品管理流程
1. 显示商品管理菜单
2. 根据用户选择执行相应操作
3. 操作完成后返回商品管理菜单或主菜单

### 5.3 销售流程
1. 创建新购物车
2. 添加商品到购物车
3. 询问是否是会员，如是则输入会员ID
4. 计算折扣和最终金额
5. 询问是否使用积分或优惠券
6. 确认结账，生成交易记录
7. 更新库存和会员积分
8. 打印小票

### 5.4 退货流程
1. 输入交易ID
2. 验证交易记录
3. 确认退货商品和数量
4. 更新库存
5. 如有会员，恢复积分
6. 生成退货记录

### 5.5 会员管理流程
1. 显示会员管理菜单
2. 根据用户选择执行相应操作
3. 操作完成后返回会员管理菜单或主菜单

## 6. 系统限制和约束

1. 最大支持商品数量：1000种
2. 最大支持会员数量：500人
3. 购物车最大商品种类：100种
4. 单次交易最大商品种类：100种
5. 会员等级划分：
   - 普通会员（累计消费0-1000元）：98折
   - 银卡会员（累计消费1000-5000元）：95折
   - 金卡会员（累计消费5000元以上）：9折
6. 积分规则：消费1元累计1积分
7. 积分兑换：100积分可兑换10元优惠券
