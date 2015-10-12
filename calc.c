#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Calculator module");
MODULE_AUTHOR("Pasevina Polina");

#define DEVICE_NAME "calc"
#define DIVISION_BY_ZERO 1
#define INVALID_SIGN 2

static long a;
static long b;
static char operand;
static long result;

static int msg_size;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t , loff_t *);
static int device_init(void);
static void device_exit(void);
static int calculate(void);


static int major_number;


static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .read    = device_read,
    .write   = device_write,
    .open    = device_open,
    .release = device_release
 };


static int calculate(void)
{
    switch(operand) {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case '*':
        result = a * b;
        break;
    case '/':
        if(b == 0) {
            return DIVISION_BY_ZERO;
        }
        result = a / b;
        break;
    default:
        return INVALID_SIGN;
    }
    return 0;
}


static ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t * offset )
{
    int err = 0;
    int major;
    int minor;
    char msg[256];

    major = MAJOR(file_inode(file)->i_rdev);
    minor = MINOR(file_inode(file)->i_rdev);
    memset(msg, 0, sizeof(msg));

    switch(minor) {
    case 0:
        snprintf(msg, sizeof(msg), "a = %ld\n", a); 
        break;
    case 1:
        snprintf(msg, sizeof(msg), "b = %ld\n", b); 
        break;
    case 2:
        snprintf(msg, sizeof(msg), "operand = %c\n", operand); 
        break;
    case 3:
        err = calculate();
        switch(err)
        {
        case 0:
            snprintf(msg, sizeof(msg), "Result: %ld\n", result);
            break;
        case DIVISION_BY_ZERO:
            snprintf(msg, sizeof(msg), "Division by zero\n");
            break;
        case INVALID_SIGN:
            snprintf(msg, sizeof(msg), "Invalid operand\n");
            break;
        default:
            break;
        }
        break;
    default: 
        break;  
    }

    msg_size = strlen(msg);

    if(msg[*offset] == '\0') {
        printk(KERN_INFO "Already sent");
        return 0;
    }

    copy_to_user(buffer, &msg[*offset], 1);
    *offset += 1;

    return 1;
}



static ssize_t device_write(struct file *file, const char *buffer, size_t length, loff_t * offset)
{
    int major;
    int minor;
    char msg[256];

    major = MAJOR(file_inode(file)->i_rdev);
    minor = MINOR(file_inode(file)->i_rdev);
    memset(msg, 0, sizeof(msg));

    copy_from_user(msg, buffer, length);

    switch(minor) {
    case 0:
        kstrtol(msg, 10, &a);
        printk("a = %ld", a); 
        break;
    case 1:
        kstrtol(msg, 10, &b);
        printk("b = %ld", b); 
        break;
    case 2:
        operand = msg[0];
        printk("operand = %s", operand); 
        break;
    default: 
        break;  
    }

    msg_size = strlen(msg);
    return msg_size;
}


static int device_open(struct inode *inode, struct file *file)
{
    return 0;
}


static int device_release(struct inode *inode, struct file *file)
{
    return 0;
}


static int device_init(void)
{
    printk(KERN_ALERT "Calc driver loaded!\n");
    major_number = register_chrdev(200, DEVICE_NAME, &fops);

    if(major_number < 0) {
        printk(KERN_ALERT "Registering the character device failed with %d\n", major_number);
        return major_number;
    }

    printk("Calc module is loaded!\n");
    printk("Create a dev file with 'mknod /dev/first c %d 0'.\n", major_number);
    printk("Create a dev file with 'mknod /dev/second c %d 1'.\n", major_number);
    printk("Create a dev file with 'mknod /dev/operand c %d 2'.\n", major_number);
    printk("Create a dev file with 'mknod /dev/result c %d 3'.\n", major_number);   
    return 0;
}


static void device_exit(void)
{
    unregister_chrdev(200, DEVICE_NAME);
    printk(KERN_ALERT "Calc module is unloaded!\n");
}

module_init(device_init);
module_exit(device_exit);
