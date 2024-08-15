# Linux下使用Webbench做网站压力测试

* 安装依赖、下载源码并安装
* 命令行使用

``` shell
sudo apt-get install exuberant-ctags // 安装依赖
wget http://blog.s135.com/soft/linux/webbench/webbench-1.5.tar.gz  // 下载源码
tar zxvf webbench-1.5.tar.gz  
cd webbench-1.5  
make && sudo make install
webbench --get // 查看指令帮助
webbench -c 500 -t 30 https://baidu.com // -c表示并发数， -t表示时间(秒)
```
