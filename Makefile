CXX = g++

VPATH = ./MemDataBaseEnginer:./DataBaseEnginer:./CenterServer: \
		./UserServer:./DataServer:./GameServer:./ConntionServer

CFLAGS = -g -Werror -O0

LIBS = 	-L./lib -lcore \
		-L/usr/lib64/mysql -lmysqlclient \
		-lpthread -lhiredis -ldl

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
$(CURR_SRC_FILES:./%.cpp=./debug/%.o) \
$(MDE_SRC_FILES:./MemDataBaseEnginer/%.cpp=./debug/%.o) \
$(DBE_SRC_FILES:./DataBaseEnginer/%.cpp=./debug/%.o) \
$(CT_SRC_FILES:./CenterServer/%.cpp=./debug/%.o) \
$(US_SRC_FILES:./UserServer/%.cpp=./debug/%.o) \
$(DS_SRC_FILES:./DataServer/%.cpp=./debug/%.o) \
$(GS_SRC_FILES:./GameServer/%.cpp=./debug/%.o) \
$(CS_SRC_FILES:./ConntionServer/%.cpp=./debug/%.o) \

./debug/%.d:%.cpp
	@set -e; rm -f $@; $(CXX) -MM $< -o $@; \
    sed -i 's,\($*\).o[ :]*,debug/\1.o : ,g' $@; \
	echo '	$(CXX) -c $(CFLAGS) $$< -o $$@' >> $@; 

server:$(SEROBJ)
	$(CXX) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

%.so:./Game/%/*.cpp
	$(CXX) $(SOFLAGS) $(CFLAGS) $^ -o $@ 

-include $(SEROBJ:.o=.d)

.PHONY:all clean 

all:clean server

clean:
	rm -f ./debug/*.o ./core.* ./log/*
