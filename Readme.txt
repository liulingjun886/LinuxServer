编译运行注意事项
1、mysqlclient:yum install -y mysql-devel
2、hiredis：下载源代码 make && make install
3、ldconfig ldconfig /usr/local/bin/ 或 /etc/ld.so.conf.d/ 下新建localuserlib文件 写入/usr/local/bin/
4、ulimit -n 65535
5、新建编译中间文件存放目录 mkdir debug
6、编译 make
7、新建日志存放目录 mkdir log
8、运行 sh run.sh