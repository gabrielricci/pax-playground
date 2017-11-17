/*------------------------------------------------------------
* FileName: keyboard.c
* Author: LuX
* Date: 2013-02-20
* Example of how to use keyboard.
* PAX does not provide the interface of how to set keypad tones.
* Developers can directly use linux input subsystem to implement the keyboard event.
------------------------------------------------------------*/
#include "constant.h"
#include "header.h"
#include "keyboard.h"

static int gfd = -1;

/**
 * open keyboard
 * [in] filename keyboard file name, such as "/dev/input/event0"
 * return keyboard device (>=0)
 */
int OpenKeyboard(const char* filename)
{
	gfd = open(filename, O_RDWR);
	return gfd;
}

/**
 * close keyboard
 * return 0 if successful, else return -1
 */
int CloseKeyboard()
{
	return close(gfd);
}

/**
 * get key value (>=0), see macro in linux/input.h, such as KEY_1, KEY_CANCEL, etc.
 * return key value
 */
int GetKey()
{
	int i;
	int ret;
	int eventNum = 0;
	struct input_event ev[64];
	struct pollfd events[1];
	int size = sizeof(struct input_event);
	int rd = 0;
	memset(events, 0, sizeof(events));
	events[0].fd = gfd;
	events[0].events = POLLIN | POLLERR;
	ret = poll(events, 1, 0);
	if ((ret <= 0) || (events[0].revents & POLLERR))
	{
		return -1;
	}
	if (events[0].revents & POLLIN)
	{
		memset(ev, 0, sizeof(ev));
		rd = read(gfd, ev, sizeof(ev));
		if (rd < size)
		{
			return -1;
		}
		eventNum = rd / size;
		for (i = 0; i < eventNum; ++i)
		{
			/* EV_KEY means type is key (not mouse, etc) */
			if (ev[i].type == EV_KEY && ev[i].value == 1)
			{
				return ev[i].code;
			}
		}
	}
	return -1;
}

/**
 * keyboard test case
 * return 0 if successful
 */
int TestKeyboard()
{
	int i;
	int rd;
	static struct input_event ev[64];
	int flag = 1;
	int event_num;
	int size = sizeof(struct input_event);
	int fd = OpenKeyboard(KEYBOARD_NAME);
	while (flag == 1)
	{
		rd = read(fd, ev, sizeof(ev));
		if (rd < size)
		{
			continue;
		}

		event_num = rd / size;
		for (i = 0; i < event_num; ++i)
		{
			/* EV_KEY means type is key (not mouse, etc) */
			if (ev[i].type == EV_KEY)
			{
				/* value 1 means press key */
				/* value 0 means release key */
				/* value 2 means long press key */
				if (ev[i].value == 1)
				{
					switch (ev[i].code)
					{
					case KEY_F1...KEY_F4:
						printf("press key F%d\n", ev[i].code - KEY_F1 + 1);
						break;
					case KEY_1...KEY_9:
						printf("press key %d\n", ev[i].code - KEY_1 + 1);
						break;
					case KEY_0:
						printf("press key 0\n");
						break;
					case KEY_KPASTERISK:
						printf("press key #\n");
						break;
					case KEY_NUMLOCK:
						printf("press key Alpha\n");
						break;
					case KEY_BACKSPACE:
						printf("press key Backspace\n");
						break;
					case KEY_ENTER:
						printf("press key Enter\n");
						break;
					case KEY_CANCEL:
						flag = 0;
						break;
					default:
						continue;
					}
				}
				else if (ev[i].value == 0)
				{
					printf("release key %d\n", ev[i].code);
				}
			}
		}
	}
	CloseKeyboard();
	return 0;
}
