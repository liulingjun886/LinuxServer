CXX = g++

VPATH = ./MemDataBaseEnginer:./DataBaseEnginer:./CenterServer: \
		./UserServer:./DataServer:./GameServer:./ConntionServer

CFLAGS = -g -Werror -O0

LIBS = 	-L./lib -lcore \
		-L/usr/lib64/mysql -lmysqlclient \
		-lhiredis -lpthread -ldl

LDFLAGS = -Wl,--export-dynamic \

SOFLAGS = -fPIC -shared

CURR_SRC_FILES 	= $(wildcard ./*.cpp)
MDE_SRC_FILES  	= $(wildcard ./MemDataBaseEnginer/*.cpp)
DBE_SRC_FILES 	= $(wildcard ./DataBaseEnginer/*.cpp)
CT_SRC_FILES 	= $(wildcard ./CenterServer/*.cpp)
US_SRC_FILES 	= $(wildcard ./UserServer/*.cpp)
DS_SRC_FILES 	= $(wildcard ./DataServer/*.cpp)
GS_SRC_FILES 	= $(wildcard ./GameServer/*.cpp)
CS_SRC_FILES 	= $(wildcard ./ConntionServer/*.cpp)

SEROBJ = \
$(CURR_SRC_FILES:./%.cpp=./build/%.o) \
$(MDE_SRC_FILES:./MemDataBaseEnginer/%.cpp=./build/%.o) \
$(DBE_SRC_FILES:./DataBaseEnginer/%.cpp=./build/%.o) \
$(CT_SRC_FILES:./CenterServer/%.cpp=./build/%.o) \
$(US_SRC_FILES:./UserServer/%.cpp=./build/%.o) \
$(DS_SRC_FILES:./DataServer/%.cpp=./build/%.o) \
$(GS_SRC_FILES:./GameServer/%.cpp=./build/%.o) \
$(CS_SRC_FILES:./ConntionServer/%.cpp=./build/%.o) \

./build/%.d:%.cpp
	@set -e; rm -f $@; $(CXX) -MM $< -o $@; \
    sed -i 's,\($*\).o[ :]*,build/\1.o : ,g' $@; \
	echo '	$(CXX) -c $(CFLAGS) $$< -o $$@' >> $@; 

server:$(SEROBJ)
	$(CXX) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

%.so:./Game/%/*.cpp
	$(CXX) $(SOFLAGS) $(CFLAGS) $^ -o $@ 

-include $(SEROBJ:.o=.d)

.PHONY:all clean 

all:clean server

clean:
	rm -f ./build/*.o ./core.* ./log/*
