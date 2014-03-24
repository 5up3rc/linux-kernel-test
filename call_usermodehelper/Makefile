obj-m += call_usermodehelper.o

#generate the path
CURRENT_PATH:=$(shell pwd)

#the absolute path
LINUX_KERNEL_PATH:=/usr/src/linux-headers-$(shell uname -r)

#complie object
all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
#clean
clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean



