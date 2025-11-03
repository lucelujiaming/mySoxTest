KERNELDIR := ../linux-zero-5.2.y_remove_sdcard
CURRENT_PATH := $(shell pwd)
obj-m    := xungao_ch32v006_driver.o
ENV:=ARCH=arm CROSS_COMPILE=~/WORK/u-boot/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi-

FPU_CFLAGS += -mfloat-abi=softfp
CFLAGS_xungao_sdp8xx_driver.o += $(FPU_CFLAGS)

build: kernel_modules

kernel_modules:
	$(MAKE) $(ENV) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
clean:
	$(MAKE) $(ENV) -C $(KERNELDIR) M=$(CURRENT_PATH) clean

