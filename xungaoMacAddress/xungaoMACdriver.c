#include <linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/uaccess.h>

#include <mach/hardware.h>
#include <mach/mac.h>
#include <mach/irq.h>
#include <mach/io.h>
#include <mach/pincontrol.h>

// #include "drivers/net/asmmac/asmmac.h"

#define NAME       "xungao_mac"
#define COUNT      3
#define KBUFSIZE   64

dev_t dev_no;
struct cdev*   cdevp = NULL;
struct class*  cls   = NULL;
struct device* devp  = NULL;

char Kbuf[KBUFSIZE] = {'\0'};
int Kbufcount = 0;

static int xungao_mac_open(struct inode *inode,struct file *filp)
{
    printk(KERN_DEBUG"[%s-%s-%d]:runned...\n",__FILE__,__func__,__LINE__);
    return 0;
}

static int xungao_mac_release(struct inode *inode, struct file *filp)
{
    printk(KERN_DEBUG"[%s-%s-%d]: runned...\n",\
           __FILE__,__func__,__LINE__);
    return 0;
}

ssize_t xungao_mac_read(struct file *filp, char __user *buf, size_t size,loff_t *pos)
{
    int i = 0;
    int iRet = 0;
    unsigned int addrh, addrl;
    unsigned char addr[6];

    static int isEnd = 0;

    addrh = as3310_readl(HW_ETH_MACA0HR);
    addrl = as3310_readl(HW_ETH_MACA0LR);

    addr[0] = (addrh & 0xff00) >> 8;
    addr[1] = (addrh & 0xff);
    addr[2] = (addrl & 0xff000000) >> 24;
    addr[3] = (addrl & 0xff0000) >> 16;
    addr[4] = (addrl & 0xff00) >> 8;
    addr[5] = (addrl & 0xff);
    
    for(i = 0; i < KBUFSIZE; i++)
    {
        Kbuf[i] = '\0';
    }
    iRet = sprintf(Kbuf, "--%02X:%02X:%02X:%02X:%02X:%02X\n", 
		addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    if(copy_to_user(buf,Kbuf,KBUFSIZE)){
        printk(KERN_ERR"[%s-%s-%d]: copy_to_user failed...\n",\
           __FILE__,__func__,__LINE__);
        return-EAGAIN;
    }

    printk(KERN_DEBUG"[%s-%s-%d]: runned and return (%d)%s...\n",\
           __FILE__,__func__,__LINE__, iRet, Kbuf);
    // Read end
    if(isEnd == 0)
    {
        isEnd = 1;
        return iRet;
    }
    else
    {
        isEnd = 0;
        return 0;
    }
}

/********************************************************************************* 
set mac address, 
high: mac register high two bytes 
low: mac register low four bytes 
first byte of mac address writes to lowwest byte of register,for example,
mac address is 00:11:22:33:44:55, then set_mac_address(0x5544,0x33221100)
*********************************************************************************/
void set_mac_address(unsigned int high,unsigned int low)
{
	as3310_writel(high,HW_ETH_MACA0HR);
        as3310_writel(low,HW_ETH_MACA0LR);
}

ssize_t xungao_mac_write(struct file *filp, const char __user *buf, size_t size,loff_t *pos)
{
    unsigned long addrh, addrl;
    if(size > KBUFSIZE){
        size = KBUFSIZE;
    }
    if(copy_from_user(Kbuf, buf, size)){
        printk(KERN_ERR"[%s-%s-%d]: copy_from_user failed...\n",\
               __FILE__, __func__, __LINE__);
        return -EAGAIN;
    }
    Kbufcount = size;
    if(Kbufcount == 6)
    {
        addrh=Kbuf[4]+(Kbuf[5]*256);
        addrl=(Kbuf[3]*256*256*256)+(Kbuf[2]*256*256)+(Kbuf[1]*256)+Kbuf[0];
        set_mac_address(addrh,addrl);
	printk(KERN_DEBUG"[%s-%s-%d]: set_mac_address:(%ld, %ld)..\n",\
		   __FILE__, __func__, __LINE__, addrh, addrl);
    }    

    printk(KERN_DEBUG"[%s-%s-%d]: Kbuf:%s.(%d)..\n",\
           __FILE__, __func__, __LINE__, Kbuf, size);
    return size;
}

struct file_operations fops={
    .owner = THIS_MODULE,
    .open = xungao_mac_open,
    .release = xungao_mac_release,
    .read = xungao_mac_read,
    .write = xungao_mac_write,
};

static int __init xungao_mac_init(void)
{
    int ret=0,i=0;
    //0、申请设备号
    ret = alloc_chrdev_region(&dev_no,0,COUNT,NAME);
    if(ret<0){
        printk(KERN_ERR"[%s-%s-%d]:alloc_chrdev_region failed...\n",\
            __FILE__,__func__,__LINE__);
    goto err0;
    }
    printk(KERN_DEBUG"[%s-%s-%d]:dev_no->major:%d--minor:%d--...\n",\
       __FILE__,__func__,__LINE__,MAJOR(dev_no),MINOR(dev_no));
    
    //1、分配cdev结构体
    cdevp = cdev_alloc();
    if(cdevp == NULL){
        printk(KERN_ERR"[%s-%s-%d]:cdev_alloc failed...\n",\
               __FILE__,__func__,__LINE__);
        ret = -ENOMEM;
        goto err1;
    }
    
    //2、初始化cdev结构体
    cdev_init(cdevp,&fops);

    //3、添加到内核中，由内核统一管理
    ret = cdev_add(cdevp,dev_no,COUNT);
    if(ret<0){
        goto err1;
    }

    //4、class create
    cls = class_create(THIS_MODULE,NAME);
    if(IS_ERR(cls)){
        printk(KERN_ERR"[%s-%s-%d]:class_create...\n",\
           __FILE__,__func__,__LINE__);
        ret = PTR_ERR(cls);
        goto err2;
    }

    //5、device create
    for(i=0; i < COUNT; i++){
        devp = device_create(cls,NULL,MKDEV(MAJOR(dev_no),i),\
                NULL,"%s%d",NAME,i);
        if(IS_ERR(devp)){
            printk(KERN_ERR"[%s-%s-%d]:device_create[%d]...\n",\
                    __FILE__,__func__,__LINE__,i);
            ret=PTR_ERR(devp);
            goto err3;
        }
    }
    return 0;
err3:
    for(--i;i>=0;i--){
        device_destroy(cls, MKDEV(MAJOR(dev_no), i));
    }
    class_destroy(cls);
err2:
    cdev_del(cdevp);
err1:
    unregister_chrdev_region(dev_no,COUNT);
err0:
    return ret;
}

static void __exit xungao_mac_exit(void)
{
    int i=0;
    for(i=0;i < COUNT;i++){
        device_destroy(cls,MKDEV(MAJOR(dev_no),i));
    }
    class_destroy(cls);
    //cdev从内核中删除
    cdev_del(cdevp);
    //设备号资源释放
    unregister_chrdev_region(dev_no,COUNT);
}

module_init(xungao_mac_init);
module_exit(xungao_mac_exit);
MODULE_LICENSE("GPL");
