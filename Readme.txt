��������ע������
1��mysqlclient:yum install -y mysql-devel
2��hiredis������Դ���� make && make install
3��ldconfig ldconfig /usr/local/bin/ �� /etc/ld.so.conf.d/ ���½�localuserlib�ļ� д��/usr/local/bin/
4��ulimit -n 65535
5���½������м��ļ����Ŀ¼ mkdir debug
6������ make
7���½���־���Ŀ¼ mkdir log
8������ sh run.sh