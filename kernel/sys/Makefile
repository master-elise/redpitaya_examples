obj-m	+= plateforme_simple.o
obj-m	+= plateforme_comm.o
obj-m	+= plateforme_comm_rw.o
obj-m	+= plateforme_comm_timer.o
obj-m	+= deux.o
obj-m	+= driver_alone.o
obj-m	+= device_alone.o
obj-m	+= driver_alone2.o

# obj-m	+= dummy_spi.o

# ccflags-y:=-E

all:
	make -C /usr/src/linux-headers-`uname -r` M=$(PWD) modules

clean:
	rm -f *.o *.ko modules.order  Module.symvers *.mod.c *.mod

#echo >&2 "  ERROR: Kernel configuration is invalid.";           \
#echo >&2 "         include/generated/autoconf.h or include/config/auto.conf are missing.";\
#echo >&2 "         Run 'make oldconfig && make prepare' on kernel src to fix it.";      \
