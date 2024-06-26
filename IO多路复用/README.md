# IO多路复用

[![7132cab2bbfa070f4bfaf61efa7a54c.png](https://i.postimg.cc/2yHzBLJ1/7132cab2bbfa070f4bfaf61efa7a54c.png)](https://postimg.cc/64vskqBK)

## Socket多线程操作

在Socket通信中，服务端有两类文件描述符，一个用于监听的，N个用于通信的

监听描述符的读缓冲区会等待客户端的connect请求，accept检测到读缓冲区有数据就会读出数据建立连接，否则一直阻塞

通信描述符的读缓冲区会等待客户端的send或者write请求，read或recv检测到读缓冲区有数据就会读出来，否则一直阻塞

通信描述符的写缓冲区，write或send操作，当写缓冲区有位置时，就写入并发送给客户端，否则阻塞

客户端的读写缓冲区操作类似

如果只有单线程，那么上述的阻塞会相互影响，read阻塞了就无法write或accept，于是有了多线程的Socket

## IO多路复用

把检测文件描述符的读写缓冲区的操作交给内核
