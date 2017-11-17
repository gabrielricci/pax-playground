#include "osal.h"
#include "debugger.h"
#include "font.h"
#include "header.h"
#include "lcd.h"
#include "keyboard.h"
#include "ui_base.h"
#include "Logo.h"
#include "signal.h"

int Print_Receipt(char * pszAmount,char *pszCardNo,int iSignFlag,int iReadCardType)
{
	int ret,fd;
	int dotLine;
	uchar szBuff[2000];
	int ret2;
	int pan_len=0;
	uchar ucSigToPrn[20000];
	int  TransNo = 0;
	int iRet=0;


	time_t tnow;
	struct tm*  mytm;
	unsigned char  tmpbuff[256]={0};
	time(&tnow);
	mytm = localtime(&tnow);
	sprintf(tmpbuff,"%d/%d/%4d                 %d:%d\n",  (mytm->tm_mon+1), mytm->tm_mday,(mytm->tm_year+1900), mytm->tm_hour, mytm->tm_min );

	uchar ucRet= 0;
	uchar buff[128]={0};
	int i = 0;

//	char str0[40]  = "     Retail's Big Show 2015     \n";
//	char str1[40]  = "         New York City,         \n";
//	char str2[40]  = "            New York            \n\n";

//	char str0[40]  = "          TRANSACT 15           \n";
//	char str1[40]  = "         Moscone Center         \n";
//	char str2[40]  = "       San Francisco, CA        \n\n";

	char str0[40]  = "          CARTES 2015           \n";
	char str1[60]  = "        Nord Villepinte\n       Exhibition Center\n";
	char str2[40]  = "         Paris, France          \n";

	char str3[40]  = "    <DEMO MODE>\n";
	char str4[40]  = "    DO NOT CASH\n";
	char str5[40]  = ""; //"%02x/%02x/20%02x   %02x:%02x:%02x\n"
	char str6[40]  = "";//"Merchant ID:           5555555555\n";
	char str7[40]  = "";//"Terminal ID:                  \n";

	char str8[40]  = "Credit Sale: \n";

	char str9[40]  = "";//"Transaction #:                 1\n";
	char str10[40] = "Card Type:              Extended\n";
	char str11[40] = "";//"Account :    **** **** **** ****\n";
	char str12[40] = "";//"Entry:                          \n";

	char str13[40] = "";//"Amount: $1.0\n";

	char str14[40] = "Ref. Number:            88888888\n";
	char str15[40] = "Auth. Code:               000000\n";
	char str16[40] = "Response:               APPROVED\n\n";

	char str17[40] = "   I AGREE TO PAY ABOVE TOTAL   \n";
	char str18[40] = " AMOUNT ACCORDING TO CARD ISSUER\n";
	char str19[40] = "  AGREEMENT (MERCHANT AGREEMENT \n";
	char str20[40] = "      IF CREDIT VOUCHER)        \n";


	memset(szBuff, 0x00, sizeof(szBuff));
/*
	if(OsPrnOpen(PRN_REAL,NULL)!=RET_OK)
	{
		printf("OsPrnOpen fail\n");
		return -1;
	}*/

	ret = OsPrnCheck();
	if (ret != RET_OK)
	{
		return ret;
	}

	OsPrnReset();
	OsPrnSetFont("simsun.ttc");

	OsPrnPutImage(PaxLogo70x32);

	OsPrnFeed(20);

	OsPrnPrintf(str0);
	OsPrnPrintf(str1);
	OsPrnPrintf(str2);

	OsPrnSelectFontSize(20,40,40,40);
	OsPrnPrintf(str3);
	OsPrnPrintf(str4);
	OsPrnSelectFontSize(12,24,24,24);

	//strcpy(str5,tmpbuff);
	OsPrnPrintf(tmpbuff);

	sprintf(str6,"Merchant ID:          5555555555\n");
	OsPrnPrintf(str6);

	memset(buff,0,sizeof(buff));
	OsRegGetValue("ro.fac.sn",buff);
	sprintf(str7,"Terminal ID:            %s\n",buff);
	OsPrnPrintf(str7);

	OsPrnSelectFontSize(20,40,40,40);
	OsPrnPrintf(str8);
	OsPrnSelectFontSize(12,24,24,24);

	sprintf(str9,"Transaction #:            %06ld\n",TransNo); TransNo++;
	OsPrnPrintf(str9);

	OsPrnPrintf(str10);

	pan_len=strlen(pszCardNo);
	sprintf(str11,"Account:                        ");
	strcpy(str11+(32-pan_len),pszCardNo);

	OsPrnPrintf(str11);
	OsPrnPrintf("\n");

	switch (iReadCardType)
	{
	case 1:
		sprintf(str12,"Entry:                    Swiped\n");
		break;
	case 2:
		sprintf(str12,"Entry:                     Taped\n");
		break;
	case 3:
		sprintf(str12,"Entry:                  Inserted\n");
		break;
	default:
		break;
	}
	OsPrnPrintf(str12);


	OsPrnFeed(10);
	sprintf(str13,"Amount: %s\n",pszAmount);
	OsPrnSelectFontSize(20,40,40,40);
	OsPrnPrintf(str13);
	OsPrnSelectFontSize(12,24,24,24);

	OsPrnPrintf(str14);
	OsPrnPrintf(str15);
	OsPrnPrintf(str16);

	OsPrnFeed(10);
	OsPrnPrintf("CARDHOLDER SIGNATURE\n");


	if(iSignFlag == 1 )
	{
		iRet = open("./res/sign.bmp", O_RDONLY);
		if(iRet < 0)
		{
			return -1;
		}
		memset(ucSigToPrn, 0, sizeof(ucSigToPrn));
		iRet = BmpToPrnLogo((unsigned char *)"./res/sign.bmp", 0, ucSigToPrn);
		OsPrnSetIndent(60,0);
		OsPrnPutImage(ucSigToPrn);
		OsPrnSetIndent(0,0);
		OsPrnPrintf("   ------------------------\n");
	}
	else
	{
		OsPrnFeed(60);
		OsPrnPrintf("   ------------------------\n");
	}
	remove("./res/sign.bmp");
	OsPrnPrintf(str17);
	OsPrnPrintf(str18);
	OsPrnPrintf(str19);
	OsPrnPrintf(str20);

	OsPrnFeed(130);

	ret = OsPrnStart();
	OsPrnClose();

	return ret;
}


