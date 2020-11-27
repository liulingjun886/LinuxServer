CC = g++
VPATH = ./CenterServer:./ConntionServer:./DataBaseEnginer:./DataServer: \
		./GameServer:./MemDataBaseEnginer:./UserServer

CURR_SRC_FILES = $(wildcard ./*.cpp)
MDE_SRC_FILES = $(wildcard ./MemDataBaseEnginer/*.cpp)
DBE_SRC_FILES = $(wildcard ./DataBaseEnginer/*.cpp)
CT_SRC_FILES = $(wildcard ./CenterServer/*.cpp)
US_SRC_FILES = $(wildcard ./UserServer/*.cpp)
DS_SRC_FILES = $(wildcard ./DataServer/*.cpp)
GS_SRC_FILES = $(wildcard ./GameServer/*.cpp)
CS_SRC_FILES = $(wildcard ./ConntionServer/*.cpp)


SEROBJ = $(CURR_SRC_FILES:./%.cpp=./debug/%.o) $(MDE_SRC_FILES:./MemDataBaseEnginer/%.cpp=./debug/%.o) \
		$(DBE_SRC_FILES:./DataBaseEnginer/%.cpp=./debug/%.o) $(CT_SRC_FILES:./CenterServer/%.cpp=./debug/%.o) \
		$(US_SRC_FILES:./UserServer/%.cpp=./debug/%.o) $(DS_SRC_FILES:./DataServer/%.cpp=./debug/%.o) \
		$(GS_SRC_FILES:./GameServer/%.cpp=./debug/%.o) $(CS_SRC_FILES:./ConntionServer/%.cpp=./debug/%.o) \


./debug/%.o:%.cpp
	$(CC) -c -g -Wall $< -o $@

server:$(SEROBJ)
	$(CC) -g -Wall -Werror -O0 $^ -o $@ -lpthread -lhiredis -ldl -L./lib -lcore -L/usr/lib64/mysql -lmysqlclient -Wl,--export-dynamic

%.so:./Game/%/Logic.cpp ./Game/%/UpGradeLogic.cpp
	g++ -fPIC -shared -g -Wall $^ -o $@
	
clean:
	rm -rf ./debug/*.o core.* *.so rm -f log/*

.PHONY:all clean server
