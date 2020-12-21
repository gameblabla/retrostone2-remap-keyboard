CC	= $(CROSS_COMPILE)gcc
CFLAGS := -Ofast -fdata-sections -ffunction-sections -flto -Wall
LDFLAGS := -lc -lgcc -pthread -lgpiod -Wl,--as-needed -Wl,--gc-sections -s -flto
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
