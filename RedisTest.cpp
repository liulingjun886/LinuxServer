#include<iostream>
#include<stdio.h>
#include<hiredis/hiredis.h>
using namespace std;
int main()
{
	redisContext* c = redisConnect("127.0.0.1", 6379);
	redisReply* reply = (redisReply*)redisCommand(c,"AUTH 123");
	reply = (redisReply*)redisCommand(c, "SCRIPT LOAD %s", "local a,b = '123 ','456' return {1,2}");
	printf("%d,%s\n", (int)reply->len,reply->str);

	reply = (redisReply*)redisCommand(c, "EVALSHA %s %d", reply->str, 0);
	printf("%d,%d\n", (int)reply->type,(int)reply->elements);
	freeReplyObject(reply);
	redisFree(c);
}
 