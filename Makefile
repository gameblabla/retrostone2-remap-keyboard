CC	= /opt/retrostone2-toolchain/bin/arm-linux-gcc
CFLAGS = -Ofast -flto -Wall
LDFLAGS = -lc -lgcc -pthread -lgpiod -flto -s
OBJS = main.o

all: brightness_daemon

brightness_daemon: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): main.o

clean:
	rm -f brightness_daemon $(OBJS)

install:
	cp brightness_daemon /usr/bin
	chmod 755 /usr/bin/brightness_daemon
