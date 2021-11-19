How to build

Just run make. The build uses a thin Makefile wrapper over the Kbuild file

https://www.kernel.org/doc/html/latest/kbuild/modules.html

# install module

sudo insmod kprobe_fork.ko

# remove module

sudo rmmod kprobe_fork.ko