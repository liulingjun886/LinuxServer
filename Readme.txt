��������ע������(Ŀǰֻ֧��64λLinuxϵͳ)

ϵͳ�汾��CentOS release 6.10
gcc�汾��4.4.7

1����װmysql�����ͻ���mysqlclient: yum install -y mysql-devel
2����װreddis������ hiredis������Դ���� make && make install
3�����ö�̬��·����ldconfig ldconfig /usr/local/bin/ �� /etc/ld.so.conf.d/ ���½�localuserlib�ļ� д��/usr/local/bin/
4�����ô��ļ�����: ulimit -n 65535
5���½������м��ļ����Ŀ¼: mkdir debug
6������: make
7���½���־���Ŀ¼: mkdir log
8������: sh run.sh

2022/3/6
ȡ��ͨ�����������ͺ��������ʹ���ҵ����ģ����ô�����Ӧ�ײ�ģ��ʱ���봴��ҵ��ģ��ص�����
Ŀǰȫ���ɱ���ͨ�����������ݿ�û�л�����δʵ�ʲ�������