###############################################################################
# Usage:
#   $ make           Compile and link (or archive)
#   $ make clean     Clean the objectives and target.
###############################################################################

CROSS_COMPILE   =
OPTIMIZE        := 
WARNINGS        := -Wall
DEFS            := 
EXTRA_CFLAGS    := -g
# EXTRA_CFLAGS    += -g -DUSE_DEBUG -rdynamic
LDFLAGS         := -pthread -lm -lrt -lstdc++ -ldl

TARGET          := libapp.so
TARGET_TYPE     := so

###########################################################
USE_MYSQL 		:= FALSE
USE_SQLITE 		:= FALSE
USE_MODBUS 		:= FALSE
USE_DEVICE 		:= FALSE

# 第三方库配置
ifeq ($(USE_MYSQL),TRUE)
EXTRA_CFLAGS    += -DUSE_MYSQL
endif

ifeq ($(USE_SQLITE),TRUE)
EXTRA_CFLAGS    += -DUSE_SQLITE
endif

ifeq ($(USE_MODBUS),TRUE)
EXTRA_CFLAGS    += -DUSE_MODBUS
endif

ifeq ($(USE_DEVICE),TRUE)
EXTRA_CFLAGS    += -DUSE_DEVICE
endif
###########################################################

OUT_DIR 	= out
OBJ_DIR   	= $(OUT_DIR)/obj

INC_DIR  	= 	./include/ \
			./include/core/ \
			./include/core/db/ \
			./include/core/db/mysql/ \
			./include/core/db/sqlite3/ \
			./include/core/dev/ \
			./include/core/utils/ \
			./include/core/net/ \
			./include/core/modbus/ \
			./include/interface/ \
			/usr/include/ \
			/usr/local/include/ \


SRC_DIR   	= 	./src/core/ \
			./src/core/net/ \
			./src/core/utils/ \

EXTRA_SRC 	= 
EXCLUDE_FILES 	= 	./src/interface/ui_interface.c \
			./src/interface/options.c \
			./src/interface/telnet_proc.c \
			./src/runtime.c \
			./include/interface/ui_interface.h \
			./include/interface/options.h \
			./include/interface/telnet_proc.h \
			./include/interface/dev_serial.h \
			./include/runtime.h \

SUFFIX 		= c h cpp hpp cc
EXTRA_OBJS  =   ./libs/libstruct.a \
		    	./libs/libzlog.a \
				./libs/libthreadpool.a \

###########################################################
# mysql数据库相关配置
ifeq ($(USE_MYSQL),TRUE)
INC_DIR	    += 	/usr/local/mysql/include/mysql \
				/usr/local/mysql/include/ \

SRC_DIR 	+= 	./src/core/db/ \
				./src/core/db/mysql/ \

EXTRA_OBJS  += 	/usr/local/mysql/lib/libmysqlclient.so \

endif
#---------------------------------------------------------#

# sqlite数据库相关配置
ifeq ($(USE_SQLITE),TRUE)
INC_DIR	    += 	

SRC_DIR 	+= 	./src/core/db/ \
				./src/core/db/sqlite3/ \

EXTRA_OBJS  += 	./libs/libSQLiteCpp.a \
				/usr/lib/i386-linux-gnu/libsqlite3.so \

endif
#---------------------------------------------------------#

# Modbus相关配置
ifeq ($(USE_MODBUS),TRUE)
INC_DIR	    +=

SRC_DIR 	+= ./src/core/modbus/ \

EXTRA_OBJS  += ./libs/libmodbus.a \

endif
#---------------------------------------------------------#

# dev模块相关配置
ifeq ($(USE_DEVICE),TRUE)
INC_DIR	    +=	

SRC_DIR 	+= 	./src/core/dev/ \

EXTRA_OBJS  += 	/usr/lib/i386-linux-gnu/libdl.so \

endif
###########################################################
# 如果编译目标是可执行程序，
# 则需要加入测试相关代码。
ifeq ($(TARGET_TYPE),app)
	EXCLUDE_FILES = 
	SRC_DIR += 	./src \
		   		./src/interface \

endif
###########################################################

.other_task:

.other_clean:

#FUNC#  Add a new line to the input stream.
define add_newline
$1

endef

#FUNC# set the variable `src-x' according to the input $1
define set_src_x
src-$1 = $(filter-out $4,$(foreach d,$2,$(wildcard $d/*.$1)) $(filter %.$1,$3))

endef

#FUNC# set the variable `obj-x' according to the input $1
define set_obj_x
obj-$1 = $(patsubst %.$1,$3%.o,$(notdir $2))

endef

#VAR# Get the uniform representation of the object directory path name
ifneq ($(OBJ_DIR),)
prefix_objdir  = $(shell echo $(OBJ_DIR)|sed 's:\(\./*\)*::')
prefix_objdir := $(filter-out /,$(prefix_objdir)/)
endif

GCC      := $(CROSS_COMPILE)g++
G++      := $(CROSS_COMPILE)g++
SRC_DIR := $(sort . $(SRC_DIR))
inc_dir = $(foreach d,$(sort $(INC_DIR) $(SRC_DIR)),-I$d)

#--# Do smart deduction automatically
$(eval $(foreach i,$(SUFFIX),$(call set_src_x,$i,$(SRC_DIR),$(EXTRA_SRC),$(EXCLUDE_FILES))))
$(eval $(foreach i,$(SUFFIX),$(call set_obj_x,$i,$(src-$i),$(prefix_objdir))))
$(eval $(foreach f,$(EXTRA_SRC),$(call add_newline,vpath $(notdir $f) $(dir $f))))
$(eval $(foreach d,$(SRC_DIR),$(foreach i,$(SUFFIX),$(call add_newline,vpath %.$i $d))))

all_objs = $(foreach i,$(SUFFIX),$(obj-$i))
all_srcs = $(foreach i,$(SUFFIX),$(src-$i))

CFLAGS       = $(EXTRA_CFLAGS) $(WARNINGS) $(OPTIMIZE) $(DEFS)
TARGET_TYPE := $(strip $(TARGET_TYPE))

ifeq ($(filter $(TARGET_TYPE),so ar app),)
$(error Unexpected TARGET_TYPE `$(TARGET_TYPE)')
endif

ifeq ($(TARGET_TYPE),so)
 CFLAGS  += -fpic -fPIC -shared
 LDFLAGS += -shared
endif

PHONY = all .mkdir clean

all: .mkdir .other_task $(TARGET)

define cmd_o
$$(obj-$1): $2%.o: %.$1  $(MAKEFILE_LIST)
	$(GCC) $(inc_dir) -Wp,-MT,$$@ -Wp,-MMD,$$@.d $(CFLAGS) -c -o $$@ $$<

endef
$(eval $(foreach i,$(SUFFIX),$(call cmd_o,$i,$(prefix_objdir))))

ifeq ($(TARGET_TYPE),ar)
$(TARGET): AR := $(CROSS_COMPILE)ar
$(TARGET): $(all_objs)
	rm -f $@
	$(AR) rcvs $@ $(all_objs)
else
$(TARGET): LD = $(if $(strip $(src-cpp) $(src-cc) $(src-cxx)),$(G++),$(GCC))
$(TARGET): $(all_objs)
	$(LD) $(LDFLAGS) $(all_objs) $(EXTRA_OBJS) -o $@
endif

.mkdir:
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi

clean: .other_clean
	rm -f $(prefix_objdir)*.o $(TARGET)

-include $(patsubst %.o,%.o.d,$(all_objs))

.PHONY: $(PHONY)

