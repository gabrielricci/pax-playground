/*------------------------------------------------------------
* FileName: framebuffer.c
* Author: LuX
* Date: 2013-02-17
* Example of how to use FrameBuffer to show pixels on the LCD.
* FrameBuffer coordinate:
* (X,0)-------(X,Y)
* |             |
* |             |
* |             |
* (0,0)-------(0,Y)
* Developers must implement their own FrameBuffer or
* use a third-party GUI library.
------------------------------------------------------------*/
#include "constant.h"
#include "framebuffer.h"
#include "header.h"
/**
 * mapped FrameBuffer device memory
 */
static void *gFb = NULL;
static void *gFbCache = NULL;
/**
 * screen information
 * see fb_var_screeninfo structure:
 * xres means point number in row
 * yres means point number in column
 * bits_per_pixels means bit number of each point
 */
static struct fb_var_screeninfo gFbVarInfo;
/**
 * open FrameBuffer
 * [in] deviceName usually "/dev/graphics/fb*"
 * return FrameBuffer handler
 */
int OpenFrameBuffer(const char *deviceName)
{
	int fd;
	unsigned int screenSize;

	/* open the FrameBuffer device. */
	fd = open(deviceName, O_RDWR);
	if (fd < 0)
	{
		return -1;
	}

	/* get the variable screen info */
	if (ioctl(fd, FBIOGET_VSCREENINFO, &gFbVarInfo) < 0)
	{
		return -1;
	}
	screenSize = (gFbVarInfo.xres * gFbVarInfo.yres * gFbVarInfo.bits_per_pixel) >> 3;
	gFb = mmap(0, screenSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	gFbCache = malloc(screenSize);
	if (gFb == MAP_FAILED)
	{
		return -1;
	}
	return fd;
}

/**
 * close FrameBuffer
 * [in] fd FrameBuffer handler
 * return 0 if successful, else return -1
 */
int CloseFrameBuffer(int fd)
{
	int ret = munmap(gFb, (gFbVarInfo.xres * gFbVarInfo.yres * gFbVarInfo.bits_per_pixel) >> 3);
	close(fd);
	free(gFbCache);
	return ret;
}

/**
 * draw a point
 * x coordinate is from bottom to top, and y coordinate is from left to right
 * [in] x x coordinate, range [0, xres - 1]
 * [in] y y coordinate, range [0, yres - 1]
 * [in] color color to fill
 * return 0 if successful
 */
int DrawPoint(int x, int y, COLOR color)
{
	if (x < 0 || (__u32)x >= gFbVarInfo.xres || y < 0 || (__u32)y >= gFbVarInfo.yres)
	{
		return -1;
	}
	COLOR* p = (COLOR*)gFbCache;
	*(p + y * gFbVarInfo.xres + x) = color;
	return 0;
}

/**
 * draw a rectangle
 * x coordinate is from bottom to top, and y coordinate is from left to right
 * [in] x0 starting x coordinate, range [0, xres - 1]
 * [in] y0 starting y coordinate, range [0, yres - 1]
 * [in] x1 ending x coordinate, range [x0, xres - 1]
 * [in] y1 ending y coordinate, range [y0, yres - 1]
 * [in] color color to fill
 * return 0 if successful
 */
int DrawFrameBuffer(int x0, int y0, int x1, int y1, COLOR color)
{
	int i, j;
	COLOR* p = (COLOR*)gFbCache;
	for (j = y0; j <= y1; ++j)
	{
		for (i = x0; i <= x1; ++i)
		{
			*(p + j * gFbVarInfo.xres + i) = color;
		}
	}
	return 0;
}

/**
 * start drawing a rectangle
 * x coordinate is from bottom to top, and y coordinate is from left to right
 * [in] x0 starting x coordinate, range [0, xres - 1]
 * [in] y0 starting y coordinate, range [0, yres - 1]
 * [in] x1 ending x coordinate, range [x0, xres - 1]
 * [in] y1 ending y coordinate, range [y0, yres - 1]
 * return 0 if successful
 */
int StartDrawFrameBuffer(int x0, int y0, int x1, int y1)
{
	int j;
	int len = x1 - x0 + 1;
	int pos;
	for (j = y0; j <= y1; ++j)
	{
		pos = (j * gFbVarInfo.xres + x0) << 1;
		memcpy((char*)gFb + pos, (char*)gFbCache + pos, (len << 1));
	}
	return 0;
}

/**
 * convert the y position of the LCD to the x position of the FrameBuffer
 * LCD coordinate: x from left to right and y from top to bottom
 * (0,0)------------(X,0)
 * |				|
 * |				|
 * |				|
 * |				|
 * (0,Y)------------(X,Y)
 *
 * FrameBuffer coordinate: x from bottom to top and y from left to right
 * (X,0)------------(X,Y)
 * |				|
 * |				|
 * |				|
 * |				|
 * (0,0)------------(0,Y)
 * [in] y the y position of the LCD
 * return the x position of the FrameBuffer
 */
int LcdY2FbX(int y)
{
	int max = gFbVarInfo.xres - 1;
	if (y < 0)
	{
		y = 0;
	}
	else if (y > max)
	{
		y = max;
	}
	return max - y;
}

/**
 * convert the x position of the LCD to the y position of the FrameBuffer
 * [in] x the x position of the LCD
 * return the y position of the FrameBuffer
 */
int LcdX2FbY(int x)
{
	int max = gFbVarInfo.yres - 1;
	if (x < 0)
	{
		x = 0;
	}
	else if (x > max)
	{
		x = max;
	}
	return x;
}

/**
 * FrameBuffer test case
 * return 0 if successful
 */
int TestFrameBuffer()
{
	int fd;
	fd = OpenFrameBuffer("/dev/fb0");
	if (fd < 0)
	{
		return -1;
	}
	DrawFrameBuffer(0, 0, gFbVarInfo.xres - 1, 0, RED);
	DrawFrameBuffer(0, 0, 0, gFbVarInfo.yres - 1, RED);
	DrawFrameBuffer(gFbVarInfo.xres - 1, 0, gFbVarInfo.xres - 1, gFbVarInfo.yres - 1, RED);
	DrawFrameBuffer(0, gFbVarInfo.yres - 1, gFbVarInfo.xres - 1, gFbVarInfo.yres - 1, RED);
	DrawFrameBuffer(gFbVarInfo.xres >> 2, gFbVarInfo.yres >> 2, (gFbVarInfo.xres * 3) >> 2, (gFbVarInfo.yres * 3) >> 2, RED);
	StartDrawFrameBuffer(0, 0, gFbVarInfo.xres, gFbVarInfo.yres);
	CloseFrameBuffer(fd);
	return 0;
}


