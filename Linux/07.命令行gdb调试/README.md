## 命令行gdb调试

```c++
#include <iostream>
    
int main(int argc, char* argv[]) {
    printf("参数个数：%d\n", argc);
    for (int i = 0; i < argc; ++i)  {      
        printf("参数 %d: %s\n", i, argv[i]);
    }
    return 0;
}
```

### 命令行指令
$ g++ main.cpp -g -Wall -o app    (-g加入调试信息)    
$ gdb app    
$ ./app 1 2 3 4 5  (给argv传入6个参数)    


### 启动gdb之后的命令：
* set args: 给main传参数
* show args: 查看已设置的参数
* start: 执行第一行代码，输入 c 继续执行
* c/continue：继续执行
* run: 跑到断点处停止
* q/quit: 退出 gdb

* l/list: 查看main代码
  * l 行号: 查看某一行的代码
  * l 函数名：查看某个函数
  * l 文件名:行号/函数名：切换到其他文件的行或函数
* set list：以上查看默认上下10行，用set list 20改为20行
* show list：显示查看代码的行数

* b/break：设置断点
  * b 行号：在当前文件的指定行打断点
  * b 行号 if i == 5：在指定行设置条件断点
  * b 文件名:行号/函数名：在其他文件设置断点
  * i b：查看已有断点，可以看到断点的编号
  * 
* d/del/delete：删除断点
  * d 编号：删除指定编号的断点
  * d 编号1 编号2 编号2...：删除多个断点
  * d 编号1-编号2：删除多个连续的断点
 
* dis/disable：设置断点无效
  * 同 d 操作

* ena/enable：设置断点生效
  * 同 d 操作

* p 和 ptype
    * p 变量：打印变量的值
    * ptype 变量：打印变量的类型

* display：自动跟踪
  * display 变量：始终打印该变量
      
