EXTRA_CFLAGS = -Wall

obj-m := LcdI2c.o

LcdI2c-y =  LcdI2c_1602_driver.o LcdI2c_1602.o
PWD := $(shell pwd)
CROSS=/home/tuan/work/bbb/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
KER_DIR=/home/tuan/work/bbb/kernelbuildscripts/KERNEL

all:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KER_DIR) M=$(PWD) modules

clean:
	make -C $(KER_DIR) M=$(PWD) clean