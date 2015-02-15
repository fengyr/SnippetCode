###############################################################################
# Usage:
#   $ make           Compile and link (or archive)
#   $ make clean     Clean the objectives and target.
###############################################################################

CROSS_COMPILE   =
OPTIMIZE        := 
WARNINGS        := -Wall -Wno-unused -Wno-format -Wextra
DEFS            := 
EXTRA_CFLAGS    :=
LDFLAGS         := -pthread -lm

# 输出目录，会自动创建
OUT_DIR 	    = out
OBJ_DIR   	    = $(OUT_DIR)/obj

# 头文件目录
INC_DIR   		= ./include \
				  ../include/ \
				  ../include/core/ \
				  ../include/core/db/ \
				  ../include/core/db/mysql/ \
			  	  ../include/core/db/sqlite3/ \
				  ../include/core/dev/ \
				  ../include/core/utils/ \
			 	  ../include/core/net/ \
				  ../include/core/modbus/ \
				  ../include/interface/ \

# 源文件目录
SRC_DIR   		= ./src \
				  ./src/interface/
# 额外包含的源文件列表
EXTRA_SRC 		= 
# 排除的文件列表
EXCLUDE_FILES 	= 
# 源文件后缀
SUFFIX 			= c cpp cc cxx
# 额外的目标文件
# 第三方的库需要在这里写全路径名
# 例如：/usr/local/lib/libprotobuf.so
# 否则无法找到符号
EXTRA_OBJS      = ./libs/libgtest.a \
				  ./libs/libgtest_main.a \
				  ../libapp_main.so

# 目标名称和类型，不需要考虑后缀名，定义多个名称实际上多个拷贝
# 如：$(OUT_DIR)/main 
TARGET          := test_main
TARGET_TYPE     := app
# TARGET_TYPE     := ar
# TARGET_TYPE     := so

# 其他的任务
.other_task:

# 其他任务的清除工作
.other_clean:

#####################################################################################
#  Do not change any part of them unless you have understood this script very well  #
#  This is a kind remind.                                                           #
#####################################################################################

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
 CFLAGS  += -fpic -shared
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

