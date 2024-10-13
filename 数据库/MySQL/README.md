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

2、聚合函数，将一列数据作为一个整体，进行纵向的计算
* count 统计数量
* max 最大值
* min 最小值
* avg 平均值
* sum 求和

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
   * select * from 表名;
* 设置别名，as关键字可省略
   * select 字段1 [as 别名1], 字段2 [as 别名2], ... from 表名;
* 去除重复记录，添加关键字distinct
   * select distinct 字段 from 表名; 

2、条件查询
* 基础语法
   * select 字段 from 表名 where 条件;
* where 条件中的运算符
   * <, <=, >, >=, =
   * <>, !=   // 不等于
   * between ... and ...  // 在某一个范围之内，含左右值
   * in(...)   // 是列表中的值之一
   * like 占位符   // 模糊匹配(_匹配单个字符，%匹配任意个字符)
   * 例：select * from user where name = '__';   // 查询名字为两个字的数据
   * 例：select * from user where name = '%5';   // 查询名字最后一个字为'5'的数据
   * is null    // 是null
   * and, &&, or, ||, not, !   // 与或非
   * is not null   // 非null
* 使用聚合函数查询
   * select count(*) from user;   // 统计表中有几条数据
   * select count(name) from user;    // 统计表中有几条数据，如果 name 值为 null，则不会被统计
   * select avg(age) from user where name = '张三';   // 计算 name 为张三的人的平均年龄
   * select max(age) from user;
   * select sum(age) from user;

3、分组查询
* 基础语法
   * select 字段 from 表名 [where 条件] group by 分组字段名 [having 分组后过滤条件];
   * 先执行 where 筛选出符合条件的数据，再进行 having 的分组操作
   * where 不能使用聚合函数，having可以
* 例子
   * 按照年龄分组，第一列返回不同年龄，第二列统计每一个年龄的人数
      * select age, count(*) from user group by age;
   * 找出 name 为张三的人，按年龄分组，找出组成员>=3的组，返回年龄和年龄对应的人数，num是count(*)的别名
      * select age, count(*) num from user where name = '张三' group by age having num >= 3;

4、排序查询
* 基础语法
   * select 字段 from 表名 order by 字段1 acs, 字段2 desc...;
   * 先按第一个字段排序，相同的话再按第二个字段排序
   * acs升序(可省略)，desc降序
* 例子
   * 按照注册时间降序，年龄升序，给表排序
   * select * from user order by enterdata decs, age acs;
   
5、分页查询
* 基础语法
   * select 字段 from 表名 limit 起始索引, 查询记录数;
   * 起始索引从0开始，查询第一页可以不写起始索引
   * 每一页的起始索引 = (页码 - 1) * 每页的记录数
* 例子
   * 将表按每页十条数据分页，查询第一页和第二页
   * select * from user limit 10;   // 第一页，省略起始索引
   * select * from user limit 10, 10;   // 第二页

6、顺序问题
* 编写顺序
   * select - from - where - group by - having - order by - limit
* 执行顺序
   * from - where - group by - having - select - order by - limit
   * 先执行的取的别名可以给后执行的用，后执行的取的别名先执行的不能用
   * select 中取的别名给 where 用会报错

### 数据控制语言DCL，用来创建数据库用户、控制数据库访问权限

1、查询用户
* use mysql;   // 在 mysql 的系统表中查询
* select * from user;

2、创建用户
* create user '用户名'@'主机名' identified by '密码';
* 用户名和密码自拟，主机名规定该用户可以在哪台主机上使用数据库
* 主机名：localhost - 当前主机，% - 所有主机

3、修改用户密码
* alter user '用户名'@'主机名' identified with mysql_native_password by '新密码';

4、删除用户
* drop user '用户名'@'主机名';

5、用户权限控制
* 权限
   * all, select, insert, update, delete, alter, drop, create
* 查询某用户的权限
   * show grants for '用户名'@'主机名';
* 授予权限
   * grant 权限 on 数据库名.表名 to '用户名'@'主机名';
* 撤销权限
   * revoke 权限 on 数据库名.表名 from '用户名'@'主机名';


## 函数

### 字符串函数

* 拼接字符串
   * concat(s1, s2, ...)
* 转换成小写，大写
   * lower(s)
   * upper(s)
* 用字符串 pad 对 s 的左边进行填充
