TOP_DIR = /media/user1/V3SSDK_BUILDROOT/v3ssdk/buildroot/out/
CROSS = $(TOP_DIR)host/bin/arm-buildroot-linux-gnueabihf-

CC = $(CROSS)gcc

test_sdp8xx_driver:
	$(CC) -D__UNIX__  dev_mem_app.c \
                -Wall -O2 -I. \
                -lpthread -lm -o dev_mem_app 
clean:
	rm dev_mem_app

