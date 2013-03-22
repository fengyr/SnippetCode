MODULE_NAME:=hello
RESMAN_CORE_OBJS:=start.o #核心文件
RESMAN_GLUE_OBJS:=stop.o  #其他附属文件
$(MODULE_NAME)-objs:= $(RESMAN_CORE_OBJS) $(RESMAN_GLUE_OBJS)
obj-m:= $(MODULE_NAME).o
KDIR:=/lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

