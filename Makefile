cc = g++
serobj = main.o Server.o NetSink.o DataBaseEnginer.o DataBaseService.o \
DataBaseSink.o MySql.o MemDataBaseEnger.o MemDataBaseService.o MemSink.o MyRedis.o \
DataSerNetSink.o ConnSerNetSink.o CliNetSink.o DataSerManager.o ConnCliNetSink.o \
GameCliNetSink.o NetGameSerManager.o NetConnSerManager.o \
RoomManager.o Room.o UserInfo.o NetSinkObj.o \


%.o:%.cpp
	$(cc) -c -g -Wall $< -o $@ -I./include

server:$(serobj)
	$(cc) -g -Wall -Werror -O0 $^ -o $@ -lpthread -lhiredis -ldl -L./lib -lcore -L/usr/lib64/mysql -lmysqlclient -Wl,--export-dynamic

%.so:./Game/%/Logic.cpp ./Game/%/UpGradeLogic.cpp
	g++ -fPIC -shared -g -Wall $^ -o $@ -I./include
	
clean:
	rm -rf *.o core.* *.so rm -f log/*

.PHONY:all clean server

