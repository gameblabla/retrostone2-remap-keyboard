CC	= /opt/retrostone-toolchain/bin/arm-buildroot-linux-musleabihf-gcc
CFLAGS = -O2 -Wall -Werror
LDFLAGS = -lc -lgcc -s
OBJS = main.o gpio.o

all: key_driver

key_driver: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): gpio.h

clean:
	rm -f pigc $(OBJS)

install:
	cp pigc /usr/local/bin
	chmod 755 /usr/local/bin/pigc
