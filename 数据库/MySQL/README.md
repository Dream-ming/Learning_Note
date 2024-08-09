# MySQL笔记

## API

包含头文件 #include <mysql.h>

### 初始化

* MYSQL *mysql_init(MYSQL *mysql);
  * 初始化时参数指定 NULL，返回一个 MYSQL* 类型
 
### 连接到 mysql 服务器


## 配置环境

### Windows

* 在mysql官方下载 mysql-8.4.0-winx64.zip 回来解压
* 以管理员身份打开 cmd，转到 mysql 的 bin 目录下
* mysqld --install 安装服务
* mysqld --initialize --console 初始化服务，会返回初始密码
* net start mysql 启动服务
* mysql -u root -p 登录验证，需要输入密码
* alter user 'root'@'localhost' identified by '111'; 修改密码为111
* exit 退出
* mysql 0u root -p 重新登录，验证密码是否修改
* 登录数据库后，CREATE DATABASE 数据库名; 创建一个数据库并命名
* 在系统环境变量里新建变量名 mysql，变量值 mysql 文件的地址
* 在 path 种添加变量 %mysql%\bin
* 在 mysql 文件下创建一个 my.ini 配置文件，内容如下
```
[mysqld]
character-set-server=utf8mb4
bind-address=0.0.0.0
port=3306
default-storage-engine=INNODB
[mysql]
default-character-set=utf8mb4
[client]
default-character-set=utf8mb4
```

* VS 使用 mysql 时
  * 在属性-VC++目录-包含目录中，添加 mysql 的 include 文件路径
  * 在属性-VC++目录-库目录中，添加 lib 的文件路径
  * 在连接器-输入-附加依赖项中，添加 libmysql.lib
  * 复制一个 libmysql.dll 到项目文件下
  * 完成以上步骤，即可使用 mysql 文件

