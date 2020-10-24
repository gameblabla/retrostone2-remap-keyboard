CC	= /opt/rg351p-toolchain/bin/aarch64-buildroot-linux-gnu-gcc
CFLAGS = -O0 -g3 -Wall -Werror -DRG351P
LDFLAGS = -lc -lgcc -pthread -levdev -s
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
