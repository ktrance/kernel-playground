# how to build

make

# install module

sudo insmod chardev.ko

# find device numbers 

Option 1:

dmesg | grep -i "pseudo"

Option 2:

cat /proc/devices | grep -i "pseudo"

# create device file (major = 241, minor = 0)

sudo mknod /dev/chardev c 241 0

# test 

cat /dev/chardev

# remove module

sudo rmmod kprobe_fork.ko