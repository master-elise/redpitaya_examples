BR?=/home/jmfriedt/buildroot-2025.05.1_redpit
PATH:=$(PATH):$(BR)/output/host/usr/bin/

obj-m += 1plateforme_simple.o 2plateforme_double.o 3device_alone.o 3driver_alone.o 4driver_alone.o 5plateforme_comm.o 6plateforme_comm_rw.o 7plateforme_comm_timer.o

all:
	echo $(PATH)
	make ARCH=arm CROSS_COMPILE=arm-buildroot-linux-gnueabihf- \
-C $(BR)/output/build/linux-xilinx-v2024.1/ M=$(PWD) modules

clean:
	rm *.o *.ko *.mod.c .*.cmd
