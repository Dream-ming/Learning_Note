## 操作系统课设

课程要求挑几个实验实现，再写个报告就行，随便挑了几个看起来简单的

### 1.进程调度实验

实现了
* 进程的数目、到达时间、执行时间及时间片大小可任意规定
* 进程执行该过程可视化
* 计算进程各自的等待时间和周转时间
* 计算所有进程的平均等待时间和平均周转时间

效果

[![image.png](https://i.postimg.cc/02MVncwv/image.png)](https://postimg.cc/2VYd8dG9)

### 2.存储器分配实验

实现了
* 使用固定分区，最佳适应算法，对PCB进行动态分区
* 对每一个PCB进行分区之后，可以查看每一个分区的适应情况，以及PCB具体利用的存储空间物理地址
* 展示内存碎片情况

效果

[![image.png](https://i.postimg.cc/W4z4QF6M/image.png)](https://postimg.cc/bGX8nv5J)

[![image.png](https://i.postimg.cc/XYzWB2dC/image.png)](https://postimg.cc/jDPmG48x)

[![image.png](https://i.postimg.cc/wTW8nWWG/image.png)](https://postimg.cc/56FRCmWB)

[![image.png](https://i.postimg.cc/qMDS1Nqf/image.png)](https://postimg.cc/MvR9TK1t)

[![image.png](https://i.postimg.cc/VLJDLDfT/image.png)](https://postimg.cc/4mTzWvhb)

### 3.虚拟存储器页面置换算法

实现了
* 分别实验LRU算法和FIFO算法对页面进行置换
* 输出每一次页面替换后，物理块存储的页面编号，并输出是否命中
* 比较LRU和FIFO算法的缺页率
* 测试数据为｛７, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, -1(结束符号)}

效果

[![image.png](https://i.postimg.cc/4dJj8qCy/image.png)](https://postimg.cc/KKH9j98h)

[![image.png](https://i.postimg.cc/wTcfrFVs/image.png)](https://postimg.cc/BXbBLTRq)

[![image.png](https://i.postimg.cc/ydR43czX/image.png)](https://postimg.cc/wy9bStM7)

[![image.png](https://i.postimg.cc/xdkBBMqD/image.png)](https://postimg.cc/JyLTkybP)

[![image.png](https://i.postimg.cc/KvqqFj6Z/image.png)](https://postimg.cc/svW94jf8)

### 4.磁盘调度算法

实现了
* 设置初始磁头在5号磁道。规定每个扇区包含10个磁道号，一共8个扇区。磁盘旋转一个扇区为一个单位旋转延迟时间。磁头移动一个磁道为一个单位训导时间
* 实验的实验数据，一共10个服务，每一个服务需要的磁道号由随机数生成，范围在0-79
* 使用不同的寻道算法，分别计算每一个算法的寻道时间和旋转延迟，并输出不同寻道算法的访问顺先
* 实现磁盘调度算法：FCFS、SSTF、SCAN

效果

[![image.png](https://i.postimg.cc/dtqZqB5z/image.png)](https://postimg.cc/t7M4khQ5)
