#include "MySql.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
	/*

	std::string str = "a'a'a'"; 
	string::size_type  pos(0); 
	for( pos = str.find("'",pos);pos != string::npos ; pos = str.find("'",++pos))
  		str.replace(pos++,1,"\'");
	str = "'" + str + "'";
	printf("%s \n",str.c_str());*/
	printf("%d\n",0x8FFFFF);
	CMySql mysql;
	if(!mysql.Init())
	{
		printf("mysql connected failer\n");
		return 0;
	}
	srand(time(NULL));
	printf("mysql connected sucess\n");
	int nCount = 0;
	do
	{
		mysql.SetSpName("AddInfo");
		mysql.AddStrParam("''james");
		mysql.AddNumParam(rand()%30);
		mysql.ExecPro();
		
		mysql.SetSpName("GetInfo");
		unsigned char a = 1;
		mysql.AddNumParam(a);
		mysql.AddOutParam("@ret");
		printf("execret = %d\n",mysql.ExecPro());
		while(mysql.HaveNext())
		{
			printf("id = %d, name = %s, age = %d, ret = %d\n",mysql.GetNumValue("Id"),mysql.GetStrValue("name"),mysql.GetNumValue("age"),mysql.GetOutNumValue("@ret"));
			mysql.MoveNext();
		}
	}
	while (++nCount < 1);

	//getchar();
}
//cancel
//ok