## Kernel module programming examples

Assumes a functional Buildroot environment including the Linux kernel
sources. Tested up to Buildroot 2024.11.1 with the Red Pitaya Zynq 7010
processor.

Check ``/proc/devices`` for already allocated major numbers when creating
a new entry in ``/dev`` with ``mknod /dev/mydev c major minor``.
