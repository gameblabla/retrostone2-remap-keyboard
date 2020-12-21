/*

Retrostone key mappign (Opendingux like)

MIT License
Copyright (c) 2019 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <linux/uinput.h>

#include <libevdev-1.0/libevdev/libevdev.h>
#include <linux/limits.h>

#include <fcntl.h>
#include <errno.h>

#include <fcntl.h>
#include <errno.h>
#include <gpiod.h>

#define NUMBER_OF_KEYS 16
#define INTERVAL 10000

static int32_t value[NUMBER_OF_KEYS];

struct libevdev* dev;
int quit = 0;
int fd;

/* emit function is identical to of the first example */

void emit(int fd, int type, int code, int val)
{
	struct input_event ie;

	ie.type = type;
	ie.code = code;
	ie.value = val;
	/* timestamp values below are ignored */
	ie.time.tv_sec = 0;
	ie.time.tv_usec = 0;

	write(fd, &ie, sizeof(ie));
}

enum
{
	DPAD_UP = 0,
	DPAD_DOWN = 1,
	DPAD_LEFT = 2,
	DPAD_RIGHT = 3,
	BUTTON_A = 4,
	BUTTON_B = 5,
	BUTTON_X = 6,
	BUTTON_Y = 7,
	BUTTON_SELECT = 8,
	BUTTON_START = 9,
	BUTTON_L = 10,
	BUTTON_R = 11,
	BUTTON_L2 = 12,
	BUTTON_R2 = 13,
	BUTTON_L3 = 14,
	BUTTON_R3 = 15,
};

static int32_t line_num[NUMBER_OF_KEYS] =
{
	224 + 19,
	224 + 7,
	224 + 4,
	224 + 16,
	224,
	224 + 11,
	224 + 12,
	224 + 20,
	224 + 23,
	224 + 22,
	224 + 15,
	224 + 14,
	224 + 27,
	224 + 26,
	64 + 18,
	64 + 22,
};

#define PRESS_KEY(a,b) if (value[a] == 0){ emit(fd, EV_KEY, b, 1); emit(fd, EV_SYN, SYN_REPORT, 1);} else{ emit(fd, EV_KEY, b, 0); emit(fd, EV_SYN, SYN_REPORT, 0); }
		
		
int main(void)
{
	struct uinput_user_dev uud;
	char *chipname = "gpiochip0";
	struct gpiod_chip *chip;
	struct gpiod_line *line[NUMBER_OF_KEYS];
	int i, ret;
	
	   
	/* Fork off the parent process */
	pid_t pid, sid;
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}
	umask(0);       

	sid = setsid();
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}
	
	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if(fd < 0)
	{
		printf("Can't open /dev/uinput\n");
		return 1;
	}
		
	/* The ioctls below will enable the device that is about to be created. */
	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ioctl(fd, UI_SET_EVBIT, EV_SYN);
	
	ioctl(fd, UI_SET_KEYBIT, KEY_UP);
	ioctl(fd, UI_SET_KEYBIT, KEY_DOWN);
	ioctl(fd, UI_SET_KEYBIT, KEY_LEFT);
	ioctl(fd, UI_SET_KEYBIT, KEY_RIGHT);
	ioctl(fd, UI_SET_KEYBIT, KEY_LEFTCTRL);
	ioctl(fd, UI_SET_KEYBIT, KEY_LEFTALT);
	ioctl(fd, UI_SET_KEYBIT, KEY_LEFTSHIFT);
	ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);
	ioctl(fd, UI_SET_KEYBIT, KEY_TAB);
	ioctl(fd, UI_SET_KEYBIT, KEY_BACKSPACE);
	ioctl(fd, UI_SET_KEYBIT, KEY_PAGEUP);
	ioctl(fd, UI_SET_KEYBIT, KEY_PAGEDOWN);
	ioctl(fd, UI_SET_KEYBIT, KEY_ENTER);
	ioctl(fd, UI_SET_KEYBIT, KEY_ESC);
	ioctl(fd, UI_SET_KEYBIT, KEY_ENTER);
	ioctl(fd, UI_SET_KEYBIT, KEY_KPSLASH);
	ioctl(fd, UI_SET_KEYBIT, KEY_KPDOT);
	//ioctl(fd, UI_SET_KEYBIT, KEY_POWER);

	//  printf("ioctl = %d, %d, %d ,%d , %d, %d\n", i1,i2,i3,i4,i5,i6);

	memset(&uud, 0, sizeof(uud));
	snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "uinput-keyboard");
	uud.id.bustype = BUS_HOST;
	uud.id.vendor  = 0x1;
	uud.id.product = 0x1;
	uud.id.version = 1;

	write(fd, &uud, sizeof(uud));
	ioctl(fd, UI_DEV_CREATE);

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		printf("Open chip failed\n");
		return 1;
	}

	for(i=0;i<NUMBER_OF_KEYS;i++)
	{
		line[i] = gpiod_chip_get_line(chip, line_num[i]);
		if (!line[i]) {
			printf("Get line failed\n");
			gpiod_chip_close(chip);
			return 1;
		}

		ret = gpiod_line_request_input(line[i], "Consumer");
		if (ret < 0) {
			printf("Request line as input failed, I %d\n", i);
			//gpiod_chip_close(chip);
			//return 1;
		}
	}

	/* Key press, report the event, send key release, and report again */
	do
	{
		for(i=0;i<NUMBER_OF_KEYS;i++)
		{
			value[i] = gpiod_line_get_value(line[i]);
		}
		PRESS_KEY(0,KEY_UP)
		PRESS_KEY(1,KEY_DOWN)
		PRESS_KEY(2,KEY_LEFT)
		PRESS_KEY(3,KEY_RIGHT)
		PRESS_KEY(4,KEY_LEFTCTRL)
		PRESS_KEY(5,KEY_LEFTALT)
		PRESS_KEY(6,KEY_LEFTSHIFT)
		PRESS_KEY(7,KEY_SPACE)
		PRESS_KEY(8,KEY_TAB)
		PRESS_KEY(9,KEY_BACKSPACE)
		PRESS_KEY(10,KEY_ESC)
		PRESS_KEY(11,KEY_ENTER)
		PRESS_KEY(12,KEY_PAGEUP)
		PRESS_KEY(13,KEY_PAGEDOWN)
		PRESS_KEY(14,KEY_KPSLASH)
		PRESS_KEY(15,KEY_KPDOT)
		usleep(INTERVAL);
	} while (!quit);
	
	ioctl(fd, UI_DEV_DESTROY);
	close(fd);
	
	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	gpiod_chip_close(chip);
	for(i=0;i<NUMBER_OF_KEYS;i++)
	gpiod_line_release(line[i]);
	

	return 0;
}
