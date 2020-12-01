CC = g++
CFLAGS = -g -Werror -o0
SHARE_FLAGS = -fPIC -shared
LIB = 	-L./lib -lcore \
		-L/usr/lib64/mysql -lmysqlclient \
		-lpthread -lhiredis -ldl

VPATH = ./MemDataBaseEnginer:./DataBaseEnginer:./CenterServer: \
		./UserServer:./DataServer:./GameServer:./ConntionServer

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
	$(CC) -c $(CFLAGS) $< -o $@

server:$(SEROBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) -Wl,--export-dynamic

%.so:./Game/%/*.cpp
	g++ $(SHARE_FLAGS) $(CFLAGS) $^ -o $@ 
	
clean:
	rm -f ./debug/*.o core.* ./log/*

all:clean server

.PHONY:all clean
