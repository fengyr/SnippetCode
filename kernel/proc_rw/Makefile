#module name
MODULE_NAME := test

#core src
RESMAN_CORE_OBJS := proc_rw.o 

#glue src
RESMAN_GLUE_OBJS :=  

$(MODULE_NAME)-objs := $(RESMAN_CORE_OBJS) $(RESMAN_GLUE_OBJS)
obj-m := $(MODULE_NAME).o

#kernel src dir
KDIR := /lib/modules/$(shell uname -r)/build

#include dir
INCLUDE := -I.

#cflags setting
EXTRA_CFLAGS := $(INCLUDE) -DMODULE

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

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

install:
	sudo insmod $(MODULE_NAME).ko

uninstall:
	sudo rmmod $(MODULE_NAME).ko


