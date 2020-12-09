编译运行注意事项
1、安装mysql开发客户端mysqlclient: yum install -y mysql-devel
2、安装reddis开发库 hiredis：下载源代码 make && make install
3、设置动态库路径：ldconfig ldconfig /usr/local/bin/ 或 /etc/ld.so.conf.d/ 下新建localuserlib文件 写入/usr/local/bin/
4、设置打开文件限制: ulimit -n 65535
5、新建编译中间文件存放目录: mkdir debug
6、编译: make
7、新建日志存放目录: mkdir log
8、运行: sh run.sh