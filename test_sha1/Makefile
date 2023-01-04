all:
	armv5l-linux-gcc -D__UNIX__ *.c -Wall -O2  -lpthread -lm -o test_sha1_armv5l
	gcc -o test_sha1_ubuntu test_sha1.c inet_Crypto_sha1.c inet_sha1.c

clean:
	rm test_sha1_armv5l test_sha1_ubuntu

