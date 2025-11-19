sudo rmmod composant
sudo rmmod dummy_platform
sudo insmod PC/composant.ko
sudo insmod PC/dummy_platform.ko
ls /sys/bus/iio/devices/iio\:device*/
# valeur initiale de value
cat /sys/bus/iio/devices/iio\:device*/out_voltage*raw
# on set value
echo "2" > /sys/bus/iio/devices/iio\:device*/in_accel100_raw
# on relit value
cat /sys/bus/iio/devices/iio\:device*/out_voltage101_raw
# value s'incremente a chaque lecture
cat /sys/bus/iio/devices/iio\:device*/out_voltage101_raw
cat /sys/bus/iio/devices/iio\:device*/out_voltage101_raw

