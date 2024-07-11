# C语言学习笔记

## 1、sscanf

将字符串分割为想要的格式

使用正则表达式分割，%作为分隔符
* \[1-9]: 取出数字
* \[a-z]: 取出小写字母
* \[^A-Z]: 一直取，直到出现大写字母
* %\[^:]://%\[^:]:%\[1-9]: 一直取直到碰到冒号，跳过://一直取直到碰到冒号，跳过:取出后面所有的数字

```c++
const char *s = "http://www.baidu.com:1234
char protocol[32] = {0};
char host[128] = {0};
char port[8] = {0};
sscanf(s, "%[^:]://%[^:]:%[1-9]", protocol, host, port);
// protocol: http
// host: www.baidu.com
// port: 1234
```
