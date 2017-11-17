/*------------------------------------------------------------
* FileName: lcd.h
* Author: LuX
* Date: 2013-02-17
------------------------------------------------------------*/
#ifndef LCD_H
#define LCD_H
#include "constant.h"

/**
 * open LCD
 * return 0 if successful, else return -1
 */
int OpenLcd();

/**
 * close LCD
 * return 0 if successful, else return -1
 */
int CloseLcd();

/**
 * draw LCD rectangle
 * [in] x0 starting x coordinate, range [0, w - 1]
 * [in] y0 starting y coordinate, range [0, h - 1]
 * [in] x1 ending x coordinate, range [x0, w - 1]
 * [in] y1 ending y coordinate, range [y0, h - 1]
 * [in] color color to fill
 * return 0 if successful
 */
int DrawLcd(int x0, int y0, int x1, int y1, COLOR color);

/**
 * start draw LCD rectangle
 * [in] x0 starting x coordinate, range [0, w - 1]
 * [in] y0 starting y coordinate, range [0, h - 1]
 * [in] x1 ending x coordinate, range [x0, w - 1]
 * [in] y1 ending y coordinate, range [y0, h - 1]
 * return 0 if successful
 */
int StartDrawLcd(int x0, int y0, int x1, int y1);

/**
 * get LCD size
 * Call "OpenLcd" first before use this function
 * [out] w LCD width
 * [out] h LCD height
 * return 0 if successful
 */
int GetLcdSize(int* w, int* h);

#endif
