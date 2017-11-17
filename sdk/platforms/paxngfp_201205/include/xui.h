#ifndef _XUI_H_
#define _XUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#define XUI_RET_OK				   0	/* Succeed */
#define XUI_ERR_INVALID_PARAM	(-1)	/* Error parameter */
#define XUI_ERR_SUSPEND			(-2)	/* the xui is suspend*/
#define XUI_ERR_BUSY			(-3)	/* the xui has opened */
#define XUI_ERR_NOT_SUPPORT 	(-4)	/* Error parameter */
#define XUI_ERR_SAVE_FILE_FAIL  (-5)	/* Error parameter */
#define XUI_ERR_NOT_OPEN		(-6)	/* the xui Not Open */

#define XUI_HIDE 0
#define XUI_SHOW 1

/* Key values */
#define XUI_KEY1			2	/* 1 */
#define XUI_KEY2			3	/* 2 */
#define XUI_KEY3			4	/* 3 */
#define XUI_KEY4			5	/* 4 */
#define XUI_KEY5			6	/* 5 */
#define XUI_KEY6			7	/* 6 */
#define XUI_KEY7			8	/* 7 */
#define XUI_KEY8			9	/* 8 */
#define XUI_KEY9			10	/* 9 */
#define XUI_KEY0			11	/* 0 */
#define XUI_KEYCANCEL		223	/* Cancel */
#define XUI_KEYCLEAR		14	/* Clear */
#define XUI_KEYENTER		28	/* Enter */
#define XUI_KEYALPHA		69	/* Alpha */
#define XUI_KEYSHARP		55	/* # */
#define XUI_KEYF1			59
#define XUI_KEYF2			60
#define XUI_KEYF3			61
#define XUI_KEYF4			62
#define XUI_KEYFUNC         102
#define XUI_KEYUP           103
#define XUI_KEYDOWN         108
#define XUI_KEYMENU         139
#define XUI_KEYCAMERA       212

#define XUI_KEYENTER1		30	/* Enter+1 */
#define XUI_KEYENTER2		31	/* Enter+2 */
#define XUI_KEYENTER3		32	/* Enter+3 */
#define XUI_KEYENTER4		33	/* Enter+4 */
#define XUI_KEYENTER5		34	/* Enter+5 */
#define XUI_KEYENTER6		35	/* Enter+6 */
#define XUI_KEYENTER7		36	/* Enter+7 */
#define XUI_KEYENTER8		37	/* Enter+8 */
#define XUI_KEYENTER9		38	/* Enter+9 */
#define XUI_KEYENTER0		39	/* Enter+0 */


#define XUI_SOFTKEYBOARD_KEY               0xff
#define XUI_SOFTKEYBOARD_KEYBACKSPACE      (XUI_SOFTKEYBOARD_KEY+0x08)   /* backspace */

#define XUI_SOFTKEYBOARD_KEYSPACE          (XUI_SOFTKEYBOARD_KEY+0x20)   /* space */
#define XUI_SOFTKEYBOARD_KEYEXCLAM         (XUI_SOFTKEYBOARD_KEY+0x21)   /* ! */
#define XUI_SOFTKEYBOARD_KEYDOUBLEQUOTE    (XUI_SOFTKEYBOARD_KEY+0x22)   /* " */
#define XUI_SOFTKEYBOARD_KEYSHARP          (XUI_SOFTKEYBOARD_KEY+0x23)   /* # */
#define XUI_SOFTKEYBOARD_KEYDOLLAR         (XUI_SOFTKEYBOARD_KEY+0x24)   /* $ */
#define XUI_SOFTKEYBOARD_KEYPERCENT        (XUI_SOFTKEYBOARD_KEY+0x25)   /* % */
#define XUI_SOFTKEYBOARD_KEYAMPERSAND      (XUI_SOFTKEYBOARD_KEY+0x26)   /* & */
#define XUI_SOFTKEYBOARD_KEYSINGLEQUOTE    (XUI_SOFTKEYBOARD_KEY+0x27)   /* ' */
#define XUI_SOFTKEYBOARD_KEYPARENLEFT      (XUI_SOFTKEYBOARD_KEY+0x28)   /* ( */
#define XUI_SOFTKEYBOARD_KEYPARENRIGHT     (XUI_SOFTKEYBOARD_KEY+0x29)   /* ) */
#define XUI_SOFTKEYBOARD_KEYASTERISK       (XUI_SOFTKEYBOARD_KEY+0x2a)   /* * */
#define XUI_SOFTKEYBOARD_KEYPLUS           (XUI_SOFTKEYBOARD_KEY+0x2b)   /* + */
#define XUI_SOFTKEYBOARD_KEYCOMMA          (XUI_SOFTKEYBOARD_KEY+0x2c)   /* , */
#define XUI_SOFTKEYBOARD_KEYMINUS          (XUI_SOFTKEYBOARD_KEY+0x2d)   /* - */
#define XUI_SOFTKEYBOARD_KEYPERIOD         (XUI_SOFTKEYBOARD_KEY+0x2e)   /* . */
#define XUI_SOFTKEYBOARD_KEYSLASH          (XUI_SOFTKEYBOARD_KEY+0x2f)   /* / */

#define XUI_SOFTKEYBOARD_KEY0              (XUI_SOFTKEYBOARD_KEY+0x30)   /* 0 */
#define XUI_SOFTKEYBOARD_KEY1              (XUI_SOFTKEYBOARD_KEY+0x31)   /* 1 */
#define XUI_SOFTKEYBOARD_KEY2              (XUI_SOFTKEYBOARD_KEY+0x32)   /* 2 */
#define XUI_SOFTKEYBOARD_KEY3              (XUI_SOFTKEYBOARD_KEY+0x33)   /* 3 */
#define XUI_SOFTKEYBOARD_KEY4              (XUI_SOFTKEYBOARD_KEY+0x34)   /* 4 */
#define XUI_SOFTKEYBOARD_KEY5              (XUI_SOFTKEYBOARD_KEY+0x35)   /* 5 */
#define XUI_SOFTKEYBOARD_KEY6              (XUI_SOFTKEYBOARD_KEY+0x36)   /* 6 */
#define XUI_SOFTKEYBOARD_KEY7              (XUI_SOFTKEYBOARD_KEY+0x37)   /* 7 */
#define XUI_SOFTKEYBOARD_KEY8              (XUI_SOFTKEYBOARD_KEY+0x38)   /* 8 */
#define XUI_SOFTKEYBOARD_KEY9              (XUI_SOFTKEYBOARD_KEY+0x39)   /* 9 */

#define XUI_SOFTKEYBOARD_KEYCOLON          (XUI_SOFTKEYBOARD_KEY+0x3a)   /* : */
#define XUI_SOFTKEYBOARD_KEYSEMICOLON      (XUI_SOFTKEYBOARD_KEY+0x3b)   /* ; */
#define XUI_SOFTKEYBOARD_KEYLESS           (XUI_SOFTKEYBOARD_KEY+0x3c)   /* < */
#define XUI_SOFTKEYBOARD_KEYEQUAL          (XUI_SOFTKEYBOARD_KEY+0x3d)   /* = */
#define XUI_SOFTKEYBOARD_KEYGREATER        (XUI_SOFTKEYBOARD_KEY+0x3e)   /* > */
#define XUI_SOFTKEYBOARD_KEYQUESTION       (XUI_SOFTKEYBOARD_KEY+0x3f)   /* ? */
#define XUI_SOFTKEYBOARD_KEYAT             (XUI_SOFTKEYBOARD_KEY+0x40)   /* @ */

#define XUI_SOFTKEYBOARD_KEYA              (XUI_SOFTKEYBOARD_KEY+0x41)   /* A */
#define XUI_SOFTKEYBOARD_KEYB              (XUI_SOFTKEYBOARD_KEY+0x42)   /* B */
#define XUI_SOFTKEYBOARD_KEYC              (XUI_SOFTKEYBOARD_KEY+0x43)   /* C */
#define XUI_SOFTKEYBOARD_KEYD              (XUI_SOFTKEYBOARD_KEY+0x44)   /* D */
#define XUI_SOFTKEYBOARD_KEYE              (XUI_SOFTKEYBOARD_KEY+0x45)   /* E */
#define XUI_SOFTKEYBOARD_KEYF              (XUI_SOFTKEYBOARD_KEY+0x46)   /* F */
#define XUI_SOFTKEYBOARD_KEYG              (XUI_SOFTKEYBOARD_KEY+0x47)   /* G */
#define XUI_SOFTKEYBOARD_KEYH              (XUI_SOFTKEYBOARD_KEY+0x48)   /* H */
#define XUI_SOFTKEYBOARD_KEYI              (XUI_SOFTKEYBOARD_KEY+0x49)   /* I */
#define XUI_SOFTKEYBOARD_KEYJ              (XUI_SOFTKEYBOARD_KEY+0x4a)   /* J */
#define XUI_SOFTKEYBOARD_KEYK              (XUI_SOFTKEYBOARD_KEY+0x4b)   /* K */
#define XUI_SOFTKEYBOARD_KEYL              (XUI_SOFTKEYBOARD_KEY+0x4c)   /* L */
#define XUI_SOFTKEYBOARD_KEYM              (XUI_SOFTKEYBOARD_KEY+0x4d)   /* M */
#define XUI_SOFTKEYBOARD_KEYN              (XUI_SOFTKEYBOARD_KEY+0x4e)   /* N */
#define XUI_SOFTKEYBOARD_KEYO              (XUI_SOFTKEYBOARD_KEY+0x4f)   /* O */
#define XUI_SOFTKEYBOARD_KEYP              (XUI_SOFTKEYBOARD_KEY+0x50)   /* P */
#define XUI_SOFTKEYBOARD_KEYQ              (XUI_SOFTKEYBOARD_KEY+0x51)   /* Q */
#define XUI_SOFTKEYBOARD_KEYR              (XUI_SOFTKEYBOARD_KEY+0x52)   /* R */
#define XUI_SOFTKEYBOARD_KEYS              (XUI_SOFTKEYBOARD_KEY+0x53)   /* S */
#define XUI_SOFTKEYBOARD_KEYT              (XUI_SOFTKEYBOARD_KEY+0x54)   /* T */
#define XUI_SOFTKEYBOARD_KEYU              (XUI_SOFTKEYBOARD_KEY+0x55)   /* U */
#define XUI_SOFTKEYBOARD_KEYV              (XUI_SOFTKEYBOARD_KEY+0x56)   /* V */
#define XUI_SOFTKEYBOARD_KEYW              (XUI_SOFTKEYBOARD_KEY+0x57)   /* W */
#define XUI_SOFTKEYBOARD_KEYX              (XUI_SOFTKEYBOARD_KEY+0x58)   /* X */
#define XUI_SOFTKEYBOARD_KEYY              (XUI_SOFTKEYBOARD_KEY+0x59)   /* Y */
#define XUI_SOFTKEYBOARD_KEYZ              (XUI_SOFTKEYBOARD_KEY+0x5a)   /* Z */

#define XUI_SOFTKEYBOARD_KEYBRACKETLEFT    (XUI_SOFTKEYBOARD_KEY+0x5b)   /* [ */
#define XUI_SOFTKEYBOARD_KEYBACKSLASH      (XUI_SOFTKEYBOARD_KEY+0x5c)   /* \ */
#define XUI_SOFTKEYBOARD_KEYBRACKETRIGHT   (XUI_SOFTKEYBOARD_KEY+0x5d)   /* ] */
#define XUI_SOFTKEYBOARD_KEYCARET          (XUI_SOFTKEYBOARD_KEY+0x5e)   /* ^ */
#define XUI_SOFTKEYBOARD_KEYUNDERSCORE     (XUI_SOFTKEYBOARD_KEY+0x5f)   /* _ */
#define XUI_SOFTKEYBOARD_KEYBACKQUOTE      (XUI_SOFTKEYBOARD_KEY+0x60)   /* ` */

#define XUI_SOFTKEYBOARD_KEYa              (XUI_SOFTKEYBOARD_KEY+0x61)   /* a */
#define XUI_SOFTKEYBOARD_KEYb              (XUI_SOFTKEYBOARD_KEY+0x62)   /* b */
#define XUI_SOFTKEYBOARD_KEYc              (XUI_SOFTKEYBOARD_KEY+0x63)   /* c */
#define XUI_SOFTKEYBOARD_KEYd              (XUI_SOFTKEYBOARD_KEY+0x64)   /* d */
#define XUI_SOFTKEYBOARD_KEYe              (XUI_SOFTKEYBOARD_KEY+0x65)   /* e */
#define XUI_SOFTKEYBOARD_KEYf              (XUI_SOFTKEYBOARD_KEY+0x66)   /* f */
#define XUI_SOFTKEYBOARD_KEYg              (XUI_SOFTKEYBOARD_KEY+0x67)   /* g */
#define XUI_SOFTKEYBOARD_KEYh              (XUI_SOFTKEYBOARD_KEY+0x68)   /* h */
#define XUI_SOFTKEYBOARD_KEYi              (XUI_SOFTKEYBOARD_KEY+0x69)   /* i */
#define XUI_SOFTKEYBOARD_KEYj              (XUI_SOFTKEYBOARD_KEY+0x6a)   /* j */
#define XUI_SOFTKEYBOARD_KEYk              (XUI_SOFTKEYBOARD_KEY+0x6b)   /* k */
#define XUI_SOFTKEYBOARD_KEYl              (XUI_SOFTKEYBOARD_KEY+0x6c)   /* l */
#define XUI_SOFTKEYBOARD_KEYm              (XUI_SOFTKEYBOARD_KEY+0x6d)   /* m */
#define XUI_SOFTKEYBOARD_KEYn              (XUI_SOFTKEYBOARD_KEY+0x6e)   /* n */
#define XUI_SOFTKEYBOARD_KEYo              (XUI_SOFTKEYBOARD_KEY+0x6f)   /* o */
#define XUI_SOFTKEYBOARD_KEYp              (XUI_SOFTKEYBOARD_KEY+0x70)   /* p */
#define XUI_SOFTKEYBOARD_KEYq              (XUI_SOFTKEYBOARD_KEY+0x71)   /* q */
#define XUI_SOFTKEYBOARD_KEYr              (XUI_SOFTKEYBOARD_KEY+0x72)   /* r */
#define XUI_SOFTKEYBOARD_KEYs              (XUI_SOFTKEYBOARD_KEY+0x73)   /* s */
#define XUI_SOFTKEYBOARD_KEYt              (XUI_SOFTKEYBOARD_KEY+0x74)   /* t */
#define XUI_SOFTKEYBOARD_KEYu              (XUI_SOFTKEYBOARD_KEY+0x75)   /* u */
#define XUI_SOFTKEYBOARD_KEYv              (XUI_SOFTKEYBOARD_KEY+0x76)   /* v */
#define XUI_SOFTKEYBOARD_KEYw              (XUI_SOFTKEYBOARD_KEY+0x77)   /* w */
#define XUI_SOFTKEYBOARD_KEYx              (XUI_SOFTKEYBOARD_KEY+0x78)   /* x */
#define XUI_SOFTKEYBOARD_KEYy              (XUI_SOFTKEYBOARD_KEY+0x79)   /* y */
#define XUI_SOFTKEYBOARD_KEYz              (XUI_SOFTKEYBOARD_KEY+0x7a)   /* z */

#define XUI_SOFTKEYBOARD_KEYBRACELEFT      (XUI_SOFTKEYBOARD_KEY+0x7b)   /* { */
#define XUI_SOFTKEYBOARD_KEYBAR            (XUI_SOFTKEYBOARD_KEY+0x7c)   /* | */
#define XUI_SOFTKEYBOARD_KEYBRACERIGHT     (XUI_SOFTKEYBOARD_KEY+0x7d)   /* } */
#define XUI_SOFTKEYBOARD_KEYTILDE          (XUI_SOFTKEYBOARD_KEY+0x7e)   /* ~ */

typedef struct
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
} XuiColor;

typedef enum
{
	XUI_ROTATE_0,
	XUI_ROTATE_90,
	XUI_ROTATE_180,
	XUI_ROTATE_270,
	XUI_FLIP_VERT,
	XUI_FLIP_HORIZ
} XuiTransform;

typedef enum
{
	XUI_WIN_CANVAS,
	XUI_WIN_BUTTON,
	XUI_WIN_GIF,
	XUI_WIN_SIGBOARD
} XuiWindowType;

typedef enum
{
	XUI_BTN_NORMAL,
	XUI_BTN_PRESSED
} XuiButtonStatType;

typedef enum
{
	XUI_BG_NORMAL = 0,
	XUI_BG_TILE,
	XUI_BG_CENTER,
	XUI_BG_FOUR_CORNER
} XuiBgStyle;

typedef enum
{
	XUI_BMP_PERPIX_1 = 1,
	XUI_BMP_PERPIX_24 = 24
} XuiBmpBpp;

typedef enum
{
	XUI_SHOW_NORMAL = 0,
	XUI_SHOW_MIRROR,
	XUI_SHOW_ALL
} XuiShowMode;

typedef enum
{
	XUI_TRANSLATION = 0,
	XUI_POLL,
	XUI_SCALE
} XuiAnimationType;

struct _XuiImg {
	unsigned int width;
	unsigned int height;
	void *priv;
};
typedef struct _XuiImg XuiImg;

struct _XuiFont {
	char *fontfile;
	int index;
	int cidx;
	int mono;
	void *priv;
};
typedef struct _XuiFont XuiFont;

typedef enum{
	XUI_FONT_GREY,
	XUI_FONT_MONO
}XuiFontSet;

typedef enum{
	XUI_TEXT_NORMAL,
	XUI_TEXT_BOLD,
	XUI_TEXT_ITALIC,
	XUI_TEXT_BOLD_ITALIC
}XuiTextStyle;

typedef enum{
	XUI_SIG_FLAT,
	XUI_SIG_NORMAL
}XuiSigPenFlat;

struct _XuiWindow {
	unsigned int width;
	unsigned int height;
	void *widget;
	XuiWindowType type;
	int key;
};
typedef struct _XuiWindow XuiWindow;

struct _XuiButtonStat {
	int btn_round;
	XuiColor btn_bg;
	char *text;
	XuiColor text_fg;
	XuiFont  *text_font;
	int text_x;
	int text_y;
	int text_height;
	XuiImg *img;
	int img_x;
	int img_y;
	XuiBgStyle img_style;
};
typedef struct _XuiButtonStat XuiButtonStat;

struct _XuiSigBoardStat{
	int btn_round;
	XuiColor btn_bg;
	XuiColor text_fg;
	XuiFont *text_font;
	int text_x;
	int text_y;
	int text_height;
	XuiBgStyle img_style;
	XuiImg* img;
	int img_x;
	int img_y;
	char *text;
	XuiColor pen_fg;
	int pen_width;
	int pen_flat;
} ;
typedef struct _XuiSigBoardStat XuiSigBoardStat;

typedef struct _XUI_SIGN_POINT
{
	unsigned short x;
	unsigned short y;
} XuiSignPoint;

#define XUI_SIGN_MAX_ARRAY 		(76800)	/* 320*240=76800 */

typedef struct _XUI_SIGN_DATA
{
	/* OUT array of point */
	XuiSignPoint point_array[XUI_SIGN_MAX_ARRAY];
	/* OUT valid point count */
	unsigned int point_len;
} XuiSignData;

struct _XuiImeAttr{
	XuiColor ime_bg;
	XuiColor text_fg;
	XuiColor focus_fg;
	XuiFont *text_font;
	unsigned int 	 text_size;
	XuiImg* img;
	XuiWindow *parent;
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	int alpha_key;
	int sharp_key;
};
typedef struct _XuiImeAttr XuiImeAttr;

struct _XuiGetStrAttr{
	XuiWindow *parent;
	XuiColor fg;
	unsigned int x;
	unsigned int y;
	XuiFont *font;
	unsigned int size;
	int alpha_key;
};
typedef struct _XuiGetStrAttr XuiGetStrAttr;


typedef enum{
	XUI_GESTURE_FLINGLEFT,
	XUI_GESTURE_FLINGRIGHT,
	XUI_GESTURE_FLINGUP,
	XUI_GESTURE_FLINGDOWN,
	XUI_GESTURE_FLINGZOOMOUT,
	XUI_GESTURE_FLINGZOOMIN,

	XUI_GESTURE_SCROLLLEFT,
	XUI_GESTURE_SCROLLRIGHT,
	XUI_GESTURE_SCROLLUP,
	XUI_GESTURE_SCROLLDOWN,
	XUI_GESTURE_SCROLLZOOMOUT,
	XUI_GESTURE_SCROLLZOOMIN,
	
	XUI_GESTURE_CLICKDOWN,
	XUI_GESTURE_CLICKUP,
}XuiGestureType;

typedef struct {
	XuiGestureType type;
    float velocity;
    float distance;
	int down_x;
	int down_y;
	int cur_x;
	int cur_y;
  } XuiGesture;

#define XUI_RIGHT_X(_x, _width,  _extend) (((_x) + (_width) - (_extend))>0?((_x) + (_width) - (_extend)):(_x))
#define XUI_CENTER_X(_x, _width,  _extend) ((_x) + (((_width) - (_extend)) >> 1))
#define XUI_CENTER_Y(_y, _height, _extend) ((_y) + (((_height) - (_extend)) >> 1))

int 		XuiOpen					(int argc, char **argv);
int			XuiSuspend				(void);
int			XuiResume				(void);
int 		XuiIsRunning			(void);
void 		XuiClose				(void);
int 		XuiHasKey				(void);
int 		XuiGetKey				(void);
void 		XuiClearKey				(void);
XuiWindow*	XuiRootCanvas			(void);
XuiWindow*	XuiStatusbarCanvas		(void);
XuiFont *	XuiCreateFont			(char *fontfile, int index, XuiFontSet fontset);
void 		XuiDestroyFont			(XuiFont *font);
int 		XuiCanvasDrawText		(XuiWindow *window, unsigned int x, unsigned int y, unsigned int height,XuiFont *font, XuiTextStyle textstyle, XuiColor fg, const char *text);
int 		XuiCanvasDrawImg		(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, XuiBgStyle bgstyle, XuiImg *img);
int 		XuiCanvasDrawRect		(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, unsigned int height,  XuiColor fg, int round, int fill);
int 		XuiClearArea			(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
int 		XuiTextWidth			(XuiFont   *font,   int size, const char *text);
int 		XuiTextWidthEx			(XuiFont   *font,   int size, XuiTextStyle textstyle, const char *text);
XuiWindow*	XuiCreateCanvas			(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
XuiWindow*  XuiCreateCanvasEx		(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int vh);
void 		XuiCanvasMoveToY		(XuiWindow * window, unsigned int my);
void 		XuiDestroyWindow		(XuiWindow *window);
void 		XuiShowWindow			(XuiWindow *window, int show, int flag);
int 		XuiShowSoftKeyboard		(int type, int show);
void 		XuiCanvasSetBackground	(XuiWindow *window, XuiBgStyle bgstyle, XuiImg *img, XuiColor bg);
XuiWindow*	XuiCreateButton			(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
int 		XuiButtonSetStat		(XuiWindow *window, XuiButtonStatType type, XuiButtonStat *stat);
int 		XuiButtonSetKey			(XuiWindow *window, int key);
XuiWindow*	XuiCreateSignatureBoard	(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
int 		XuiSigBoardSetStat		(XuiWindow *window, XuiSigBoardStat *stat);
XuiImg*		XuiSigBoardImg			(XuiWindow *window);
XuiSignData*XuiSigBoardSignData		(XuiWindow *window);
XuiImg*		XuiCaptureScreen		(void);
XuiImg*		XuiCaptureCanvas		(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
XuiImg*		XuiImgLoadFromFile		(const char *file);
XuiImg*		XuiImgLoadFromMem		(unsigned char *address, unsigned long length, int type);
XuiImg*  	XuiImgCompose			(XuiImg* img1,  XuiImg* img2, unsigned int rate1, unsigned int rate2, int type);
int 		XuiImgSaveToFile		(XuiImg *img, const char *file);
void 		XuiImgFree				(XuiImg *img);
int 		XuiImgToRgba			(XuiImg *img, char *rgba);
XuiImg*  	XuiRgbaToImg			(char *rgba, unsigned int len, unsigned int width, unsigned int height);
int 		XuiImgTransform			(XuiImg *img, XuiTransform transform);
int		 	XuiImgResize			(XuiImg *img, unsigned int width, unsigned int height);
XuiWindow*	XuiCreateGif			(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char* path);

int 		XuiSetStatusbarIcon		(int index, const char* path);
int 		XuiGetHzString			(XuiImeAttr attr, char *outstr,unsigned int maxlen, unsigned int timeout);
int 		XuiGetString			(XuiGetStrAttr attr, char *outstr,unsigned char mode,unsigned int minlen,unsigned int maxlen);
char * 		XuiBidiStrdup			(const char *str);
int 		XuiCanvasAnimation		(XuiWindow *front, XuiWindow *background,unsigned int front_rate, unsigned int background_rate, int type);
int 		XuiGetGesture			(XuiGesture* gesture);
int 		XuiSetGestureRect		(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
void 		XuiClearGesture			(void);


#ifdef __cplusplus
}
#endif

#endif
