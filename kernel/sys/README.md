* ``1plateforme_simple.c``: device and driver in same module source file

* ``2plateforme_double.c``: one driver and two devices in same source file

* ``3device_alone.c``, ``3driver_alone.c``: separate module with driver and device source

* ``4driver_alone.c``: separate driver module using ``module_platform_driver()``

* ``5plateforme_comm.c``: driver with communication

* ``6plateforme_comm_rw.c``: driver with communication

* ``7plateforme_comm_timer.c``: driver with communication

To demonstrate the output of the pre-processor, keep intermediate files
by <a href="https://stackoverflow.com/questions/21177935/kernel-module-source-file-after-preprocessing">adding the ``EXTRA_CFLAGS=’-save-temps’``</a> option.
