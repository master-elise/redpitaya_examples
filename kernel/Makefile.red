BR?=../buildroot-2024.11.1_redpit
PATH:=$(PATH):$(BR)/output/host/usr/bin/

obj-m +=mymod.o 0mymod.o 1mymod.o 2mymod.o 3mymod_gpiolib_devicetree.o 4mymod.o 6mymod_int.o 7mymod_task.o 4mymod_version_gpiolib.o timer_mutex_read.o jmf.o

all:
	echo $(PATH)
	make ARCH=arm CROSS_COMPILE=arm-buildroot-linux-gnueabihf- \
-C $(BR)/output/build/linux-xilinx-v2024.1/ M=$(PWD) modules

clean:
	rm *.o *.ko *.mod.c .*.cmd
