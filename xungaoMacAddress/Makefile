KERN_DIR = ../linux-2.6.30

all:
	make -C $(KERN_DIR) M=`pwd` modules

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m    += xungaoMACdriver.o
