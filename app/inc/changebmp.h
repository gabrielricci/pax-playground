/*
 ============================================================================
 Name        : changebmp.h
 Author      : Rex
 Version     : 1.0.0
 Copyright   : PAX Computer Technology(Shenzhen) CO., LTD
 Description : EMV transaction
 History     :
 1.0.0 Nov 06, 2012 Rex
 ============================================================================
 */

#ifndef _CHANGEBMP_H
#define _CHANGEBMP_H

/*************************************************************************************
仄1�7.BMP为扩展名的图片由以下三个部分组成:

1.位图文件处1�7(BITMAPFILEHEADER) 数据结构
2.位图信息(BITMAPINFO)数据结构
3.位图阵列

当biBitCount=1时，8个像素占1个字芄1�7
当biBitCount=4时，2个像素占1个字芄1�7
当biBitCount=8时，1个像素占1个字芄1�7
当biBitCount=24旄1�7,1个像素占3个字芄1�7
当biBitCount=32旄1�7,1个像素占4个字芄1�7

注意:存在字节对齐的问预1�7
*************************************************************************************/

typedef struct tagBITMAP_FILE_HEADER
{
	unsigned short 	bfType;	//表示文件的类型，该�1�7�必霄1�7昄1�74D42，也就是字符'BM'
	unsigned int	bfSize;	//表示该位图文件的大小，用字节为单佄1�7
	unsigned short 	bfReserved1;//保留，必须设置为0
	unsigned short 	bfReserved2;//保留，必须设置为0
	unsigned int 	bfOffBits;	//表示从文件头弄1�7始到实际的图象数据之间的字节的偏移量。这个参数是非常有用的，因为位图信息头和调色板的长度会根据不同情况�1�7�变化，扄1�7以你可以用这个偏移�1�7�迅速的从文件中读取到位数据
} BITMAP_FILE_HEADER;

#define FILE_HEADER_LEN 14

typedef struct tagBITMAP_INFO_HEADER
{
	unsigned int biSize;		//表示BITMAPINFOHEADER结构扄1�7霄1�7要的字节敄1�7
	unsigned int biWidth;		//表示图象的宽度，以象素为单位
	unsigned int biHeight;		//表示图象的高度，以象素为单位,并�1�7�向正向的标忄1�7
	unsigned short biPlanes;	//为目标设备说明位面数，其值将总是被设丄1�7
	unsigned short biBitCount;	//表示比特敄1�7/象素，其值为1〄1�7�1�7�1�7〄1�7、或32
	unsigned int biCompression;	//表示图象数据压缩的类垄1�7
	unsigned int biSizeImage;	//表示图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0
	unsigned int biXPelsPerMeter;//表示水平分辨率，用象約1�7/米表礄1�7
	unsigned int biYPelsPerMeter;//表示垂直分辨率，用象約1�7/米表礄1�7
	unsigned int biClrUsed;		 //表示位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用扄1�7有调色板项）
	unsigned int biClrImportant; //表示对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要
} BITMAP_INFO_HEADER;

#define INFO_HEADER_LEN 40

//bimColor[]是一个颜色表,用于说明位图中的颜色.它有若干个表顄1�7,每一表项是一个RGBQUAD类型的结构1�7,定义了一种颜艄1�7.RGBQUAD的定乄1�7
//颜色表用于说明位图中的颜艄1�7,真彩色图＄1�7位的BMP）就不需要颜色表，因为位图中的RGB值就代表了每个象素的颜色。但昄1�7位r5g6b5位域彩色图像霄1�7要颜色表
typedef struct tagRGBQUAD
{
	unsigned char rgbBlue;	//值范围为0-255
	unsigned char rgbGreen;	//值范围为0-255
	unsigned char rgbRed;	//值范围为0-255
	unsigned char rgbReserved;	//保留，必须为0
}RGBQUAD;

#define RGBQUAD_LEN 4

//位图信息数据结构含有位图文件的尺寸和颜色等信恄1�7
typedef struct tagBITMAPINFO
{
	BITMAP_INFO_HEADER bmiHeader;
	RGBQUAD bmiColor[1];
}BITMAP_INFO;

//文件信息头和位图信息组成位图文件，BITMAPFILE结构定义如下
typedef struct tagBITMAP
{
	BITMAP_FILE_HEADER bfHeader;
	BITMAP_INFO biInfo;
}BITMAP_FILE;


#ifdef __cplusplus
extern "C"
{
#endif

	int Change24BmpToGrey(char *changefilename, char *greyfilename);

#ifdef __cplusplus
};
#endif
#endif /* _CHANGEBMP_H */

