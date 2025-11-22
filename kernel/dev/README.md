## Kernel module programming examples

Assumes a functional Buildroot environment including the Linux kernel
sources. Tested up to Buildroot 2024.11.1 with the Red Pitaya Zynq 7010
processor.

Check ``/proc/devices`` for already allocated major numbers when creating
a new entry in ``/dev`` with ``mknod /dev/mydev c major minor``.

``ln -s Makefile.red Makefile`` to compile for the Red Pitaya (or symbolic link of ``Makefile.pc``
to compile for the host). All compilation is completed on the host using the
Buildroot toolchain in the ``output/host/usr/bin`` subdirectory: never compile
on the target !

* ``0mymod.c``: first example, ``module_init`` and ``module_exit``, getting familiar with the 
compilation procedure and the ``Makefile``
* ``1mymod.c``: communication and definition of the system call handling functions. Assumes a
``/dev/entry`` with major number 91 was created using ``mknod``
* ``2mymod.c``: uses the misc device to auomatically create the entry in ``/dev/``
* ``2mymod.kernel_panic``
* ``3mymod_gpiolib_devicetree.c``: using gpiolib for accessing hardware
* ``4mymod.c``: blinking LED triggered by timer
* ``4mymod_pc.c``: periodic event on an x86 based PC (not relying on GPIO hardware)
* ``4mymod_version_gpiolib.c``: blinking LED triggered by timer, using gpiolib
* ``5mymod_int.c``: interrupt handling (MIO10 of Red Pitaya). Assumes PIO10 is changed from SPI to GPIO
with ``devmem 0xF8000728 32 0x00001600``
* ``6mymod_int.c``: interrupt handling (MIO10 of Red Pitaya) and sending a signal to a userspace program
(through SIGUSR1). To demonstrate beyond the ``dmesg`` message, compile (``arm-linux-gcc -o exec 6user_prog.c``) and
execute the userspace program to handle the signal in userspace whenever PIO10 is connected to ground.
* ``7mymod_task.c``: using a tasklet to delay handling the interrupt when the scheduler allows
* ``Makefile.pc``: Makefile for compiling on the host (usually a Debian GNU/Linux PC)
* ``Makefile.red``: Makefile for cross-compiling to the target
