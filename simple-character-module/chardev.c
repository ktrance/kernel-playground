/**
 * This file contains a simple pseudo charater device driver
 * 
 * Kernel modules have a different include path than user land development. 
 * Include the search paths to your development environment config. 
 * For VSCode it does not work optimally as you cannot specify $(uname -r) in the paths, 
 * and need to upgrade the paths manually after kernel upgrade.
 * 
 * TODO: Ths driver ONLY supports one device right now....
 */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEVICE_MEMORY_SIZE 1024
char device_buffer[DEVICE_MEMORY_SIZE]; 

dev_t device_number;

struct cdev character_device;

static int char_open(struct inode *i, struct file *f) {
    int minor = MINOR(i->i_rdev);
    pr_info("Pseudo character device open minor = %d", minor);

    //todo:

    return 0;
}

static int char_close(struct inode *i, struct file *f) {
    int minor = MINOR(i->i_rdev);
    
    pr_info("Pseudo character device close minor = %d", minor);

    //todo:
    return 0;
}

static ssize_t char_read(struct file *f, char __user *buf, size_t len, loff_t *f_pos) {

    char memory_buffer = 'a';

    if(*f_pos >= DEVICE_MEMORY_SIZE) {
        return 0;
    }

    copy_to_user(buf, &memory_buffer, 1);
    *f_pos+=1;
    return 1;
}

struct file_operations character_device_fops = {
    .owner =    THIS_MODULE,
    .read =     char_read,
    .open =     char_open,
    .release =  char_close,
};

/**
 * For the driver registration to be succesful you need the following steps:
 * 1. Allocate or register the device numbers
 * 2. Create the cdev structure with the supported file operations 
 * 3. Create a device(cdev) structure 
 * 4. Register the device structure with VFS 
 */

static int __init char_driver_init(void) {
    int ret;

    pr_info("Initializing pseudo charater driver");
    /**
     * Dynamically allocate a new charater device region. This will later be used to tie the driver to the VFS.  
     * Parameters:
     * 1. The device number structure to fill
     * 2. The major number to start from
     * 3. The number of devices
     * 4. The name of the driver
     * 
     * A register_chrdev_region() function exist but is only useful if you already know the numbers. 
     * Best practice to avoid collisions is to use the alloc_ function.
     */
    
    if((ret = alloc_chrdev_region(&device_number, 0, 1, "pseudo_character_driver")) < 0) {
        pr_err("Unable to allocate region for pseudo character device");
        return ret;
    }

    pr_info("Initialized pseudo character device with (major, minor) (%d, %d)", MAJOR(device_number), MINOR(device_number));

    /**
     * Initialize the charater device 
     */
    cdev_init(&character_device, &character_device_fops);

    /**
     * Register the device with VFS
     * Third parameter is the number of devices
     */
    character_device.owner = THIS_MODULE;
    if((ret = cdev_add(&character_device, device_number, 1)) < 0) { 
        pr_err("Unable add pseudo character device");
        return ret;
    }

    return 0;
}

/**
 * todo: remember to use printk_ratelimit() before printing messages
 */

//done
static void __exit char_driver_exit(void) {
    pr_info("Cleaning up pseudo charater driver");
    cdev_del(&character_device);
    unregister_chrdev_region(device_number, 1);
}

module_init(char_driver_init);
module_exit(char_driver_exit);

MODULE_LICENSE("GPL");