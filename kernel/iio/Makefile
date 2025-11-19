obj-m	+= composant1.o
obj-m	+= composant2.o
obj-m	+= dummy_platform.o

# ccflags-y:=-E

all:
	make -C /usr/src/linux-headers-`uname -r` M=$(PWD) modules
	mkdir -p PC
	mv *.ko PC

clean:
	rm -f *.o *.ko modules.order  Module.symvers *.mod.c
