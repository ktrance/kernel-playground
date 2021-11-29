# how to build

make

# install module

sudo insmod chardev.ko

# find device numbers 

Option 1:
cat /proc/devices | grep pseudo

Option 2:

dmesg | grep "Initialized pseudo"

# create device file (major = 241, minor = 0)

sudo mknod /dev/chardev c 241 0

# test 

cat /dev/chardev

# remove module

sudo rmmod kprobe_fork.ko