#include<iostream>
#include<stdio.h>
#include"./include/hiredis/hiredis.h"
using namespace std;
int main()
{
	redisContext* c = redisConnect("127.0.0.1", 6379);
	printf("error = %d\n",c->err);

	redisReply* reply = (redisReply*)redisCommand(c,"AUTH 123");
	printf("%d\n",reply->type);
	freeReplyObject(reply);

	
	reply = (redisReply*)redisCommand(c, "SCRIPT LOAD %s","return");
	printf("%d,%d,%s\n",reply->type, (int)reply->len,reply->str);

//	reply = (redisReply*)redisCommand(c, "EVALSHA %s %d", reply->str, 0);
//	printf("%d,%d\n", (int)reply->type,(int)reply->elements);
	freeReplyObject(reply);
	redisFree(c);
}
 
