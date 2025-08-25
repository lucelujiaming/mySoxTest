#include <linux/module.h>
#include <linux/clk-provider.h>
#include <linux/i2c.h>
#include <linux/bcd.h>
#include <linux/rtc.h>
 
#define SDP8XX_CNT 1
#define SDP8XX_NAME "sdp8xx"

#define SDP800_TEMPCOMP_MASS_FLOW                   1
#define SDP800_TEMPCOMP_DIFFERNTIAL_PRESSURE        2

#define   SDP800_AVERAGING_NONE                     1
#define   SDP800_AVERAGING_TILL_READ                2

typedef enum {
  ERROR_NONE              = 0x00, // no error
  ERROR_ACK               = 0x01, // no acknowledgment error
  ERROR_CHECKSUM          = 0x02, // checksum mismatch error
  ERROR_IVALID_PARAMETER  = 0xFF, // invalid parameter
} Error;

// Sensor Commands
typedef enum {
    /// Undefined dummy command.
    COMMAND_UNDEFINED                       = 0x0000,
    /// Start continous measurement                     \n
    /// Temperature compensation: Mass flow             \n
    /// Averaging: Average till read
    COMMAND_START_MEASURMENT_MF_AVERAGE     = 0x3603,
    /// Start continous measurement                     \n
    /// Temperature compensation: Mass flow             \n
    /// Averaging: None - Update rate 1ms
    COMMAND_START_MEASURMENT_MF_NONE        = 0x3608,
    /// Start continous measurement                     \n
    /// Temperature compensation: Differential pressure \n
    /// Averaging: Average till read
    COMMAND_START_MEASURMENT_DP_AVERAGE     = 0x3615,
    /// Start continous measurement                     \n
    /// Temperature compensation: Differential pressure \n
    /// Averaging: None - Update rate 1ms
    COMMAND_START_MEASURMENT_DP_NONE        = 0x361E,
    // Stop continuous measurement.
    COMMAND_STOP_CONTINOUS_MEASUREMENT      = 0x3FF9
} Command;
 
struct sdp8xx_dev
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
static int tempComp  = SDP800_TEMPCOMP_DIFFERNTIAL_PRESSURE;
static int averaging = SDP800_AVERAGING_TILL_READ;

static const float scaleFactorTemperature = 200;
static struct sdp8xx_dev sdp8xxdev;
 
static Error read_raw_results(struct sdp8xx_dev *dev, int16_t*  diffPressureTicks,
                                       int16_t*  temperatureTicks,
                                       uint16_t* scaleFactor);
 
/******************************I2C 设备数据收发处理***********************************/
 
/* 读取AP3216C的N个寄存器值 */
static int sdp8xx_read_regs(struct sdp8xx_dev *dev, void *val, int len)
{
    struct i2c_msg msg[2];
 
    struct i2c_client *client = (struct i2c_client *)dev->private_data;
 
    /* msg[0]读取数据 */
    msg[0].addr = client->addr; /* 从机地址，也就是AP3216C地址*/
    printk("[%s:%d] msg[0].addr = 0x%02X\n", __func__, __LINE__, msg[0].addr);
    msg[0].flags = I2C_M_RD;    /* 表示读数据*/
    msg[0].buf = val;           /* 接收到的从机发送的数据*/
    msg[0].len = len;           /* 要读取的寄存器长度*/
 
    return i2c_transfer(client->adapter, msg, 1);
}
 
/* 向AP3216C写N个寄存器的数据 */
static int sdp8xx_write_regs(struct sdp8xx_dev *dev, u8 *buf, u8 len)
{
    u8 b[256];
    struct i2c_msg msg;
 
    struct i2c_client *client = (struct i2c_client *)dev->private_data;
 
    memcpy(b, buf, len);
 
    /* msg[0]发送要读取的寄存器首地址 */
    msg.addr = device_reg_address; /* 从机地址，也就是AP3216C地址*/
    msg.flags = 0;           /* 表示为要发送的数据*/
    msg.buf = b;             /* 表示为要发送的数据,也就是寄存器地址*/
    msg.len = len;       /* 要发送的数据长度：寄存器地址长度+实际的数据长度*/
 
    return i2c_transfer(client->adapter, &msg, 1);
}

Error DRV_SDP800_StartMeasurement(struct sdp8xx_dev *dev, int  tempComp,
                                       int averaging)
{
  unsigned char buf[2];
  Error error= ERROR_NONE;
  Command command = COMMAND_UNDEFINED;
  
  // determine command code
  switch(tempComp) 
	{
    case SDP800_TEMPCOMP_MASS_FLOW:
      switch(averaging) 
			{
        case SDP800_AVERAGING_TILL_READ:
          command = COMMAND_START_MEASURMENT_MF_AVERAGE;
          break;
        case SDP800_AVERAGING_NONE:
          command = COMMAND_START_MEASURMENT_MF_NONE;
          break;
      }
      break;
    case SDP800_TEMPCOMP_DIFFERNTIAL_PRESSURE:
      switch(averaging) 
			{
        case SDP800_AVERAGING_TILL_READ:
          command = COMMAND_START_MEASURMENT_DP_AVERAGE;
          break;
        case SDP800_AVERAGING_NONE:
          command = COMMAND_START_MEASURMENT_DP_NONE;
          break;
      }
      break;
  }
  
  if(COMMAND_UNDEFINED != command) 
  {
    buf[0] = command >> 8;
    buf[1] = command & 0xFF;
    sdp8xx_write_regs(dev, buf, 2);
  }
  return error;
}

Error DRV_SDP800_StopMeasurement(struct sdp8xx_dev *dev)
{
    unsigned char buf[2];
    Command command = COMMAND_STOP_CONTINOUS_MEASUREMENT;
    buf[0] = command >> 8;
    buf[1] = command & 0xFF;
    sdp8xx_write_regs(dev, buf, 2);
    printk("[%s:%d] DRV_SDP800_StopMeasurement\r\n", __func__, __LINE__);
    return ERROR_NONE;
}


Error DRV_SDP800_ReadResults(struct sdp8xx_dev *dev, 
        int16_t* diffPressure, int16_t* temperature, uint16_t* scaleFactorDiffPressure)
{
  Error error= ERROR_NONE;
  int16_t  diffPressureTicks;
  int16_t  temperatureTicks;
  uint16_t scaleFactorDiffPressureTicks;
  
  error = read_raw_results(dev, &diffPressureTicks, &temperatureTicks,
                                    &scaleFactorDiffPressureTicks);
  
  if(ERROR_NONE == error) {
    // *diffPressure = (float)diffPressureTicks / (float)scaleFactorDiffPressure;
    // *temperature  = (float)temperatureTicks / scaleFactorTemperature;
    * diffPressure = diffPressureTicks;
    * temperature  = temperatureTicks;
    * scaleFactorDiffPressure = scaleFactorDiffPressureTicks;
  }
  
  return error;
}

Error DRV_SDP800_SoftReset(struct sdp8xx_dev *dev)
{
    unsigned char buf;
    buf = 0x06;
    sdp8xx_write_regs(dev, &buf, 1);
    return ERROR_NONE;
}


static Error read_raw_results(struct sdp8xx_dev *dev, int16_t*  diffPressureTicks,
                                       int16_t*  temperatureTicks,
                                       uint16_t* scaleFactor)
{
    unsigned char data[9];
    sdp8xx_read_regs(dev, data, 9);
    
	*diffPressureTicks= (((uint16_t)data[0])<<8)|data[1];
	*temperatureTicks= (((uint16_t)data[3])<<8)|data[4];
	*scaleFactor= (((uint16_t)data[6])<<8)|data[7];
    return  ERROR_NONE;
}
/*************************************文件操作集****************************************/
 
static int sdp8xx_open(struct inode *inode, struct file *filp)
{
    // unsigned char value = 0;
    filp->private_data = &sdp8xxdev;
    printk("[%s:%d] sdp8xx_open\r\n", __func__, __LINE__);
    return 0;
}
 
static int sdp8xx_release(struct inode *inode, struct file *filp)
{
    // struct sdp8xx_dev *dev = (struct sdp8xx_dev*)filp->private_data;
    printk("[%s:%d] sdp8xx_release\r\n", __func__, __LINE__);
    return 0;
}
 
ssize_t sdp8xx_read(struct file *filp, char __user *buf, size_t cnt, loff_t *off)
{
    int16_t lc_press, lc_temp;
    uint16_t scaleFactor;
    long err = 0;
    char data[6];    
    struct sdp8xx_dev *dev = (struct sdp8xx_dev *)filp->private_data;
    DRV_SDP800_ReadResults(dev, &lc_press, &lc_temp, &scaleFactor);

    memset(data, 0, 6);
    // printk("[%s:%d] sdp8xx_read lc_press=%04X, lc_temp=%04X, scaleFactor=%04X\r\n", 
    //    __func__, __LINE__, lc_press, lc_temp, scaleFactor);
    memcpy(data, &lc_press, sizeof(int16_t));
    memcpy(data + 2, &lc_temp, sizeof(int16_t));
    memcpy(data + 4, &scaleFactor, sizeof(int16_t));
    
    printk("data is <%02X> <%02X> <%02X> <%02X> <%02X> <%02X> .", 
            data[0], data[1], data[2], data[3], data[4], data[5]);
    err = copy_to_user(buf, data, sizeof(data));
    return sizeof(data);
}
 
static const struct file_operations sdp8xx_fops = {
    .owner = THIS_MODULE,
    .open = sdp8xx_open,
    .read = sdp8xx_read,
    .release = sdp8xx_release,
};
 
 
/*************************************i2c设备驱动注册与注销*****************************/
 
/*
 * @description : i2c 驱动的 probe 函数，当驱动与
 * 设备匹配以后此函数就会执行
 * @param - client : i2c 设备
 * @param - id : i2c 设备 ID
 * @return : 0，成功;其他负值,失败
 */
static int sdp8xx_probe(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
    struct device_node *node;
    int ret = 0;
    node = dev_of_node(&client->dev);
    // printk("[%s:%d] sdp8xx_probe!\r\n", __func__, __LINE__);
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
    sdp8xxdev.major = 0; /* 由系统分配主设备号 */
 
    if (sdp8xxdev.major)
    { /* 给定主设备号 */
        sdp8xxdev.devid = MKDEV(sdp8xxdev.major, 0);
        ret = register_chrdev_region(sdp8xxdev.devid, SDP8XX_CNT, SDP8XX_NAME);
    }
    else
    { /* 没有给定主设备号 */
        ret = alloc_chrdev_region(&sdp8xxdev.devid, 0, SDP8XX_CNT, SDP8XX_NAME);
        sdp8xxdev.major = MAJOR(sdp8xxdev.devid);
        sdp8xxdev.minor = MINOR(sdp8xxdev.devid);
    }
    if (ret < 0)
    {
        printk("[%s:%d] sdp8xx chrdev_region err!\r\n", 
        __func__, __LINE__);
        goto fail_devid;
    }
    printk("[%s:%d] sdp8xx major=%d, minor=%d\r\n", 
        __func__, __LINE__, sdp8xxdev.major, sdp8xxdev.minor);
 
    /* 2、注册设备 */
    sdp8xxdev.cdev.owner = THIS_MODULE;
    cdev_init(&sdp8xxdev.cdev, &sdp8xx_fops);
    ret = cdev_add(&sdp8xxdev.cdev, sdp8xxdev.devid, SDP8XX_CNT);
    if (ret < 0)
    {
        goto fail_cdev;
    }
 
    /******** 自动创建设备节点 *******/
 
    /* 3、创建类 */
    sdp8xxdev.class = class_create(THIS_MODULE, SDP8XX_NAME);
    if (IS_ERR(sdp8xxdev.class))
    {
        ret = PTR_ERR(sdp8xxdev.class);
        goto fail_class;
    }
 
    /* 4、创建设备 */
    sdp8xxdev.device = device_create(sdp8xxdev.class, NULL,
                                      sdp8xxdev.devid, NULL, SDP8XX_NAME);
    if (IS_ERR(sdp8xxdev.device))
    {
        ret = PTR_ERR(sdp8xxdev.device);
        goto fail_device;
    }
 
    sdp8xxdev.private_data = client;
    
    // DRV_SDP800_Init(0x25);
    printk("[%s:%d] sdp8xx_open\r\n", __func__, __LINE__);
	DRV_SDP800_SoftReset(&sdp8xxdev);
    printk("[%s:%d] DRV_SDP800_SoftReset\r\n", __func__, __LINE__);
	DRV_SDP800_StartMeasurement(&sdp8xxdev, tempComp, averaging);
    printk("[%s:%d] DRV_SDP800_SoftReset tempComp=%d, averaging=%d\r\n", 
        __func__, __LINE__, tempComp, averaging);
        
    return 0;
 
fail_device:
    class_destroy(sdp8xxdev.class);
fail_class:
    cdev_del(&sdp8xxdev.cdev);
fail_cdev:
    unregister_chrdev_region(sdp8xxdev.devid, SDP8XX_CNT);
fail_devid:
    return ret;
}
 
/*
* @description : i2c 驱动的 remove 函数，移除 i2c 驱动此函数会执行
* @param – client : i2c 设备
* @return : 0，成功;其他负值,失败
*/
static int sdp8xx_remove(struct i2c_client *client)
{
    DRV_SDP800_StopMeasurement(&sdp8xxdev);
    /* 1,删除字符设备 */
    cdev_del(&sdp8xxdev.cdev);
 
    /* 2,注销设备号 */
    unregister_chrdev_region(sdp8xxdev.devid, SDP8XX_CNT);
 
    /* 3,摧毁设备 */
    device_destroy(sdp8xxdev.class, sdp8xxdev.devid);
 
    /* 4,摧毁类 */
    class_destroy(sdp8xxdev.class);
 
    return 0;
}
 
/* 传统匹配方式 ID 列表 */
static struct i2c_device_id sdp8xx_id[] = {
    {"sensirion,sdp810", 0},
    {}
 
};
 
/* 设备树匹配表 */
static const struct of_device_id sdp8xx_of_match[] = {
    {
        .compatible = "sensirion,sdp810",
    },
    {}
 
};
 
/* i2c_driver */
static struct i2c_driver sdp8xx_driver = {
    .probe = sdp8xx_probe,
    .remove = sdp8xx_remove,
    .driver = {
        .name = "sdp8xx",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(sdp8xx_of_match),
    },
    .id_table = sdp8xx_id,
};

module_param(tempComp, int, S_IRUSR);
module_param(averaging, int, S_IRUSR); 

/*驱动入口函数*/
static int __init sdp8xx_init(void)
{
    int ret;
    ret = i2c_add_driver(&sdp8xx_driver);
    if (ret != 0)
    {
        pr_err("AP3216C I2C registration failed %d\n", ret);
        return ret;
    }
    return 0;
}
 
/*驱动出口函数*/
static void __exit sdp8xx_exit(void)
{
    i2c_del_driver(&sdp8xx_driver);
}
 
module_init(sdp8xx_init);
module_exit(sdp8xx_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("supersmart");
