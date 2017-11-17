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

//鏉烆剚宕�娴ｅ秶娈態MP閺傚洣娆㈡稉鍝勫礋閼瑰弶鏋冩禒锟�
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

	//濞夈劍鍓扮�涙濡�靛綊缍堥惃鍕６妫帮拷
	memset(cTempBuf, 0, sizeof(cTempBuf));
	iRet = read(opfile, cTempBuf, FILE_HEADER_LEN);
	if(iRet < 0 || iRet != FILE_HEADER_LEN)
	{
		close(opfile);
		return 2;
	}

	//閸掋倖鏌囬崶鎯у剼閺傚洣娆㈡径锟�
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

	//娴ｅ秴娴樼�硅棄瀹�
	iWidth = opBitmapInfoHead.biWidth;
	if(iWidth > 384)
	{
		close(opfile);
		return  5;
	}
	
	//娴ｅ秴娴樻妯哄
	iHeight = opBitmapInfoHead.biHeight;

	//write file header
	memset(cWrFileBuf, 0, sizeof(cWrFileBuf));
	//	bfType
	cWrFileBuf[0] = 0x42;
	cWrFileBuf[1] = 0x4D;

	//bfSize
	iLen = ((iWidth*1 + 31) >> 5) << 2;	//閸楁洝澹奲iBitCount=1
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
	iLen = ((iWidth*1 + 31) >> 5) << 2;	//閸楁洝澹奲iBitCount=1
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

	//妫版粏澹婄悰锟界拋鍓х枂娴ｅ秴娴樻稉顓犳畱妫版粏澹�
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

	//娴ｅ秴娴橀梼闈涘灙
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

           	gray = (unsigned char)(( 77 * r + 151 * g + 28 * b) >> 8); // 24娴ｅ秷娴�ｅ秵鐗宠箛鍐暬濞夛拷
			if (gray >= 128)   //閻х晫鍋�
			{
				writein |= 1;
			}
				
			++k;
				
			if (k == 8)
			{
				pitchcount++;    //閺堝鏅ョ悰灞炬殶閹诡喖鐡ч懞鍌涳拷閺侊拷1
				iRet = write(wrfile, &writein, 1);
				if(iRet < 0 || iRet != 1)
				{
					close(wrfile);
					return 4;
				}
				writein = 0;
				k = 0;
			}

			writein <<= 1;       //閸欏磭些1娴ｏ拷
		}

		if (k != 0)              //娑撳秷鍐绘稉锟介嚋鐎涙濡悽锟芥繅顐㈠帠閸撯晙缍�
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
			
		if (pitchcount%4 != 0)  //4鐎涙濡�靛綊缍�
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

	//鑾峰彇鍘熷鍥剧墖鏂囦欢鐨勫ぇ灏�
	opFileSize = GetFileSize(changefilename);
	OsLog(LOG_DEBUG,"-----bmpfile size = %d",opFileSize);

	//open file 鎵撳紑24浣嶅師濮嬪浘鐗囨枃浠�
	opfile = open(changefilename, O_RDONLY);
	if(opfile < 0)
	{
		OsLog(LOG_DEBUG,"-----opfile = %d",opfile);
		close(opfile);
		return 1;
	}

	//灏�浣嶅師濮嬪浘鐗囨枃浠� 璇诲叆鍒癰uf涓�
	memset(opFileBuf, 0, sizeof(opFileBuf));
	iRet = read(opfile, opFileBuf, opFileSize);
	if(iRet < 0 || iRet != opFileSize)
	{
		close(opfile);
		return 2;
	}
	opReadCount=0;

	//璇诲彇鍥剧墖鏂囦欢澶达紝FILE_HEADER_LEN 闀垮害14
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

	//璇诲彇鍥剧墖鏂囦欢澶翠俊鎭紝INFO_HEADER_LEN 40
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

	//璇诲彇鍘熷鍥剧墖鏂囦欢 瀹藉害
	iWidth = opBitmapInfoHead.biWidth;
	if(iWidth > 384)
	{
		close(opfile);
		return  5;
	}

	//璇诲彇鍘熷鍥剧墖鏂囦欢 楂樺害
	iHeight = opBitmapInfoHead.biHeight;


	//write file header
	memset(cWrFileBuf, 0, sizeof(cWrFileBuf));
	//	bfType
	cWrFileBuf[0] = 0x42; //B
	cWrFileBuf[1] = 0x4D; //M

	//bfSize
	iLen = ((iWidth*1 + 31) >> 5) << 2;	//閸楁洝澹奲iBitCount=1
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

	//鍒涘缓鐩爣榛戠櫧鍥剧墖鏂囦欢
/*
	wrfile = open(greyfilename, O_WRONLY | O_CREAT | O_TRUNC,  S_IREAD|S_IWRITE);
	if(wrfile < 0)
	{
		LOGI("File can not creat!\n");
		close(wrfile);
		return 1;
	}

	//瀵瑰叆鐩爣榛戠櫧鍥剧墖 鍐欏叆 鏂囦欢澶翠俊鎭�
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


	//鍑嗗鐩爣榛戠櫧鏂囦欢 鏂囦欢淇℃伅澶存暟鎹�
	//write info header
	wrBitmapInfoHead.biSize		= INFO_HEADER_LEN;
	wrBitmapInfoHead.biWidth	= opBitmapInfoHead.biWidth;
	wrBitmapInfoHead.biHeight	= opBitmapInfoHead.biHeight;
	wrBitmapInfoHead.biPlanes	= 1;
	wrBitmapInfoHead.biBitCount = 1;
	wrBitmapInfoHead.biCompression	 = 0;
	iLen = ((iWidth*1 + 31) >> 5) << 2;	//閸楁洝澹奲iBitCount=1
	iLen *= iHeight;
	wrBitmapInfoHead.biSizeImage	 = iLen;
	wrBitmapInfoHead.biXPelsPerMeter = opBitmapInfoHead.biXPelsPerMeter;
	wrBitmapInfoHead.biYPelsPerMeter = opBitmapInfoHead.biYPelsPerMeter;
	wrBitmapInfoHead.biClrUsed		 = 0;
	wrBitmapInfoHead.biClrImportant  = opBitmapInfoHead.biClrImportant;

	//瀵圭洰鏍囬粦鐧藉浘鐗� 鍐欏叆 鏂囦欢淇℃伅澶存暟鎹�
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

	//瀵圭洰鏍囬粦鐧藉浘鐗� 鍐欏叆 璋冭壊鏉挎暟鎹�
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

		//瀵圭洰鏍囬粦鐧藉浘鐗� 鍒嗚 鍐欏叆 浣嶅浘鏁版嵁淇℃伅
		for (j = 0; j < iWidth; j++)
		{
			/*
			iRet = read(opfile, &rgb, 3); //24浣� 鍥剧墖 鐢ㄤ笁涓瓧鑺傝〃绀轰竴涓儚绱犵偣淇℃伅
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


			//鏍规嵁鍥惧儚鐨勯鑹插皢24浣嶈浆鎹负鍗曡壊锛岄粦鐧藉浘鍍�嶈〃绀轰竴涓儚绱狅紝闈為粦鍗崇櫧锛岄潪1鍗�
           	// rgb: 0x00bbggrr
           	b = (unsigned char)((rgb)>>16);
           	g = (unsigned char)((rgb)>>8);
           	r = (unsigned char)(rgb);

           	gray = (unsigned char)(( 77 * r + 151 * g + 28 * b) >> 8); // 24娴ｅ秷娴�ｅ秵鐗宠箛鍐暬濞�
			if (gray >= 128)
			{
				writein |= 1; //濡傛灉鏄粦鑹诧紝褰撳墠鍍忕礌浣嶏紝涔熷氨鏄渶浣庝綅.
			}

			++k; //榛戠櫧鍥惧儚 鍍忕礌浣嶈鏁板櫒

			//褰撲綅鏁板埌杈�讹紝璁颁负涓�涓瓧鑺�
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
			//鍐欏叆
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

		//绯荤粺鏄�楄妭瀵归綈锛屽鏋滃彂鐜颁笉瀵归綈锛屽垯琛ヨ冻澶熸暣闄�勫瓧鑺傛暟
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

	//瀵瑰叆鐩爣榛戠櫧鍥剧墖 鍐欏叆 鏂囦欢澶翠俊鎭�
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

