ifneq ($(KERNELRELEASE),)
obj-m := simple_char.o 
else
 
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd) 
 
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	@echo "\n"
	@echo "-----------------------------------"
	@echo "写操作可能导致/var/log/kern.log增大"
	@echo "-----------------------------------"
endif 

clean:
	rm -rf *.o *.order *.symvers *.ko *.mod.c
