CC	= /opt/retrostone2-toolchain/bin/arm-linux-gcc
CFLAGS = -O0 -g3 -Wall
LDFLAGS = -lc -lgcc -pthread -lgpiod -s
OBJS = main.o

all: key_driver

key_driver: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): main.o

clean:
	rm -f key_driver $(OBJS)

install:
	cp key_driver /usr/bin
	chmod 755 /usr/bin/key_driver
