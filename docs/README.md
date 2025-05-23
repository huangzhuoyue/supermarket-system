# C语言超市管理系统说明文档

## 项目概述

本项目是一个基于C语言开发的命令行超市管理系统，旨在满足小型超市的日常管理需求。系统包含商品管理、销售管理和会员管理三个核心模块，采用模块化设计，使用文本文件存储数据。

## 系统特点

1. **完整的功能模块**：虽属"小型"系统，但包含了软件中常见的技术要素和功能，如软件封面界面、用户登录、主菜单、功能化模块、操作帮助等。

2. **模块化设计**：系统采用模块化设计，各功能模块相对独立，便于维护和扩展。

3. **数据持久化**：使用文本文件(.txt)存储数据，确保数据的持久性和可读性。

4. **用户友好界面**：提供清晰的菜单导航和操作提示，降低用户学习成本。

5. **安全性考虑**：实现了用户登录功能，不同角色拥有不同的操作权限。

## 系统架构

系统采用分层架构设计，主要分为以下几层：

1. **表示层**：负责与用户交互，显示菜单和信息
2. **业务逻辑层**：实现各功能模块的核心业务逻辑
3. **数据访问层**：负责数据的读写和持久化存储

## 核心功能

### 1. 商品管理
- 商品信息录入、修改、删除
- 库存更新和价格调整
- 按分类或供应商筛选商品

### 2. 销售管理
- 购物车功能（支持多商品累加、折扣计算）
- 交易记录（保存每笔销售明细，支持退货处理）
- 实时库存同步（销售后自动扣减库存，库存不足时触发预警）

### 3. 会员管理
- 积分系统（根据消费金额累计积分，支持积分兑换优惠券）
- 等级制度（按消费总额划分会员等级，不同等级享受不同折扣）

## 技术实现

### 开发环境
- 操作系统：支持Windows、Linux等多种操作系统
- 编译器：GCC (GNU Compiler Collection)
- 开发语言：C语言

### 关键技术
- 结构体设计：使用结构体表示商品、会员、交易等实体
- 文件操作：使用文件I/O函数实现数据的读写
- 内存管理：合理分配和释放内存，避免内存泄漏
- 字符串处理：安全地处理字符串，防止缓冲区溢出
- 模块化编程：使用头文件和源文件分离接口和实现

## 系统限制

1. 最大支持商品数量：1000种
2. 最大支持会员数量：500人
3. 购物车最大商品种类：100种
4. 单次交易最大商品种类：100种

## 项目文件结构

```
supermarket_system/
├── main.c                 # 主程序入口
├── common.h               # 公共头文件，定义常量和数据结构
├── common.c               # 公共函数实现
├── file_utils.h           # 文件操作函数声明
├── file_utils.c           # 文件操作函数实现
├── ui.h                   # 用户界面函数声明
├── ui.c                   # 用户界面函数实现
├── auth.h                 # 用户认证函数声明
├── auth.c                 # 用户认证函数实现
├── product.h              # 商品管理函数声明
├── product.c              # 商品管理函数实现
├── product_ui.h           # 商品管理界面函数声明
├── product_ui.c           # 商品管理界面函数实现
├── sales.h                # 销售管理函数声明
├── sales.c                # 销售管理函数实现
├── sales_ui.h             # 销售管理界面函数声明
├── sales_ui.c             # 销售管理界面函数实现
├── member.h               # 会员管理函数声明
├── member.c               # 会员管理函数实现
├── member_ui.h            # 会员管理界面函数声明
├── member_ui.c            # 会员管理界面函数实现
├── user_manual.md         # 用户手册
├── developer_guide.md     # 开发者指南
├── test_report.md         # 测试报告
└── README.md              # 项目说明文档
```

## 编译与运行

### 编译命令
```bash
gcc -o supermarket main.c common.c file_utils.c ui.c auth.c product.c product_ui.c sales.c sales_ui.c member.c member_ui.c
```

### 运行命令
```bash
./supermarket
```

## 使用说明

详细的使用说明请参考 `user_manual.md` 文件，其中包含了系统的功能介绍、操作指南和常见问题解答。

## 开发指南

如需进一步开发或修改系统，请参考 `developer_guide.md` 文件，其中包含了系统架构、数据结构设计、文件存储格式和主要功能模块说明等内容。

## 测试情况

系统已经过全面测试，测试结果请参考 `test_report.md` 文件，其中包含了测试内容、测试结果和发现的问题及修复情况。

## 参考资料

1. 《C程序设计》，谭浩强主编，清华大学出版社，2017年8月第五版
2. 《C程序设计习题解答与上机指导》谭浩强主编，清华大学出版社，2017年8月第五版

## 版权声明

本系统为课程设计作品，仅用于教学和学习目的，不得用于商业用途。
