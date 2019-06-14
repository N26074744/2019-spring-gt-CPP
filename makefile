TARGET = CPP.exe

SRC_DIR = ./src/
INC_DIR = ./include/

SRC = $(wildcard $(SRC_DIR)*.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))
INC = -I $(INC_DIR)
FAKE_INC = ./fake-mininet-master/lib/
FAKE_LIB = ./fake-mininet-master/

CC = g++
CFLAGS = -Wall -g -O3

${TARGET} : ${OBJ}
	${CC} ${CFLAGS} $^ -o $@ -L ${FAKE_LIB} -lfakemn

$(SRC_DIR)%.o : $(SRC_DIR)%.cpp
	${CC} ${CFALGS} -c ${INC} -I ${FAKE_INC} $< -o $@

.PHONY : clean
clean :
	-rm ${TARGET} ${OBJ}
