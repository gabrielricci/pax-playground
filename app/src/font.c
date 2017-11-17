/*------------------------------------------------------------
* FileName: font.c
* Author: LuX
* Date: 2013-05-14
* Example of how to load font and print characters.
* For Prolin 2.4, please add -freetype and -pthread link option when building the project.
* You can also use the freetype library instead PAX OSAL library, see www.freetype.org.
------------------------------------------------------------*/
#include "constant.h"
#include "font.h"
#include "framebuffer.h"
#include "header.h"
#include "lcd.h"
#include "osal.h"
/* distance for 2 neighboring characters */
#define CHARACTER_DISTANCE  2
/* for Chinese language, every character requires 4 bytes
 * for English language, every character requires 2 bytes
 */
#ifdef LAN_CN
#define CHARACTER_SIZE 4
#else
#define CHARACTER_SIZE 2
#endif
/* font handler */
static int gFont;

/**
 * open font
 * [in] filename font file name, such as "paxfont.ttf"
 * return font handle(>=0)
 */
int OpenFont(const char *filename)
{
	/* file paxfont.ttf is the default font file in the prolin system */
	gFont = OsOpenFont(filename);
	if (gFont < 0)
	{
		return -1;
	}
	/* better to wait 3 seconds to let the system loads dot matrix before calling OsGetFontDot function */
	/* sleep(3); */
	return gFont;
}

/**
 * close font
 */
void CloseFont()
{
	OsCloseFont(gFont);
}

/**
 * get dots of a string
 * [in] str the string to display
 * [out] ftDots dots of the string
 * [in] height initialized height of the string
 */
int GetStringDots(const char *str, FT_DOT ftDots[], int height)
{
	int i;
	int ret;
	char character[CHARACTER_SIZE];
	int len = strlen(str);
	/* the with is better set as half length of the height */
	int width = height >> 1;
	/* the last byte must be 0 */
	character[CHARACTER_SIZE - 1] = '\0';
	for (i = 0; i < len; ++i)
	{
		memcpy(character, str + i * (CHARACTER_SIZE - 1), CHARACTER_SIZE - 1);
		/* load the dot matrix of the character. */
		/* if use FONT_STYLE_BOLD, the inner algorithm is very complicate and takes for a long time */
		ret = OsGetFontDot(gFont, character, width, height, FONT_STYLE_NONE, &ftDots[i]);
		if (ret != RET_OK)
		{
			return -1;
		}
	}
	return 0;
}

/**
 * get displaying length of a string
 * If you want to shown a string in the center horizontally of the LCD,
 * just set starting x position: (screen_width - GetStringLength()) / 2
 * [in] ftDots dots array of the string
 * [in] ftDotsNum element number of dots array
 * [in] height initialized height of the string
 * return displaying length if successful, else return -1
 */
int GetStringLength(const char* str, int height)
{
	int i;
	int widthSum = 0;
	int len = strlen(str);
	/* the with is better set as half length of the height */
	int width = height >> 1;
	FT_DOT ftDots[len];
	if (GetStringDots(str, ftDots, height) < 0)
	{
		return -1;
	}
	for (i = 0; i < len; ++i)
	{
		/* with of blank space " " is 0 */
		widthSum += (ftDots[i].Width == 0)?width:ftDots[i].Width;
	}
	widthSum += (CHARACTER_DISTANCE * len);
	return widthSum;
}

/**
 * draw font dot
 * [in] pFtDot pointer to the FT_DOT
 * [in] x starting x coordinate
 * [in] y starting y coordinate
 * [in] color color to draw
 * return 0 if successful
 */
int DrawFontDot(const FT_DOT *pFtDot, int x, int y, COLOR color)
{
	int i, j, k;
	unsigned char dot;
	/* byte number of every line, must align to an integer multiple of 8 */
	int lineLen = (pFtDot->Width + 7) >> 3;
	int _x0 = LcdY2FbX(y);
	int _y0 = LcdX2FbY(x);
	int _x1 = _x0;
	int _y1 = _y0;
	for (i = 0; i < pFtDot->Height; ++i)
	{
		for (j = 0; j < lineLen; ++j)
		{
			/* current dot */
			dot = pFtDot->Dot[i * lineLen + j];
			/* get every bit */
			for (k = 7; k >= 0; --k)
			{
				/* if the bit value is 1, set color */
				if ((dot >> k) & 0x1)
				{
					DrawPoint(_x1, _y1, color);
				}
				/* set y position of next bit */
				++_y1;
			}
		}
		/* set the starting position of the next line */
		_y1 = _y0;
		--_x1;
	}
	return 0;
}

/**
 * draw string
 * [in] str string to draw
 * [in] height initialized height
 * [in] x starting x coordinate
 * [in] y starting y coordinate
 * [in] color color to draw
 * return 0 if successful
 */
int DrawString(const char *str, int height, int x, int y, COLOR color)
{
	int i;
	/* the with is better set as half length of the height */
	int width = height >> 1;
	int size = strlen(str);
	FT_DOT ftDots[size];
	/* get dots according to the string */
	GetStringDots(str, ftDots, height);
	for (i = 0; i < size; ++i)
	{
		DrawFontDot(&ftDots[i], x, y + (height - ftDots[i].Top), color);
		/* if use the following, then the characters will bottom align
		DrawFontDot(&ftDots[i], x, y + (height - ftDots[i].Height), color);
		*/
		x += (ftDots[i].Width == 0)?width:ftDots[i].Width;
		x += CHARACTER_DISTANCE;
	}
	return 0;
}

/**
 * font test case
 * return 0 if successful, else return -1
 */
int TestFont()
{
	int w, h;
	int height = 24;
	const char *str1 = "Top Left";
	const char *str2 = "Middle";
	int strLen;
	/* open font */
	gFont = OpenFont("paxfont.ttf");
	if (gFont < 0)
	{
		return -1;
	}
	/* draw the characters */
	DrawString(str1, height, 0, 0, RED);
	/* get displaying length of a string */
	strLen = GetStringLength(str2, height);
	/* get screen size */
	GetLcdSize(&w, &h);
	DrawString(str2, height, (w - strLen) >> 1, (h - height) >> 1, RED);
	StartDrawLcd(0, 0, w, h);
	/* close font */
	CloseFont();
	return 0;
}
