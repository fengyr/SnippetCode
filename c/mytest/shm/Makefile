CXX = gcc

TARGET = shm_test

C_FLAGS += -g -Wall 

LIB_FLAGS = -pthread 

INC = -I.

LIB_INC = -I.

all: $(TARGET)

$(TARGET): shm_test.o
	$(CXX) -o $@ $^  $(LIB_FLAGS) $(LIB_INC) $(C_FLAGS) $(INC)


clean:
	-rm -f *.o $(TARGET) 


