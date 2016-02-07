/*
 * Copyright (C) 2016 kopanitsa
 *
 * hello_world device driver for OpenWRT
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <linux/version.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/types.h>
#include <linux/cdev.h>

static unsigned int driver_major = 0; // dynamic allocation
static struct cdev driver_cdev;
static int simple_char_bufsize = 1;

static int hello_open(struct inode* inode, struct file* filp )
{
    printk( KERN_INFO "hello_driver: open!!\n" );
    return 0;
}

static int hello_release(struct inode *inode, struct file *filp)
{
    printk( KERN_INFO "hello_driver: close!!\n" );
    return 0;
}

static ssize_t hello_write(struct file *filp, const char *buf,
                         size_t count, loff_t *ppos)
{
    printk( KERN_INFO "hello_driver: write: %s\n", buf);
    return count;
}

static ssize_t hello_read(struct file *filp, char *buf, size_t count,
                        loff_t *ppos)
{
    printk( KERN_INFO "hello_driver: read: \n");
    // *pos += count;
    return count;
}

static const struct file_operations hello_fops = {
    .owner      = THIS_MODULE,
    .read       = hello_read,
    .write      = hello_write,
    .open       = hello_open,
    .release    = hello_release,
};
// static char buf[256];


// loaded to below
// /lib/modules/3.18.23/hello_driver.ko
// /rom/lib/modules/3.18.23/hello_driver.ko
static int __init hello_driver_init(void)
{
    printk(KERN_INFO "hello_driver loaded -s\n");
    int major;
    dev_t dev = MKDEV(driver_major, 0);
    int cdev_err = 0;

    if ( register_chrdev( 60, "hello_driver", &hello_fops ) ) {
        printk( KERN_INFO "hello_driver init failed\n" );
        return -EBUSY;
    }

    printk(KERN_INFO "hello_driver loaded -2\n");
    // generate driver major
    driver_major = major = MAJOR(dev);

    // generate character device
    cdev_init(&driver_cdev, &hello_fops);
    driver_cdev.owner = THIS_MODULE;

    // add file to /dev
    cdev_err = cdev_add(&driver_cdev, MKDEV(driver_major, 0), 1);
    if ( cdev_err ) {
        printk( KERN_INFO "hello_driver add failed\n" );
        return -EBUSY;
    }


    printk(KERN_INFO "hello_driver loaded -e\n");
    return 0;
}

static void __exit hello_driver_exit (void)
{
    unregister_chrdev(60, "hello_driver");
    printk(KERN_INFO "Successfully removed hello_driver\n");
}

module_param(simple_char_bufsize, int, S_IRUGO| S_IWUSR);
module_init(hello_driver_init);
module_exit(hello_driver_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("okada okada.takahiro111@gmail.com");
MODULE_DESCRIPTION("for openwrt device driver study");


