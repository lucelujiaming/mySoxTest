# TOP_DIR = /media/user1/V3SSDK_BUILDROOT/v3ssdk/buildroot/out/
# CROSS = $(TOP_DIR)host/bin/arm-buildroot-linux-gnueabihf-

# CC = $(CROSS)gcc
CC = gcc
# LIBMODBUS_PATH=modbus

test_ini_file:
	$(CC) -D__UNIX__ *.c \
                -Wall -O2 -I. -lpthread -lm -o test_ini_file 
clean:
	rm test_ini_file 
