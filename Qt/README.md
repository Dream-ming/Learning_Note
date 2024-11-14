# Qt 笔记

## 一、基础知识

### 1、窗口

* QWidget 窗口
	* 所有窗口类的基类
	* 可以内嵌到其他窗口的内部（创建实例时，指定父窗口）
	* 可以作为独立的窗口显示（不指定父窗口），需要手动 show

* QDialog
	* QWidget 的子类
	* 非模态显示：与其他窗口平级显示（show）
	* 模态显示：阻塞其他窗口，不点掉不能用其他窗口（exec）
	* 不能内嵌

* QMainWindow

	* QWidget 的子类
	
	* 主窗口类，可以包含菜单栏、工具栏、状态栏
	
	* 不能内嵌
	
### 2、坐标系

* 坐标原点在左上角
* 向右 x 轴递增
* 向下 y 轴递增
* 内嵌窗口的坐标原点为父窗口的左上角

### 3、内存回收

* Qt 中大部分类都是 QObject 的子类
* 当 QObject 析构时，会将所有的子类对象自动析构
* 需要注意在构造时，给子类对象指定父类指针，即可实现自动回收
* 如果构造时没有指定父对象，可以调用 api 指定父对象
	* void Qwidget::setParent (QWidget *parent);
	* void QObject::setParent (QObject *parent); 

### 4、基础类型



## 二、Qt 库

### 1、QMessageBox

* 弹窗
* 指定弹窗标题
	* msgBox.setWindowTitle(QString);
* 指定弹窗内容
	* msgBox.setText(QString);
* 指定弹窗图标
	* msgBox.setIcon(QMessageBox::Icon);
	* QMessageBox::Information     - 信息提示
	* QMessageBox::Warning     	   - 警告提示
	* QMessageBox::Critical            - 错误提示
	* QMessageBox::Question         - 问题提示，一般是询问操作
* 指定弹窗内的按钮
	* msg.setStandardButtons(QMessageBox::StandardButtons);
	* QMessageBox::Ok                                       - 确定按钮
	* QMessageBox::Yes | QMessageBox::No     - 是/否按钮
	* QMessageBox::Cancel                                - 取消按钮
	* QMessageBox::Retry                                   - 重试按钮
* 设置详细信息
	* setDetailedText(QString);
	* 设置详细信息，点击 "详细信息" 按钮才能查看
* 设置默认按钮
	* setDefaultButton(QPushButton);
	* 设置默认按钮，用户按 "Enter" 即可选中该按钮
* setCheckBox(QCheckBox);
	* 在弹窗中加入复选框，比如用于 "记住我的选择" 功能

## 三、Qt Designer 控件

### 1、分割器 (QSplitter)

控件描述：一个分割控件，可以允许放左右两个垂直排列器，或上下两个水平排列器

问题描述：在 Qt Designer 的控件盒子中并不能直接找到该控件

解决方法：先随便放两个控件，然后在右边选中两个控件，右键 -> 布局 -> 使用拆分器水平布局 或 使用拆分器垂直布局，就会就两个控件放在分割器里了

## 四、杂项

### 1、版本问题

问题描述：在使用 Qt 6 跑项目的时候，弹 "无法定位程序输入点 xxx 于动态链接库 xxx 上"。

原因：之前下载过 Qt 5 ，卸载的时候直接删除了文件夹，没有用uninstall，动态库还留在电脑里，导致环境中同时存在两个版本的库，编译器不知道还用哪一个

解决：在 C 盘中搜索 Qt ，把相关的文件全部删除，再重新下载 Qt 6 的文件

### 2、头文件循环嵌套问题

问题描述：Qt 中使用窗口很容易会碰到的问题。父窗口想要调用子窗口，需要包含子窗口头文件。子窗口想要使用访问父窗口，又需要包含父窗口头文件。导致头文件循环嵌套

解决：前向声明。前向声明允许在一个类还没有完整定义的时候，就先声明。

前向声明步骤：

* parent.h 中不包含 son.h ，但是声明一个类（class son)，然后使用 son 的内容
* son.h 中包含 parent.h ，然后使用  parent 的内容
* parent.cpp 和 son.cpp 中都包含 son.h
* 即可相互调用内容

### 3、Qt 自动格式化代码

问题描述：公司规定的格式比较 crazy，两格空格tab，大括号不换行，代码间不留空行

解决：

* 下载 [美化器](https://sourceforge.net/projects/astyle/) 
* 在 Qt Creator 的编辑 - preferences - 美化器中，艺术风格使用自定义风格，值导入下述配置

````
--style=attach
indent=spaces=2	   # 缩进采用2个空格
indent-switches         # -S  设置 switch 整体缩进
indent-cases 	          # -K  设置 cases 整体缩进
indent-namespaces       # -N  设置 namespace 整体缩进
indent-preproc-block    # -xW 设置预处理模块缩进
indent-preproc-define   # -w  设置宏定义模块缩进	
pad-oper                # -p  操作符前后填充空格
delete-empty-lines      # -xe 删除多余空行
add-braces              # -j  单行语句加上大括号
align-pointer=name      # *、&这类字符靠近变量名字
#align-pointer=type       *、&这类字符靠近类型
````

