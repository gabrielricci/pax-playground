#include "fcntl.h"
#include "unistd.h"
#include "changebmp.h"
#include "errno.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "osal.h"
#include <stdio.h>
#include <stdlib.h>

/********************** Internal macros declaration ************************/


#define NULL ((void *) 0)
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
int changebmp(char *changefilename, char *greyfilename);
int changebmp1(char *changefilename, char *greyfilename);
/********************** Internal variables declaration *********************/
/********************** external reference declaration *********************/
//extern int ConVerTo8BitGray(char *);
extern int ConVerToMono(char *filepath);
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

int Change24BmpToGrey(char *changefilename, char *greyfilename)
{
	int iRet;

	iRet = changebmp1(changefilename, greyfilename);
	//iRet = ConVerTo8BitGray(greyfilename);
	if(iRet != 0)
	{
		return iRet;
	}

	return 0;
}

//杞崄1�7浣嶇殑BMP鏂囦欢涓哄崟鑹叉枃浠ￄ1�7
int changebmp(char *changefilename, char *greyfilename)
{	
	int opfile, wrfile, iRet, iLen;
	int iWidth, iHeight;
	char cWrFileBuf[20], cTempBuf[20];
	BITMAP_INFO_HEADER opBitmapInfoHead, wrBitmapInfoHead;
	RGBQUAD rgbquad[2];

	int k, i, j, m;
	int rgb, pitchcount;
	unsigned char r, g, b;
	unsigned char gray;
	char writein;

	//open file
	opfile = open(changefilename, O_RDONLY);
	if(opfile < 0)
	{
		close(opfile);
		return 1;
	}

	//娉ㄦ剰�1�7�楄妭�1�7�归綈鐨勯棶棰�
	memset(cTempBuf, 0, sizeof(cTempBuf));
	iRet = read(opfile, cTempBuf, FILE_HEADER_LEN);
	if(iRet < 0 || iRet != FILE_HEADER_LEN)
	{
		close(opfile);
		return 2;
	}

	//鍒ゆ柇鍥惧儚鏂囦欢澶ￄ1�7
	if(!(cTempBuf[0]=='B' && cTempBuf[1]=='M'))
	{
		close(opfile);
		return  3;
	}

	memset(&opBitmapInfoHead, 0, INFO_HEADER_LEN);
	iRet = read(opfile, &opBitmapInfoHead, INFO_HEADER_LEN);
	if(iRet < 0 || iRet != INFO_HEADER_LEN)
	{
		close(opfile);
		return 2;
	}

	//浣嶅浘�1�7�藉宄1�7
	iWidth = opBitmapInfoHead.biWidth;
	if(iWidth > 384)
	{
		close(opfile);
		return  5;
	}
	
	//浣嶅浘楂樺害
	iHeight = opBitmapInfoHead.biHeight;

	//write file header
	memset(cWrFileBuf, 0, sizeof(cWrFileBuf));
	//	bfType
	cWrFileBuf[0] = 0x42;
	cWrFileBuf[1] = 0x4D;

	//bfSize
	iLen = ((iWidth*1 + 31) >> 5) << 2;	//鍗曡壊biBitCount=1
	iLen *= iHeight;
	iLen += FILE_HEADER_LEN + INFO_HEADER_LEN + (2*RGBQUAD_LEN);
	memcpy(cWrFileBuf+2, &iLen, 4);

	//bfReserved1&bfReserved2
	cWrFileBuf[6] = 0x00;
	cWrFileBuf[7] = 0x00;
	cWrFileBuf[8] = 0x00;
	cWrFileBuf[9] = 0x00;

	//bfOffBits
	iLen = FILE_HEADER_LEN + INFO_HEADER_LEN + (2*RGBQUAD_LEN);
	memcpy(cWrFileBuf+9, &iLen, 4);

	wrfile = open(greyfilename, O_WRONLY | O_CREAT | O_TRUNC,  S_IREAD|S_IWRITE);
	if(wrfile < 0)
	{
		close(wrfile);
		return 1;
	}
	iRet = write(wrfile, cWrFileBuf, FILE_HEADER_LEN);
	if(iRet < 0 || iRet != FILE_HEADER_LEN)
	{
		close(wrfile);
		return 4;
	}

	//write info header
	wrBitmapInfoHead.biSize		= INFO_HEADER_LEN;
	wrBitmapInfoHead.biWidth	= opBitmapInfoHead.biWidth;
	wrBitmapInfoHead.biHeight	= opBitmapInfoHead.biHeight;
	wrBitmapInfoHead.biPlanes	= 1;
	wrBitmapInfoHead.biBitCount = 1;
	wrBitmapInfoHead.biCompression	 = 0;
	iLen = ((iWidth*1 + 31) >> 5) << 2;	//鍗曡壊biBitCount=1
	iLen *= iHeight;
	wrBitmapInfoHead.biSizeImage	 = iLen;
	wrBitmapInfoHead.biXPelsPerMeter = opBitmapInfoHead.biXPelsPerMeter;
	wrBitmapInfoHead.biYPelsPerMeter = opBitmapInfoHead.biYPelsPerMeter;
	wrBitmapInfoHead.biClrUsed		 = 0;
	wrBitmapInfoHead.biClrImportant  = opBitmapInfoHead.biClrImportant;

	iRet = write(wrfile, &wrBitmapInfoHead, sizeof(BITMAP_INFO_HEADER));
	if(iRet < 0 || iRet != sizeof(BITMAP_INFO_HEADER))
	{
		close(wrfile);
		return 4;
	}

	//棰滆壊琛�璁剧疆浣嶅浘涓殑棰滆壄1�7
	for(i=0; i<2; i++)
    {
        rgbquad[i].rgbBlue     = i? 0xFF:0;
        rgbquad[i].rgbGreen    = i? 0xFF:0;
        rgbquad[i].rgbRed      = i? 0xFF:0;
        rgbquad[i].rgbReserved = 0;
    }
	iRet = write(wrfile, rgbquad, (2*sizeof(RGBQUAD)));
	if(iRet < 0 || iRet != (2*sizeof(RGBQUAD)))
	{
		close(wrfile);
		return 4;
	}

	//浣嶅浘闃靛垪
	for(i = 0; i < iHeight; i++)
	{
		writein = 0;
		pitchcount = 0;
		k = 0;

		for (j = 0; j < iWidth; j++)
		{
			iRet = read(opfile, &rgb, 3);
			if(iRet < 0)
			{
				close(opfile);
				return 2;
			}
			if(iRet < 3)
			{
				break;
			}

           	// rgb: 0x00bbggrr
           	b = (unsigned char)((rgb)>>16);
           	g = (unsigned char)((rgb)>>8);
           	r = (unsigned char)(rgb);

           	gray = (unsigned char)(( 77 * r + 151 * g + 28 * b) >> 8); // 24浣嶈浄1�7�嶆牳蹇冪畻娉�
			if (gray >= 128)   //鐧界偄1�7
			{
				writein |= 1;
			}
				
			++k;
				
			if (k == 8)
			{
				pitchcount++;    //鏈夋晥琛屾暟鎹瓧鑺傛�鏁�1
				iRet = write(wrfile, &writein, 1);
				if(iRet < 0 || iRet != 1)
				{
					close(wrfile);
					return 4;
				}
				writein = 0;
				k = 0;
			}

			writein <<= 1;       //鍙崇Щ1浣�
		}

		if (k != 0)              //涓嶈冻涓�釜瀛楄妭鐢�濉厖鍓╀綄1�7
		{
			writein <<= (8-k);
			iRet = write(wrfile, &writein, 1);
			if(iRet < 0 || iRet != 1)
			{
				close(wrfile);
				return 4;
			}
			pitchcount++;
		}
			
		if (pitchcount%4 != 0)  //4瀛楄妭�1�7�归綄1�7
		{
			writein = 0;
			for (m = 0; m < 4-pitchcount%4; m++)
			{
				iRet = write(wrfile, &writein, 1);
				if(iRet < 0 || iRet != 1)
				{
					close(wrfile);
					return 4;
				}
			}
		}
	}
		
	close(opfile);
	close(wrfile);

	return 0;
}


int GetFileSize(const char *pszFile)
{
	int iRet = -1;
	FILE *fp;
	fp = fopen(pszFile, "r");
	if(NULL == fp)
	{
		return -1;
	}

	fseek(fp, 0L, SEEK_END);
	iRet = ftell(fp);
	fclose(fp);

	return iRet;
}

int changebmp1(char *changefilename, char *greyfilename)
{
	int opfile, wrfile, iRet, iLen;
	int iWidth, iHeight;
	char cWrFileBuf[20], cTempBuf[20];
	BITMAP_INFO_HEADER opBitmapInfoHead, wrBitmapInfoHead;
	RGBQUAD rgbquad[2];

	int k, i, j, m;
	int rgb, pitchcount;
	unsigned char r, g, b;
	unsigned char gray;
	char writein;

	int opReadCount=0,opWriteCount=0,wrReadCount=0,wrWriteCount=0;

	int opFileSize=0,wrWriteSize=0;

	char opFileBuf[256*1024]={0}, wrFileBuf[256*1024]={0};

	//获取原始图片文件的大射1�7
	opFileSize = GetFileSize(changefilename);
	OsLog(LOG_DEBUG,"-----bmpfile size = %d",opFileSize);

	//open file 打开24位原始图片文仄1�7
	opfile = open(changefilename, O_RDONLY);
	if(opfile < 0)
	{
		OsLog(LOG_DEBUG,"-----opfile = %d",opfile);
		close(opfile);
		return 1;
	}

	//射1�7位原始图片文仄1�7 读入到buf丄1�7
	memset(opFileBuf, 0, sizeof(opFileBuf));
	iRet = read(opfile, opFileBuf, opFileSize);
	if(iRet < 0 || iRet != opFileSize)
	{
		close(opfile);
		return 2;
	}
	opReadCount=0;

	//读取图片文件头，FILE_HEADER_LEN 长度14
	memset(cTempBuf, 0, sizeof(cTempBuf));
/*
	iRet = read(opfile, cTempBuf, FILE_HEADER_LEN);
	if(iRet < 0 || iRet != FILE_HEADER_LEN)
	{
		LOGI("read opFileHead error!\n");
		close(opfile);
		return 2;
	}
*/
	memcpy(cTempBuf, opFileBuf+opReadCount, FILE_HEADER_LEN);
	opReadCount+=FILE_HEADER_LEN;


	if(!(cTempBuf[0]=='B' && cTempBuf[1]=='M'))
	{
		close(opfile);
		return  3;
	}

	//读取图片文件头信息，INFO_HEADER_LEN 40
	memset(&opBitmapInfoHead, 0, INFO_HEADER_LEN);
/*
	iRet = read(opfile, &opBitmapInfoHead, INFO_HEADER_LEN);
	if(iRet < 0 || iRet != INFO_HEADER_LEN)
	{
		LOGI("read opBitmapInfoHead error!\n");
		close(opfile);
		return 2;
	}
*/
	memcpy(&opBitmapInfoHead, opFileBuf+opReadCount, INFO_HEADER_LEN);
	opReadCount+=INFO_HEADER_LEN;

	//读取原始图片文件 宽度
	iWidth = opBitmapInfoHead.biWidth;
	if(iWidth > 384)
	{
		close(opfile);
		return  5;
	}

	//读取原始图片文件 高度
	iHeight = opBitmapInfoHead.biHeight;


	//write file header
	memset(cWrFileBuf, 0, sizeof(cWrFileBuf));
	//	bfType
	cWrFileBuf[0] = 0x42; //B
	cWrFileBuf[1] = 0x4D; //M

	//bfSize
	iLen = ((iWidth*1 + 31) >> 5) << 2;	//鍗曡壊biBitCount=1
	iLen *= iHeight;
	iLen += FILE_HEADER_LEN + INFO_HEADER_LEN + (2*RGBQUAD_LEN);
	memcpy(cWrFileBuf+2, &iLen, 4);

	//bfReserved1&bfReserved2
	cWrFileBuf[6] = 0x00;
	cWrFileBuf[7] = 0x00;
	cWrFileBuf[8] = 0x00;
	cWrFileBuf[9] = 0x00;

	//bfOffBits
	iLen = FILE_HEADER_LEN + INFO_HEADER_LEN + (2*RGBQUAD_LEN);
	memcpy(cWrFileBuf+9, &iLen, 4);

	//创建目标黑白图片文件
/*
	wrfile = open(greyfilename, O_WRONLY | O_CREAT | O_TRUNC,  S_IREAD|S_IWRITE);
	if(wrfile < 0)
	{
		LOGI("File can not creat!\n");
		close(wrfile);
		return 1;
	}

	//对入目标黑白图片 写入 文件头信恄1�7
	iRet = write(wrfile, cWrFileBuf, FILE_HEADER_LEN);
	if(iRet < 0 || iRet != FILE_HEADER_LEN)
	{
		LOGI("write wrFileHead error!\n");
		LOGI("iRet:%d, errno=%d\n", iRet, errno);
		close(wrfile);
		return 4;
	}
*/
	memcpy(wrFileBuf+wrWriteCount, cWrFileBuf, FILE_HEADER_LEN);
	wrWriteCount+=FILE_HEADER_LEN;


	//准备目标黑白文件 文件信息头数捄1�7
	//write info header
	wrBitmapInfoHead.biSize		= INFO_HEADER_LEN;
	wrBitmapInfoHead.biWidth	= opBitmapInfoHead.biWidth;
	wrBitmapInfoHead.biHeight	= opBitmapInfoHead.biHeight;
	wrBitmapInfoHead.biPlanes	= 1;
	wrBitmapInfoHead.biBitCount = 1;
	wrBitmapInfoHead.biCompression	 = 0;
	iLen = ((iWidth*1 + 31) >> 5) << 2;	//鍗曡壊biBitCount=1
	iLen *= iHeight;
	wrBitmapInfoHead.biSizeImage	 = iLen;
	wrBitmapInfoHead.biXPelsPerMeter = opBitmapInfoHead.biXPelsPerMeter;
	wrBitmapInfoHead.biYPelsPerMeter = opBitmapInfoHead.biYPelsPerMeter;
	wrBitmapInfoHead.biClrUsed		 = 0;
	wrBitmapInfoHead.biClrImportant  = opBitmapInfoHead.biClrImportant;

	//对目标黑白图牄1�7 写入 文件信息头数捄1�7
/*
	iRet = write(wrfile, &wrBitmapInfoHead, sizeof(BITMAP_INFO_HEADER));
	if(iRet < 0 || iRet != sizeof(BITMAP_INFO_HEADER))
	{
		LOGI("write wrBitmapInfoHead error!\n");
		LOGI("iRet:%d, errno=%d\n", iRet, errno);
		close(wrfile);
		return 4;
	}
*/
	memcpy(wrFileBuf+wrWriteCount, &wrBitmapInfoHead, sizeof(BITMAP_INFO_HEADER));
	wrWriteCount+=sizeof(BITMAP_INFO_HEADER);

	//对目标黑白图牄1�7 写入 调色板数捄1�7
	for(i=0; i<2; i++)
    {
        rgbquad[i].rgbBlue     = i? 0xFF:0;
        rgbquad[i].rgbGreen    = i? 0xFF:0;
        rgbquad[i].rgbRed      = i? 0xFF:0;
        rgbquad[i].rgbReserved = 0;
    }

  /*
	iRet = write(wrfile, rgbquad, (2*sizeof(RGBQUAD)));
	if(iRet < 0 || iRet != (2*sizeof(RGBQUAD)))
	{
		LOGI("write rgbquad error!\n");
		LOGI("iRet:%d, errno=%d\n", iRet, errno);
		close(wrfile);
		return 4;
	}
	*/
	memcpy(wrFileBuf+wrWriteCount, rgbquad, (2*sizeof(RGBQUAD)));
	wrWriteCount+=(2*sizeof(RGBQUAD));

	for(i = 0; i < iHeight; i++)
	{
		writein = 0;
		pitchcount = 0;
		k = 0;

		//对目标黑白图牄1�7 分行 写入 位图数据信息
		for (j = 0; j < iWidth; j++)
		{
			/*
			iRet = read(opfile, &rgb, 3); //24佄1�7 图片 用三个字节表示一个像素点信息
			if(iRet < 0)
			{
				LOGI("read rgb error!\n");
				LOGI("iRet:%d, errno=%d\n", iRet, errno);
				close(opfile);
				return 2;
			}
			if(iRet < 3)
			{
				break;
			}
			*/
			iRet = opFileSize - opReadCount;
			if (iRet>=3)
			{
				memcpy(&rgb, opFileBuf+opReadCount, 3);
				opReadCount+=3;
			}
			else
			{
				memcpy(&rgb, opFileBuf+opReadCount, iRet);
				opReadCount+=iRet;
			}
			if(iRet<3)
			{
				break;
			}


			//根据图像的颜色将24位转换为单色，黑白图僄1�7�表示一个像素，非黑即白，非1卄1�7
           	// rgb: 0x00bbggrr
           	b = (unsigned char)((rgb)>>16);
           	g = (unsigned char)((rgb)>>8);
           	r = (unsigned char)(rgb);

           	gray = (unsigned char)(( 77 * r + 151 * g + 28 * b) >> 8); // 24浣嶈浄1�7�嶆牳蹇冪畻娄1�7
			if (gray >= 128)
			{
				writein |= 1; //如果是黑色，当前像素位，也就是最低位.
			}

			++k; //黑白图像 像素位计数器

			//当位数到辄1�7�，记为丄1�7个字芄1�7
			if (k == 8)
			{
				pitchcount++;
				/*
				iRet = write(wrfile, &writein, 1);
				if(iRet < 0 || iRet != 1)
				{
					LOGI("write writein error!\n");
					LOGI("iRet:%d, errno=%d\n", iRet, errno);
					close(wrfile);
					return 4;
				}
				*/
				memcpy(wrFileBuf+wrWriteCount, &writein, 1);
				wrWriteCount+=1;

				writein = 0;
				k = 0;
			}
			//写入
			writein <<= 1;
		}

		if (k != 0)
		{
			writein <<= (8-k);
			/*
			iRet = write(wrfile, &writein, 1);
			if(iRet < 0 || iRet != 1)
			{
				LOGI("write writein error!\n");
				LOGI("iRet:%d, errno=%d\n", iRet, errno);
				close(wrfile);
				return 4;
			}
			*/
			memcpy(wrFileBuf+wrWriteCount, &writein, 1);
			wrWriteCount+=1;

			pitchcount++;
		}

		//系统昄1�7�节对齐，如果发现不对齐，则补足够整附1�7�字节数
		if (pitchcount%4 != 0)  //4
		{
			writein = 0;
			for (m = 0; m < 4-pitchcount%4; m++)
			{
				/*
				iRet = write(wrfile, &writein, 1);
				if(iRet < 0 || iRet != 1)
				{
					LOGI("write writein error!\n");
					LOGI("iRet:%d, errno=%d\n", iRet, errno);
					close(wrfile);
					return 4;
				}
				*/
				memcpy(wrFileBuf+wrWriteCount, &writein, 1);
				wrWriteCount+=1;
			}
		}
	}

	wrfile = open(greyfilename, O_WRONLY | O_CREAT | O_TRUNC,  S_IREAD|S_IWRITE);
	if(wrfile < 0)
	{
		close(wrfile);
		return 1;
	}

	//对入目标黑白图片 写入 文件头信恄1�7
	iRet = write(wrfile, wrFileBuf, wrWriteCount);
	if(iRet < 0 || iRet != wrWriteCount)
	{
		close(wrfile);
		return 4;
	}

	close(opfile);
	close(wrfile);

	return 0;
}


// end of file

