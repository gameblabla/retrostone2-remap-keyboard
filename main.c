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
#include <pthread.h>

#include <libevdev-1.0/libevdev/libevdev.h>
#include <linux/limits.h>

#include <fcntl.h>
#include <errno.h>

#ifdef RG351P
static const char* EVDEV_NAME = "/dev/input/by-path/platform-ff300000.usb-usb-0:1.2:1.0-event-joystick";
#else
static const char* EVDEV_NAME = "/dev/input/by-path/pci-0000:01:00.0-usb-0:5:1.0-event-joystick";
#endif

static int32_t value[16];

const uint32_t ButtonState_Pressed = 1;
const uint32_t ButtonState_Released = 0;
struct libevdev* dev;
pthread_t thread_id;

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
	BUTTON_Y = 6,
	BUTTON_X = 7,
	BUTTON_SELECT = 8,
	BUTTON_START = 9,
	BUTTON_L = 10,
	BUTTON_R = 11,
	BUTTON_L2 = 12,
	BUTTON_R2 = 13,
	BUTTON_L3 = 14,
	BUTTON_R3 = 15,
	BUTTON_POWER = 16
};

pthread_t thread_id;

		
#define PRESS_KEY(a,b) if (value[a] == 1){ emit(fd, EV_KEY, b, 1); emit(fd, EV_SYN, SYN_REPORT, 1);} else{ emit(fd, EV_KEY, b, 0); emit(fd, EV_SYN, SYN_REPORT, 0); }
		
/*static void* input_task(void* arg)
{
	//struct input_event ev;
    // Events
	while (!quit)
	{
		//const int abs_x_max = libevdev_get_abs_maximum(dev, ABS_X);
		//const int abs_y_max = libevdev_get_abs_maximum(dev, ABS_Y);

		//printf("abs: x_max=%d, y_max=%d\n", abs_x_max, abs_y_max);
		
		// Get current state
		value[DPAD_UP] = libevdev_get_event_value(dev, EV_KEY, BTN_DPAD_UP) ? ButtonState_Pressed : ButtonState_Released;
		value[DPAD_DOWN] = libevdev_get_event_value(dev, EV_KEY, BTN_DPAD_DOWN) ? ButtonState_Pressed : ButtonState_Released;
		value[DPAD_LEFT] = libevdev_get_event_value(dev, EV_KEY, BTN_DPAD_LEFT) ? ButtonState_Pressed : ButtonState_Released;
		value[DPAD_RIGHT] = libevdev_get_event_value(dev, EV_KEY, BTN_DPAD_RIGHT) ? ButtonState_Pressed : ButtonState_Released;

		value[BUTTON_A] = libevdev_get_event_value(dev, EV_KEY, BTN_EAST) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_B] = libevdev_get_event_value(dev, EV_KEY, BTN_SOUTH) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_X] = libevdev_get_event_value(dev, EV_KEY, BTN_NORTH) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_Y] = libevdev_get_event_value(dev, EV_KEY, BTN_WEST) ? ButtonState_Pressed : ButtonState_Released;

		value[BUTTON_SELECT] = libevdev_get_event_value(dev, EV_KEY, BTN_TL) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_START] = libevdev_get_event_value(dev, EV_KEY, BTN_TR) ? ButtonState_Pressed : ButtonState_Released;

		//input->current_state.buttons[Go2InputButton_F1] = libevdev_get_event_value(dev, EV_KEY, BTN_TRIGGER_HAPPY1) ? ButtonState_Pressed : ButtonState_Released;
		//input->current_state.buttons[Go2InputButton_F2] = libevdev_get_event_value(dev, EV_KEY, BTN_TRIGGER_HAPPY2) ? ButtonState_Pressed : ButtonState_Released;
		//input->current_state.buttons[Go2InputButton_F3] = libevdev_get_event_value(dev, EV_KEY, BTN_TRIGGER_HAPPY3) ? ButtonState_Pressed : ButtonState_Released;
		//input->current_state.buttons[Go2InputButton_F4] = libevdev_get_event_value(dev, EV_KEY, BTN_TRIGGER_HAPPY4) ? ButtonState_Pressed : ButtonState_Released;
		//input->current_state.buttons[Go2InputButton_F5] = libevdev_get_event_value(dev, EV_KEY, BTN_TRIGGER_HAPPY5) ? ButtonState_Pressed : ButtonState_Released;
		//input->current_state.buttons[Go2InputButton_F6] = libevdev_get_event_value(dev, EV_KEY, BTN_TRIGGER_HAPPY6) ? ButtonState_Pressed : ButtonState_Released;

		value[BUTTON_L] = libevdev_get_event_value(dev, EV_KEY, BTN_TL2) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_R] = libevdev_get_event_value(dev, EV_KEY, BTN_TR2) ? ButtonState_Pressed : ButtonState_Released;

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

    return NULL;
}*/
		
int main(void)
{
	struct uinput_user_dev uud;
	int fd_ev;
	//int result = 0;
	int rc = 1;
	//int i;
	   
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
	uud.id.product = 0x2;
	uud.id.version = 1;

	write(fd, &uud, sizeof(uud));
	ioctl(fd, UI_DEV_CREATE);

	fd_ev = open(EVDEV_NAME, O_RDONLY);
    if (fd_ev < 0)
    {
        printf("Joystick: No gamepad found.\n");
        return 1;
    }
    else
    {    
        rc = libevdev_new_from_fd(fd_ev, &dev);
        if (rc < 0) {
            printf("Joystick: Failed to init libevdev (%s)\n", strerror(-rc));
            return 1;
        }
    
        // printf("Input device name: \"%s\"\n", libevdev_get_name(result->dev));
        // printf("Input device ID: bus %#x vendor %#x product %#x\n",
        //     libevdev_get_id_bustype(result->dev),
        //     libevdev_get_id_vendor(result->dev),
        //     libevdev_get_id_product(result->dev));

        /*if(pthread_create(&thread_id, NULL, input_task, NULL) < 0)
        {
            printf("could not create input_task thread\n");
            return 1;
        }*/
    }

	/* Key press, report the event, send key release, and report again */
	do
	{
        struct input_event ev;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        
        switch(ev.type)
        {
			/*case EV_KEY:
				switch(ev.code)
				{
					case 305:
					break;
				}
			break;*/
			case EV_ABS:
				switch(ev.code)
				{
					case 17:
						if (ev.value < -1)
						{
							value[DPAD_UP] = 1;
							value[DPAD_DOWN] = 0;
						}
						else
						{
							value[DPAD_UP] = 0;
							value[DPAD_DOWN] = 1;
						}
					break;
					case 16:
						if (ev.value < -1)
						{
							value[DPAD_LEFT] = 1;
							value[DPAD_RIGHT] = 0;
						}
						else
						{
							value[DPAD_LEFT] = 0;
							value[DPAD_RIGHT] = 1;
						}
					break;
					
					// Those are for the analog sticks but they should be mapped already thanks to kernel
					/*
					case 2:
					break;
					case 3:
					break;
					case 4:
					break;
					case 5:
					break;
					*/
				}
			break;
			
		}
		
		value[BUTTON_A] = libevdev_get_event_value(dev, EV_KEY, 304) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_B] = libevdev_get_event_value(dev, EV_KEY, 305) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_X] = libevdev_get_event_value(dev, EV_KEY, 307) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_Y] = libevdev_get_event_value(dev, EV_KEY, 308) ? ButtonState_Pressed : ButtonState_Released;

		value[BUTTON_SELECT] = libevdev_get_event_value(dev, EV_KEY, 311) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_START] = libevdev_get_event_value(dev, EV_KEY, 310) ? ButtonState_Pressed : ButtonState_Released;

		value[BUTTON_L] = libevdev_get_event_value(dev, EV_KEY, 308) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_R] = libevdev_get_event_value(dev, EV_KEY, 314) ? ButtonState_Pressed : ButtonState_Released;

		value[BUTTON_L2] = libevdev_get_event_value(dev, EV_KEY, 315) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_R2] = libevdev_get_event_value(dev, EV_KEY, 309) ? ButtonState_Pressed : ButtonState_Released;

		value[BUTTON_L3] = libevdev_get_event_value(dev, EV_KEY, 2) ? ButtonState_Pressed : ButtonState_Released;
		value[BUTTON_R3] = libevdev_get_event_value(dev, EV_KEY, 4) ? ButtonState_Pressed : ButtonState_Released;

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
		PRESS_KEY(10,KEY_PAGEUP)
		PRESS_KEY(11,KEY_PAGEDOWN)
		PRESS_KEY(12,KEY_ENTER)
		PRESS_KEY(13,KEY_ESC)
		
		PRESS_KEY(14,KEY_KPSLASH)
		PRESS_KEY(15,KEY_KPDOT)
		//PRESS_KEY(16,KEY_ESC)
		
	} while (rc == 1 || rc == 0 || rc == -EAGAIN || !quit);
	
	ioctl(fd, UI_DEV_DESTROY);
	close(fd);
	
	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	libevdev_free(dev);

	return 0;
}
