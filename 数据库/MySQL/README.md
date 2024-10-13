# MySQL笔记

## SQL语句

### 基础内容

1、数据类型：
* 数值类
    * 在关键字后面加 unsigned 使用无符号类型
    * tinyint、smallint、mediumint、int、bigint: 都是整数，分别占1、2、3、4、8个字节
        * tinyint unsigned：无符号1字节的整数 
    * float、double，浮点数
        * double(4, 1)：一共四位，小数占一位的浮点数类型
    * decimal，小数值

* 字符串类型
    * char、varchar(n): 定长(10)字符串，变长字符串
    * tinyblob、blob、mediumblob、longblob: 二进制文本，长度不同
        * 视频、音频、软件安装包是二进制文本，但是一般存放在单独的资源文件夹里，不放在数据库
    * tinytext、text、mediumtext、longtext: 不同长度的文本

* 日期时间类型
    * date: YYYY-MM-DD
    * time: HH:MM:SS
    * year: YYYY
    * datetime: YYYY-MM-DD HH:MM:SS

2、函数


### 数据定义语言DDL，定义数据库对象（数据库、表、字段）

1、数据库操作：   
* 查询当前存在的数据库
    * show databases;
* 查询数据库
    * select database();
* 创建数据库，中括号中的内容可有可无
    * create database [if not exists] 数据库名 [default charset 字符规则如utf8mb4] [collate 排序规则];
* 删除数据库
    * drop database [if exists] 数据库名;
* 使用数据库
    * use 数据库名;

2、表操作：   
* 查询当前数据库中所有的表
    * show tables;
* 查询表内容
    * desc 表名;
* 查询表的建表语句
    * show create table 表名;
* 创建一张表
    * create table 表名(字段1 类型 [comment 注释], 字段2 类型 [comment 注释]) [comment 表注释];
    * 表内不同字段用逗号分开，最后一个字段切记不要加逗号
    * 注释需要写comment关键字
* 修改表名
    * alter table 表名 rename to 新表名;
* 删除表
    * drop table [if exists] 表名;
* 删除表，并重新创建该表，相当于清空表操作
    * truncate table 表名;

3、字段操作：
* 添加字段
    * alter table 表名 add 字段名 类型(长度) [comment 注释] [约束];
* 仅修改字段的数据类型
    * alter table 表名 modify 字段名 新数据类型(长度);
* 修改字段名和字段类型
    * alter table 表名 change 旧字段名 新字段名 类型(长度) [comment 注释] [约束];
* 删除字段
    * alter table 表名 drop 字段名;

### 数据操作语言DML，对表中的数据进行增删改

1、添加数据(insert)    
* 添加数据，只给几个字段赋值
   * insert into 表名 (字段1, 字段2, ...) values (值1, 值2, ...);
* 添加数据，给全部字段都赋值
   * insert into 表名 values (值1, 值2, ...);
* 批量添加数据
   * insert into 表名 (字段1, 字段2, ...) values (值1, 值2, ...), (值1, 值2, ...), (值1, 值2, ...);
   * insert into 表名 values (值1, 值2, ...), (值1, 值2, ...), (值1, 值2, ...);
* 如果插入的值是字符串和日期类型，要加单引号

2、修改数据(update)    
* 修改数据，中括号中的内容可以没有，如果不指定条件，则修改整张表的数据
   * update 表名 set 字段1 = 值1, 字段2 = 值2, ... [where 条件];
 
3、删除数据(delete)
* 删除数据，如果不指定条件，会删除整张表
* 无法删除某一个字段的内容，可以用update修改为null
   * delete from 表名 [where 条件];

### 数据查询语言DQL，查询表中的记录

1、基本查询
* 查询多个字段
   * select 字段1, 字段2, 字段3, ... from 表名;
* 查询整张表
* 

### 数据控制语言DCL，用来创建数据库用户、控制数据库访问权限
