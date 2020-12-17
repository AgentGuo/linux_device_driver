#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("flygoast");
MODULE_DESCRIPTION("A Simple Character Device driver module");

static struct cdev cdev;
static dev_t  devno;

static char tmp[128] = {0};

// 打开设备
static int
charDevice_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "open\n");
    return 0;
}

// 释放设备
static int
charDevice_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "release\n");
    return 0;
}

// 设备读操作
static ssize_t
charDevice_read(struct file *filep, char __user *buf, size_t count, loff_t *offset)
{
    printk(KERN_INFO "read offset:%lld\\n", *offset);
    if(count <= *offset){
        // 如果count小于偏移量，那么直接读取count个字节
        if (copy_to_user(buf, tmp + *offset - count, count) != 0) {
            // 读取失败
            return -EFAULT;
        }
        // 读取成功，偏移量减去count，并且返回读取字节数
        *offset -= count;
        return count;
    }
    else{
        // count大于偏移量，无法读取count个字节，读取失败
        return -EFAULT;
    }
}

// 设备写操作
static ssize_t
charDevice_write(struct file *filep, const char __user *buf, size_t count,
            loff_t *offset)
{
    size_t  avail;

    printk(KERN_INFO "write offset:%lld\n", *offset);
    // 缓冲区剩余字节数
    avail = sizeof(tmp) - *offset;

    memset(tmp + *offset, 0, avail);

    if (count > avail) {
        // 写入字节数count大于剩余字节数，那么就写入剩余字节数（有多少写多少）
        if (copy_from_user(tmp + *offset, buf, avail) != 0) {
            // 写入失败
            return -EFAULT;
        }
        // 写入成功，偏移量加上写入字节数，并返回
        *offset += avail;
        return avail;

    } else {
        // 写入字节数count小于剩余字节数，直接写入count个字节
        if (copy_from_user(tmp + *offset, buf, count) != 0) {
            // 写入失败
            return -EFAULT;
        }
        *offset += count;
        return count;
    }
}

// 重定位offset
static loff_t
charDevice_llseek(struct file *filep, loff_t off, int whence)
{
    loff_t  newpos;

    switch (whence) {
    case 0: /* SEEK_SET */
        newpos = off;
        break;
    case 1: /* SEEK_CUR */
        newpos = filep->f_pos + off;
        break;
    case 2: /* SEEK_END */
        newpos = sizeof(tmp) + off;
        break;
    default:
        return -EINVAL;
    }

    if (newpos < 0) {
        return -EINVAL;
    }

    filep->f_pos = newpos;
    return newpos;
}

// 文件操作结构映射
static const struct file_operations  fops = {
    .owner = THIS_MODULE,
    .open = charDevice_open,
    .release = charDevice_release,
    .read = charDevice_read,
    .llseek = charDevice_llseek,
    .write = charDevice_write,
};

// 设备注册
static int __init charDevice_init(void) {
    int    ret;

    printk(KERN_INFO "Load charDevice\n");

    devno = MKDEV(113, 0);  // 主设备号
    ret = register_chrdev_region(devno, 1, "charDevice");  // 主设备号，次设备号，设备名称

    if (ret < 0) {
        return ret;
    }

    cdev_init(&cdev, &fops);
    cdev.owner = THIS_MODULE;

    cdev_add(&cdev, devno, 1);

    return 0;
}
// 设备注销
static void __exit charDevice_cleanup(void) {
    printk(KERN_INFO "cleanup charDevice\n");
    unregister_chrdev_region(devno, 1);
    cdev_del(&cdev);
}

module_init(charDevice_init);
module_exit(charDevice_cleanup);

MODULE_LICENSE("GPL");