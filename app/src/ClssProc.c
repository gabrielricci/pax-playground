/*------------------------------------------------------------
* FileName: ClssProc.c
* Author: jiangjy
* Date: 2015-03-19
------------------------------------------------------------*/
#include "ClssProc.h"
#include "Global.h"
#include "emvlib.h"

//*******************************Global variable*****************************************************//
typedef struct _tagClssData
{
	uchar ucCardType;
	uchar ucSchemeId;

}CLSSDATA;

CLSSDATA sgClssData;

//*****************************************************************************************************//
/********************** external variable declaration *********************/
extern uchar gl_aucOutcomeParamSet[8];
extern uchar gl_aucUserInterReqData[22];
extern uchar gl_aucErrIndication[6];
extern CLSS_DISC_DATA_MC gl_DiscretionayData;
static uchar gl_ucDRLSupportFlg = 1;
//the element flag in discretionary data exist or not


int OsPiccDetect(unsigned char Mode, unsigned char *CardType, unsigned
char *SerialInfo, unsigned char *CID, unsigned char *Other);

int ClssDetectCard(void);
unsigned char PiccIsoCommand(unsigned char cid, APDU_SEND *ApduSend, APDU_RESP *ApduRecv);
static uchar convert_return_code(int iReturnCode);

int ClssTransInit();
void ClssBaseParameterSet_pboc();
// 璇诲彇IC鍗＄閬撲俊鎭��/鍗″彿淇℃伅绛��
int GetCLSSTrackData(unsigned char PathType,unsigned char kernType,char *pszCardNo);



static uchar convert_return_code(int iReturnCode)
{
	uchar ucRet;

	switch(iReturnCode)
	{
		case 0:
			ucRet = 0x00;
			break;

		case PCD_ERR_PAR_FLAG:
		case PCD_ERR_CRC_FLAG:
		case PCD_ERR_ECD_FLAG:
			ucRet = 0x06;
			break;

		case PCD_ERR_WTO_FLAG:
			ucRet = 0x03;		// time out
//			ucRet = 0x15;		// not found
			break;

		case PCD_ERR_COLL_FLAG:
			ucRet = 0x04;
			break;

		case PCD_ERR_EMD_FLAG:
		case PCD_ERR_COM_FLAG:
		case PCD_ERR_TRANSMIT_FLAG:
		case PCD_ERR_CARRIER_OBTAIN_FLAG:
			ucRet = 0x17;
			break;

		case PCD_ERR_AUT_FLAG:
			ucRet = 0x18;
			break;

		case PCD_ERR_PROTOCOL_FLAG:
			ucRet = 0x16;
			break;

		case PCD_ERR_PARAMFILE_FLAG:
		case PCD_ERR_USER_CANCEL:
		case PCD_ERR_CONFIG_FLAG:
			ucRet = iReturnCode;
			break;

		case PCD_ERR_NOT_ALLOWED_FLAG:
			ucRet = 0x01;
			break;

		case PCD_CHIP_ABNORMAL:
			ucRet = 0xFF;
			break;

		case PCD_CHIP_NOT_OPENED:
			ucRet = 0x02;
			break;

		case PCD_CHIP_CARDEXIST:
			ucRet = 0x1B;
			break;

		case PCD_ERR_NOT_IDLE_FLAG:
		case PCD_ERR_NOT_POLLING_FLAG:
		case PCD_ERR_NOT_WAKEUP_FLAG:
		case PCD_ERR_NOT_ACTIVE_FLAG:
			ucRet = 0x13;
			break;

		default:
			ucRet = iReturnCode;
			break;
	}

	return ucRet;
}


//////////////////////////callback functions/////////////////////////////////////



uchar cPiccIsoCommand_Wave(uchar cid,APDU_SEND *ApduSend,APDU_RESP *ApduRecv)
{
	return PiccIsoCommand(cid, ApduSend, ApduRecv);
}
int  cClssGetUnknowTLVData_Wave (unsigned short usTag, unsigned char *pucData, int nLen)
{
	//return PiccIsoCommand(cid, ApduSend, ApduRecv);
	return 0;
}
unsigned char cPiccIsoCommand_MC(uchar cid, APDU_SEND *ApduSend, APDU_RESP *ApduRecv)
{
	return PiccIsoCommand(cid, ApduSend, ApduRecv);
}

int cClssCheckExceptionFile_Pboc(uchar *pucPAN, int nPANLen, uchar *pucPANSeq)
{
	return EMV_OK;
}

unsigned char PiccIsoCommand(unsigned char cid, APDU_SEND *ApduSend, APDU_RESP *ApduRecv)
{
	int iRet;
	ST_APDU_REQ ApduReq;
	ST_APDU_RSP ApduRsp;


	if(cid < 0 || cid > 14)
	{
		OsLog(LOG_DEBUG, "PiccIsoCommand:cid error=%d", cid);
		return convert_return_code(PCD_ERR_NOT_ALLOWED_FLAG);
	}

	if(ApduSend == NULL || ApduRecv == NULL)
	{
		OsLog(LOG_DEBUG, "PiccIsoCommand:p2 or p3 is NULL");
		return convert_return_code(PCD_ERR_NOT_ALLOWED_FLAG);
	}

	memset(&ApduReq, 0, sizeof(ST_APDU_REQ));
	memset(&ApduRsp, 0, sizeof(ST_APDU_RSP));

 	memcpy(ApduReq.Cmd, ApduSend->Command, 4);

	ApduReq.LC = ApduSend->Lc;
	ApduReq.LE = ApduSend->Le;

	if(ApduSend->Lc != 0)
	{
		ApduReq.LC = ApduSend->Lc;
		memcpy(ApduReq.DataIn, ApduSend->DataIn, ApduSend->Lc);
	}

	iRet = OsPiccIsoCommand(cid, &ApduReq, &ApduRsp);
	if(iRet != 0)
	{
		OsLog(LOG_DEBUG, "PiccIsoCommand:OsPiccIsoCommand return:%d", iRet);
		OsLog(LOG_DEBUG, &ApduReq, sizeof(ST_APDU_REQ));
		return convert_return_code(iRet);
	}

	memcpy(ApduRecv->DataOut, ApduRsp.DataOut, sizeof(ApduRsp.DataOut));
	ApduRecv->LenOut = ApduRsp.LenOut;
	ApduRecv->SWA = ApduRsp.SWA;
	ApduRecv->SWB = ApduRsp.SWB;

	///////////////////////////////////////////////////////////////////
	{
		char szLogBuf[1024];
		int iLogCnt;
#define LINEMAXNUM	30
		int iCurLineNum;

		OsLog(LOG_DEBUG, "PICC_IsoCommand");
		for(iLogCnt = 0; iLogCnt < 4; iLogCnt++)
		{
			sprintf(szLogBuf+iLogCnt*3, "%02X ", *(ApduSend->Command+iLogCnt));
		}
		if (ApduSend->Lc != 0)
		{

			iCurLineNum = 0;

			sprintf(szLogBuf+4*3, "%02X ", (char)ApduSend->Lc);
			for(iLogCnt = 0; iLogCnt < ApduSend->Lc; iLogCnt++)
			{
				sprintf(szLogBuf+(iLogCnt+5)*3+iCurLineNum, "%02X ", *(ApduSend->DataIn+iLogCnt));
				if ((iLogCnt+5)%LINEMAXNUM == 0)
				{
					sprintf(szLogBuf+(iLogCnt+5+1)*3+iCurLineNum, "\n");
					iCurLineNum++;
				}
			}
			if (ApduSend->Le != 0)
			{
				sprintf(szLogBuf+(5+ApduSend->Lc)*3+iCurLineNum, "%02X ", (char)ApduSend->Le);
			}
		}
		else
		{
			sprintf(szLogBuf+4*3, "%02X ", (char)ApduSend->Le);
		}
		OsLog(LOG_DEBUG, "POS-->PICC CARD[%s]", szLogBuf);

		iCurLineNum = 0;
		for(iLogCnt = 0; iLogCnt < ApduRecv->LenOut; iLogCnt++)
		{
			sprintf(szLogBuf+iLogCnt*3+iCurLineNum, "%02X ", *(ApduRecv->DataOut+iLogCnt));
			if (iLogCnt%LINEMAXNUM == 0 && iLogCnt != 0)
			{
				sprintf(szLogBuf+(iLogCnt+1)*3+iCurLineNum, "\n");
				iCurLineNum++;
			}
		}
		sprintf(szLogBuf+iLogCnt*3+iCurLineNum, "%02X ", ApduRecv->SWA);
		sprintf(szLogBuf+(iLogCnt+1)*3+iCurLineNum, "%02X ", ApduRecv->SWB);
		OsLog(LOG_DEBUG, "POS<--PICC CARD[%s]", szLogBuf);
	}
	///////////////////////////////////////////////////////////////////

	return 0;
}


unsigned char cPiccIsoCommand_Entry(uchar cid, APDU_SEND *ApduSend, APDU_RESP *ApduRecv)
{
	return PiccIsoCommand(cid, ApduSend, ApduRecv);
}

int OsPiccDetect(unsigned char Mode, unsigned char *CardType, unsigned
char *SerialInfo, unsigned char *CID, unsigned char *Other)
{
     int iRet, iLen;
     char cPiccType;
     unsigned char ucTempBuf[300];
     unsigned char ucATQx[32], ucUID[32], ucSAK[32], ucRATS[32];
     unsigned char *pucATS = NULL;

     //闂佽法鍠庤ぐ銊ф媼鐟欏嫬顏堕梺璺ㄥ枑閺嬪骞忛悜鑺ユ櫢闁哄倶鍊栫��氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾绘晸閿燂拷1锟��     if( Mode != 0x00 && Mode != 0x01
     if( Mode != 0x00 && Mode != 0x01
        && Mode != 'A' && Mode != 'a'
        && Mode != 'B' && Mode != 'b'
        && Mode != 'M' && Mode != 'm'
       )
     {
         return PCD_ERR_NOT_ALLOWED_FLAG;
     }


     if(CID != NULL)
     {
         CID = 0;
     }

     iLen = 0;
     cPiccType = 0;
     memset(ucATQx, 0, sizeof(ucATQx));
     memset(ucTempBuf, 0, sizeof(ucTempBuf));
     iRet = OsPiccPoll(&cPiccType, ucATQx);
     if(iRet != 0)
     {
         return iRet;
     }

     iLen = strlen((char *)ucATQx);
     ucTempBuf[1] = iLen;
     iLen += 1;
     ucTempBuf[0] = iLen;
     memcpy(&ucTempBuf[2], ucATQx, iLen);

     if(Other != NULL)
     {
         memcpy(Other, ucTempBuf, strlen((char *)ucTempBuf));
     }

     //Grace
     OsLog(LOG_DEBUG, "cPiccType");
     OsLog(LOG_DEBUG, "cPiccType=%02x",cPiccType);



     if(cPiccType == 'M')
     {

         if(Mode == 'M' || Mode == 'm')
         {
             if(CardType != NULL)
             {
                 *CardType = 'M';
             }
         }
         else
         {
             if(CardType != NULL)
             {
                 *CardType = cPiccType;
             }

             return PCD_ERR_NOT_ALLOWED_FLAG;
         }
     }
     else if(cPiccType == 'B')
     {

         if(Mode == 0x00 || Mode == 0x01 || Mode == 'B' || Mode == 'b')
         {
             if(CardType != NULL)
             {
                 *CardType = 'B';
             }
         }
         else
         {
             if(CardType != NULL)
             {
                 *CardType = cPiccType;
             }

             return PCD_ERR_NOT_ALLOWED_FLAG;
         }
     }
     else if(cPiccType == 'A')
     {

         if(Mode == 0x00 || Mode == 0x01 || Mode == 'A' || Mode == 'a')
         {
             if(CardType != NULL)
             {
                 *CardType = 'A';
             }
         }
         else
         {
             if(CardType != NULL)
             {
                 *CardType = cPiccType;
             }

             return PCD_ERR_NOT_ALLOWED_FLAG;
         }
     }
     else
     {
         if(CardType != NULL)
         {
             *CardType = cPiccType;
         }

         return PCD_ERR_NOT_ALLOWED_FLAG;
     }

     iRet = OsPiccAntiSel(cPiccType, ucUID, ucATQx[0], ucSAK);
     if(iRet != 0)
     {
    	 OsLog(LOG_INFO, "OsPiccAntiSel iRet=%d",iRet);

         return iRet;
     }


     iLen += 1;
     ucTempBuf[iLen] = strlen((char *)ucSAK);
     iLen += strlen((char *)ucSAK);;
     ucTempBuf[0] = iLen;
     strcat((char *)ucTempBuf, (char *)ucSAK);

     if(Other != NULL)
     {
         memcpy(Other, ucTempBuf, strlen((char *)ucTempBuf));
     }

     if(SerialInfo != NULL)
     {
         SerialInfo[0] = strlen((char *)ucUID);
         memcpy(&SerialInfo[1], ucUID, strlen((char *)ucUID));
     }

     if(Mode == 'M' || Mode == 'm')
     {

         return 0;
     }

     if(iRet != 0)
     {
         return iRet;
     }


     iLen += 1;
     ucTempBuf[iLen] = strlen((char *)ucSAK);
     iLen += strlen((char *)ucSAK);
     ucTempBuf[0] = iLen;
     strcat((char *)ucTempBuf, (char *)ucRATS);

     if(Other != NULL)
     {
         memcpy(Other, ucTempBuf, strlen((char *)ucTempBuf));
     }


     OsLog(LOG_INFO, "cPiccType %02x",cPiccType);
     iRet =  OsPiccActive(cPiccType,pucATS);

      if(iRet != 0)
         {
             return iRet;
         }

     return 0;
}

unsigned char  cPiccIsoCommand_Pboc (unsigned char cid,APDU_SEND *ApduSend,APDU_RESP *ApduRecv)
{
	return PiccIsoCommand(cid, ApduSend, ApduRecv);
}
//SM2閺佺増宓佹宀冪槈
unsigned char cClssSM2Verify_Pboc(unsigned char *paucPubkeyIn,unsigned char *paucMsgIn,int nMsglenIn,unsigned char *paucSignIn, int nSignlenIn)
{
	//TO DO
	return 0;
}

int cClssGetUnknowTLVData_Pboc(unsigned short usTag, unsigned char *pucData, int nLen)
{
	//TO DO
	return 0;
}


int ClssDetectCard(void)
{
	int	 iRet, iTime,  iLen, i;
	unsigned char ucKey, ucTemp[100], iType, iAcType, iFlag;
	unsigned short tag;
	ST_TIMER Timer;



    //set timer here

	printf("-----------------DETECT CARD--------------------!\n");
            //tap card
            OsPiccClose();
            while(1)
            {

                iRet = OsPiccOpen();
                if(iRet != 0)
                {
                	printf("-----------------PICC OPEN FAIL--------------------!\n");
                    OsPiccClose();
                    //callback to display,beep

                    return ERR_NEED_INSERT;
                }
               	OsTimerSet(&Timer,10);
                while(1)
                {
                	//Grace
                   // OsLog(LOG_DEBUG, "while OsPiccDetect  ");

                	iRet = OsPiccDetect(0, NULL, NULL, NULL, NULL);
                	OsLog(LOG_DEBUG, "OsPiccDetect iret = %d ",iRet);
                    if(0 == iRet)
                    {
                        break;
                    }
                    else if(OsTimerCheck(&Timer)<=0)//modify
                    {
                    	OsLog(LOG_DEBUG, "time out ");
                    	return -1;
                    }
                    else if(PCD_ERR_WTO_FLAG == iRet)//-2903
                    {
                    	continue;
                    }
                    else if(PCD_ERR_COLL_FLAG == iRet)
                    {
                        OsPiccRemove();
                        OsPiccClose();

                        return ERR_NEED_RETRY;
                    }

                    else
                    {
                        OsPiccRemove();
                        OsPiccClose();
                        OsPiccOpen();
                        continue;
                    }
                }
                break;
            }
            printf("-----------------contactless OK --------------------!\n");

            return 0;
  }
//////////////////////////////PRE-PROCESS//////////////////////////////////////////////////////////

void ClssBaseParameterSet_wave()
{
	Clss_ReaderParam ClssParam;
	Clss_VisaAidParam ClssVisaAidParam;
	Clss_SchemeID_Info atSchemeInfo[6]={0};

    //璁剧疆璇诲崱鍣ㄥ簲鐢ㄧ浉鍏冲弬鏁��
	Clss_GetReaderParam_Wave(&ClssParam);
	memcpy(ClssParam.aucTmCap,"\xE0\xe1\xC8",3);
	memcpy(ClssParam.aucTmCapAd,"\xe0\x00\xf0\xa0\x01",5);
	ClssParam.ucTmType = 0x22;

	memcpy(ClssParam.aucTmCntrCode, "\x08\x40", 2);
	memcpy(ClssParam.aucTmRefCurCode,"\x08\x40", 2);
	memcpy(ClssParam.aucTmTransCur, "\x08\x40", 2);
	OsLog(LOG_DEBUG,"Clss_SetReaderParam_Wave begin");
//	Clss_SetReaderParam_Wave(&ClssParam);
	OsLog(LOG_DEBUG,"Clss_SetReaderParam_Wave succ");

	// add for paywave schemeinfor [2/9/2015 jiangjy]
	memset(atSchemeInfo, 0, sizeof(atSchemeInfo));
	atSchemeInfo[0].ucSchemeID = SCHEME_VISA_MSD_20;
	atSchemeInfo[0].ucSupportFlg = 1;
	atSchemeInfo[1].ucSchemeID = SCHEME_VISA_WAVE_3;
	atSchemeInfo[1].ucSupportFlg = 1;

	atSchemeInfo[2].ucSchemeID = SCHEME_VISA_WAVE_2;// VISA AP WAVE2
	atSchemeInfo[2].ucSupportFlg = 1;
	atSchemeInfo[3].ucSchemeID = SCHEME_JCB_WAVE_2;// VISA AP WAVE2
	atSchemeInfo[3].ucSupportFlg = 1;

	Clss_SetRdSchemeInfo_Wave(4, atSchemeInfo);

}


void ClssBaseParameterSet_pboc()
{
	int iRet;
	Clss_ReaderParam ClssReaderParam;
	 OsLog(LOG_DEBUG,"4 \n");
	//璁剧疆璇诲崱鍣ㄥ簲鐢ㄧ浉鍏冲弬鏁��
	Clss_GetReaderParam_Pboc(&ClssReaderParam);
	memcpy(ClssReaderParam.aucTmCap,"\xE0\xe1\x40",3);
	memcpy(ClssReaderParam.aucTmCapAd,"\xe0\x00\xf0\xa0\x01",5);
	ClssReaderParam.ucTmType = 0x22;
	memcpy(ClssReaderParam.aucTmCntrCode, "\x01\x56", 2);
	memcpy(ClssReaderParam.aucTmRefCurCode, "\x01\x56", 2);
	memcpy(ClssReaderParam.aucTmTransCur, "\x01\x56", 2);
	 OsLog(LOG_DEBUG,"6 \n");
	iRet = Clss_SetReaderParam_Pboc(&ClssReaderParam);
	OsLog(LOG_DEBUG,"Clss_SetReaderParam_Pboc iret =%d \n",iRet);
	OsLog(LOG_DEBUG,"7 \n");
}




int ClssTransInit()
{
	int i, iRet;
	EMV_APPLIST EMV_APP;
	Clss_PreProcInfo Clss_PreProcInfoIn;

	Clss_SetCBFun_SendTransDataOutput_MC(cClssSendTransDataOutput_MC);
	Clss_DelAllAidList_Entry();
	Clss_DelAllPreProcInfo();
	vAppInitSchemeId();
	OsLog(LOG_DEBUG,"before  for loop \n");
	for (i=0; i<MAX_APP_NUM; i++)
	{
		memset(&EMV_APP, 0, sizeof(EMV_APPLIST));
		iRet = EMVGetApp(i, &EMV_APP);
		if(iRet != EMV_OK)
		{
			continue;
		}

		iRet = Clss_AddAidList_Entry(EMV_APP.AID, EMV_APP.AidLen, EMV_APP.SelFlag, KERNTYPE_DEF);
		if(iRet != EMV_OK)
		{
			continue;
		}

		memset(&Clss_PreProcInfoIn, 0, sizeof(Clss_PreProcInfo));
		Clss_PreProcInfoIn.ulTermFLmt = 110000;
		Clss_PreProcInfoIn.ulRdClssTxnLmt = 1000000000;
		Clss_PreProcInfoIn.ulRdCVMLmt = 10000;
		Clss_PreProcInfoIn.ulRdClssFLmt = 110000;

		memcpy(Clss_PreProcInfoIn.aucAID, EMV_APP.AID, EMV_APP.AidLen);
		Clss_PreProcInfoIn.ucAidLen = EMV_APP.AidLen;

		Clss_PreProcInfoIn.ucKernType = KERNTYPE_DEF;

		Clss_PreProcInfoIn.ucCrypto17Flg = 1;
		Clss_PreProcInfoIn.ucZeroAmtNoAllowed = 0;
		Clss_PreProcInfoIn.ucStatusCheckFlg = 0;
		memcpy(Clss_PreProcInfoIn.aucReaderTTQ, "\x36\x00\x00\x80", 4);//36:onlin pin & signature;  Qvsdc38:34 online pin;  Qvsdc39:32 signature;  // 9f66 QBOC B4b8 =1 FDDA SUPPORTED [2/11/2015 jiangjy]

		Clss_PreProcInfoIn.ucTermFLmtFlg = 1;
		Clss_PreProcInfoIn.ucRdClssTxnLmtFlg = 1;
		Clss_PreProcInfoIn.ucRdCVMLmtFlg = 1;
		Clss_PreProcInfoIn.ucRdClssFLmtFlg=1;
		Clss_SetPreProcInfo_Entry(&Clss_PreProcInfoIn);
	}
	 OsLog(LOG_DEBUG,"before  for InitPbocAid_clss \n");
	InitPbocAid_clss();
	 OsLog(LOG_DEBUG,"1 \n");
	ClssBaseParameterSet_wave();
	 OsLog(LOG_DEBUG,"2 \n");
	//ClssBaseParameterSet_pboc();
	 OsLog(LOG_DEBUG,"3 \n");
	Clss_SetMCVersion_Entry(3);

	OsLog(LOG_DEBUG,"before memset  ClssTransParam \n");
	//pre-process
	memset(&ClssTransParam, 0, sizeof(Clss_TransParam));
	ClssTransParam.ulAmntAuth = 500;
	ClssTransParam.ulAmntOther = 0;
	ClssTransParam.ucTransType = 0x00;//0x20:refund


	 OsLog(LOG_DEBUG,"set ClssTransParam \n");
	//settime here
	 memcpy(ClssTransParam.aucTransDate,"\x11\x01\x01",3);
	 memcpy(ClssTransParam.aucTransTime,"\x11\x01\x01",3);
	//PubAsc2Bcd(glProcInfo.stTranLog.szDateTime+2, 6, ClssTransParam.aucTransDate);
	//PubAsc2Bcd(glProcInfo.stTranLog.szDateTime+8, 6, ClssTransParam.aucTransTime);

	ClssTransParam.ulTransNo = 1;
	iRet = Clss_PreTransProc_Entry(&ClssTransParam);
	OsLog(LOG_DEBUG,"Clss_PreTransProc_Entry iret = %d\n",iRet);

	if(iRet != EMV_OK)
	{
		if (iRet == CLSS_USE_CONTACT)
		{
			return 0;
		}
		return iRet;
	}

}


// 璇诲彇IC鍗＄閬撲俊鎭��/鍗″彿淇℃伅绛��
int GetCLSSTrackData(unsigned char PathType,unsigned char kernType,char *pszCardNo)
{
	int		iRet, iLength;
	uchar	sTemp[50], szCardNo[20+1];
	int		i, bReadTrack2, bReadPan;

	uchar szExpDate[8];

	memset(szTrack2,0,sizeof(szTrack2));

	// Read Track 2 Equivalent Data
	bReadTrack2 = 0;
	memset(sTemp, 0, sizeof(sTemp));
	OsLog(LOG_INFO, "enter GetCLSSTrackData ");

	if(kernType == KERNTYPE_MC)
	{
		if(PathType == CLSS_MC_MAG)
		{
			iRet = Clss_GetTLVDataList_MC("\x9F\x6B",2, sizeof(sTemp),sTemp, &iLength);
			OsLog(LOG_DEBUG, " 9F6B Iret = %d ",iRet);
		}
		else if (PathType == CLSS_MC_MCHIP)
		{
			iRet = Clss_GetTLVDataList_MC("\x57",1, sizeof(sTemp),sTemp, &iLength);
			OsLog(LOG_DEBUG, " 57 iret = %d ",iRet);
		}

		if( iRet==EMV_OK )
		{
			bReadTrack2 = 1;
			PubBcd2Asc0(sTemp, iLength, szTrack2);
			PubTrimTailChars(szTrack2, 'F');	// erase padded 'F' chars
			for(i=0; szTrack2[i]!='\0'; i++)		// convert 'D' to '='
			{
				if( szTrack2[i]=='D' )
				{
					szTrack2[i] = '=';
					break;
				}
			}
		}

		// read PAN
		bReadPan = 0;
		memset(sTemp, 0, sizeof(sTemp));
		iRet = Clss_GetTLVDataList_MC("\x5A",1, sizeof(sTemp),sTemp, &iLength);
		OsLog(LOG_DEBUG, " 5A Iret = %d ",iRet);
		if( iRet==EMV_OK )
		{
			PubBcd2Asc0(sTemp, iLength, szCardNo);
			PubTrimTailChars(szCardNo, 'F');		// erase padded 'F' chars
			if( bReadTrack2 && !MatchTrack2AndPan(szTrack2, szCardNo) )
			{
				// 濡傛灉Track2 & PAN 鍚屾椂瀛樺湪,鍒欏繀椤诲尮閰��
				// if Track2 & PAN exist at the same time, must match
				//PubDispString(_T("CARD ERROR"), 4|DISP_LINE_LEFT);
				//PubBeepErr();
				//PubWaitKey(3);
				//PromptRemoveICC();
				return ERR_NO_DISP;
	//			return ERR_TRAN_FAIL;
			}
			sprintf((char *)pszCardNo, "%.19s", szCardNo);
			bReadPan = 1;
		}
		else if ( !bReadTrack2 )	// 濡傛灉Track 2 鍜�� PAN 閮芥病鏈��,鍒欎氦鏄撳け璐��
		{
			return ERR_TRAN_FAIL;
		}
		if( !bReadPan )	// 娌℃湁璇诲彇PAN锛屼絾鏄湁track 2
		{
			iRet = GetPanFromTrack_1(pszCardNo, szExpDate);
			OsLog(LOG_DEBUG, " GetPanFromTrack_1 Iret = %d ",iRet);
			if( iRet!=0 )
			{
				return ERR_TRAN_FAIL;
			}
		}
#ifdef DEBUG
		OsLog(LOG_DEBUG, " cAccount = ");
		OsLog(LOG_DEBUG, "%s ",pszCardNo);
//		for(i = 0;i<20;i++)
//		{
//			OsLog(LOG_DEBUG, "%c ",&cAccount[i]);
//		}
#endif
		return 0;
	}
	else if(kernType == KERNTYPE_VIS)
	{
		if(PathType == CLSS_VISA_MSD||PathType ==CLSS_VISA_WAVE2)
		{
			iRet = Clss_nGetTrack2MapData_Wave(szTrack2, &iLength);
			OsLog(LOG_DEBUG, " Clss_nGetTrack2MapData_Wave Iret = %d ",iRet);
		}
		else if (PathType == CLSS_VISA_QVSDC)
		{
			iRet = Clss_GetTLVData_Wave(0X57,sTemp, &iLength);
			OsLog(LOG_DEBUG, " 57 Iret = %d ",iRet);
		}

		if( iRet==EMV_OK )
		{
			bReadTrack2 = 1;
			PubBcd2Asc0(sTemp, iLength, szTrack2);
			PubTrimTailChars(szTrack2, 'F');	// erase padded 'F' chars
			for(i=0; szTrack2[i]!='\0'; i++)		// convert 'D' to '='
			{
				if( szTrack2[i]=='D' )
				{
					szTrack2[i] = '=';
					break;
				}
			}
		}

		// read PAN
		bReadPan = 0;
		memset(sTemp, 0, sizeof(sTemp));
		iRet = Clss_GetTLVData_Wave(0x5A,sTemp, &iLength);
		OsLog(LOG_DEBUG, " 5A Iret = %d ",iRet);
		if( iRet==EMV_OK )
		{
			PubBcd2Asc0(sTemp, iLength, szCardNo);
			PubTrimTailChars(szCardNo, 'F');		// erase padded 'F' chars
			if( bReadTrack2 && !MatchTrack2AndPan(szTrack2, szCardNo) )
			{
				return ERR_NO_DISP;
			}
			sprintf((char *)pszCardNo, "%.19s", szCardNo);
			bReadPan = 1;
		}
		else if ( !bReadTrack2 )	// 濡傛灉Track 2 鍜�� PAN 閮芥病鏈��,鍒欎氦鏄撳け璐��
		{
			return ERR_TRAN_FAIL;
		}
		if( !bReadPan )	// 娌℃湁璇诲彇PAN锛屼絾鏄湁track 2
		{
			iRet = GetPanFromTrack_1(pszCardNo, szExpDate);
			OsLog(LOG_DEBUG, " GetPanFromTrack_1 Iret = %d ",iRet);
			if( iRet!=0 )
			{
				return ERR_TRAN_FAIL;
			}
		}
#ifdef DEBUG
		OsLog(LOG_DEBUG, " cAccount = ");

		OsLog(LOG_DEBUG, "%s ",pszCardNo);

#endif
		return 0;
	}

}


int ClssProcTrans(char *pszCardNo)
{
	int	 iRet, iTime,   i;
	uchar ucKey, ucTemp[200], ucTranAct;
	unsigned short tag;



	//PICC_PARA PiccPara;
	//Clss_TransParam ClssTransParam;
	Clss_PreProcInterInfo ClssProcInterInfo;
	uchar ucKernType;//kernel type
	uchar ucAcType;
	uchar ucPathType;
	uchar aucAid[17],aucProID[17];
	int nProIDLen = 0;

	uchar ucOn = 0;
	uchar ucRet = 0, ucRetContact=0;
	uchar ucResend = 0;


    int iLen = 0 ;//DF69
    Clss_ReaderParam ptParam;

	//modify for PAYPASS 3.0
	Clss_VisaAidParam tVisaAidParam = {0};
	CLSS_OUTCOME_DATA stOutComeData;

	uchar Dataout[5];//for test
	uint iDataoutLen;//for test

	memset(&stOutComeData, 0, sizeof(CLSS_OUTCOME_DATA));

	OsLog(LOG_DEBUG,"ENTER Clss_AppSlt_Entry");
 	//app select
 	iRet = Clss_AppSlt_Entry(0,0);
 	if (iRet)
 	{
 		vAppCreateOutcomeData_MC(iRet);
 	}
 	if(iRet == EMV_NO_APP || iRet == ICC_BLOCK || iRet == EMV_APP_BLOCK)
 	{
 		PiccClose();
 		return iRet;
 	}
 	if(iRet != EMV_OK)
 	{
 		PiccClose();
 		return ERR_TRAN_FAIL;
 	}


	while(2)
	{
		vAppInitPaymentData_MC();
		//final select
		ucKernType = 0;
		iLen = 0;
		memset(ucTemp, 0, sizeof(ucTemp));
		OsLog(LOG_DEBUG,"ENTER Clss_FinalSelect_Entry");

		iRet = Clss_FinalSelect_Entry(&ucKernType, ucTemp, &iLen);
		OsLog(LOG_DEBUG,"ENTER Clss_FinalSelect_Entry iret = %d",iRet);
		if(iRet == EMV_NO_APP || iRet == CLSS_USE_CONTACT)
		{
			PiccClose();// modify v1.00.0018  [2/10/2015 jiangjy]
			return iRet;
		}
		if(iRet != EMV_OK)
		{
			PiccClose();// modify v1.00.0018  [2/10/2015 jiangjy]
            return ERR_TRAN_FAIL;
		}


		if(ucKernType != KERNTYPE_VIS && ucKernType != KERNTYPE_MC &&ucKernType != KERNTYPE_PBOC) //modify pboc[2/10/2015 jiangjy]
		{
			PiccClose();// modify v1.00.0018  [2/10/2015 jiangjy]
			return ERR_TRAN_FAIL;
		}

		if(ucKernType == KERNTYPE_MC)
		{
			uchar tmp[3];
			memset(tmp,0,sizeof(tmp));
			memcpy(tmp,"\x9C\x01",2);
			memcpy(&tmp[2],&ClssTransParam.ucTransType,1);
			Clss_SetTLVDataList_MC(tmp,3);
		}
		else if (ucKernType == KERNTYPE_VIS)
		{
			Clss_SetTLVData_Wave(0x9c, &ClssTransParam.ucTransType, 1);
		}
/*		else if (ucKernType == KERNTYPE_PBOC)
		{
			Clss_SetTLVData_Pboc(0x9c, &ClssTransParam.ucTransType, 1);
		}*/
		sgClssData.ucCardType = ucKernType;

		//get pre-process data
		memset(&ClssProcInterInfo, 0, sizeof(Clss_PreProcInterInfo));
		iRet = Clss_GetPreProcInterFlg_Entry(&ClssProcInterInfo);
		if(iRet == CLSS_USE_CONTACT )
		{
			PiccClose();// modify v1.00.0018  [2/10/2015 jiangjy]
			return iRet;
		}
		if(iRet != EMV_OK)
		{
			PiccClose();// modify v1.00.0018  [2/10/2015 jiangjy]
			return ERR_TRAN_FAIL;
		}

		//get final-setect data
		iLen = 0;
		memset(ucTemp, 0, sizeof(ucTemp));
		iRet = Clss_GetFinalSelectData_Entry(ucTemp, &iLen);
		if(iRet != EMV_OK)
		{
			PiccClose();// modify v1.00.0018  [2/10/2015 jiangjy]
			return ERR_TRAN_FAIL;
		}

		iRet = nAppSetCurAppType(ucKernType);//CoreInit  //kernel type// scheme id

		if(ucAppGetAppType() == KERNTYPE_VIS)
		{
#ifdef CLSS_DEBUG
Clss_SetDebug_Wave(1);
#endif

			//璁剧疆WAVE搴旂敤鐩稿叧鍙傛暟
			memset(&tVisaAidParam,0,sizeof(Clss_VisaAidParam));
			tVisaAidParam.ucCvmReqNum = 2;
			tVisaAidParam.aucCvmReq[0] = RD_CVM_REQ_SIG;
			tVisaAidParam.aucCvmReq[1] = RD_CVM_REQ_ONLINE_PIN;

			tVisaAidParam.ucDomesticOnly = 0x00; // 01(default):only supports domestic cl transaction
			tVisaAidParam.ucEnDDAVerNo = 0;// fDDA ver 00 & 01 are all supported
			tVisaAidParam.ulTermFLmt = 10000;
			Clss_SetVisaAidParam_Wave(&tVisaAidParam);
			vSetFinalSelectAID(ucTemp+1, ucTemp[0]);
			iRet = Clss_SetFinalSelectData_Wave(ucTemp, iLen);
			if(iRet != EMV_OK)
			{
				PiccClose();
				return ERR_TRAN_FAIL;
			}

			//if DRL supported, process DRL   program id  [1/14/2015 jiangjy] if DRL supported
// 			if ( gl_ucDRLSupportFlg == 1)
// 			{
// 				if (Clss_GetTLVData_Wave(0x9F5A, aucProID, &nProIDLen) == 0)
// 			}

			iRet = Clss_SetTransData_Wave(&ClssTransParam, &ClssProcInterInfo);
#ifdef DEBUG
			OsLog(LOG_DEBUG," Clss_SetTransData_Wave iRet= %d",iRet);
#endif
			if(iRet != EMV_OK)
			{
				PiccClose();
				return ERR_TRAN_FAIL;
			}

			ucAcType= 0;
			memset(ucTemp, 0, sizeof(ucTemp));
			iRet = Clss_Proctrans_Wave(ucTemp, &ucAcType);
#ifdef DEBUG
			OsLog(LOG_DEBUG,"Clss_Proctrans_Wave iRet= %d",iRet);
#endif
			sgClssData.ucSchemeId = ucTemp[0];//  [2/9/2015 jiangjy]save for upload, customer implement this
			ucPathType = ucTemp[0];
			if (iRet == CLSS_RESELECT_APP) // GPO
			{
				iRet = Clss_DelCurCandApp_Entry();
				if (iRet)
				{
					//disp_clss_err(iRet);//not sure
					PiccClose();
					return ERR_TRAN_FAIL;
				}
				continue;
			}
			//see phone
			else if (ucKernType == KERNTYPE_VIS && iRet == CLSS_REFER_CONSUMER_DEVICE &&
				((ClssProcInterInfo.aucReaderTTQ[0] & 0x20) == 0x20))
			{

				//Inter_DisplayMsg(Msg_SEE_PHONE);
				iRet= App_Try_Again;

				//vAppDisplayMsgByRet(iRet);
				PiccClose();
				OsSleep(1200);
				//goto DETECTCARD;
				return App_Try_Again;//鏄惁鍙互缁х画璇诲崱鍙��

			}
			else
			{
				vAppSetTransPath(ucPathType);
				//read pan
				iRet = GetCLSSTrackData(ucPathType,ucKernType,pszCardNo);
#ifdef DEBUG
                OsLog(LOG_DEBUG,"GetCLSSTrackData iret = %d",iRet);
#endif
				return iRet;
			}

		}

		else if (ucAppGetAppType() == KERNTYPE_MC)
		{
#ifdef CLSS_DEBUG
Clss_SetDebug_MC(1);
#endif
            vSetFinalSelectAID(ucTemp+1, ucTemp[0]);
			SetClssTxnParam(&ClssTransParam);
			SetTermParam_MC();
			iRet = Clss_SetFinalSelectData_MC(ucTemp, iLen);
			// 杩斿洖鍊肩殑鍒ゆ柇锛孖f the return code is not EMV_OK, Application should get DF8129). [12/29/2014 jiangjy]
			if(iRet == CLSS_RESELECT_APP)
			{
				iRet = Clss_DelCurCandApp_Entry();
				if (iRet != 0)
				{
					vInitPaymentData();
					gl_aucUserInterReqData[0]=MI_ERROR_OTHER_CARD;
					gl_aucUserInterReqData[1]=MI_NOT_READY;
					memcpy(gl_aucUserInterReqData+2, MSG_HOLD_TIME_VALUE, 3);

					gl_aucErrIndication[5]=MI_ERROR_OTHER_CARD;
					gl_aucErrIndication[1] = L2_EMPTY_CANDIDATE_LIST;//S51.11
					gl_aucOutcomeParamSet[0] = OC_END_APPLICATION;
					gl_DiscretionayData.ucErrIndicFlg = 1;
					//					nSendTransDataOutput_MC(T_UIRD | T_OCPS | T_DISD);//S51.11 S51.12
					break;
				}
				continue;
			}
			else if(iRet)
			{
				//disp_clss_err(iRet);
				PiccClose();
				return ERR_TRAN_FAIL;
			}
			iRet = Clss_InitiateApp_MC();
			//杩斿洖鍊肩殑鍒ゆ柇 杩斿洖鍊肩殑鍒ゆ柇锛孖f the return code is not EMV_OK, Application should get DF8129)
			if (iRet == CLSS_RESELECT_APP) // GPO
			{
				iRet = Clss_DelCurCandApp_Entry();
				if (iRet)
				{
					vInitPaymentData();// paypass 3.0.1 by zhoujie
					gl_aucUserInterReqData[0]=MI_ERROR_OTHER_CARD;//S51.11 for paypass 3.0.1 by zhoujie
					gl_aucUserInterReqData[1]=MI_NOT_READY;
					memcpy(gl_aucUserInterReqData+2, MSG_HOLD_TIME_VALUE, 3);

					gl_aucErrIndication[5]=MI_ERROR_OTHER_CARD;
					gl_aucErrIndication[1] = L2_EMPTY_CANDIDATE_LIST;//S51.11
					gl_aucOutcomeParamSet[0] = OC_END_APPLICATION;
					gl_DiscretionayData.ucErrIndicFlg = 1;
					//					nSendTransDataOutput_MC(T_UIRD | T_OCPS | T_DISD);//S51.11 S51.12
					break;
				}
				continue;
			}
			else if(iRet)
			{
				break;
			}
			ucPathType = 0;
			iRet = Clss_ReadData_MC(&ucPathType);
			//杩斿洖鍊肩殑鍒ゆ柇锛孖f the return code is not EMV_OK, Application should get DF8129)
			if(iRet == CLSS_RESELECT_APP)
			{
				iRet = Clss_DelCurCandApp_Entry();
				if (iRet)
				{
					vInitPaymentData();
					gl_aucUserInterReqData[0]=MI_ERROR_OTHER_CARD;
					gl_aucUserInterReqData[1]=MI_NOT_READY;
					memcpy(gl_aucUserInterReqData+2, MSG_HOLD_TIME_VALUE, 3);

					gl_aucErrIndication[5]=MI_ERROR_OTHER_CARD;
					gl_aucErrIndication[1] = L2_EMPTY_CANDIDATE_LIST;//S51.11
					gl_aucOutcomeParamSet[0] = OC_END_APPLICATION;
					gl_DiscretionayData.ucErrIndicFlg = 1;

					break;
				}
				continue;
			}
			if(iRet == 0)
			{
				vAppSetTransPath(ucPathType);
				//get pan
                iRet =  GetCLSSTrackData(ucPathType,ucAppGetAppType(),pszCardNo);
#ifdef DEBUG
                OsLog(LOG_DEBUG,"GetCLSSTrackData iret = %d",iRet);
#endif
                return iRet;

			}

		}
		else
		{
			// other kernel type proccess
			iRet = -1;
			break;
		}
	}//end of while(2)
	if (iRet)
	{
/*		if (iRet == ICC_CMD_ERR)
		{
			uchar ucRet;
			ucRet = ucDetOtherCancelCmd();//add for check whether this should be a MSR trans  ???
			if (ucRet)
			{
				if (ucRet == CLSS_USE_CONTACT)
				{
					return 0;//Compatible EDC
				}
				return ucRet;
			}
		}

		if (iRet == CLSS_USE_CONTACT)//Paywave GPO return -23,exit   QVSDC53
		{
			//Inter_DisplayMsg(Msg_TRY_ANOTHER_INTERFACE);
			return EMV_OK;
		}*/
		//ProcError_Picc(iRet);
		return ERR_TRAN_FAIL;
	}
/*	*********************Proctrans*************************************
	//PBOC
	//WAVE
	if (ucAppGetAppType() == KERNTYPE_PBOC)
	{

		iRet = nAppTransProc_PBOC(ucPathType, ucAcType);

	}

	//WAVE
	if (ucAppGetAppType() == KERNTYPE_VIS)
	{
		//return(Clss_transmit(iFlag));

		iRet = nAppTransProc_VISA(ucPathType, ucAcType);

	}*/

/*	//MC
	if (ucAppGetAppType() == KERNTYPE_MC)
	{

		iRet = nAppTransProc_MC(ucPathType, &ucAcType);

		if (gl_aucUserInterReqData[0] == MI_SEE_PHONE)
		{
			Inter_DisplayMsg(Msg_SEE_PHONE);
		}
		if (gl_aucOutcomeParamSet[0] == OC_TRY_AGAIN || gl_aucOutcomeParamSet[1] != OC_NA)
		{
			iRet = App_Try_Again;
		}
	}*/

	//AC TYPE
/*	if (iRet == 0 && ucAcType == AC_AAC)
	{
		iRet = EMV_DENIAL;
	}*/
	//***********************outcome process*****************************//
/*
	iRet = AppConv_CreateOutCome(iRet, ucAcType, &stOutComeData);
	if (iRet)
	{
		//闄eclined涔嬪鐨勯敊璇爜
		vAppDisplayMsgByRet(iRet);
		PiccClose();
		DelayMs(1200);
	}
	if (iRet == App_Try_Again)
	{
		goto DETECTCARD;
	}

	if(iRet == 0 && stOutComeData.ucTransRet ==CLSS_ONLINE_REQUEST)
	{
		ScrCls();
		DispTransName();
		PubDispString(_T("NEED ONLINE PROC"), 4|DISP_LINE_LEFT);
		Beep();
		getkey();
	}
//defined in global.h
// #ifdef DEBUGfLAG
// 	ScrCls();
// 	ScrPrint(0,0,0,"TransRet = %02x \n",stOutComeData.ucTransRet);
// 	ScrPrint(0,2,0,"PathType = %02x \n",stOutComeData.unPathType);
// 	ScrPrint(0,4,0,"CVMType = %02x \n",stOutComeData.ucCVMType);
// 	getkey();
// #endif
*/
	/***********************************************************************************
	* Add the online process here accoding to the stOutComeData(see definition of CLSS_OUTCOME_DATA ):
	* pack data element for uploading
	* process online result received from host
	* complete the transaction process.
	*
	***********************************************************************************/
/*	if (iRet == CLSS_USE_CONTACT)//
	{
		return EMV_OK;
	}
	if (iRet)
	{
		return ERR_NO_DISP;
	}*/
	return 	0;

}
