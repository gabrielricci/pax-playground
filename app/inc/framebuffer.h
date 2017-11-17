/*------------------------------------------------------------
* FileName: framebuffer.h
* Author: LuX
* Date: 2013-02-17
------------------------------------------------------------*/
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "constant.h"

/**
 * open FrameBuffer
 * [in] deviceName usually "/dev/fb0"
 * return FrameBuffer handler
 */
int OpenFrameBuffer(const char *deviceName);

/**
 * close FrameBuffer
 * [in] fd FrameBuffer handler
 * return 0 if success, else return -1
 */
int CloseFrameBuffer(int fd);

/**
 * draw a point
 * x coordinate is from bottom to top, and y coordinate is from left to right
 * [in] x x coordinate, range [0, xres - 1]
 * [in] y y coordinate, range [0, yres - 1]
 * [in] color color to fill
 * return 0 if successful
 */
int DrawPoint(int x, int y, COLOR color);

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
int DrawFrameBuffer(int x0, int y0, int x1, int y1, COLOR color);

/**
 * start drawing a rectangle
 * x coordinate is from bottom to top, and y coordinate is from left to right
 * [in] x0 starting x coordinate, range [0, xres - 1]
 * [in] y0 starting y coordinate, range [0, yres - 1]
 * [in] x1 ending x coordinate, range [x0, xres - 1]
 * [in] y1 ending y coordinate, range [y0, yres - 1]
 * return 0 if successful
 */
int StartDrawFrameBuffer(int x0, int y0, int x1, int y1);

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
int LcdY2FbX(int y);

/**
 * convert the x position of the LCD to the y position of the FrameBuffer
 * [in] x the x position of the LCD
 * return the y position of the FrameBuffer
 */
int LcdX2FbY(int x);

#endif
