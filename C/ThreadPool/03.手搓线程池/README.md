## 跟着 [大佬](https://www.bilibili.com/video/BV1jV411J795/?spm_id_from=333.999.0.0&vd_source=9c56e925395f2439a0d359b116a85277) 手搓线程池

### VS 上 ssh 远程连接 Linux 

首先创建空项目时，找到适用于 Linux 终端的选项，没有的话下载拓展

[![ab9789b35f87d7b20b1e2a127293ed3.png](https://i.postimg.cc/hP7XZGnD/ab9789b35f87d7b20b1e2a127293ed3.png)](https://postimg.cc/1fsRfs42)
   
前置：Linux 需要下载ssh

VS导航栏 -- 工具 -- 选项 -- 下滑找跨平台 -- 连接管理器 -- 输入自己的Linux信息 -- 连接成功即可

完了更新一下远程标头，就可以有Linux库的自动补全了

写完代码编译或运行，就会在连接的Linux用户~目录下生成内容，可以在VS上跑，也可以在Linux上跑

具体VS在Linux上的体现，右键VS的项目，选最下面的"属性"，可以改配置。(默认在~下生成一个project文件)
