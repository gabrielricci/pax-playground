
/****************************************************************************
打印BMP图片：print bmp

百富计算机技朄1�7(深圳)有限公司

修改历史＄1�7
1.0.0 2011幄1�7朄1�7旄1�7    任家乄1�7

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

/*文件头结构1�7*/
typedef struct tagBITMAPFILEHEADER
{
	unsigned short bfType;			/*文件类型，BM为BMP*/
	unsigned long bfSize;           /*文件的大射1�7(字节)*/
	unsigned short bfReserved1;     /*保留*/
	unsigned short bfReserved2;     /*保留*/
	unsigned long bfOffBits;        /*本结构开始处至图形阵列的偏移字节敄1�7*/
}BITMAPFILEHEADER;

/*文件信息头结构1�7*/
typedef struct tagBITMAPINFOHEADER
{
	unsigned long	biSize;           /*本结构的字节敄1�7*/
	unsigned long	biWidth;          /*以像素数表示图形的宽庄1�7*/
	unsigned long	biHeight;         /*以像素数表示图形的高庄1�7*/
	unsigned short  biPlanes;         /*色彩平面数，必须丄1�7/
	unsigned short  biBitCount;       /*丄1�7个像素所占的位数，可以是1〄1�7�1�7�1�7*/
	unsigned long	biCompression;    /*图形压缩形式＄1�7�不压缩*/
	unsigned long	biSizeImage;      /*图形的字节数*/
	unsigned long	biXPelsperMeter;  /*1米内的水平像素数*/
	unsigned long	biYPelsPerMeter;  /*1米内的垂直像素数*/
	unsigned long	biClrUsed;        /*图形中实际用到的像素敄1�7*/
	unsigned long	biClrlmportant;   /*图形中的重要的颜色数*/
}BITMAPINFOHEADER;

/*颜色*/
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
