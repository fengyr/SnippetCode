CXX = gcc

TARGET = a.out

C_FLAGS += -g -Wall 

LIB_FLAGS = -pthread 

INC = -I.

LIB_INC = -I.

all: $(TARGET)

a.out:  mutex_cond.o
	$(CXX) -o $@ $^  $(LIB_FLAGS) $(LIB_INC) $(C_FLAGS) $(INC)


clean:
	-rm -f *.o $(TARGET) 

