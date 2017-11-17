/*------------------------------------------------------------
* FileName: constant.h
* Author: LuX
* Date: 2013-02-17
------------------------------------------------------------*/
#ifndef CONSTANT_H
#define CONSTANT_H

/**
 * basic data type
 */
#ifndef BYTE
#define BYTE	unsigned char
#endif

#ifndef WORD
#define WORD	unsigned short
#endif

#ifndef COLOR
#define COLOR	WORD
#endif

/**
 * RGB 888
 */
// the r, g, b mask of RGB888 color is 0xFF0000, 0xFF00, 0xFF
#define R_RGB888(x)		(unsigned char) (((x) >> 16) & 0xFF)
#define G_RGB888(x)		(unsigned char) (((x) >> 8) & 0xFF)
#define B_RGB888(x)		(unsigned char) ((x) & 0xFF)
// Generating RGB565 pixel by composing r, g, b components
#ifndef RGB888
#define RGB888(r, g ,b)  ((DWORD) (((BYTE) (r) | ((WORD) (g) << 8)) | (((DWORD) (BYTE) (b)) << 16)))
#endif

/**
 * RGB 565
 */
// the r, g, b value of RGB565 color is 0xF800, 0x7E0, 0x1F
// R R R R    R G G G    G G G B    B B B B
#define R_RGB565(x)		(unsigned char) (((x) >> 8) & 0xF8)
#define G_RGB565(x)		(unsigned char) (((x) >> 3) & 0xFC)
#define B_RGB565(x)		(unsigned char) ((x) << 3)
// Generating RGB565 pixel by composing r, g, b components
#ifndef RGB565
#define RGB565(r, g, b)	(COLOR) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))
#endif

/**
 * color of RGB565
 */
#define WHITE	RGB565(0xff, 0xff, 0xff)
#define BLACK	RGB565(0x00, 0x00, 0x00)
#define RED		RGB565(0xff, 0, 0)
#define GREEN	RGB565(0, 0xff, 0)
#define BLUE	RGB565(0, 0, 0xff)
#define GRAY	RGB565(0xbe, 0xbe, 0xbe)

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define FONT_NAME			"paxfont.ttf"
#define KEYBOARD_NAME		"/dev/input/event0"
#define TOUCHSCREEN_NAME	"/dev/input/event2"
#define LCD_NAME			"/dev/graphics/fb0"

#endif
