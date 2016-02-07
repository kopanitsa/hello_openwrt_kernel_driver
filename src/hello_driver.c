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
#include <asm/uaccess.h> // copy_from_user, copy_to_user

#define MODNAME "hello_driver"
#define MINOR_COUNT 1 // 接続するマイナー番号数
static dev_t dev_id;  // デバイス番号
static struct cdev c_dev; // キャラクタデバイス構造体

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
    .opctl      = NULL,
};


// loaded to below
// /lib/modules/3.18.23/hello_driver.ko
// /rom/lib/modules/3.18.23/hello_driver.ko
static int __init hello_driver_init(void)
{
    printk(KERN_INFO "hello_driver loaded -s\n");

    int ret;

    ret = alloc_chrdev_region( &dev_id, // 最初のデバイス番号が入る
                               0,  // マイナー番号の開始番号
                               MINOR_COUNT, // 取得するマイナー番号数
                               MODNAME // モジュール名
        );
    if( ret < 0 ){
        printk( KERN_WARNING "alloc_chrdev_region failed\n" );
        return ret;
    }

    // キャラクタデバイス初期化
    // ファイルオペレーション構造体の指定もする
    cdev_init( &c_dev, &hello_fops );
    c_dev.owner = THIS_MODULE;

    // キャラクタデバイスの登録
    // MINOR_COUNT が 1 でマイナー番号の開始番号が 0 なので /dev/modtest0 が
    // 対応するスペシャルファイルになる
    ret = cdev_add( &c_dev, dev_id, MINOR_COUNT );
    if( ret < 0 ){
        printk( KERN_WARNING "cdev_add failed\n" );
        return ret;
    }

    printk(KERN_INFO "hello_driver loaded -e\n");
    printk( KERN_INFO "hello major = %d\n", MAJOR( dev_id ) );
    printk( KERN_INFO "hello minor = %d\n", MINOR( dev_id ) );
    return 0;
}

static void __exit hello_driver_exit (void)
{
    // キャラクタデバイス削除
    cdev_del( &c_dev );

    // デバイス番号の返却
    unregister_chrdev_region( dev_id, MINOR_COUNT );

    printk(KERN_INFO "Successfully removed hello_driver\n");
}

module_init(hello_driver_init);
module_exit(hello_driver_exit);

MODULE_DESCRIPTION(MODNAME);
MODULE_LICENSE("GPL2");


