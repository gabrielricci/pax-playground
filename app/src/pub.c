/*
 ============================================================================
 Name        : pub.c
 Author      : Sabrina Fung
 Version     : 1.0.0
 Copyright   : PAX Computer Technology(Shenzhen) CO., LTD
 Description : PAX POS Shared Library
 ============================================================================
 */


#include "pub.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "string.h"


#define NULL ((void *)0)


#ifdef __cplusplus
extern "C"
{
#endif


/********************** Internal macros declaration ************************/

/********************** Internal structure declaration *********************/

/********************** Internal functions declaration *********************/
static char IsLeapYear(int iYear);


/********************** Internal variables declaration *********************/

const int iConstMonthday[12]=
{
	0,
	31,  //1
	31+28,  //2
	31+28+31,  //3
	31+28+31+30,  //4
	31+28+31+30+31,  //5
	31+28+31+30+31+30,  //6
	31+28+31+30+31+30+31,  //7
	31+28+31+30+31+30+31+31,  //8
	31+28+31+30+31+30+31+31+30,  //9
	31+28+31+30+31+30+31+31+30+31,  //10
	31+28+31+30+31+30+31+31+30+31+30,  //11
};

/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

//1.字符串类型转换类

//将二进制源串分解成双倍长度可读的16进制丄1�7?7,0x12AB-->"12AB"
//Convert BIN string to readable HEX string, which have double length of BIN string. 0x12AB-->"12AB"
void PubBcd2Asc(const char *psIn, int iLen, char *psOut)
{
	static const char sHexToChar[16] = {"0123456789ABCDEF"};
	int   iCnt;

	if ((psIn == NULL) || (psOut == NULL))
	{
		return;
	}

	for (iCnt = 0; iCnt < iLen; iCnt++)
	{
		psOut[2*iCnt]   = sHexToChar[(psIn[iCnt] >> 4)];
		psOut[2*iCnt + 1] = sHexToChar[(psIn[iCnt] & 0x0F)];
	}
}

//同vOneTwo()函数,并在目标串后添一 '\0'
//Similar with function PubOne2Two(), and add '\0' at the end of object string
void PubBcd2Asc0(const char *psIn, int iLen, char *pszOut)
{
	if ((psIn == NULL) || (pszOut == NULL))
	{
		return;
	}

	PubBcd2Asc(psIn, iLen, pszOut);
	pszOut[2*iLen] = 0;
}

//将可读的16进制串合并成其一半长度的二进制串,"12AB"-->0x12AB
//Convert readable HEX string to BIN string, which only half length of HEX string. "12AB"-->0x12AB
void PubAsc2Bcd(const char *psIn, int iLen, char *psOut)
{
	char   tmp;
	int    i;
    
	if ((psIn == NULL) || (psOut == NULL))
	{
		return;
	}

	for (i = 0; i < iLen; i += 2)
	{
		tmp = psIn[i];
		if (tmp > '9')
		{
			tmp = toupper((int)tmp) - 'A' + 0x0A;
		}
		else
		{
			tmp &= 0x0F;
		}
		psOut[i / 2] = (tmp << 4);

		tmp = psIn[i+1];
		if (tmp > '9')
		{
			tmp = toupper((int)tmp) - 'A' + 0x0A;
		}else
		{
			tmp &= 0x0F;
		}
		psOut[i/2] |= tmp;
	}
}

//将数字串转变成整垄1�7,相当于atoi(),但是该函数不要求零字符结射1�7
//Convert number string to integer, similar to atoi(). This function don't request end with '\0'.
int PubAsc2Int(const char *psString, int iLen, int *piOut)
{
	char   szBuff[15+1];
	int    iTmp;

	sprintf(szBuff, "%.*s", iLen <= 15 ? iLen : 15, psString);
	iTmp = atoi(szBuff);
	if (piOut != NULL)
	{
		*piOut = iTmp;
	}

	return iTmp;
}

//整型数转换成高位在前的字符串表示(按照网络字节格式存放)
//Convert integer to string which high bit at the front. (Store it according to network byte format)
void PubInt2Char(int iSource, int iTargetLen, char *psTarget)
{
	int i;

	for (i = 0; i < iTargetLen; i++)
	{
		psTarget[i] = iSource >> (8 * (iTargetLen - i - 1));
	}
}

//高位在前的字符串表示转换为整型数
//Convert the character string with high bit in the front to integer
int PubChar2Int(const char *psSource, int iSrcLen, int *piTarget)
{
	int i, iTmp;
    
	if (psSource == NULL)
	{
		return 0;
	}

	iTmp = 0;
	for (i = 0; i < iSrcLen; i++)
	{
		iTmp |= ((int)psSource[i] << 8 * (iSrcLen - i - 1));
	}

	if (piTarget != NULL)
	{
		*piTarget = iTmp;
	}
	return iTmp;
}

//将整数转换为BCD格式的串存放,1234 --> 0x12 0x34
//Converted integer to BCD string,1234 --> 0x12 0x34
void PubInt2Bcd(int iSource, int iTargetLen, char *psTarget)
{
	char    szFmt[30], szBuf[30];

	sprintf(szFmt, "%%0%dlu", iTargetLen * 2);
	sprintf(szBuf, szFmt, iSource);

	if (psTarget != NULL)
	{
		PubAsc2Bcd(szBuf, iTargetLen * 2, psTarget);
	}
}

//转换BCD串为整数
//Convert BCD string to integer
int PubBcd2Int(const char *psSource, int iSrcLen, int *piTarget)
{
	char   szBuf[30];
	int   iTmp;

	if ((psSource == NULL))
	{
		return 0;
	}

	PubBcd2Asc0(psSource, iSrcLen, szBuf);
	iTmp = atoi(szBuf);
	if (piTarget != NULL)
	{
		*piTarget = iTmp;
	}

	return iTmp;
}


//2. 字符串格式化
//转换数字串到金额格式，如HKD$****.**, RMB****.**
void PubConvAmount(const char *pszPrefix, const char *pszIn, char cDeciPos, char *pszOut, char cMisc)
{
	int		ii, iInLen, iIntegerLen;
	char	bSeparator, bNegative, szBuff[40+1], szTemp[40];
	char	*pRead, *pWr;

	if ((pszIn == NULL) || (pszOut == NULL) || (strlen(pszIn) > 40) )
	{
		return;
	}

	sprintf(szBuff, "%.40s", pszIn);
	PubTrimHeadChars(szBuff, '0');
	iInLen = strlen(szBuff);		//get input amount length

	pRead  = szBuff;
	pWr = pszOut;

	bSeparator = cMisc & GA_SEPARATOR;		//whether use ',' to separate huge amount
	bNegative  = cMisc & GA_NEGATIVE;		//whether display negative amount

	if ((pszPrefix != NULL) && (*pszPrefix != 0))
	{
		pWr += sprintf(pWr, "%.3s ", pszPrefix);
	}

	if (bNegative)
	{
		*(pWr++) = '-';
	}

    // before decimal point
	if (iInLen > cDeciPos)	// if value>1
	{
		iIntegerLen = iInLen - cDeciPos;
		ii = iIntegerLen;
		while (ii--)
		{
			*(pWr++) = *(pRead++);
			if (bSeparator && (ii%3 == 0) && ii)
			{
				*(pWr++) = ',';
			}
		}
	}
	else
	{
		*(pWr++) = '0';
	}

    // decimal point and afterwards
	if (cDeciPos != 0)
	{
		sprintf(szTemp, "%.*s", cDeciPos, pRead);
		PubAddHeadChars(szTemp, cDeciPos, '0');
		sprintf(pWr, ".%s", szTemp);
		pWr += (cDeciPos+1);
	}
	else
	{
		*(pWr++) = 0;
	}
}


//将字符串转换为大冄1�7
//Convert the character string to capital
void PubStrUpper(char *pszStrInOut)
{
	if ((pszStrInOut == NULL))
	{
		return;
	}

	while (*pszStrInOut)
	{
		*pszStrInOut = toupper((int)*pszStrInOut);
		pszStrInOut++;
	}
}

//将字符串转换为小冄1�7
//Convert the character string to lowercase
void PubStrLower(char *pszStrInOut)
{
	if ((pszStrInOut == NULL))
	{
		return;
	}

	while (*pszStrInOut)
	{
		*pszStrInOut = tolower((int)*pszStrInOut);
		pszStrInOut++;
	}
}

//删除字符串中的空白字笄1�7(包括,空格/回车/换行/TAB字符)
//Delete the characters include blank,enter,newline,TAB in the string
void PubTrimStr(char *pszStrInOut)
{
#define ISSPACE(ch) ( ((ch) == ' ')  || ((ch) == '\t') || \
	((ch) == '\n') || ((ch) == '\r') )

	char *p, *q;

	if (!pszStrInOut || !*pszStrInOut)
	{
		return;
	}

	p = &pszStrInOut[strlen(pszStrInOut) - 1];
	while ((p > pszStrInOut) && ISSPACE(*p))
	{
		*p-- = 0;
	}

	if ((p == pszStrInOut) && ISSPACE(*p))
	{
		*p = 0;
	}

	for (p = pszStrInOut; *p && ISSPACE(*p); p++);

	if (p != pszStrInOut)
	{
		q = pszStrInOut;
		while (*p)
		{
			*q++ = *p++;
		}

		*q = 0;
	}
#undef ISSPACE
}


//删除字符串两端的特定字符
//Delete the specified character in string
void PubTrimSpcStr(char *pszStrInOut, char cSpcChar)
{
	char *p, *q;

	if (!pszStrInOut || !*pszStrInOut)
	{
		return;
	}

	p = &pszStrInOut[strlen(pszStrInOut) - 1];
	while ((p > pszStrInOut) && ((*p) == cSpcChar))
	{
		*p-- = 0;
	}

	if (p == pszStrInOut && ((*p) == cSpcChar))
	{
		*p = 0;
	}

	for (p = pszStrInOut; *p && ((*p) == cSpcChar); p++);
	if (p != pszStrInOut )
	{
		q = pszStrInOut;
		while (*p)
		{
			*q++ = *p++;
		}
		*q = 0;
	}
}

//删除前导字符
// delete the specified continuous characters on the left of the string
void PubTrimHeadChars(char *pszStrInOut, char cRemoveChar)
{
	char	*p;

	if (!pszStrInOut || !*pszStrInOut)
	{
		return;
	}

	for (p = pszStrInOut; *p && *p==cRemoveChar; p++);
	if (p != pszStrInOut)
	{
		while ((*pszStrInOut++ = *p++));
	}
}


//删除字符串右边连续的特定字符
//Delete the specified continuous characters on the right of the string
void PubTrimTailChars(char *pszStrInOut, char cRemoveChar)
{
	int		i, iLen;

	if (!pszStrInOut || !*pszStrInOut)
	{
		return;
	}

	iLen = strlen(pszStrInOut);
	for (i = iLen - 1; i >= 0; i--)
	{
		if (pszStrInOut[i] != cRemoveChar)
		{
			break;
		}
		pszStrInOut[i] = 0;
	}
}


// 填充前导字符,以便达到iTargetLen长度
// Pad leading charactes till specific length.
void PubAddHeadChars( char *pszStrInOut, int iTargetLen, char cAddChar )
{
	int	iLen;

	if (pszStrInOut == NULL)
	{
		return;
	}

	iLen = (int)strlen(pszStrInOut);
	if (iLen >= iTargetLen)
	{
		return;
	}

	memmove(pszStrInOut + iTargetLen - iLen, pszStrInOut, iLen + 1);
	memset(pszStrInOut, cAddChar, iTargetLen - iLen);
}


//反转字符丄1�7
//reverse the string
void PubStrReverse(char *pszStringInOut)
{
	int     i, j, iLength;
	char    cTmp;

	if (pszStringInOut == NULL)
	{
		return;
	}

	iLength = strlen(pszStringInOut);
	for (i = 0,j = iLength - 1; i < iLength / 2; i++,j--)
	{
		cTmp = pszStringInOut[i];
		pszStringInOut[i] = pszStringInOut[j];
		pszStringInOut[j] = cTmp;
	}
}


//取字节的高四佄1�7
//get the high 4 bit of the byte
char PubHigh4Bit(const char cInChar)
{
	return (cInChar / 16);
}


//取字节的低四佄1�7
//get the low 4 bit of the byte
char PubLow4Bit(const char cInChar)
{
	return (cInChar & 0x0F);
}


//3. 字符串间运算

//不区分大小写的字符串比较函数
//Compare two strings, non case-sensitive.
int PubStrNoCaseCmp(const char *pszStr1, const char *pszStr2)
{
	if ((pszStr1 == NULL) || (pszStr2 == NULL))
	{
		return -1;
	}

	while (*pszStr1 && *pszStr2)
	{
		if (toupper((int)*pszStr1) != toupper((int)*pszStr2))
		{
			return (toupper((int)*pszStr1) - toupper((int)*pszStr2));
		}

		pszStr1++;
		pszStr2++;
	}

	if (!*pszStr1 && !*pszStr2)
	{
		return 0;
	}

	if (!*pszStr1)
	{
		return -1;
	}

	return 1;
}

//将源串与目标串异或后存入目标串（两个串长度相等）
//Source string XOR object string, then store the result in object string.(The two string are same length)
void PubXor(const char *psVect1, const char *psVect2, int iLength, char *psOut)
{
	int   iCnt;

	if ((psVect1 == NULL) || (psVect2 == NULL) || (psOut == NULL))
	{
		return;
	}

	memcpy(psOut, psVect1, iLength);

	for (iCnt = 0; iCnt < iLength; iCnt++)
	{
		psOut[iCnt] ^= psVect2[iCnt];
	}
}


//两个ASC字符串相筄1�7,结果放在pszProduct丄1�7
//multiply one ASC string by another
void PubAscMul(const char *pszFaciend, const char *pszMultiplier, char *pszProduct)
{
	char	*p, cTemp, cCarryBit, szBuff[100+1];
	int	iFaciLen, iMulLen, iProdPos, iCnt;

	if ((pszFaciend == NULL) || (pszMultiplier == NULL) || (pszProduct == NULL))
	{
		return;
	}

	iFaciLen = strlen(pszFaciend);
	iMulLen  = strlen(pszMultiplier);

	if (iFaciLen + iMulLen > 100)
	{
		return;
	}

	sprintf(szBuff, "%0*ld", iFaciLen+iMulLen, 0L);

	for (iProdPos = 0; iFaciLen > 0; iFaciLen--, iProdPos++)
	{
		cCarryBit = 0;
		p = &szBuff[iProdPos];

		for (iCnt = iMulLen; iCnt>0; iCnt--)
		{
			cTemp = (pszFaciend[iFaciLen-1] & 0x0F) *
					(pszMultiplier[iCnt-1] & 0x0F) +
					cCarryBit + (*p & 0x0F);
			*p++   = (cTemp % 10) + '0';
			cCarryBit = cTemp / 10;
		}

		if (cCarryBit != 0)
		{
			*p++ = cCarryBit + '0';
		}
	}
	PubTrimTailChars(szBuff, '0');
	PubStrReverse(szBuff);

	if (szBuff[0] == 0)
	{
		sprintf(szBuff, "0");
	}
	if (pszProduct != NULL)
	{
		sprintf(pszProduct, "%s", szBuff);
	}
}

//长度相等的两个ASC字符串相劄1�7
// Addition of two equal length ASCII string(length specified, no need '\0' ending)
void PubAscAdd(const char *psAddend1, const char *psAddend2, int iLen, char *pszSum)
{
	char	*pszResult, cCarryBit, cTemp, szBuff[100+1];

	if ((psAddend1 == NULL) || (psAddend2 == NULL) || (pszSum == NULL) || (iLen > 100))
	{
		return;
	}

	cCarryBit = 0;
	pszResult  = szBuff;
	while (iLen > 0)
	{
		cTemp = (psAddend1[iLen-1] & 0x0F) + (psAddend2[iLen-1] & 0x0F) + cCarryBit;
		*pszResult++ = (cTemp % 10) + '0';
		cCarryBit   = (cTemp>9) ? 1 : 0;
		iLen--;
	}
	if (cCarryBit != 0)
	{
		*pszResult++ = '1';
	}
	*pszResult = 0;

	PubStrReverse(szBuff);
	if (pszSum != NULL)
	{
		sprintf(pszSum, "%s", szBuff);
	}
}

//长度相等的两个ASC字符串相凄1�7(psMinuend>psSubtrahend)
// Subtraction of two equal length ASCII string(length specified, no need '\0' ending)
void PubAscSub(const char *psMinuend, const char *psSubtrahend, int iLen, char *pszResult)
{
	char	*pszOut, cCarryBit, cTemp, szBuff[100+1];

	if ((psMinuend == NULL) || (psSubtrahend == NULL) || (pszResult == NULL) || (iLen > 100))
	{
		return;
	}

	pszOut = szBuff;
	cCarryBit = 0;
	while (iLen > 0)
	{
		cTemp = (psMinuend[iLen-1] & 0x0F) - (psSubtrahend[iLen-1] & 0x0F) - cCarryBit + 10;
		*pszOut++  = (cTemp % 10) + '0';
		cCarryBit = (psMinuend[iLen-1] < psSubtrahend[iLen-1] + cCarryBit) ? 1 : 0;
		iLen--;
	}
	*pszOut = 0;

	PubStrReverse(szBuff);
	if (pszResult != NULL)
	{
		sprintf(pszResult, "%s", szBuff);
	}
}

//ASC字符串减1,结果放在原串丄1�7有返囄1�7
//subtract 1 from the ASC string, the result store in that ASC string
int PubAscDec(char *psAscStrInOut, int iStrLen)
{
	if ((psAscStrInOut == NULL) || (iStrLen == 0))
	{
    	return 1;
	}

	psAscStrInOut += iStrLen - 1;
	while (--(*psAscStrInOut) < '0')
	{
		*psAscStrInOut-- = '9';
		if (--iStrLen == 0)
		{
			return 1;
		}
	}
	return 0;
}


//ASC字符串加1,结果放在原串丄1�7,有返囄1�7
//Add 1 to the ASC string, the result store in that ASC string
int PubAscInc(char *psAscStrInOut, int iStrLen)
{
	if ((psAscStrInOut == NULL) || (iStrLen == 0))
	{
			return 1;
	}

	psAscStrInOut += iStrLen - 1;
	while (++(*psAscStrInOut) > '9')
	{
		*psAscStrInOut-- = '0';
		if (--iStrLen == 0) 
		{
			return 1;
		}
	}

	return 0;
}


//长度相等的两个BCD串相加，结果放在psBcdStr1丄1�7
//Addition of 2 equal length BCD string. length specified and no need '\0' ending. fill leading 0 if necessary.
void PubBcdAdd(const char *psAddend1, const char *psAddend2, int iLen, char *psResult)
{
	char	sAdd1[100+1], sAdd2[100];

	if ((psAddend1 == NULL) || (psAddend2 == NULL) || (psResult == NULL) || (iLen > 50))
	{
		return;
	}

	PubBcd2Asc(psAddend1, iLen, sAdd1);
	PubBcd2Asc(psAddend2, iLen, sAdd2);
	PubAscAdd(sAdd1, sAdd2, iLen * 2, sAdd1);
	PubAsc2Bcd(sAdd1, iLen * 2, psResult);
}


//长度相等的两个BCD串相减，结果放在psAddend1丄1�7
//subtraction of 2 equal length BCD string. length specified and no need '\0' ending. fill leading 0 if necessary.
void PubBcdSub(const char *psMinuend, const char *psSubtrahend, int iLen, char *psResult)
{
	char	sMinuend[100+1], sSubtrahend[100];

	if ((psMinuend == NULL) || (psSubtrahend == NULL) || (psResult == NULL) || (iLen > 50))
	{
		return;
	}

	PubBcd2Asc(psMinuend, iLen, sMinuend);
	PubBcd2Asc(psSubtrahend, iLen, sSubtrahend);
	PubAscSub(sMinuend, sSubtrahend, iLen * 2, sMinuend);
	PubAsc2Bcd(sMinuend, iLen * 2, psResult);
}


//BCD字符串减1,结果放在原串丄1�7,有返囄1�7
//subtracted from the BCD string, the result store in that BCD string
int PubBcdDec(char *psBcdStrInOut, int iStrLen)
{
	if ((psBcdStrInOut == NULL) || (iStrLen == 0))
	{
		return 1;
	}

	psBcdStrInOut += iStrLen - 1;
	while (--(*psBcdStrInOut) == 0xff)
	{
		*psBcdStrInOut = 0x99;
		psBcdStrInOut--;
		if (--iStrLen == 0) 
		{
			return 1;
		}
	}

	if (((*psBcdStrInOut) & 0x0f) > 9)
	{
		*psBcdStrInOut -= 0x06;
	}

	return 0;
}


//BCD字符串加1,结果放在原串丄1�7,有返囄1�7
//Add 1 to the BCD string, the result store in that BCD string
int PubBcdInc(char *psBcdStrInOut, int iStrLen)
{
	if ((psBcdStrInOut == NULL) || (iStrLen == 0))
	{
		return 1;
	}

	psBcdStrInOut += iStrLen - 1;
	while (++(*psBcdStrInOut) > 0x99)
	{
		*psBcdStrInOut = 0;
		psBcdStrInOut--;
		if ((--iStrLen) == 0) 
		{
			return 1;
		}
	}

	if (((*psBcdStrInOut) & 0x0f) > 9)
	{
		*psBcdStrInOut += 0x06;
	}

	return 0;
}


//4. 标准算法

// 计算LRC
void PubCalcLRC(const char *psData, int iLength, char *pcInit)
{
	char cInit;

	if ((psData == NULL) || (pcInit == NULL))
	{
		return;
	}

	cInit = *psData++;
	iLength--;

	while (iLength > 0)
	{
		cInit ^= *psData++;
		iLength--;
	}

	*pcInit = cInit;
}



//校验卡号是否满足校验
int PubChkCardNo(const char *pszPanNo)
{
	char	bFlag, cTemp, cResult;
	char	szPanNoCopy[19+1];
	char* pszTemp;

	if ((pszPanNo == NULL) || (strlen(pszPanNo) == 0))
	{
		return 1;
	}

	memset(szPanNoCopy, 0, sizeof(szPanNoCopy));
	memcpy(szPanNoCopy, pszPanNo, strlen(pszPanNo));

	// (2121算法)
	bFlag    = 0;
	pszTemp  = &szPanNoCopy[strlen(szPanNoCopy)-1];
	cResult = 0;
	while (pszTemp >= szPanNoCopy)
	{
		cTemp = (*pszTemp--) & 0x0F;
		if (bFlag == 1)
		{
			cTemp *= 2;
		}
		if (cTemp > 9)
		{
			cTemp -= 9;
		}
		cResult = (cTemp + cResult) % 10;
		bFlag = !bFlag;
	}

	if (cResult != 0)
	{
		return 1;
	}

	return 0;
}


//5. 时间函数
//将一个int型整数转换为日期+时间字符丄1�7
void PubInt2Time(int iSecond,char *pszYYYYMMDDhhmmss)
{
	int iYear,iMonth,iDay,iDaySecond,iWeek;

	if (pszYYYYMMDDhhmmss == NULL)
	{
		return;
	}

	iDay = iSecond / (24 * 3600l);
	iWeek = (iDay + STAR_WEEK) % 7;
	iWeek++;
	iDaySecond = iSecond % (24 * 3600);
	iYear = 0;

	while (iDay > 365)
	{
		if (IsLeapYear(iYear + STAR_YEAR))
		{
			iDay--;
		}
		iYear++;
		iDay -= 365;
	}

	if (iDay == 365)
	{
		if (IsLeapYear(iYear + STAR_YEAR))
		{
			iDay = 31;
			iMonth = 12;
		}
		else
		{
			iYear++;
			iDay = 1;
			iMonth = 1;
		}
	}
	else
	{
		for(iMonth = 1; iMonth <= 11; iMonth++)
		{
			if (iMonth == 3)
			{
				if (IsLeapYear(iYear + STAR_YEAR))
				{
					if (iDay == iConstMonthday[2])
					{
						iMonth = 2;
						break;
					}
					iDay--;
				}
			}
			if (iDay < iConstMonthday[iMonth])
			{
				break;
			}
		}

		iDay = iDay - iConstMonthday[iMonth - 1];
		iDay++;
	}

	sprintf(&(pszYYYYMMDDhhmmss[0]), "%04d", iYear + STAR_YEAR);
	sprintf(&pszYYYYMMDDhhmmss[4], "%02d", iMonth);
	sprintf(&pszYYYYMMDDhhmmss[6], "%02d", iDay);
	sprintf(&pszYYYYMMDDhhmmss[8], "%02d", iDaySecond/3600);
	iDaySecond = iDaySecond % 3600;
	sprintf(&pszYYYYMMDDhhmmss[10], "%02d", iDaySecond/60);
	iDaySecond = iDaySecond%60;
	sprintf(&pszYYYYMMDDhhmmss[12], "%02d", iDaySecond);
//	sprintf(&pszYYYYMMDDhhmmss[14],"%02lu",ulWeek); //no need week yet
	pszYYYYMMDDhhmmss[14] = 0;
}

//将一个日朄1�7+时间字符串转换为int型整敄1�7
int PubTime2Int(const char *psYYYYMMDDhhmmss)
{
	int iRet;
	int iTemp;

	if (psYYYYMMDDhhmmss == NULL)
	{
		return 0;
	}

	iRet = PubDay2Int(psYYYYMMDDhhmmss);
	iRet = iRet * 24;

	iTemp = PubAsc2Int(&psYYYYMMDDhhmmss[8], 2, NULL);
	iRet = iRet + iTemp;
    /* convert to minute */
	iRet = iRet * 60;

	iTemp = PubAsc2Int(&psYYYYMMDDhhmmss[10], 2, NULL);
	iRet = iRet + iTemp;
    /* convert to seconds */
	iRet = iRet * 60;

	iTemp = PubAsc2Int(&psYYYYMMDDhhmmss[12], 2, NULL);
	iRet = iRet + iTemp;

	return iRet;
}

//将一个日期字符串转换为int型整敄1�7
int PubDay2Int(const char *psYYYYMMDD)
{
	int iYear,iMonth,iDate,iNowYear;

	int iRet;

	if (psYYYYMMDD == NULL)
	{
		return 0;
	}

	iNowYear = PubAsc2Int(&psYYYYMMDD[0],4,NULL);

	iYear = iNowYear-STAR_YEAR;

    /* Leap year */
    //ulRet=iYear*365l+(iYear/4)+(iYear/400)-(iYear/100);
	iRet = 0;

	iRet = (iNowYear / 4) - (STAR_YEAR / 4);
	iRet = iRet + (STAR_YEAR / 100) - (iNowYear / 100);
	iRet = iRet + (iNowYear / 400) - (STAR_YEAR / 400);

	iRet = iYear * 365l + iRet;
	iMonth = PubAsc2Int(&psYYYYMMDD[4], 2, NULL);

	iDate = iConstMonthday[iMonth - 1];
	iRet = iRet + iDate;
	if (IsLeapYear(iNowYear))
	{
		if (IsLeapYear(STAR_YEAR))
		{
			if (iMonth >= 3)
			{
				/* Leap year */
				iRet = iRet+1;
			}
		}
		else
		{
			if (iMonth < 3)
			{
				/* End year is leap year, But start year isn't */
				iRet = iRet - 1;
			}
		}
	}
	else if (IsLeapYear(STAR_YEAR))
	{
		/* End year isn't leap year, But start year is */
		iRet = iRet + 1;
	}

	iDate = PubAsc2Int(&psYYYYMMDD[6], 2, NULL);
	iDate = iDate - 1;
	iRet = iRet + iDate;
	return iRet;
}

//将一个日期字符串转换为int型整敄1�7
void PubInt2Day(int iDay,char *pszYYYYMMDD)
{
	int iYear,iMonth;

	if (pszYYYYMMDD == NULL)
	{
		return;
	}

	iYear = 0;
	while (iDay > 365)
	{
		if (IsLeapYear(iYear + STAR_YEAR))
		{
			iDay--;
		}
		iYear++;
		iDay -= 365;
	}

	if (iDay == 365)
	{
		if (IsLeapYear(iYear + STAR_YEAR))
		{
			iDay = 31;
			iMonth = 12;
		}
		else
		{
			iYear++;
			iDay = 1;
			iMonth = 1;
		}
	}
	else
	{
		for (iMonth = 1; iMonth <= 11; iMonth++)
		{
			if (iMonth == 3)
			{
				if (IsLeapYear(iYear + STAR_YEAR))
				{
					if (iDay == iConstMonthday[2])
					{
						iMonth = 2;
						break;
					}
					iDay--;
				}
			}
			if (iDay < iConstMonthday[iMonth])
			{
				break;
			}
		}
		iDay = iDay - iConstMonthday[iMonth - 1];
		iDay++;
	}
	sprintf(&pszYYYYMMDD[0], "%04d", iYear + STAR_YEAR);
	sprintf(&pszYYYYMMDD[4], "%02d", iMonth);
	sprintf(&pszYYYYMMDD[6], "%02d", iDay);
	pszYYYYMMDD[8] = 0;
}


//按照MASK定义的格式，棄1�7查日期时间格式是否有敄1�7
int PubIsValidTime(const char *psDateTime, const char *psMaskYYYYMMDDhhmmss)
{
#define ERROR_MASK_DATATIME  9

	const char cMonthDay[12]={31,29,31,30,31,30,31,31,30,31,30,31};
	int  iYear=0, iMonth=0, iDay=0, iHour=0, iMinute=0, iSecond=0;
	int iOffset=0;

	if (psMaskYYYYMMDDhhmmss == NULL)
	{
		return ERROR_MASK_DATATIME;							//Mask error
	}

	if (strlen(psMaskYYYYMMDDhhmmss) == 0 || strlen(psMaskYYYYMMDDhhmmss) > 14 || (strlen(psMaskYYYYMMDDhhmmss) % 2) == 1)
	{
		return ERROR_MASK_DATATIME;							//Mask error
	}

    //processing Year
	if (memcmp(psMaskYYYYMMDDhhmmss,"YYYY",4) == 0)
	{
		iYear = PubAsc2Int(psDateTime, 4, NULL);
		iOffset += 4;
		if ((iYear < 1900) || (iYear > 2099))
		{
			return 1;										//Year error
		}
	}
	else if (memcmp(psMaskYYYYMMDDhhmmss, "YY", 2) == 0)
	{
		iYear = PubAsc2Int(psDateTime, 2, NULL);
		iYear += 2000;
		iOffset += 2;
	}

    //processing Month
	if (psMaskYYYYMMDDhhmmss[iOffset] == 0)
	{
		return 0;		//Finished and return OK
	}

	if (iYear !=0)
	{
		if (memcmp(psMaskYYYYMMDDhhmmss + iOffset, "MM", 2) != 0)
		{
			return ERROR_MASK_DATATIME;	//Mask error
		}
	}
	if (memcmp(psMaskYYYYMMDDhhmmss + iOffset, "MM", 2) == 0)
	{
		iMonth = PubAsc2Int(psDateTime + iOffset, 2, NULL);
		iOffset += 2;
		if (iMonth==0 || iMonth>12)
		{
			return 2;										//Month error
		}
	}

    //processing Day
	if (psMaskYYYYMMDDhhmmss[iOffset] == 0)
	{
		return 0;		//Finished and return OK
	}
	if (iMonth !=0)
	{
		if (memcmp(psMaskYYYYMMDDhhmmss + iOffset, "DD", 2) != 0)
		{
			return ERROR_MASK_DATATIME;	//Mask error
		}
	}
	if (memcmp(psMaskYYYYMMDDhhmmss + iOffset, "DD", 2) ==0 )
	{
		iDay = PubAsc2Int(psDateTime + iOffset, 2, NULL);
		iOffset += 2;
		if ((iDay == 0) || (iDay > 31))
		{
			return 3;										//Day error
		}
		if (iMonth != 0)
		{
			if (iDay > cMonthDay[iMonth-1])
			{
				return 3;										//Day error
			}
			if ((iYear != 0) && (iMonth == 2) && (iDay == 29))
			{
				if (IsLeapYear(iYear) == 0)
				{
					return 3;										//Day error
				}
			}
		}
	}

    //processing Hour
	if (psMaskYYYYMMDDhhmmss[iOffset] == 0)
	{
		return 0;		//Finished and return OK
	}
	if (iDay !=0)
	{
		if (memcmp(psMaskYYYYMMDDhhmmss + iOffset, "hh", 2) != 0)
		{
			return ERROR_MASK_DATATIME;	//Mask error
		}
	}
	if (memcmp(psMaskYYYYMMDDhhmmss + iOffset, "hh", 2) == 0)
	{
		iHour = PubAsc2Int(psDateTime + iOffset, 2, NULL);
		iOffset += 2;
		if (iHour > 23)
		{
			return 4;										//Hour error
		}
		iHour++;		//for not equal 0
	}

    //processing Minute
	if (psMaskYYYYMMDDhhmmss[iOffset] == 0)
	{
		return 0;		//Finished and return OK
	}
	if (iHour !=0)
	{
		if (memcmp(psMaskYYYYMMDDhhmmss + iOffset, "mm", 2) != 0)
		{
			return ERROR_MASK_DATATIME;	//Mask error
		}
	}
	if (memcmp(psMaskYYYYMMDDhhmmss + iOffset, "mm", 2) == 0)
	{
		iMinute = PubAsc2Int(psDateTime + iOffset, 2, NULL);
		iOffset += 2;
		if (iMinute > 59)
		{
			return 5;										//Minute error
		}
		iMinute++;		//for not equal 0
	}

    //processing Second
	if (psMaskYYYYMMDDhhmmss[iOffset] == 0)
	{
		return 0;		//Finished and return OK
	}
	if (iMinute !=0)
	{
		if (memcmp(psMaskYYYYMMDDhhmmss + iOffset, "ss", 2) != 0 )
		{
			return ERROR_MASK_DATATIME;	//Mask error
		}
	}
	if (memcmp(psMaskYYYYMMDDhhmmss + iOffset, "ss", 2) == 0)
	{
		iSecond = PubAsc2Int(psDateTime + iOffset, 2, NULL);
		iOffset += 2;
		if (iSecond > 59)
		{
			return 6;										//Minute error
		}
		iSecond++;		//for not equal 0
	}

	if (iOffset==0)
	{
		return ERROR_MASK_DATATIME;		//Mask error
	}
	return 0;
}



/* Check if leap year or not */
static char IsLeapYear(int iYear)
{
	if ((iYear % 400) == 0)
	{
		return 1;
    }
	else if ((iYear % 100) == 0)
	{
		return 0;
	}
	else if ((iYear % 4) == 0)
	{
		return 1;
	}

	return 0;
}

//计算从输入时间开姄1�7,经过间隔时间后的时间炄1�7,有闰年处琄1�7
void PubCalDateTime(const char *psDateTimeIn, char *pszDateTimeOut, int iInterval, char *psCalMode)
{//YYYYMMDDhhmmss format
	int iYear,iMonth,iTemp;

	if (psDateTimeIn == NULL || pszDateTimeOut == NULL || psCalMode == NULL)
	{
		return;
	}

	if (psCalMode != NULL)
	{
		switch(psCalMode[0])
		{
		case 'Y':
			iYear = PubAsc2Int(psDateTimeIn, 4, NULL);
			sprintf(pszDateTimeOut, "%04d", iYear + iInterval);
			memcpy(pszDateTimeOut+4, psDateTimeIn + 4, 10);
			pszDateTimeOut[14] = 0;

			return;  //not break but return

		case 'M':
			iYear = PubAsc2Int(psDateTimeIn, 4, NULL);
			iMonth = PubAsc2Int(psDateTimeIn + 4, 2, NULL);
			iYear = iYear + (iMonth - 1 + iInterval) / 12;
			iMonth = (iMonth -1 + iInterval) % 12 + 1;
			sprintf(pszDateTimeOut, "%04d", iYear);
			sprintf(pszDateTimeOut + 4, "%02d", iMonth);
			memcpy(pszDateTimeOut + 6, psDateTimeIn + 6, 8);
			pszDateTimeOut[14] = 0;

			return;  //not break but return

		case 'D':
			iInterval = iInterval * 3600 * 24;
			break;

		case 'h':
			iInterval = iInterval * 3600;
			break;

		case 'm':
			iInterval = iInterval * 60;
			break;

		case 's':
		default:
			break;
		}
	}

	iTemp = PubTime2Int(psDateTimeIn) + iInterval;
	PubInt2Time(iTemp,pszDateTimeOut);
}

//计算两个时间点之间的间隔
int PubCalInterval(char *psTimeBegin, char *psTimeEnd, char *psCalMode, char *pcSignal)
{
	int iBegin,iEnd,iRet;
	char *psBig,*psSmall;

	if (psTimeBegin == NULL || psTimeEnd == NULL || psCalMode == NULL || pcSignal == NULL)
	{
		return 0;
	}

	*pcSignal = 0;
	iBegin = PubTime2Int(psTimeBegin);
	iEnd   = PubTime2Int(psTimeEnd);

	if (iEnd >= iBegin)
	{
		*pcSignal = 0;
		iRet = iEnd - iBegin;
		psBig   = psTimeEnd;
		psSmall = psTimeBegin;
	}
	else
	{
		*pcSignal = 1;
		iRet = iBegin - iEnd;
		psBig   = psTimeBegin;
		psSmall = psTimeEnd;
	}


	switch(psCalMode[0])
	{
	case 'Y':
		iRet = PubAsc2Int(psBig, 4, NULL) - PubAsc2Int(psSmall, 4, NULL);
		if (memcmp(psBig + 4, psSmall + 4, 10) < 0)
		{
			iRet--;
		}

		return iRet; //not break but return

	case 'M':
		iRet =  PubAsc2Int(psBig, 4, NULL) * 12 + PubAsc2Int(psBig + 4, 2, NULL) -
		PubAsc2Int(psSmall, 4, NULL) * 12 - PubAsc2Int(psSmall + 4, 2, NULL);
		if (memcmp(psBig + 6, psSmall + 6, 8) < 0)
		{
			iRet--;
		}

		return iRet; //not break but return

	case 'D':
		iRet = iRet / 3600 / 24;
		break;

	case 'h':
		iRet = iRet / 3600;
		break;

	case 'm':
		iRet = iRet / 60;
		break;

	case 's':
		break;

	default:
		break;
	}

	return iRet;
}



// 7. 获取信息

//获取错误信息
char* PubGetLastErrMsg(void)
{
	return "No Err Message";
}

//获取版本信息
char* PubGetVer(void)
{
	return "PUB 1.0.3";
}


#ifdef __cplusplus
};
#endif

