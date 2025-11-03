TOP_DIR = /media/user1/V3SSDK_BUILDROOT/v3ssdk/buildroot/out/
CROSS = $(TOP_DIR)host/bin/arm-buildroot-linux-gnueabihf-

CC = $(CROSS)gcc

# You can compile it and test it by BacnetScan.
# It has one device which includes 10 Analog Values and 10 Binary Values.
bac442c_app:
	$(CC) -D__UNIX__ BACnet/*.c BACnet/src/*.c BACnet/handler/*.c \
                VAV/*.c Filter/*.c Util/*.c HAL/*.c Modbus/*.c\
                DRV/*.c  *.c \
                -Wall -g -O -I. \
                -IBACnet/ \
                -IBACnet/include \
                -IModbus/ \
                -IUtil/ \
                -IFilter/ \
                -IHAL/ \
                -IDRV/ \
                -IVAV/ \
                -lpthread -lm -o bac442c_app 
clean:
	rm bac442c_app 
