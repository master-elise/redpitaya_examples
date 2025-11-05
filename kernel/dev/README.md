## Kernel module programming examples

Assumes a functional Buildroot environment including the Linux kernel
sources. Tested up to Buildroot 2024.11.1 with the Red Pitaya Zynq 7010
processor.

Check ``/proc/devices`` for already allocated major numbers when creating
a new entry in ``/dev`` with ``mknod /dev/mydev c major minor``.

``ln -s Makefile.red Makefile`` to compile on the Red Pitaya (or symbolic link of ``Makefile.pc``
to compile on the host)

* ``0mymod.c``: first example, ``module_init`` and ``module_exit``, getting familiar with the 
compilation procedure and the ``Makefile``
* ``1mymod.c``: communication and definition of the system call handling functions
* ``2mymod.c``
* ``2mymod.kernel_panic``
* ``3mymod_gpiolib_devicetree.c``: using gpiolib for accessing hardware
* ``4mymod.c``
* ``4mymod_pc.c``
* ``4mymod_version_gpiolib.c``
* ``5mymod_int.c``
* ``6mymod_int.c``
* ``6user_prog.c``
* ``7mymod_task.c``
* ``Makefile.pc``: Makefile for compiling on the host (usually a Debian GNU/Linux PC)
* ``Makefile.red``: Makefile for cross-compiling to the target
