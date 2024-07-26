# screen

screen 的作用：让程序在服务器后台持续运行

使用流程：   
* sudo apt-get install screen
  * 下载 screen 程序
* screen -S name
  * 创建一个 screen，给其命名
* 在新跳出来的窗口执行想要的命令
  * 注意当前所在的目录，运行命名时上级下级目录
  * 比例运行服务器项目 ./Reactor.out
* ctrl + a + d
  * 保存并退出
  * 此时screen将刚才执行的命令放在后台执行，终端可以干其他事情
 
其他命令：    
* screen -ls
  * 查看所有创建的 screen
  * 会显示 PID.name
* screen -r -d PID
  * 进入编号为 PID 的 screen
  * 运行产生的结果会显示出来
  * 可以执行命令，然后保存退出，继续放在后台执行
* screen -S PID -X quit
  * 删除 PID 这个 screen
