/*------------------------------------------------------------
* FileName: main.c
* Author: LuX
* Date: 2013-10-24
* Example of how to use Prolin OSAL API. It can only run on Prolin OS 2.4 or higher.
*
*	 						Warning
* This code is for reference only. I do not guarantee the correctness, safety, efficiency or completeness.
* You are welcome to send an e-mail to me(lux@paxsz.com) if you have any questions.
------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <osal.h>
#include <xui.h>

#include "config.h"

char *KeyToStr(int key)
{
	static char buf[24];
	switch (key)
	{
	case XUI_KEY0: return "0";
	case XUI_KEY1: return "1";
	case XUI_KEY2: return "2";
	case XUI_KEY3: return "3";
	case XUI_KEY4: return "4";
	case XUI_KEY5: return "5";
	case XUI_KEY6: return "6";
	case XUI_KEY7: return "7";
	case XUI_KEY8: return "8";
	case XUI_KEY9: return "9";
	case XUI_KEYCANCEL: return "CANCEL";
	case XUI_KEYCLEAR: return "CLEAR";
	case XUI_KEYENTER: return "ENTER";
	case XUI_KEYALPHA: return "Alpha";
	case XUI_KEYSHARP: return "#";
	case XUI_KEYF1: return "F1";
	case XUI_KEYF2: return "F2";
	case XUI_KEYF3: return "F3";
	case XUI_KEYF4: return "F4";
	case XUI_KEYFUNC: return "FUNC";
	case XUI_KEYUP: return "UP";
	case XUI_KEYDOWN: return "DOWN";
	case XUI_KEYMENU: return "MENU";
	default:
		snprintf(buf, sizeof(buf), "value %u", key);
		return buf;
	}
	return "";
}

static int GuiInit(int statusbar_height)
{
	char value[128];
	char rotate_str[32];
	char statusbar_str[32];
	int ret;
	char *xui_argv[10];
	int  xui_argc;

	ret = OsRegGetValue("ro.fac.lcd.rotate", value);
	if (ret > 0) {
		snprintf(rotate_str, sizeof(rotate_str), "ROTATE=%s", value);
	}
	else {
		strcpy(rotate_str, "ROTATE=0");
	}

	if (statusbar_height > 0) {
		snprintf(statusbar_str, sizeof(statusbar_str), "STATUSBAR=%d", statusbar_height);
	}
	else {
		strcpy(statusbar_str, "STATUSBAR=0");
	}

	xui_argv[0] = rotate_str;
	xui_argv[1] = statusbar_str;
	xui_argv[2] = NULL;
	xui_argc = 2;

	ret = XuiOpen(xui_argc, xui_argv);
	if (ret == XUI_RET_OK) {
		return RET_OK;
	}
	else {
		return -1;
	}
}

static void GuiDeinit(void)
{
	XuiClose();
}

static void CrashReportInit(void)
{
	signal(SIGILL,    OsSaveCrashReport);
	signal(SIGABRT,   OsSaveCrashReport);
	signal(SIGBUS,    OsSaveCrashReport);
	signal(SIGFPE,    OsSaveCrashReport);
	signal(SIGSEGV,   OsSaveCrashReport);
	signal(SIGSTKFLT, OsSaveCrashReport);
	signal(SIGPIPE,   OsSaveCrashReport);
}

void UpdateStatusbar(XuiFont *font)
{
	static int i = 0;
	static ST_TIME oldTime = {0};
	ST_TIME newTime;
	int clockX;
	int clockY;
	int clockWidth;
	int clockHeight;
	XuiColor colorClock;
	char buf[16];
	char *batterys[] = {
			"./res/battery0c.png",
			"./res/battery25c.png",
			"./res/battery50c.png",
			"./res/battery75c.png",
			"./res/battery100c.png",
	};
	OsGetTime(&newTime);
	if (memcmp(&oldTime, &newTime, sizeof(oldTime)) != 0) {
		oldTime = newTime;
		snprintf(buf, sizeof(buf), "%02d:%02d:%02d", newTime.Hour, newTime.Minute, newTime.Second);
		clockHeight = 14;
		clockWidth = XuiTextWidth(font, clockHeight, "88:88:88");
		clockX = XUI_CENTER_X(0, XuiStatusbarCanvas()->width, clockWidth);
		clockY = XUI_CENTER_Y(0, XuiStatusbarCanvas()->height, clockHeight);
		XuiClearArea(XuiStatusbarCanvas(), clockX, 0, clockWidth, clockHeight+2);
		colorClock.r = 0xb8;
		colorClock.g = 0xb8;
		colorClock.b = 0xb8;
		colorClock.a = 0xff;
		XuiCanvasDrawText(XuiStatusbarCanvas(), clockX, clockY, clockHeight, font,
				XUI_TEXT_NORMAL, colorClock, buf);

		XuiSetStatusbarIcon(7, batterys[i]);
		i++;
		if (i >= sizeof(batterys)/sizeof(batterys[0])) {
			i = 0;
		}
	}
}

void StatusbarInit(void)
{
	XuiSetStatusbarIcon(0,"./res/mobile100.png");
	XuiSetStatusbarIcon(1,"./res/modem.png");
	XuiSetStatusbarIcon(2,"./res/ethernet.png");
	XuiSetStatusbarIcon(3,"./res/wifi100.png");
	XuiSetStatusbarIcon(4,"./res/lock.png");
	XuiSetStatusbarIcon(5,"./res/card.png");
	XuiSetStatusbarIcon(6,"./res/print.png");
	XuiSetStatusbarIcon(7,"./res/battery0c.png");
}

int HelloWorld(void)
{
	char *bgfilename;
	XuiImg *imgBg;
	XuiImg *imgFish;
	XuiImg *imgWallet;
	XuiFont *font;
	XuiFont *fontClock;
	XuiColor colorTitleFg;
	XuiColor colorTitleBg;
	XuiColor colorMsgFg;
	XuiColor colorBtnBg;
	int titleWidth;
	int titleHeight = 24;
	int titleX;
	int titleY;
	int imgX;
	int imgY;
	char *title = get_app_name();
	XuiWindow *btnCapture;
	XuiWindow *gifSoldier;
	XuiButtonStat statNormal;
	XuiButtonStat statPressed;
	int key;
	char buf[128];

	if (XuiRootCanvas()->width >=320) {
		bgfilename = "./res/bg_320x240.png";
	}
	else {
		bgfilename = "./res/bg_240x320.png";
	}

	/* load font */
	font = XuiCreateFont("/usr/font/paxfont.ttf", 0, 0);
	/* use custom font to display clock string */
	fontClock = XuiCreateFont("./res/clockopia.ttf", 0, 0);

	/* load image */
	imgBg = XuiImgLoadFromFile(bgfilename);
	imgFish = XuiImgLoadFromFile("./res/babelfish.png");
	imgWallet = XuiImgLoadFromFile("./res/mobilewallet.png");

	/* set background */
	colorTitleBg.r = 0x10;
	colorTitleBg.g = 0x00;
	colorTitleBg.b = 0xfe;
	colorTitleBg.a = 0xff;
	XuiCanvasSetBackground(XuiRootCanvas(), XUI_BG_CENTER, imgBg, colorTitleBg);
	/* after SetBackground, the imgBg can free it */
	XuiImgFree(imgBg);
	imgBg = NULL;

	/* init statusbar */
	StatusbarInit();

	/* draw title */
	colorTitleBg.r = 0x10;
	colorTitleBg.g = 0x00;
	colorTitleBg.b = 0xfe;
	colorTitleBg.a = 0x70;
	XuiCanvasDrawRect(XuiRootCanvas(), 0, 0, XuiRootCanvas()->width, 40, colorTitleBg, 0, 1);
	titleWidth = XuiTextWidth(font, titleHeight, title);
	titleX = XUI_CENTER_X(0, XuiRootCanvas()->width, titleWidth);
	titleY = XUI_CENTER_Y(0, 40, titleHeight);

	colorTitleFg.r = 0xef;
	colorTitleFg.g = 0xef;
	colorTitleFg.b = 0xef;
	colorTitleFg.a = 0xff;
	XuiCanvasDrawText(XuiRootCanvas(), titleX, titleY, titleHeight, font, XUI_TEXT_NORMAL,
			colorTitleFg, title);

	/* draw image */
	imgX = XUI_RIGHT_X(0, XuiRootCanvas()->width, imgWallet->width);
	imgY = 80;
	XuiCanvasDrawImg(XuiRootCanvas(), imgX, imgY, imgWallet->width, imgWallet->height, XUI_BG_NORMAL, imgWallet);

	/* create button */
	btnCapture = XuiCreateButton(XuiRootCanvas(), 10, 45, 64, 64);
	memset(&statNormal, 0, sizeof(statNormal));
	colorBtnBg.r = 0x0f;
	colorBtnBg.g = 0x0f;
	colorBtnBg.b = 0x7f;
	colorBtnBg.a = 0x70;
	statNormal.btn_bg = colorBtnBg;
	statNormal.btn_round = 0;
	statNormal.img_style = XUI_BG_NORMAL;
	statNormal.img_x = XUI_CENTER_X(0, 64, 48);
	statNormal.img_y = 2;
	statNormal.img = imgFish;
	statNormal.text_fg = colorTitleFg;
	statNormal.text_font = font;
	statNormal.text_height = 12;
	statNormal.text = "3.Capture";
	statNormal.text_x = XUI_CENTER_X(0, 64, XuiTextWidth(statNormal.text_font, statNormal.text_height, statNormal.text));
	statNormal.text_y = 50;
	statPressed = statNormal;
	colorBtnBg.a = 0xa0;
	statPressed.btn_bg = colorBtnBg;

	XuiButtonSetStat(btnCapture, XUI_BTN_NORMAL, &statNormal);
	XuiButtonSetStat(btnCapture, XUI_BTN_PRESSED, &statPressed);
	XuiButtonSetKey(btnCapture, XUI_KEY3);
	XuiShowWindow(btnCapture, 1, 0);

	/* create gif */
	gifSoldier = XuiCreateGif(XuiRootCanvas(), 10, 120, 66, 80, "./res/soldier.gif");
	XuiShowWindow(gifSoldier, 1, 0);

	/* loop & wait for keys */
	while(1) {
		UpdateStatusbar(fontClock);
		if (!XuiHasKey()) {
			continue;
		}
		key = XuiGetKey();
		/* display key info */
		snprintf(buf, sizeof(buf), "Press %s", KeyToStr(key));
		XuiClearArea(XuiRootCanvas(), 10, 190, 200, 16+2);
		colorMsgFg.r = 0x00;
		colorMsgFg.g = 0x00;
		colorMsgFg.b = 0x00;
		colorMsgFg.a = 0xff;
		XuiCanvasDrawText(XuiRootCanvas(), 10, 190, 16, font,
				XUI_TEXT_NORMAL, colorMsgFg, buf);

		if (key == XUI_KEY3) {
			XuiImg *imgTmp;
			imgTmp = XuiCaptureScreen();
			XuiImgSaveToFile(imgTmp, "/tmp/screen.png");
			XuiImgFree(imgTmp);

		}
		else if (key == XUI_KEYCANCEL) {
			break;
		}
	}

	/* free res */
	/* first, free windows */
	XuiShowWindow(gifSoldier, 0, 0);
	XuiDestroyWindow(gifSoldier);

	XuiShowWindow(btnCapture, 0, 0);
	XuiDestroyWindow(btnCapture);

	/* second, free images */
	XuiImgFree(imgWallet);
	XuiImgFree(imgFish);

	/* third, free fonts */
	XuiDestroyFont(font);
	XuiDestroyFont(fontClock);

	return 0;

}

int main(int argc, char **argv)
{
	CrashReportInit();
	GuiInit(18);
	HelloWorld();
	GuiDeinit();
	return 0;
}
