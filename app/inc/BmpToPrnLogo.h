
/****************************************************************************
鎵撳嵃BMP鍥剧墖锛歱rint bmp

鐧惧瘜璁＄畻鏈烘妧鏈�(娣卞湷)鏈夐檺鍏徃

淇敼鍘嗗彶锛�
1.0.0 2011骞�鏈�鏃�    浠诲涔�

****************************************************************************/

#ifndef _BMPTOPRNLOGO_H
#define _BMPTOPRNLOGO_H

#define DATA_ERROR1  1
#define DATA_ERROR2  2
#define DATA_ERROR3  3
#define DATA_ERROR4  4 
#ifndef NULL
typedef ((void *) 0) NULL;
#endif

struct PAX_PRN
{
	unsigned char  count[2];
	unsigned char  content[384];
} Pax_prn;

struct PAX_LOGO
{
	unsigned char  line;
	struct PAX_PRN  prn_txt[255];
} Pax_logo;

/*鏂囦欢澶寸粨鏋�*/
typedef struct tagBITMAPFILEHEADER
{
	unsigned short bfType;			/*鏂囦欢绫诲瀷锛孊M涓築MP*/
	unsigned long bfSize;           /*鏂囦欢鐨勫ぇ灏�(瀛楄妭)*/
	unsigned short bfReserved1;     /*淇濈暀*/
	unsigned short bfReserved2;     /*淇濈暀*/
	unsigned long bfOffBits;        /*鏈粨鏋勫紑濮嬪鑷冲浘褰㈤樀鍒楃殑鍋忕Щ瀛楄妭鏁�*/
}BITMAPFILEHEADER;

/*鏂囦欢淇℃伅澶寸粨鏋�*/
typedef struct tagBITMAPINFOHEADER
{
	unsigned long	biSize;           /*鏈粨鏋勭殑瀛楄妭鏁�*/
	unsigned long	biWidth;          /*浠ュ儚绱犳暟琛ㄧず鍥惧舰鐨勫搴�*/
	unsigned long	biHeight;         /*浠ュ儚绱犳暟琛ㄧず鍥惧舰鐨勯珮搴�*/
	unsigned short  biPlanes;         /*鑹插僵骞抽潰鏁帮紝蹇呴』涓�/
	unsigned short  biBitCount;       /*涓�涓儚绱犳墍鍗犵殑浣嶆暟锛屽彲浠ユ槸1銆���*/
	unsigned long	biCompression;    /*鍥惧舰鍘嬬缉褰㈠紡锛�轰笉鍘嬬缉*/
	unsigned long	biSizeImage;      /*鍥惧舰鐨勫瓧鑺傛暟*/
	unsigned long	biXPelsperMeter;  /*1绫冲唴鐨勬按骞冲儚绱犳暟*/
	unsigned long	biYPelsPerMeter;  /*1绫冲唴鐨勫瀭鐩村儚绱犳暟*/
	unsigned long	biClrUsed;        /*鍥惧舰涓疄闄呯敤鍒扮殑鍍忕礌鏁�*/
	unsigned long	biClrlmportant;   /*鍥惧舰涓殑閲嶈鐨勯鑹叉暟*/
}BITMAPINFOHEADER;

/*棰滆壊*/
typedef struct tagrgb
{
	unsigned char B;   
	unsigned char G;   
	unsigned char R;   
}RGBDATA;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
	void conv_bmp(unsigned char *dest, unsigned char *src, int len, int jplen, unsigned char off);
	int BmpToPrnLogo(unsigned char *filename,int mode,unsigned char *gMallocBuffer);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* _BMPTOPRNLOGO_H */
