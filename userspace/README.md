## Userspace examples

Run
```bash
make
```
to compile for the host (Intel PC), or
```bash
CC=arm-linux-gcc make
```
to compile for the target (Red Pitaya) assuming the Buildroot ``output/host/bin`` containing
the cross-compiler has been added to the ``PATH`` with
```bash
export PATH=$PATH:$BR/output/host/usr/bin
```
with ``$BR`` the location of the Buildroot targeting the Red Pitaya.
