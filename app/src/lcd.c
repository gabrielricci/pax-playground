/*------------------------------------------------------------
* FileName: lcd.c
* Author: LuX
* Date: 2013-02-17
* Example of how to use FrameBuffer to show pixels on the LCD.
* LCD coordinate:
* (0,0)-------(X,0)
* |             |
* |             |
* |             |
* (0,Y)-------(X,Y)
------------------------------------------------------------*/
#include "constant.h"
#include "framebuffer.h"
#include "header.h"
#include "lcd.h"
#include "osal.h"

/* FrameBuffer handler */
static int gFd;

/**
 * open LCD
 * return 0 if successful, else return -1
 */
int OpenLcd()
{
	gFd = OpenFrameBuffer(LCD_NAME);
	if (gFd < 0)
	{
		printf("open framebuffer device failed!\n");
		return -1;
	}
	return 0;
}

/**
 * close LCD
 * return 0 if successful, else return -1
 */
int CloseLcd()
{
	return CloseFrameBuffer(gFd);
}

/**
 * draw LCD rectangle
 * [in] x0 starting x coordinate, range [0, w - 1]
 * [in] y0 starting y coordinate, range [0, h - 1]
 * [in] x1 ending x coordinate, range [x0, w - 1]
 * [in] y1 ending y coordinate, range [y0, h - 1]
 * [in] color color to fill
 * return 0 if successful
 */
int DrawLcd(int x0, int y0, int x1, int y1, COLOR color)
{
	int _x0 = LcdY2FbX(y1);
	int _x1 = LcdY2FbX(y0);
	int _y0 = LcdX2FbY(x0);
	int _y1 = LcdX2FbY(x1);
	return DrawFrameBuffer(_x0, _y0, _x1, _y1, color);
}

/**
 * start draw LCD rectangle
 * [in] x0 starting x coordinate, range [0, w - 1]
 * [in] y0 starting y coordinate, range [0, h - 1]
 * [in] x1 ending x coordinate, range [x0, w - 1]
 * [in] y1 ending y coordinate, range [y0, h - 1]
 * return 0 if successful
 */
int StartDrawLcd(int x0, int y0, int x1, int y1)
{
	int _x0 = LcdY2FbX(y1);
	int _x1 = LcdY2FbX(y0);
	int _y0 = LcdX2FbY(x0);
	int _y1 = LcdX2FbY(x1);
	StartDrawFrameBuffer(_x0, _y0, _x1, _y1);
	return 0;
}

/**
 * get LCD size
 * Call "OpenLcd" first before use this function
 * [out] w LCD width
 * [out] h LCD height
 * return 0 if successful
 */
int GetLcdSize(int* w, int* h)
{
	struct fb_var_screeninfo fbScrVar;
	if (ioctl(gFd, FBIOGET_VSCREENINFO, &fbScrVar) < 0) {
		*w = 0;
		*h = 0;
		return -1;
	}
	*w = fbScrVar.yres;
	*h = fbScrVar.xres;
	return 0;
}

/**
 * LCD test case
 * return 0 if successful
 */
int TestLcd()
{
	int ret;
	int w, h;


	ret = OpenLcd();
	if (ret < 0)
	{
		printf("open lcd failed!\n");
		return -1;
	}
	GetLcdSize(&w, &h);
	DrawLcd(0, 0, w - 1, 0, RED);			// Top left to right
	DrawLcd(0, 0, 0, h - 1, RED);			// left top to bottom
	DrawLcd(w - 1, 0, w - 1, h - 1, RED);	// right top to bottom
	DrawLcd(0, h - 1, w - 1, h - 1, RED);	// bottom left to right
	DrawLcd(w >> 2, h >> 2, (w * 3) >> 2, (h * 3) >> 2, RED);
	StartDrawLcd(0, 0, w, h);

	CloseLcd();
	return 0;
}

