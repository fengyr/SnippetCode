CXX = gcc
SRC_DIR= ./
OBJ_DIR= ./
OBJ_EXT= .o
CSRC_EXT= .c

C_FLAGS += -g -Wall 
INC = -I.
LIB_FLAGS = -pthread -lm
LIB_INC = -L.

# Comment/uncomment the following line to disable/enable debugging
DEBUG = y
# Add your debugging flag (or not) to C_FLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -DUSE_DEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif
C_FLAGS += $(DEBFLAGS)
# Debug setting end

EXE_TARGET = malloc_test
OBJECTS = $(OBJ_DIR)malloc_test$(OBJ_EXT)
STATIC_LIB = 

all: $(EXE_TARGET) $(OBJECTS)

$(EXE_TARGET): $(OBJECTS)
	@echo
	@echo "Compiling $< ==> $@..."
	$(CXX) -o $@ $^  $(LIB_FLAGS) $(LIB_INC) $(C_FLAGS) $(INC) $(STATIC_LIB)

$(OBJ_DIR)%$(OBJ_EXT): $(SRC_DIR)%$(CSRC_EXT)
	@echo
	@echo "Compiling $< ==> $@..."
	$(CXX)  $(INC) $(C_FLAGS) -c $< -o $@

clean:
	-rm -f $(EXE_TARGET) $(OBJECTS) GPATH GRTAGS GTAGS tags *~


