CC = g++
CFLAGS = -g -O2 -Wall -I${DIR_INC} -I${INCLUDE}
INCLUDE = -L/usr/local/include -lev
LIBS  = -I/usr/local/lib/
DIR_INC = ./
DIR_SRC = .
DIR_OBJ = .
DIR_BIN = .

# SRC = $(wildcard ${DIR_SRC}/*.cpp )
SRC_Server = $(wildcard ${DIR_SRC}/server.cpp ${DIR_SRC}/test.cpp)
SRC_Client = $(wildcard ${DIR_SRC}/client.cpp)

OBJ_server = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC_Server} ))
OBJ_client = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC_Client} ))

TARGET_Server = tcp_server
TARGET_Client = tcp_client

BIN_TARGET_Server = ${DIR_BIN}/${TARGET_Server}
BIN_TARGET_Client = ${DIR_BIN}/${TARGET_Client}

TARGET = ${DIR_BIN}/${TARGET_Server}  ${DIR_BIN}/${TARGET_Client}  

all: $(TARGET)
.PHONY : all
${DIR_OBJ}/%.o : ${SRC_Server} ${SRC_Client}
	@echo "开始编译文件"
	$(CC) ${CFLAGS} -c $?
	-mv *.o $(DIR_OBJ)/
	@echo "编译文件完成"

${BIN_TARGET_Client} : ${OBJ_client}
	@echo "链接生成client文件目标文件"
	${CC} ${CFLAGS} ${OBJ_client}  -o $@ $(LIBS)  

${BIN_TARGET_Server} : ${OBJ_server}
	@echo "链接生成Server文件目标文件"
	${CC} ${CFLAGS} ${OBJ_server}  -o $@ $(LIBS)

.PHONY : all
.PHONY : clean 
clean:
	@echo "删除编译结果文件"
	-rm -f ${DIR_OBJ}/*.o ${BIN_TARGET_Server} ${BIN_TARGET_Client}

