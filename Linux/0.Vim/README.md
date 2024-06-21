# Vim使用手册

## 1.自动补全

装了个ycm，本来想来写个记录

发现根本记不得来时的路

太混乱了，依托依托的网络教程

最后应该是无脑跟官方的README做了一遍成功的

中间也不知道有没有用到其他教程里的东西

## 2.复制粘贴

将vim里的内容复制到其他进程

首先下载需要的工具

$ sudo apt-get install vim vim-scripts vim-gtk

下载完成后

$ vim --version

可以看到 +clipboard 和 +xterm_clipboard ，表示可支持系统剪切板

复制操作：
* ESC进入正常模式，ggvG，选中所有内容
* :进入命令行模式，"+y，三个字符，将选中的内容复制到系统的+寄存器中
* 在别的 vim 使用"+p，在别的进程 Ctrl v 即可粘贴

## 3.撤销反撤销
* u：撤销一次
* 3 u：撤销三次
* :u 或 :undo：末行模式撤销一次
* CTRL r 或 :red 或 :redo：撤销撤销
