## GCC、G++用法

用法：    

gcc code.c -o aa  

g++ code.cpp -o aa 

直接生成可执行文件aa，有多个源文件时，直接写在一起，用空格隔开

-o：用于指定生成文件的名称，不写的话，默认生成可执行文件 a.out    
-E：预处理指定源文件，不进行编译    
-S：编译指定源文件，不进行汇编    
-c：编译、汇编指定源文件，不进行链接        
-o：指定生成文件的名称    
-I directory：指定头文件的目录    
-g：在编译的时候加入调试信息    
-D：在编译的时候指定一个宏(#IFDEF 宏 #ENDIF 在编译的时候用-D加宏，可以当日志输出)    
-w：不生成警告信息    
-Wall：生成所有警告信息    
-On（-O2）：添加On优化    
-std (-std=c++17)：指定语言版本   
-L：指定编译的时候，搜索的库的路径   
-l：指定使用的库的名字(libxxx.a，只写xxx)    
-fpic/FPIC：制作动态库的时候必须添加，将动态库的文件放到动态库的代码加载区    

----

### g++工作流程

-E -S -c -o用法    

文件：code.cpp

预处理命令：g++ -E code.cpp -o code.i    
文件：code.cpp(源文件)、code.i(源文件)

源文件编译：g++ -S code.i -o code.s    
文件：code.cpp(源文件)、code.i(源文件)、code.s(汇编文件)

汇编器编译：g++ -c code.s -o code.o    
文件：code.cpp(源文件)、code.i(源文件)、code.s(汇编文件)、code.o(二进制文件)

链接：g++ code.o -o code    
文件：文件：code.cpp(源文件)、code.i(源文件)、code.s(汇编文件)、code.o(二进制文件)、code(可执行文件)

执行：./code    
终端输出运行结果

----
