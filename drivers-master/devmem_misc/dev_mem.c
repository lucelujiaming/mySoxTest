
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
// #include <asm/system.h>
#include <linux/clk-provider.h>
#include <linux/i2c.h>
#include <linux/bcd.h>
#include <linux/rtc.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/platform_device.h>

struct platform_device memory_dev = {
 .name   = "dev_mem", 
 .id    = -1,
 .dev = 
	 {
	  //.release = memory_release,
	 },
};
  
static int __init memory_dev_init(void)
{
    platform_device_register(&memory_dev);

    return 0;
}

static void __exit memory_dev_exit(void)
{
    platform_device_unregister(&memory_dev);
}

module_init(memory_dev_init);
module_exit(memory_dev_exit);
MODULE_LICENSE("GPL");


