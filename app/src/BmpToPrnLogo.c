#include "unistd.h"
#include "BmpToPrnLogo.h"
#include "changebmp.h"
#include "fcntl.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "osal.h"
#include <stdio.h>
#include <stdlib.h>


/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
/********************** Internal variables declaration *********************/
unsigned char gMallocBuffer[1000];

/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
int IniFileSize(const char *pszFile);

/********************** Internal variables declaration *********************/
/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

void conv_bmp(unsigned char *dest, unsigned char *src, int len, int jplen, unsigned char off)
{
	int i, tlen;

	//the last pix need reversed
	if (off != 0)
	{
		tlen = len - jplen-1;
	}
	else
	{
		tlen = len - jplen;
	}

	for(i=0; i<tlen; i++)
	{
		dest[i] = src[i] ^ 0xff;
	}
	
	if(off!= 0)
	{
		dest[tlen] = src[tlen] ^ (0xff<<(8-off));
	}
}

int BmpToPrnLogo(unsigned char *filename,int mode,unsigned char *gMallocBuffer)
{
	int fp;
	unsigned char bmp[36864];
	int iLen, LineByte;
	int i=0;
	int iFileLen,iBmpTotalLen, iBmpOffSet;
	int iBmpPixWidth, iBmpWidth, iBmpFillLen,iBmPixHeight ,iBmpPixBitCount,iBmpByteSize;
	unsigned char ucOff;

	LineByte = IniFileSize((char *)filename);
	if(LineByte<= 0 || LineByte>sizeof(bmp))
	{
		return DATA_ERROR3;
	}

	memset(bmp,0,sizeof(bmp));
	fp=open((char *)filename, O_RDONLY);  //鎵撳紑鏂囦欢
	if(fp < 0)
	{   
		return -1;
	}
	iLen = read(fp, bmp, LineByte);
	if(iLen != LineByte)
	{
		return -2;
	}

	if(!(bmp[0x00]=='B' && bmp[0x01]=='M'))		//鍥惧儚鏂囦欢澶�
	{    
		close(fp);
		return  DATA_ERROR1;
	}
	
	iBmpOffSet =  ((int)bmp [0x0d]) * 16777216 +	//浠庢枃浠跺紑濮嬪埌浣嶅浘鏁版嵁涔嬮棿鐨勫亸绉婚噺
		((int)bmp [0x0c]) * 65536 + 
		((int)bmp [0x0b]) * 256  + 
		(int)bmp [0x0a];
	iFileLen = ((int)bmp [0x05]) * 16777216 +		//鏁翠釜鏂囦欢澶у皬
		((int)bmp [0x04]) * 65536 + 
		((int)bmp [0x03]) * 256  + 
		(int)bmp [0x02];
	iBmpTotalLen = iFileLen-iBmpOffSet;
	
	iBmpPixWidth = ((int)bmp [0x15]) * 16777216 +	//浣嶅浘�1�7�藉害锛屼互鍍忕礌涓哄崟浣�
		((int)bmp [0x14]) * 65536 +
		((int)bmp [0x13]) * 256  + 
		(int)bmp [0x12];

	if(iBmpPixWidth>384) 
	{
		close(fp);
		return  DATA_ERROR4;
	}

	iBmPixHeight = ((int)bmp[0x19]) * 16777216 +    //浣嶅浘楂樺害锛屼互鍍忕礌涓哄崟浣ￄ1�7
	    ((int)bmp [0x18]) * 65536 +
		((int)bmp [0x17]) * 256  + 
		(int)bmp [0x16];


	iBmpPixBitCount = ((int)bmp[0x1d]) * 256 + ((int)bmp[0x1c]); //姣忎釜鍍忕礌鐨勪綅鏁ￄ1�7�哄崟鑹ￄ1�7
	if(iBmpPixBitCount != 1)
	{
		close(fp);
		return DATA_ERROR2;
	}

	iBmpByteSize = ((int)bmp[0x25]) * 16777216 +     //鐢ㄥ瓧鑺傛暟琛ㄧず鐨勪綅鍥炬暟鎹殑澶у皬锛岃鏁板�蹇呴』鏄�鐨勫�鏁�
	    ((int)bmp [0x24]) * 65536 + 
		((int)bmp [0x23]) * 256  + 
		(int)bmp [0x22];

	if(iBmpByteSize > 36864)
	{                  
		close(fp);
		return DATA_ERROR3;
	}

	ucOff = iBmpPixWidth%8;
	if(ucOff !=0)
	{
		iBmpPixWidth +=8;
	}
	
	if(((iBmpPixWidth/8)%4) != 0)
	{
		iBmpFillLen = 4-(iBmpPixWidth/8)%4;
	}
	else
	{
		iBmpFillLen = 0;
	}

	iBmpWidth = iBmpPixWidth/8+iBmpFillLen;
	
	Pax_logo.line = bmp[0x16];
	for(i=0; i<Pax_logo.line; i++)
	{
		memset(Pax_logo.prn_txt[i].content, 0, 72);
		Pax_logo.prn_txt[i].count[0] = 0;
		Pax_logo.prn_txt[i].count[1] = 72;
		if(mode)
		{
			memcpy(Pax_logo.prn_txt[i].content, &bmp[iFileLen-(i+1)*iBmpWidth], iBmpWidth);
		}
		else
		{
			conv_bmp(Pax_logo.prn_txt[i].content, &bmp[iFileLen-(i+1)*iBmpWidth], iBmpWidth, iBmpFillLen, ucOff);
		}
#if 0
		if(((i+1)*iBmpWidth)<=iBmpTotalLen)
		{
			logo.prn_txt[i].count[0] = 0;
			logo.prn_txt[i].count[1] = 72;
			memcpy(logo.prn_txt[i].content, &bmp[iFileLen-(i+1)*iBmpWidth], iBmpWidth);
		}
		else
		{
			logo.prn_txt[i].count[0] = 0;
			logo.prn_txt[i].count[1] = 72;
			//conv_bmp(logo.prn_txt[i].byte_content, &bmp[0], iLen-i*48);
			memcpy(logo.prn_txt[i].content, &bmp[iBmpOffSet], iFileLen-i*iBmpWidth);
		}
#endif
	}
	gMallocBuffer[0] = Pax_logo.line;

	for(i= 0;i<Pax_logo.line; i++)
	{
		memcpy(&gMallocBuffer[i*74+1], Pax_logo.prn_txt[i].count, 2);
		memcpy(&gMallocBuffer[i*74+2+1], Pax_logo.prn_txt[i].content, 72);
	}

	close(fp);
	return 0;
}

int IniFileSize(const char *pszFile)
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
// end of file
