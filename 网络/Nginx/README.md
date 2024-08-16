# Nginx

## 功能说明

nginx可以将端口接收到的报文转发给其他端口   

比如：一个运行在10000端口的服务器，客户端访问的时候默认走 80 或 443 端口，就可以通过nginx转发到 10000 端口

## 配置

``` nginx
# 先下载
sudo apt update
sudo apt install nginx
# 进入配置文件
vim /etc/nginx/sites-available/default
# 在文件中配置
server {
    # 此 server 监听80端口来自 example.com 域名的请求，如果接收到数据，就转发到本机的10000端口
    listen 80;
    server_name example.com;

    location / {
        proxy_pass http://127.0.0.1:10000;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }
}
# 以上是一个server配置，如果有多个要求，配置多个 server{} 即可
```

## 负载均衡

应对高并发时，nginx可以将同一个域名的请求，均衡转发给多个服务器

```nginx
# 用于承接并发的服务器所监听的端口
upstream backend {
    server 127.0.0.1:10000;
    server 127.0.0.1:10001;
}

# server 中使用backend指定转发的端口
server {
    listen 80;
    server_name example.com;

    location / {
        proxy_pass http://backend;  # 均衡转发给backend指定的端口
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }
}
```
