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

#include <fcntl.h>
#include <errno.h>

#include <fcntl.h>
#include <errno.h>
#include <gpiod.h>

#define NUMBER_OF_KEYS 2
#define INTERVAL 100000
#define READ_BRIGHTNESS_PATH "/sys/class/backlight/backlight/actual_brightness"
#define BRIGHTNESS_PATH "/sys/class/backlight/backlight/brightness"

static int32_t value[NUMBER_OF_KEYS];

int quit = 0;
int fd;

static int32_t line_num[NUMBER_OF_KEYS] =
{
	64 + 18,
	64 + 22,
};

int getBacklight()
{
	char buf[32] = "-1";
	FILE *f = fopen(READ_BRIGHTNESS_PATH, "r");
	if (f) {
		fgets(buf, sizeof(buf), f);
	}
	fclose(f);
	return atoi(buf);
}

void setBacklight(int level) {
	char buf[200] = {0};
	sprintf(buf, "echo %d > %s", level, BRIGHTNESS_PATH);
	system(buf);
}
		
int main(void)
{
	char *chipname = "gpiochip0";
	struct gpiod_chip *chip;
	struct gpiod_line *line[NUMBER_OF_KEYS];
	int i, ret;
	int bright = 50;
	
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
	
	bright = getBacklight();

	/* Key press, report the event, send key release, and report again */
	do
	{
		for(i=0;i<NUMBER_OF_KEYS;i++)
		{
			value[i] = gpiod_line_get_value(line[i]);
		}
		
		if (value[1] == 0)
		{
			bright-= 5;
			if (bright < 1) bright = 1;
			setBacklight(bright);
		}
		else if (value[0] == 0)
		{
			bright+= 5;
			if (bright > 99) bright = 100;
			setBacklight(bright);
		}

		usleep(INTERVAL);
	} while (!quit);
	
	gpiod_chip_close(chip);
	for(i=0;i<NUMBER_OF_KEYS;i++)
	gpiod_line_release(line[i]);
	

	return 0;
}
