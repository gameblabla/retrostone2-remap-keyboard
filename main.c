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

#include "gpio.h"

static int32_t value[14];

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
	DPAD_UP = 12,
	DPAD_DOWN = 5,
	DPAD_LEFT = 0,
	DPAD_RIGHT = 11,
	BUTTON_A = 14,
	BUTTON_B = 15,
	BUTTON_Y = 9,
	BUTTON_X = 13,
	BUTTON_SELECT = 4,
	BUTTON_START = 1,
	BUTTON_L2 = 6,
	BUTTON_R2 = 7,
	BUTTON_L = 8,
	BUTTON_R = 3
};

		
#define PRESS_KEY(a,b) if (value[a] == 0){ emit(fd, EV_KEY, b, 1); emit(fd, EV_SYN, SYN_REPORT, 1);} else{ emit(fd, EV_KEY, b, 0); emit(fd, EV_SYN, SYN_REPORT, 0); }
		
int main(void)
{
	struct uinput_user_dev uud;
	int fd;
	   
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

	sunxi_gpio_init();
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', DPAD_UP), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', DPAD_DOWN), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', DPAD_LEFT), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', BUTTON_A), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', BUTTON_B), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', BUTTON_Y), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', BUTTON_X), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', BUTTON_SELECT), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', BUTTON_START), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', BUTTON_L), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', BUTTON_R), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', BUTTON_L2), SUNXI_GPIO_INPUT);
	sunxi_gpio_set_cfgpin(SUNXI_GPIO_PIN('D', BUTTON_R2), SUNXI_GPIO_INPUT);

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
	ioctl(fd, UI_SET_KEYBIT, KEY_END);
	ioctl(fd, UI_SET_KEYBIT, KEY_3);
	ioctl(fd, UI_SET_KEYBIT, KEY_ENTER);
	ioctl(fd, UI_SET_KEYBIT, KEY_ESC);

	//  printf("ioctl = %d, %d, %d ,%d , %d, %d\n", i1,i2,i3,i4,i5,i6);

	memset(&uud, 0, sizeof(uud));
	snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "uinput-keyboard");
	uud.id.bustype = BUS_HOST;
	uud.id.vendor  = 0x1;
	uud.id.product = 0x2;
	uud.id.version = 1;

	write(fd, &uud, sizeof(uud));
	ioctl(fd, UI_DEV_CREATE);

	/* Key press, report the event, send key release, and report again */
	for(;;)
	{
		value[0] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', DPAD_UP));
		value[1] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', DPAD_DOWN));
		value[2] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', DPAD_LEFT));
		value[3] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', DPAD_RIGHT));
		value[4] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', BUTTON_A));
		value[5] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', BUTTON_B));
		value[6] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', BUTTON_X));
		value[7] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', BUTTON_Y));
		value[8] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', BUTTON_L));
		value[9] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', BUTTON_R));
		value[10] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', BUTTON_L2));
		value[11] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', BUTTON_R2));
		value[12] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', BUTTON_START));
		value[13] = sunxi_gpio_input(SUNXI_GPIO_PIN('D', BUTTON_SELECT));

		/* 0 means pressed, 1 means not pressed. Makes China-sense. */
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
		PRESS_KEY(10,KEY_END)
		PRESS_KEY(11,KEY_3)
		PRESS_KEY(12,KEY_ENTER)
		PRESS_KEY(13,KEY_ESC)
	}
	
	ioctl(fd, UI_DEV_DESTROY);
	close(fd);
	
	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	return 0;
}
