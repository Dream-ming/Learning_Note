# Windows下打包Qt项目为可执行程序

### 参考链接：

* https://blog.csdn.net/a1547998353/article/details/140413232
* https://blog.csdn.net/yaojinjian1995/article/details/106420089

### 操作流程

* Qt 构建并允许一个 release 版本
* 在项目路径的 /build/release/release 中找到 .exe 文件，单独拿出
* 打开终端
  * 搜索 Qt for Desktop
  * 没有的话，在 Qt 的 MinGW 文件中，shift + 右键，在终端中打开
* 使用 windeployqt 命令为 .exe 文件补充动态库
  * windeployqt D/xxx/xxx/xx.exe
* 使用 [Enigma Virtual Box](https://enigmaprotector.com/en/downloads.html) 封包
  * 选择需要封包的 xx.exe
  * 默认在同级目录下生成 xx_boxed.exe
  * 左下角 Add Folder Recursive 指定存储动态库的目录
* 生成的 xx_boxed.exe 就是封装好的，可以单独运行的执行程序
