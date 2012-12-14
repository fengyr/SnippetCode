CXX = gcc

TARGET = mm_read mm_write mm_fork

C_FLAGS += -g -Wall 

LIB_FLAGS = -pthread 

INC = -I.

LIB_INC = -I.

all: $(TARGET)

mm_read: mm_read.o
	$(CXX) -o $@ $^  $(LIB_FLAGS) $(LIB_INC) $(C_FLAGS) $(INC)

mm_write: mm_write.o
	$(CXX) -o $@ $^  $(LIB_FLAGS) $(LIB_INC) $(C_FLAGS) $(INC)

mm_fork: mm_fork.o
	$(CXX) -o $@ $^  $(LIB_FLAGS) $(LIB_INC) $(C_FLAGS) $(INC)

clean:
	-rm -f *.o $(TARGET) 

