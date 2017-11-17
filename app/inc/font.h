/*------------------------------------------------------------
* FileName: font.h
* Author: LuX
* Date: 2013-05-14
------------------------------------------------------------*/
#ifndef FONT_H
#define FONT_H
#include "constant.h"

/**
 * open font
 * [in] filename font file name, such as "paxfont.ttf"
 * return font handle(>=0)
 */
int OpenFont(const char *filename);
/**
 * close font
 */
void CloseFont();

/**
 * get displaying length of a string
 * If you want to shown a string in the center horizontally of the LCD,
 * just set starting x position: (screen_width - GetStringLength()) / 2
 * [in] ftDots dots array of the string
 * [in] ftDotsNum element number of dots array
 * [in] height initialized height of the string
 * return displaying length if successful, else return -1
 */
int GetStringLength(const char* str, int height);

/**
 * draw string
 * [in] str string to draw
 * [in] height initialized height
 * [in] x starting x coordinate
 * [in] y starting y coordinate
 * [in] color color to draw
 * return 0 if successful
 */
int DrawString(const char *str, int height, int x, int y, COLOR color);

/**
 * font test case
 * return 0 if successful, else return -1
 */
int TestFont();

#endif
