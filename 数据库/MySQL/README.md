# MySQL笔记

## SQL语句

数据类型：
* 数值类
    * 在关键字后面加 unsigned 使用无符号类型
    * tinyint、smallint、mediumint、int、bigint: 都是整数，分别占1、2、3、4、8个字节
        * tinyint unsigned：无符号1字节的整数 
    * float、double，浮点数
        * double(4, 1)：一共四位，小数占一位的浮点数类型
    * decimal，小数值

* 字符串类型
    * char、varchar: 定长(10)字符串，变长字符串
    * tinyblob、blob、mediumblob、longblob: 二进制文本，长度不同
        * 视频、音频、软件安装包是二进制文本，但是一般存放在单独的资源文件夹里，不放在数据库
    * tinytext、text、mediumtext、longtext: 不同长度的文本

* 日期时间类型
    * date: YYYY-MM-DD
    * time: HH:MM:SS
    * year: YYYY
    * datetime: YYYY-MM-DD HH:MM:SS


### 数据定义语言DDL，定义数据库对象（数据库、表、字段）

数据库操作：   
* show databases;  // 查询所有的数据库
* select database();  // 查询当前数据库
* create database [if not exists] 数据库名 [default charset 字符规则如utf8mb4] [collate 排序规则];
   * 创建数据库，中括号的内容可有可无 
* drop database [if exists] 数据库名;  // 删除数据库
* use 数据库名;  // 使用数据库

表操作：   
* show tables;  // 查询当前数据库所有表
* desc 表名;  // 查询表内容
* show create table 表名;  // 查询表的建表语句
* create table 表名(字段1 类型 [comment 注释], 字段2 类型 [comment 注释]) [comment 表注释];
   * 创建一张表
   * 表内不同字段用逗号分开，最后一个字段切记不要加逗号
   * 注释需要写comment关键字
* alter table 表名 rename to 新表名;  // 修改表名
* drop table [if exists] 表名;  // 删除表
* truncate table 表名;  // 删除表，并重新创建该表，等价于清空表的操作

字段操作：
* 添加字段
    * alter table 表名 add 字段名 类型(长度) [comment 注释] [约束];
    * 别忘了分号
* 仅修改字段的数据类型
    * alter table 表名 modify 字段名 新数据类型(长度);
* 修改字段名和字段类型
    * alter table 表名 change 旧字段名 新字段名 类型(长度) [comment 注释] [约束];
* 删除字段
    * alter table 表名 drop 字段名;

### 数据操作语言DML，对数据库表中的数据进行增删改



### 数据查询语言DQL，查询数据库中表的记录

### 数据控制语言DCL，用来创建数据库用户、控制数据库访问权限
