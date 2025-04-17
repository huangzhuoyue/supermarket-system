# C语言超市管理系统开发文档

## 系统架构

本系统采用模块化设计，主要分为以下几个模块：
- 用户界面模块：负责与用户交互，显示菜单和信息
- 用户认证模块：负责用户登录验证
- 商品管理模块：负责商品的增删改查和分类查询
- 销售管理模块：负责购物车、交易记录和库存同步
- 会员管理模块：负责会员信息、积分和等级管理
- 文件操作模块：负责数据的读写和持久化存储

## 数据结构设计

系统使用以下主要数据结构：

### 商品结构体
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

### 会员结构体
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

### 购物车结构体
```c
typedef struct {
    CartItem items[MAX_CART_ITEMS];   // 购物车项数组
    int item_count;        // 购物车中的商品数量
    float total_amount;    // 总金额
    float discount_amount; // 折扣金额
    float final_amount;    // 最终金额
} ShoppingCart;
```

### 交易记录结构体
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

## 文件存储格式

系统使用以下文本文件存储数据：

### users.txt
存储系统用户信息，格式：
```
username,password,role
admin,admin123,1
cashier1,cash123,2
```

### products.txt
存储商品信息，格式：
```
id,name,category,supplier,price,stock,production_date,shelf_life
P001,可口可乐,饮料,可口可乐公司,3.50,100,2025-03-01,180
P002,康师傅方便面,食品,康师傅,4.50,80,2025-02-15,365
```

### members.txt
存储会员信息，格式：
```
id,name,phone,reg_date,total_amount,points,level
M001,张三,13812345678,2024-01-15,1580.50,1580,2
M002,李四,13987654321,2024-02-20,350.00,350,1
```

### transactions.txt
存储交易记录，格式：
```
id,date_time,member_id,item_count,total_amount,discount_amount,final_amount,points_earned,points_used,coupon_used
20250415-0001,2025-04-15 10:30:25,M001,3,56.50,5.65,50.85,51,0,0
20250415-0002,2025-04-15 14:22:10,NONE,2,18.00,0.00,18.00,0,0,0
```

### transaction_details.txt
存储交易明细，格式：
```
transaction_id,product_id,product_name,price,quantity,subtotal
20250415-0001,P001,可口可乐,3.50,2,7.00
20250415-0001,P002,康师傅方便面,4.50,1,4.50
```

### coupons.txt
存储优惠券信息，格式：
```
id,member_id,amount,expire_date,is_used
C001,M001,10.00,2025-05-15,0
C002,M002,5.00,2025-04-30,1
```

## 主要功能模块说明

### 1. 商品管理模块
- **addProduct**: 添加新商品
- **deleteProduct**: 删除商品
- **updateProduct**: 更新商品信息
- **findProductById**: 按ID查询商品
- **findProductByName**: 按名称查询商品
- **findProductsByCategory**: 按分类查询商品
- **findProductsBySupplier**: 按供应商查询商品
- **displayProduct**: 显示单个商品信息
- **displayAllProducts**: 显示所有商品

### 2. 销售管理模块
- **initShoppingCart**: 初始化购物车
- **addToCart**: 添加商品到购物车
- **updateCartItemQuantity**: 更新购物车商品数量
- **removeFromCart**: 从购物车移除商品
- **clearCart**: 清空购物车
- **calculateDiscount**: 计算折扣
- **checkout**: 结账并生成交易记录
- **processReturn**: 处理退货
- **displayCart**: 显示购物车内容
- **displayTransaction**: 显示交易记录

### 3. 会员管理模块
- **addMember**: 添加会员
- **updateMember**: 更新会员信息
- **findMemberById**: 按ID查询会员
- **findMemberByPhone**: 按手机号查询会员
- **addPoints**: 增加会员积分
- **usePoints**: 使用会员积分
- **updateMemberLevel**: 更新会员等级
- **createCoupon**: 创建优惠券
- **useCoupon**: 使用优惠券
- **displayMember**: 显示会员信息
- **displayAllMembers**: 显示所有会员

## 编译与运行说明

### 编译命令
```bash
gcc -o supermarket main.c common.c file_utils.c ui.c auth.c product.c product_ui.c sales.c sales_ui.c member.c member_ui.c
```

### 运行命令
```bash
./supermarket
```

## 系统限制与约束

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

## 开发注意事项

1. 所有输入都应进行有效性检查
2. 文件操作应检查返回值，确保操作成功
3. 内存分配应检查是否成功，并在不再需要时释放
4. 字符串操作应注意缓冲区大小，防止溢出
5. 用户界面应提供清晰的提示和反馈
6. 数据修改后应及时保存到文件

## 未来扩展方向

1. 增加库存预警和自动订货功能
2. 增加销售统计和报表功能
3. 增加员工管理和权限控制
4. 增加商品促销和组合折扣功能
5. 增加数据备份和恢复功能
6. 增加图形用户界面
7. 增加网络功能，支持多终端操作
