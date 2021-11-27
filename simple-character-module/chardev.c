/**
 * This file contains a simple pseudo charater device driver
 * 
 * Kernel modules have a different include path than user land development. 
 * Include the search paths to your development environment config. 
 * For VSCode it does not work optimally as you cannot specify $(uname -r) in the paths, 
 * and need to upgrade the paths manually after kernel upgrade.
 */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DEVICE_MEMORY_SIZE 1024

char device_buffer[DEVICE_MEMORY_SIZE];

dev_t device_number;

struct cdev character_device;
struct file_operations character_device_fops = {
    .owner =    THIS_MODULE,
    // .llseek =   char_llseek,
    // .read =     char__read,
    // .write =    char_write,
    // .open =     char_open,
    // .release =  char_release,
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
 * TODO: the driver should support 
 * 1. open (strictly not necessary)
 * 2. seek
 * 3. read(sync)
 * 4. write(sync) 
 * 5. nmap ??
 */

static void __exit char_driver_cleanup(void) {
    pr_info("Cleaning up pseudo charater driver");
    //todo:
    //unregister_chrdev_region
    //cdev_del ?
}

//int char_driver_open()

module_init(char_driver_init);
module_exit(char_driver_cleanup);

MODULE_LICENSE("GPL");