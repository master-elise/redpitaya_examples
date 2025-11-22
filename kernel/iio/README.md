``dummy_plaform``: platform device for triggering the IIO drivers

``composant1``: channel definition without macros

``composant2``: channel definition with macros

To execute:
```
make
./go.sh
```

Be aware that the number of entries in ``/sys/bus/iio`` might vary with configuration.
On a desktop PC, IIO might not be loaded at all and ``sudo lsmod | grep industrialio``
must show
```
industrialio          139264  0
```
otherwise ``sudo modprobe industrialio``. On a laptop PC, ``/sys/bus/iio`` might already
have an entry in ``/sys/bus/iio/devices`` as ``iio:device0`` and the newly loaded device
will appear as ``iio:device1``, hence the use of wildcards in the ``go.sh`` script automating
the demonstration.
