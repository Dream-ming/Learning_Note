#  GNU Screen

在Linux下，通过开启会话窗口，将不同的进程分离运行

常用指令：
* screen -S name：创建一个名为 name 的会话
* Ctrl + a + d：退出当前 screen，将程序放在后台执行
* screen -ls：查看当前已有的 screen，会有ID和name信息
* screen -r 会话ID或name：进入会话
* screen -S name -X quit：删除名为name的会话
