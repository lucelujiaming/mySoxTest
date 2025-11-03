#include <linux/module.h>
#include <linux/clk-provider.h>
#include <linux/i2c.h>
#include <linux/bcd.h>
#include <linux/rtc.h>

#define CH32V006_CNT 1
#define CH32V006_NAME "ch32v006"
#define CH32V006_FILE_SIZE  21

struct ch32v006_dev
{
    int major;
    int minor;
    dev_t devid;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    void *private_data;
    unsigned short ir, als, ps;
};

static unsigned int device_reg_address = 0;
static struct ch32v006_dev ch32v006dev;
static unsigned int file_pos = 0;

/******************************I2C 设备数据收发处理***********************************/
/* 读取N个寄存器值 */
static int ch32v006_read_regs(struct ch32v006_dev *dev, uint8_t byte_addr, void *buf, int len)
{
    struct i2c_msg msg[3];

    struct i2c_client *client = (struct i2c_client *)dev->private_data;

    /* msg[0]写入需要读取的地址 */
    msg[0].addr = client->addr;     /* 从机地址，也就是CH32V006地址 */
    printk("[%s:%d] Write msg[0].addr = 0x%02X\n", __func__, __LINE__, msg[0].addr);
    msg[0].flags = 0;               /* 0 - 表示要发送的数据*/
    msg[0].buf = &byte_addr;        /* 表示为要发送的数据,也就是寄存器地址*/
    printk("[%s:%d] byte_addr = 0x%02X\n", __func__, __LINE__, byte_addr);
    msg[0].len = 1;                 /* 要发送的数据长度：寄存器地址长度+实际的数据长度*/
    i2c_transfer(client->adapter, msg, 1);
    
    /* msg[1]读取数据 */
    msg[0].addr = client->addr;     /* 从机地址，也就是AP3216C地址*/
    printk("[%s:%d] Read msg[0].addr = 0x%02X\n", __func__, __LINE__, msg[0].addr);
    msg[0].flags = I2C_M_RD;        /* 表示读数据*/
    msg[0].buf = buf;               /* 接收到的从机发送的数据*/
    msg[0].len = len;               /* 要读取的寄存器长度*/

    return i2c_transfer(client->adapter, msg, 1);
}

/* 写N个寄存器的数据 */
static int ch32v006_write_regs(struct ch32v006_dev *dev, uint8_t byte_addr, u8 *buf, u8 len)
{
    struct i2c_msg msg[3];
    struct i2c_client *client = (struct i2c_client *)dev->private_data;

    /* msg[0]写入需要读取的地址 */
    msg[0].addr = client->addr;     /* 从机地址，也就是CH32V006地址 */
    printk("[%s:%d] msg[0].addr = 0x%02X\n", __func__, __LINE__, msg[0].addr);
    msg[0].flags = 0;               /* 0 - 表示要发送的数据*/
    msg[0].buf = &byte_addr;        /* 表示为要发送的数据,也就是寄存器地址*/
    msg[0].len = 1;                 /* 要发送的数据长度：寄存器地址长度+实际的数据长度*/
    /* msg[1]读取数据 */
    msg[1].addr = client->addr;     /* 从机地址，也就是AP3216C地址*/
    printk("[%s:%d] msg[0].addr = 0x%02X\n", __func__, __LINE__, msg[1].addr);
    msg[1].flags = 0;               /* 表示读数据*/
    msg[1].buf = buf;               /* 接收到的从机发送的数据*/
    msg[1].len = len;               /* 要读取的寄存器长度*/
    
    return i2c_transfer(client->adapter, msg, 2);
}

void DRV_CH32_WriteBlock(struct ch32v006_dev *dev,
        uint8_t byte_addr, uint8_t* buff, uint16_t size)
{
    printk("[%s:%d] DRV_CH32_WriteBlock at %d\r\n", __func__, __LINE__, byte_addr);
    ch32v006_write_regs(dev, byte_addr, buff, size);
}

void DRV_CH32_ReadBlock(struct ch32v006_dev *dev,
        uint8_t byte_addr, uint8_t* buff, uint16_t size)
{
    int i = 0;
	if(size== 0)
		return;
    printk("[%s:%d] DRV_CH32_ReadBlock\r\n", __func__, __LINE__);
    ch32v006_read_regs(dev, byte_addr, buff, size);
    printk("ch32v006_read_regs read  %d\n", size);
    for(i = 0 ; i < size; i++)
    {
     	printk("<%02X> ", buff[i]);
    }
    printk("\nEnd of ch32v006_read_regs and len is %d\n", size);
}

/*************************************文件操作集****************************************/

static int ch32v006_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &ch32v006dev;
    printk("[%s:%d] ch32v006_open\r\n", __func__, __LINE__);
    return 0;
}

static int ch32v006_release(struct inode *inode, struct file *filp)
{
    printk("[%s:%d] ch32v006_release\r\n", __func__, __LINE__);
    return 0;
}

/***************************************************************************
 * loff_t (*llseek) (struct file * fp, loff_t offset, int whence);
 *         fp：文件描述符，用于指定要操作的文件。通过open函数打开
 *            文件时返回的标识，代表了一个已打开的文件或设备。
 *     offset：偏移量，以字节为单位，用于指定从参考点（由whence参数指定）开始移动的距离。
 *            可以是正数（表示向前移动）、负数（表示向后移动）或零（表示不移动）。
 *     whence：指定偏移的参考点，取值及含义如下：
 *            SEEK_SET：从文件开头开始计算偏移量，此时offset的值就是新的文件偏移量。
 *            SEEK_CUR：从当前文件位置开始计算偏移量，新的文件偏移量为当前位置加上offset的值。
 *            SEEK_END：从文件末尾开始计算偏移量，新的文件偏移量为文件大小加上offset的值。
 **************************************************************************/
static loff_t ch32v006_llseek (struct file * filp, loff_t off, int pos)
{
    if(pos == SEEK_SET)
    {
        file_pos = off;
        printk("[%s:%d] file_pos = %d at SEEK_SET.\n", __func__, __LINE__, file_pos);
    }
    else if(pos == SEEK_CUR)
    {
        file_pos += off;
        printk("[%s:%d] file_pos = %d at SEEK_CUR.\n", __func__, __LINE__, file_pos);
    }
    else if(pos == SEEK_END)
    {
        if(off <= 0)
        {
            file_pos = CH32V006_FILE_SIZE + off;
        }
        printk("[%s:%d] file_pos = %d at SEEK_END.\n", __func__, __LINE__, file_pos);
    }
    else {
        printk("[%s:%d] file_pos = %d at UNKNOWN_SEEK.\n", __func__, __LINE__, file_pos);
    }
    if(file_pos >= CH32V006_FILE_SIZE) {
        file_pos = CH32V006_FILE_SIZE;
        printk("[%s:%d] file_pos = %d and at SEEK_END of file.\n", __func__, __LINE__, file_pos);
    }
    return file_pos;
}

ssize_t ch32v006_read(struct file *filp, char __user *buf, size_t cnt, loff_t *off)
{
    long err = 0;
    void *dataPtr = kzalloc(16, GFP_KERNEL);
    struct ch32v006_dev *dev = (struct ch32v006_dev *)filp->private_data;
    printk("[%s:%d] ch32v006_read file_pos = %d.\n", __func__, __LINE__, file_pos);
    DRV_CH32_ReadBlock(dev, file_pos, dataPtr, 16);
    err = copy_to_user(buf, dataPtr, 16);
    kfree(dataPtr);
    printk("[%s:%d] ch32v006_read return %d.\n", __func__, __LINE__, cnt);
    return cnt;
}

ssize_t ch32v006_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *off)
{
    int i = 0 ;
    long err = 0;
    char *dataPtr = (char *)kzalloc(cnt, GFP_KERNEL);
    struct ch32v006_dev *dev = (struct ch32v006_dev *)filp->private_data;
    printk("[%s:%d] return %d.\n", __func__, __LINE__, cnt);
    err = copy_from_user(dataPtr, buf, cnt);

    printk("ch32v006_write write  %d\n", cnt);
    for(i = 0 ; i < cnt; i++)
    {
     	printk("<%02X> ", dataPtr[i]);
    }
    printk("\nEnd of ch32v006_write and len is %d\n", cnt);

    printk("[%s:%d] return %d.\n", __func__, __LINE__, cnt);
    printk("[%s:%d] ch32v006_write file_pos = %d.\n", __func__, __LINE__, file_pos);
    DRV_CH32_WriteBlock(dev, file_pos, dataPtr, cnt);
    printk("[%s:%d] return %d.\n", __func__, __LINE__, cnt);
    kfree(dataPtr);
    printk("[%s:%d] return %d.\n", __func__, __LINE__, cnt);
    return cnt;
}

static const struct file_operations ch32v006_fops = {
    .owner   = THIS_MODULE,
    .open    = ch32v006_open,
    .llseek  = ch32v006_llseek,
    .read    = ch32v006_read,
    .write   = ch32v006_write,
    .release = ch32v006_release,
};

/*************************************i2c设备驱动注册与注销*****************************/

/*
 * @description : i2c 驱动的 probe 函数，当驱动与
 * 设备匹配以后此函数就会执行
 * @param - client : i2c 设备
 * @param - id : i2c 设备 ID
 * @return : 0，成功;其他负值,失败
 */
static int ch32v006_probe(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
    struct device_node *node;
    int ret = 0;
    node = dev_of_node(&client->dev);
    // printk("[%s:%d] ch32v006_probe!\r\n", __func__, __LINE__);
    if(node)
    {
        printk("[%s:%d] device_reg_address init to 0x%02X \r\n", __func__, __LINE__, device_reg_address);
        ret = of_property_read_u32(node, "reg", &device_reg_address);
        if(ret)
        {
            printk("[%s:%d] ret = %d \r\n", __func__, __LINE__, ret);
            // return ret;  // 返回错误码
        }
        else
        {
            printk("[%s:%d] device_reg_address set to 0x%02X \r\n", __func__, __LINE__, device_reg_address);
            // return ret;  // 返回错误码
        }
    }
    // printk("[%s:%d] dev_of_node over !\r\n", __func__, __LINE__);
    /* 搭建字符设备驱动框架，在/dev/下 */
    /* 1、构建设备号 */
    ch32v006dev.major = 0; /* 由系统分配主设备号 */

    if (ch32v006dev.major)
    { /* 给定主设备号 */
        ch32v006dev.devid = MKDEV(ch32v006dev.major, 0);
        ret = register_chrdev_region(ch32v006dev.devid, CH32V006_CNT, CH32V006_NAME);
    }
    else
    { /* 没有给定主设备号 */
        ret = alloc_chrdev_region(&ch32v006dev.devid, 0, CH32V006_CNT, CH32V006_NAME);
        ch32v006dev.major = MAJOR(ch32v006dev.devid);
        ch32v006dev.minor = MINOR(ch32v006dev.devid);
    }
    if (ret < 0)
    {
        printk("[%s:%d] ch32v006dev chrdev_region err!\r\n", __func__, __LINE__);
        goto fail_devid;
    }
    printk("[%s:%d] ch32v006dev major=%d, minor=%d\r\n",
        __func__, __LINE__, ch32v006dev.major, ch32v006dev.minor);

    /* 2、注册设备 */
    ch32v006dev.cdev.owner = THIS_MODULE;
    cdev_init(&ch32v006dev.cdev, &ch32v006_fops);
    ret = cdev_add(&ch32v006dev.cdev, ch32v006dev.devid, CH32V006_CNT);
    if (ret < 0)
    {
        printk("[%s:%d] ch32v006dev cdev_add err!\r\n", __func__, __LINE__);
        goto fail_cdev;
    }

    /******** 自动创建设备节点 *******/

    /* 3、创建类 */
    ch32v006dev.class = class_create(THIS_MODULE, CH32V006_NAME);
    if (IS_ERR(ch32v006dev.class))
    {
        printk("[%s:%d] ch32v006dev class_create err!\r\n", __func__, __LINE__);
        ret = PTR_ERR(ch32v006dev.class);
        goto fail_class;
    }

    /* 4、创建设备 */
    ch32v006dev.device = device_create(ch32v006dev.class, NULL,
                                      ch32v006dev.devid, NULL, CH32V006_NAME);
    if (IS_ERR(ch32v006dev.device))
    {
        printk("[%s:%d] ch32v006dev device_create err!\r\n", __func__, __LINE__);
        ret = PTR_ERR(ch32v006dev.device);
        goto fail_device;
    }

    printk("[%s:%d] ch32v006dev major=%d, minor=%d\r\n",
        __func__, __LINE__, ch32v006dev.major, ch32v006dev.minor);
    ch32v006dev.private_data = client;

    return 0;

fail_device:
    class_destroy(ch32v006dev.class);
fail_class:
    cdev_del(&ch32v006dev.cdev);
fail_cdev:
    unregister_chrdev_region(ch32v006dev.devid, CH32V006_CNT);
fail_devid:
    return ret;
}

/*
* @description : i2c 驱动的 remove 函数，移除 i2c 驱动此函数会执行
* @param – client : i2c 设备
* @return : 0，成功;其他负值,失败
*/
static int ch32v006_remove(struct i2c_client *client)
{
    /* 1,删除字符设备 */
    cdev_del(&ch32v006dev.cdev);

    /* 2,注销设备号 */
    unregister_chrdev_region(ch32v006dev.devid, CH32V006_CNT);

    /* 3,摧毁设备 */
    device_destroy(ch32v006dev.class, ch32v006dev.devid);

    /* 4,摧毁类 */
    class_destroy(ch32v006dev.class);

    return 0;
}

/* 传统匹配方式 ID 列表 */
static struct i2c_device_id ch32v006_id[] = {
    {"wch,ch32v006", 0},
    {}

};

/* 设备树匹配表 */
static const struct of_device_id ch32v006_of_match[] = {
    {
        .compatible = "wch,ch32v006",
    },
    {}
};

/* i2c_driver */
static struct i2c_driver ch32v006_driver = {
    .probe = ch32v006_probe,
    .remove = ch32v006_remove,
    .driver = {
        .name = "ch32v006",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(ch32v006_of_match),
    },
    .id_table = ch32v006_id,
};

/*驱动入口函数*/
static int __init ch32v006_init(void)
{
    int ret;
    ret = i2c_add_driver(&ch32v006_driver);
    if (ret != 0)
    {
        pr_err("CH32V006 I2C registration failed %d\n", ret);
        return ret;
    }
    return 0;
}

/*驱动出口函数*/
static void __exit ch32v006_exit(void)
{
    i2c_del_driver(&ch32v006_driver);
}

module_init(ch32v006_init);
module_exit(ch32v006_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("supersmart");
