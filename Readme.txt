��������ע������
1����װmysql�����ͻ���mysqlclient: yum install -y mysql-devel
2����װreddis������ hiredis������Դ���� make && make install
3�����ö�̬��·����ldconfig ldconfig /usr/local/bin/ �� /etc/ld.so.conf.d/ ���½�localuserlib�ļ� д��/usr/local/bin/
4�����ô��ļ�����: ulimit -n 65535
5���½������м��ļ����Ŀ¼: mkdir debug
6������: make
7���½���־���Ŀ¼: mkdir log
8������: sh run.sh