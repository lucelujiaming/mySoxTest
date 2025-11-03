/*************************************************************
 * https://www.cnblogs.com/lovemengx/p/16989675.html
 * 整个代码非常四个部分。
 *   1. 首先在my_adap_init中调用platform_driver_register注册
 *      了一个platform_driver结构体。
 *   2. 这个platform_driver结构体的probe函数为my_adap_probe函数。
 *   3. my_adap_probe函数创建的my_adap结构体的algo指向
 *      一个i2c_algorithm结构体。
 *   3. 这个i2c_algorithm结构体实现了传输功能和适配器功能。
 *************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/platform_device.h>

struct my_adap {
	struct i2c_adapter	adap;
	struct device       *dev;
};

static int my_adap_xfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)
{
	int i = 0, j = 0;
	struct my_adap *i2c = (struct my_adap *)adap->algo_data;
	struct device *dev = i2c->dev;
	
	dev_notice(dev, "my_adap_xfer() num:%d\n", num);
	for(i = 0; i < num; i++){
		dev_notice(dev, "msgs[%d]->len:%d\n", i, msgs[i].len);
		dev_notice(dev, "msgs[%d]->addr:0x%.2X\n", i, msgs[i].addr);
		dev_notice(dev, "msgs[%d]->flags:0x%.2X\n", i, msgs[i].flags);
		for(j = 0; j < msgs->len; j++){
			dev_notice(dev, "msgs[%d]->buf[%d]:0x%.2X ", i, j, msgs[i].buf[j]);
		}
	}
	return num;
}

static unsigned int my_adap_functionality(struct i2c_adapter *adap)
{
	struct my_adap *i2c = (struct my_adap *)adap->algo_data;
	struct device *dev = i2c->dev;
	
	dev_notice(dev, "my_adap_functionality() ...\n");
	return I2C_FUNC_I2C|I2C_FUNC_10BIT_ADDR|I2C_FUNC_SMBUS_EMUL;
}

static const struct i2c_algorithm my_adap_algorithm = {
	.master_xfer	  = my_adap_xfer,
	.functionality	  = my_adap_functionality,
};

static int my_adap_probe(struct platform_device *pdev)
{
	struct my_adap *i2c = NULL;
	struct device *dev = &pdev->dev;
	
	dev_notice(dev, "my_adap_probe() ...\n");
	if (!(i2c = kzalloc(sizeof(struct my_adap), GFP_KERNEL))){
		dev_err(dev, "kzalloc failed...\n");
		return -ENOMEM;
	}

	i2c->dev = &pdev->dev;
	i2c->adap.owner   = THIS_MODULE;
	i2c->adap.nr      = -1;	// 自动分配
	i2c->adap.retries = 3;
	i2c->adap.timeout = 5*HZ;
	i2c->adap.class   = I2C_CLASS_HWMON | I2C_CLASS_SPD;
	
	i2c->adap.algo_data  = i2c;
	i2c->adap.dev.parent = &pdev->dev;
	i2c->adap.algo = &my_adap_algorithm;
	i2c->adap.dev.of_node = pdev->dev.of_node;
	snprintf(i2c->adap.name, sizeof(i2c->adap.name), "myadap");
	
	if (i2c_add_numbered_adapter(&i2c->adap) < 0) {
		dev_err(dev, "failed to add adapter\n");
		kfree(i2c);
		return -ENODEV;
	}
	platform_set_drvdata(pdev, i2c);
	
	return 0;
}

static int my_adap_remove(struct platform_device *pdev)
{
	struct my_adap *i2c = platform_get_drvdata(pdev);
	struct device *dev = &pdev->dev;
	
	dev_notice(dev, "my_adap_remove() ...\n");
	platform_set_drvdata(pdev, NULL);
	i2c_del_adapter(&i2c->adap);
	kfree(i2c);
	return 0;
}

static const struct of_device_id my_adap_match[] = {
	{ .compatible = "myadap", },
	{},
};
MODULE_DEVICE_TABLE(of, my_adap_match);

static struct platform_driver my_adap_driver = {
	.probe		= my_adap_probe,
	.remove		= my_adap_remove,
	.driver		= {
		.name	= "myadap",
		.owner	= THIS_MODULE,
		.of_match_table = my_adap_match,
	},
};

/*data relating*/
static struct platform_device my_adap_device = {
	.name	= "myadap",
};

static int __init my_adap_init(void)
{
	int err = 0;
	if ((err = platform_device_register(&my_adap_device)) < 0) {
		return err;
	}
	return platform_driver_register(&my_adap_driver);
}

static void __exit my_adap_exit(void)
{
	platform_driver_unregister(&my_adap_driver);
	platform_device_register(&my_adap_device);
}

fs_initcall(my_adap_init);
module_exit(my_adap_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Bus Driver");

