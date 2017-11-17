/*------------------------------------------------------------
* FileName: ProcTrans.c
* Author: jiangjy
* Date: 2015-03-20
------------------------------------------------------------*/
#include "ProcTrans.h"


//*******************************Global variable*****************************************************//
unsigned char gl_aucOutcomeParamSet[8]={0};
unsigned char gl_aucUserInterReqData[22]={0};
unsigned char gl_aucErrIndication[6]={0};
static App_SchemeId gl_stSchemeId;
CLSS_DISC_DATA_MC gl_DiscretionayData;//the element flag in discretionary data exist or not
static int gl_nAppTornLogNum = 0;//number of Tornlog
static CLSS_TORN_LOG_RECORD gl_atAppTornTransLog[5];//Torn Transaction Log
static unsigned char gl_ucAppType = KERNTYPE_DEF;
unsigned char gl_ucTransPath=0;
static unsigned char gl_ucACType = 0;//ACTYPE


unsigned char gl_aucFinalAID[17]={0};
unsigned char gl_ucFinalAIDLen=0;
Clss_TransParam gl_tTransParam;

//*****************************************************************************************************//
extern unsigned char PubWaitKey(short iWaitTime);


//outcome
void vAppCreateOutcomeData_MC(int nRet)
{
	int nErrorCode=0;

	switch(nRet)
	{
	case CLSS_USE_CONTACT:
		gl_aucErrIndication[1]=0;
		gl_aucErrIndication[3]=0;
		gl_aucErrIndication[4]=0;

		gl_aucErrIndication[1] = L2_EMPTY_CANDIDATE_LIST;//S53.13

		gl_aucUserInterReqData[0]=MI_ERROR_OTHER_CARD;//S53.14 for paypass 3.0.1 by zhoujie
		gl_aucUserInterReqData[1]=MI_IDLE;
		memcpy(gl_aucUserInterReqData+2, MSG_HOLD_TIME_VALUE, 3);
		//		nSendTransDataOutput_MC(CLSS_DATA_UIRD);//S53.14

		gl_aucOutcomeParamSet[0] = OC_END_APPLICATION;//S53.15 for paypass 3.0.1 by zhoujie
		gl_aucErrIndication[5]=MI_ERROR_OTHER_CARD;
		gl_DiscretionayData.ucErrIndicFlg = 1;
		//		nSendTransDataOutput_MC(CLSS_DATA_OCPS | CLSS_DATA_DISD);//S53.15
		break;
	case ICC_BLOCK:
		gl_aucUserInterReqData[0]=MI_ERROR_OTHER_CARD;//s52.8 for paypass 3.0.1 by zhoujie
		gl_aucUserInterReqData[1]=MI_IDLE;
		memcpy(gl_aucUserInterReqData+2, MSG_HOLD_TIME_VALUE, 3);

		gl_aucOutcomeParamSet[0] = OC_END_APPLICATION;//s52.9 for paypass 3.0.1 by zhoujie
		gl_aucErrIndication[5]=MI_ERROR_OTHER_CARD;
		gl_DiscretionayData.ucErrIndicFlg = 1;
		//		nSendTransDataOutput_MC(T_UIRD | T_OCPS | T_DISD);//S52.8 S52.9
		break;
	case EMV_NO_APP_PPSE_ERR:
		Clss_GetErrorCode_Entry(&nErrorCode);
		if(nErrorCode == EMV_DATA_ERR)//S52.11
		{
			gl_aucErrIndication[1]=L2_PARSING_ERROR;//S52.11 for paypass 3.0.1 by zhoujie

			gl_aucUserInterReqData[0]=MI_ERROR_OTHER_CARD;//S52.18 for paypass 3.0.1 by zhoujie
			gl_aucUserInterReqData[1]=MI_IDLE;
			memcpy(gl_aucUserInterReqData+2, MSG_HOLD_TIME_VALUE, 3);

			gl_aucOutcomeParamSet[0] = OC_END_APPLICATION;//S52.19 for paypass 3.0.1 by zhoujie
			gl_aucErrIndication[5]=MI_ERROR_OTHER_CARD;
			gl_DiscretionayData.ucErrIndicFlg = 1;
			//			nSendTransDataOutput_MC(T_UIRD | T_OCPS | T_DISD);//S52.18 S52.19
		}
		else if (nErrorCode == EMV_RSP_ERR || nErrorCode == EMV_APP_BLOCK)
		{
			gl_aucUserInterReqData[0]=MI_ERROR_OTHER_CARD;//s52.8 for paypass 3.0.1 by zhoujie
			gl_aucUserInterReqData[1]=MI_IDLE;
			memcpy(gl_aucUserInterReqData+2, MSG_HOLD_TIME_VALUE, 3);

			gl_aucOutcomeParamSet[0] = OC_END_APPLICATION;//s52.9 for paypass 3.0.1 by zhoujie
			gl_aucErrIndication[5]=MI_ERROR_OTHER_CARD;
			gl_DiscretionayData.ucErrIndicFlg = 1;
			//			nSendTransDataOutput_MC(T_UIRD | T_OCPS | T_DISD);//S52.8 S52.9
		}
		break;
	case EMV_NO_APP:
		gl_aucErrIndication[1] = L2_EMPTY_CANDIDATE_LIST;//S52.14

		gl_aucUserInterReqData[0]=MI_ERROR_OTHER_CARD;//S52.18 for paypass 3.0.1 by zhoujie
		gl_aucUserInterReqData[1]=MI_IDLE;
		memcpy(gl_aucUserInterReqData+2, MSG_HOLD_TIME_VALUE, 3);

		gl_aucOutcomeParamSet[0] = OC_END_APPLICATION;//S52.19 for paypass 3.0.1 by zhoujie
		gl_aucErrIndication[5]=MI_ERROR_OTHER_CARD;
		gl_DiscretionayData.ucErrIndicFlg = 1;
		//		nSendTransDataOutput_MC(T_UIRD | T_OCPS | T_DISD);//S52.18 S52.19
		break;
	default:
		Clss_GetErrorCode_Entry(&nErrorCode);
		if(nErrorCode == EMV_NO_DATA)
		{
			gl_aucErrIndication[1] = L2_CARD_DATA_MISSING;//S1.7
		}
		else //EMV_DATA_ERR and CLSS_RESELECT_APP
		{
			gl_aucErrIndication[1] = L2_PARSING_ERROR;//S1.7
		}
		gl_aucOutcomeParamSet[0] = OC_SELECT_NEXT;
		gl_aucOutcomeParamSet[1] = OC_C;
		vAppInitDiscData_MC();
		gl_DiscretionayData.ucErrIndicFlg = 1;
		//		nSendTransDataOutput_MC(CLSS_DATA_OCPS | CLSS_DATA_DISD);//S1.8
		break;
	}
}

void vAppInitDiscData_MC(void)
{
	memset(&gl_DiscretionayData,0,sizeof(gl_DiscretionayData));
}

void vAppSetAppType(unsigned char ucType)
{
	gl_ucAppType = ucType;
}

unsigned char ucAppGetAppType(void)
{
	return gl_ucAppType;
}

int nAppSetCurAppType(unsigned char ucKernType)
{
	int nRet=0;
	if ((ucKernType == KERNTYPE_PBOC && gl_stSchemeId.ucSupportPBOC == 1))
	{
		vAppSetAppType(KERNTYPE_PBOC);
		//Clss_CoreInit_Pboc();
	}
	else if ((ucKernType == KERNTYPE_VIS && gl_stSchemeId.ucSupportVISA == 1))
	{
		vAppSetAppType(KERNTYPE_VIS);
		//Clss_CoreInit_Wave(); // modify v1.00.0018  [2/11/2015 jiangjy]
	}
	else if (ucKernType == KERNTYPE_MC && gl_stSchemeId.ucSupportMC == 1)
	{
		vAppSetAppType(KERNTYPE_MC);
		nRet= Clss_CoreInit_MC(0x01);
		if (nRet)
		{
			return nRet;
		}
		Clss_SetParam_MC("\x04", 1);// set the timer number for kernel [1/21/2014 ZhouJie]
//		nAppCleanTornLog_MC();
	}
	//	else if (ucKernType == KERNTYPE_PBOC && ucSchemeFlgPboc == 1)
	//	{
	//		vAppSetAppType(APP_TYPE_PBOC);
	//	}
	else
	{
		return CLSS_TERMINATE;
	}
	return EMV_OK;
}


/*int nAppCleanTornLog_MC(void)
{
	unsigned char aucBuff[8];
	int nUpdatedFlg = 0, nRet=0;
	GetTime(aucBuff);//  [11/18/2011 zhoujie]
	if (gl_nAppTornLogNum == 0)
	{
		return 0;
	}
	Clss_SetTornLog_MC_MChip(gl_atAppTornTransLog, gl_nAppTornLogNum);
	nRet = Clss_CleanTornLog_MC_MChip(aucBuff, 6, 0);
	if (nRet)
	{
		return 0;
	}
	memset(gl_atAppTornTransLog, 0, sizeof(gl_atAppTornTransLog));
	gl_nAppTornLogNum = 0;
	nUpdatedFlg = 0;
	Clss_GetTornLog_MC_MChip(gl_atAppTornTransLog, &gl_nAppTornLogNum, &nUpdatedFlg);
	return 0;
}


int nAppGetTornLog_MC(CLSS_TORN_LOG_RECORD *ptTornLog, int *pnTornNum)
{
	if (ptTornLog == NULL || pnTornNum== NULL)
	{
		return EMV_PARAM_ERR;
	}
	memcpy(ptTornLog, gl_atAppTornTransLog, gl_nAppTornLogNum*sizeof(CLSS_TORN_LOG_RECORD));
	*pnTornNum = gl_nAppTornLogNum;
	return 0;
}

int nAppSaveTornLog_MC(CLSS_TORN_LOG_RECORD *ptTornLog, int nTornNum)//save Torn Log in file
{
	int nFid=0, nRet=0;
	if (ptTornLog == NULL)
	{
		return EMV_PARAM_ERR;
	}
	memcpy(gl_atAppTornTransLog, ptTornLog, nTornNum*sizeof(CLSS_TORN_LOG_RECORD));
	gl_nAppTornLogNum = nTornNum;
	nFid = open("KernalFileMCTornLog", O_CREAT|O_RDWR);
	if (nFid<0)
	{
		return EMV_FILE_ERR;
	}
	nRet = write(nFid, (unsigned char *)&nTornNum, 1);
	if (nRet<0)
	{
		return EMV_FILE_ERR;
	}
	nRet = write(nFid, (unsigned char *)ptTornLog, nTornNum*sizeof(CLSS_TORN_LOG_RECORD));
	close(nFid);
	if (nRet<0)
	{
		return EMV_FILE_ERR;
	}
	return EMV_OK;
}

int nAppLoadTornLog_MC(void)//read Torn Log from file
{
	int nFid=0, nRet=0;
	nFid = open("KernalFileMCTornLog", O_RDWR);
	if (nFid<0)
	{
		return EMV_FILE_ERR;
	}
	memset(gl_atAppTornTransLog, 0, sizeof(gl_atAppTornTransLog));
	gl_nAppTornLogNum = 0;
	nRet = read(nFid, (unsigned char *)&gl_nAppTornLogNum, 1);
	if (nRet != 1)
	{
		return EMV_FILE_ERR;
	}
	if (gl_nAppTornLogNum == 0)
	{
		return NO_TRANS_LOG;
	}
	nRet = read(nFid, (unsigned char *)gl_atAppTornTransLog, gl_nAppTornLogNum*sizeof(CLSS_TORN_LOG_RECORD));
	close(nFid);
	if (nRet<0)
	{
		return EMV_FILE_ERR;
	}
	return EMV_OK;
}*/

void vAppSetTransPath(unsigned char ucTransPath)
{
	gl_ucTransPath = ucTransPath;
}

unsigned char ucAppGetTransPath(void)
{
	return gl_ucTransPath;
}

// add for paypass 3.0 [12/30/2014 jiangjy]
void vSetFinalSelectAID(unsigned char *paucAID, unsigned char ucAIDLen)
{
	if (ucAIDLen && paucAID != NULL)
	{
		memcpy(gl_aucFinalAID, paucAID, ucAIDLen);
	}
	gl_ucFinalAIDLen = ucAIDLen;
}

void SetClssTxnParam(Clss_TransParam *pParam)
{
	memcpy(&gl_tTransParam, pParam, sizeof(Clss_TransParam));
}

void GetClssTxnParam(Clss_TransParam *pParam)
{
	memcpy(pParam, &gl_tTransParam, sizeof(Clss_TransParam));
}

void SetTermParam_MC()
{
	Clss_CoreInit_MC(0);
	PPS_MChip1_ConfigID();
}

TLV_ELEMENT_MC tTempTLV;
TLV_ELEMENT_MC *CombiTLVElement(unsigned char *pucTag, uint ucTagLen, unsigned char *pucData, uint unLen, unsigned char ucSource, uint unMaxDataLen)
{
	if (pucTag==NULL || (unLen && pucData==NULL))
	{
		return NULL;
	}
	memset(&tTempTLV, 0, sizeof (TLV_ELEMENT_MC));
	memcpy(tTempTLV.aucTag, pucTag, ucTagLen);
	tTempTLV.ucTagLen = ucTagLen;
	if (unLen)
	{
		tTempTLV.pucData=pucData;
	}

	tTempTLV.unDataLen = unLen;
	tTempTLV.ucSource = ucSource;
	tTempTLV.unMaxDataLen = unMaxDataLen;
	return &tTempTLV;
}


void PPS_MChip1_ConfigID()//for paypass
{
	unsigned char aucBuff[50],aucAmount[6];
	Clss_TransParam tTransParam;

	memset(&tTransParam, 0, sizeof(Clss_TransParam));
	GetClssTxnParam(&tTransParam);


	Clss_SetTLVDataList_MC("\x5F\x57\x00", 3);//Account type no value
	Clss_SetTLVDataList_MC("\x9F\x01\x00", 3);//Acquire id no value
	Clss_SetTLVDataList_MC("\x9F\x1E\x08\x11\x22\x33\x44\x55\x66\x77\x88", 11);//Interface Device Serial Number with any value
	Clss_SetTLVDataList_MC("\x9F\x15\x02\x00\x01", 5);//Merchant Category Code with any value
	Clss_SetTLVDataList_MC("\x9F\x16\x00", 3);//Merchant Identifier no value
	Clss_SetTLVDataList_MC("\x9F\x4E\x00", 3);//Merchant Name and Location no value
	Clss_SetTLVDataList_MC("\x9F\x33\x00", 3);//terminal capability no value
	Clss_SetTLVDataList_MC("\x9F\x1C\x00", 3);//terminal id no value

	Clss_SetTagPresent_MC("\xDF\x81\x04", 0);//Balance Before GAC
	Clss_SetTagPresent_MC("\xDF\x81\x05", 0);//Balance After GAC
	Clss_SetTagPresent_MC("\xDF\x81\x2D", 0);//Message Hold Time
	Clss_SetTLVDataList_MC("\x9F\x7E\x00", 3);//Mobile Support Indicator

	Clss_SetTLVDataList_MC("\xDF\x81\x08\x00", 4);//DS AC Type
	Clss_SetTLVDataList_MC("\xDF\x60\x00", 3);//DS Input (Card)
	Clss_SetTLVDataList_MC("\xDF\x81\x09\x00", 4);//DS Input (Term)
	Clss_SetTLVDataList_MC("\xDF\x62\x00", 3);//DS ODS Info
	Clss_SetTLVDataList_MC("\xDF\x81\x0A\x00", 4);//DS ODS Info For Reader
	Clss_SetTLVDataList_MC("\xDF\x63\x00", 3);//DS ODS Term
	Clss_SetTagPresent_MC("\x9F\x5C", 0);//DS Requested Operator ID
	Clss_SetTLVDataList_MC("\xDF\x81\x0D\x00", 4);//DSVN Term
	Clss_SetTagPresent_MC("\xDF\x81\x10", 0);//Proceed To First Write Flag Tag not present
	Clss_SetTagPresent_MC("\xDF\x81\x12", 0);//Tags To Read Tag not present
	Clss_SetTagPresent_MC("\xFF\x81\x02", 0);//Tags To Write Before Gen AC
	Clss_SetTagPresent_MC("\xFF\x81\x03", 0);//Tags To Write After Gen AC
	Clss_SetTagPresent_MC("\xDF\x81\x27", 0);//Time Out Value Tag not present


	nSetDETData("\x9F\x09", 2, "\x00\x02", 2);
	nSetDETData("\x9F\x40", 2, "\x00\x00\x00\x00\x00", 5);
	nSetDETData("\xDF\x81\x17", 3, "\x00", 1);
	nSetDETData("\xDF\x81\x18", 3, "\x60", 1);//ONLINE PIN :40 /SIG:20
	nSetDETData("\xDF\x81\x19", 3, "\x08", 1);//NO CVM
	nSetDETData("\xDF\x81\x1F", 3, "\x08", 1);//08:CDA      //40:DDA
	nSetDETData("\xDF\x81\x1A", 3, "\x9F\x6A\x04",3);//Default UDOL
	nSetDETData("\x9F\x6D", 2, "\x00\x01", 2);
	nSetDETData("\xDF\x81\x1E", 3, "\x10", 1);
	nSetDETData("\xDF\x81\x2C", 3, "\x00", 1);

	memset(aucAmount, 0, sizeof(aucAmount));
	memset(aucBuff, 0, sizeof(aucBuff));
#ifdef _LINUX_TERM
	snprintf((char *)aucBuff, 13, "%012ld", tTransParam.ulAmntAuth);
#else
	sprintf((char *)aucBuff, "%012ld", tTransParam.ulAmntAuth);
#endif
	vTwoOne_app(aucBuff, 12, aucAmount);
	nSetDETData("\x9F\x02", 2, aucAmount, 6);

	memset(aucAmount, 0, sizeof(aucAmount));
	memset(aucBuff, 0, sizeof(aucBuff));
#ifdef _LINUX_TERM
	snprintf((char *)aucBuff, 13, "%012ld", tTransParam.ulAmntOther);
#else
	sprintf((char *)aucBuff, "%012ld", tTransParam.ulAmntOther);
#endif
	vTwoOne_app(aucBuff, 12, aucAmount);
	nSetDETData("\x9F\x03", 2, aucAmount, 6);

	nSetDETData("\x9C", 1, &tTransParam.ucTransType, 1);
	nSetDETData("\x9A", 1, tTransParam.aucTransDate, 3);
	nSetDETData("\x9F\x21", 2, tTransParam.aucTransTime, 3);

	nSetDETData("\xDF\x81\x20", 3, "\x04\x00\x00\x00\x00", 5);
	if (gl_tTransParam.ucTransType == 0x20)//refund rquired AAC
	{
		nSetDETData("\xDF\x81\x21", 3, (unsigned char *)"\xFF\xFF\xFF\xFF\xFF\xFF", 5);//TAC denial

	}
	else
	{
		nSetDETData("\xDF\x81\x21", 3, "\x04\x00\x00\x00\x00", 5);
	}

	nSetDETData("\xDF\x81\x22", 3, "\xF8\x50\xAC\xF8\x00", 5);//TAC online
	//[1/9/2015 jiangjy] limit  set for AID
	if (gl_tTransParam.ucTransType == 0x20)//Refund process
	{
		nSetDETData("\xDF\x81\x23", 3, "\x00\x00\x00\x00\x00\x00", 6);//floor limit
	}
	else
	{
	    nSetDETData("\xDF\x81\x23", 3, "\x00\x00\x00\x01\x00\x00", 6);//floor limit
	}

	nSetDETData("\xDF\x81\x24", 3, "\x00\x09\x99\x99\x99\x99", 6);
	nSetDETData("\xDF\x81\x25", 3, "\x00\x09\x99\x99\x99\x99", 6);
	nSetDETData("\xDF\x81\x26", 3, "\x00\x00\x00\x00\x00\x00", 6);//cvm limit

	if(tTransParam.ucTransType == 0x01)
	{
		nSetDETData("\x9F\x35", 2, "\x14", 1);
	}
	else if (tTransParam.ucTransType == 0x00
		|| tTransParam.ucTransType == 0x09
		|| tTransParam.ucTransType == 0x20 )
	{
		nSetDETData("\x9F\x35", 2, "\x22", 1);
	}
	Clss_SetTagPresent_MC("\xDF\x81\x30", 0);
	nSetDETData("\xDF\x81\x1C", 3, "\x00\x00", 2);
	nSetDETData("\xDF\x81\x1D", 3, "\x00", 1);
	nSetDETData("\xDF\x81\x0C", 3, "\x02", 1);
	Clss_SetTagPresent_MC("\xDF\x81\x2D", 0);
	nSetDETData("\x9F\x1A", 2, "\x08\x40", 2);
	nSetDETData("\x5F\x2A", 2, "\x08\x40", 2);
	if (gl_ucFinalAIDLen == 0)
	{
		return;
	}
	if (!memcmp(gl_aucFinalAID, "\xA0\x00\x00\x00\x04\x10\x10",7))// MCHIP [1/4/2015 jiangjy]
	{
		//MCD
		nSetDETData("\xDF\x81\x1B", 3, "\x20", 1);
		//Tag 9F1D set based on definition
		Clss_AddProprietaryData_MC(CombiTLVElement((unsigned char*)"\x9F\x1D", 2, (unsigned char *)"\x6C\xF8\x00\x00\x00\x00\x00\x00", 8, CLSS_SRC_TM, 0));




		//set cvm limit based on AID
// 		memset(aucAmount, 0, sizeof(aucAmount));
// 		memset(aucBuff, 0, sizeof(aucBuff));
// 		if(tTransParam.ucTransType == 0x00)//payment
// 		{
// #ifdef _LINUX_TERM
// 			snprintf((char *)aucBuff, 13, "%012ld", 1000);
// #else
// 			sprintf((char *)aucBuff, "%012ld", 1000);
// #endif
// 		}
// 		else
// 		{
// #ifdef _LINUX_TERM
// 			snprintf((char *)aucBuff, 13, "%012ld", 20000);
// #else
// 			sprintf((char *)aucBuff, "%012ld", 20000);
// #endif
// 		}
// 		vTwoOne_app(aucBuff, 12, aucAmount);
// 		nSetDETData("\xDF\x81\x26", 3, aucAmount, 6);
	}
	else if (memcmp(gl_aucFinalAID,  "\xA0\x00\x00\x00\x04\x30\x60", 7) == 0)
	{
		//Mestro  NO MAG-STRIPE
		nSetDETData("\xDF\x81\x1B", 3, "\xA0", 1);
		//Tag 9F1D
		Clss_AddProprietaryData_MC(CombiTLVElement((unsigned char*)"\x9F\x1D", 2, (unsigned char *)"\x40\xF8\x80\x00\x00\x00\x00\x00", 8, CLSS_SRC_TM, 0));



// 		memset(aucAmount, 0, sizeof(aucAmount));
// 		memset(aucBuff, 0, sizeof(aucBuff));
// #ifdef _LINUX_TERM
// 		snprintf((char *)aucBuff, 13, "%012ld", 30000);
// #else
// 		sprintf((char *)aucBuff, "%012ld", 30000);
// #endif
// 		vTwoOne_app(aucBuff, 12, aucAmount);
// 		nSetDETData("\xDF\x81\x26", 3, aucAmount, 6);// Reader CVM Required Limit [12/30/2014 jiangjy]
	}
	// Remove AID \xB0\x12\x34\x56\x78  [1/4/2015 jiangjy]
	// 	else if (!memcmp(gl_aucFinalAID, MASTER_TEST3_APP.AID, MASTER_TEST3_APP.AidLen))// ? [12/30/2014 jiangjy]
	// 	{
	// 		nSetDETData("\xDF\x81\x1B", 3, "\x20", 1);
	//
	// 		memset(aucAmount, 0, sizeof(aucAmount));
	// 		memset(aucBuff, 0, sizeof(aucBuff));
	// 		if(tTransParam.ucTransType == 0x00)//payment
	// 		{
	// #ifdef _LINUX_TERM
	// 			snprintf((char *)aucBuff, 13, "%012ld", 1000);
	// #else
	// 			sprintf((char *)aucBuff, "%012ld", 1000);
	// #endif
	// 		}
	// 		else
	// 		{
	// #ifdef _LINUX_TERM
	// 			snprintf((char *)aucBuff, 13, "%012ld", 10000);
	// #else
	// 			sprintf((char *)aucBuff, "%012ld", 10000);
	// #endif
	// 		}
	// 		vTwoOne_app(aucBuff, 12, aucAmount);
	// 		nSetDETData("\xDF\x81\x26", 3, aucAmount, 6);
	// 	}
}

int nSetDETData(unsigned char *pucTag, unsigned char ucTagLen, unsigned char *pucData, unsigned char ucDataLen)
{
	unsigned char aucBuff[256],ucBuffLen;

	if (pucTag == NULL || pucData == NULL)
	{
		return CLSS_PARAM_ERR;
	}

	memset(aucBuff, 0, sizeof(aucBuff));
	memcpy(aucBuff, pucTag, ucTagLen);//Terminal Country Code
	ucBuffLen = ucTagLen;
	aucBuff[ucBuffLen++] = ucDataLen;
	memcpy(aucBuff+ucBuffLen, pucData, ucDataLen);
	ucBuffLen += ucDataLen;
	return Clss_SetTLVDataList_MC(aucBuff, ucBuffLen);
}



void vTwoOne_app(unsigned char *in, unsigned short in_len, unsigned char *out)
{
	unsigned char tmp;
	unsigned short i;

	for (i = 0; i < in_len; i += 2) {
		tmp = in[i];
		if (tmp > '9')
			tmp = toupper(tmp) - ('A' - 0x0A);
		else
			tmp &= 0x0f;
		tmp <<= 4;
		out[i / 2] = tmp;

		tmp = in[i + 1];
		if (tmp > '9')
			tmp = toupper(tmp) - ('A' - 0x0A);
		else
			tmp &= 0x0f;
		out[i / 2] += tmp;
	}
}

void vOneTwo_app(unsigned char *One,unsigned short len,unsigned char *Two)
{
	unsigned char  i;
	static unsigned char TAB[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	for (i = 0; i < len; i++) {
		Two[i * 2] = TAB[One[i] >> 4];
		Two[i * 2 + 1] = TAB[One[i] & 0x0f];
	}
}

void vInitPaymentData(void)//KS.2 paypass 3.0.1 by zhoujie
{
	// 	Clss_TermConfig_MC tMCTermConfig;
	//
	// 	memset(&tMCTermConfig, 0, sizeof(tMCTermConfig));
	// 	Clss_GetTermConfig_MC(&tMCTermConfig);
	// 	tMCTermConfig.ucMobileSupFlg = CLSS_TAG_EXIST_WITHVAL;
	// 	tMCTermConfig.ucMobileSup = 0x01;
	// 	Clss_SetTermConfig_MC(&tMCTermConfig);
	Clss_SetTLVDataList_MC("\x9F\x7E\x01\x01", 4);//Mobile Support Indicator

	memset(gl_aucOutcomeParamSet, 0, sizeof(gl_aucOutcomeParamSet));
	gl_aucOutcomeParamSet[0] = OC_END_APPLICATION;//for s51.13
	gl_aucOutcomeParamSet[1] = OC_NA;//s51.3
	gl_aucOutcomeParamSet[2] = OC_NA;
	gl_aucOutcomeParamSet[3] = OC_NA;
	gl_aucOutcomeParamSet[4] |= 0x10;
	gl_aucOutcomeParamSet[5] = OC_NA;
	gl_aucOutcomeParamSet[6] = 0xFF;

	memset(gl_aucUserInterReqData, 0, sizeof(gl_aucUserInterReqData));
	gl_aucUserInterReqData[0] = MI_NA;
	gl_aucUserInterReqData[1] = MI_NA;
	memcpy(gl_aucUserInterReqData+2, MSG_HOLD_TIME_VALUE, 3);

	memset(gl_aucErrIndication, 0, sizeof(gl_aucErrIndication));
	gl_aucErrIndication[5] = MI_NA;

	vInitDiscData();
	//	gl_ucTornLogRecordExist = 0;
}

void vInitDiscData(void)//Initialize Discretionary Data by zhoujie
{
	memset(&gl_DiscretionayData,0,sizeof(gl_DiscretionayData));
}

//add for check whether this should be a MSR trans
/*unsigned char ucDetOtherCancelCmd(void)
{
	int i = 0;
	char ch = 0;
	unsigned char buff[10] = {0};
	unsigned char ucRet = 0;
	if (IccDetect(0) == 0)
	{
		PiccClose();
		ScrCls();
		ScrPrint(0,1,1,"Card Inserted!");
		disp_clss_err(CLSS_USE_CONTACT);
		kbflush();
		DelayMs(2000);
		return CLSS_USE_CONTACT;
	}
	if (MagSwiped() == 0)
	{
		PiccClose();
		ScrCls();
		ScrPrint(0,1,1,"Card Swiped!");
		disp_clss_err(CLSS_USE_CONTACT);
		kbflush();
		DelayMs(2000);
		return CLSS_USE_CONTACT;
	}

	return 0;
}*/

/*
int disp_clss_err(int err)
{
#ifdef _D_SERIES_
	DtScrClrLine(2,7);
#else
	ScrClrLine(2,7);
#endif

	if(err == EMV_NO_APP)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("NO SUPPORT APP");
		DtScrSetRowFont(0, 4, 0x01);
		Lcdprintf("Not Accepted");
#else
		ScrPrint(0, 2, 0x01, "NO SUPPORT APP");
		ScrPrint(0, 4, 0x01, "Not Accepted");
#endif
	}

	if(err == CLSS_USE_CONTACT)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("PLS USE CONTACT");
		DtScrSetRowFont(0, 4, 0x01);
		Lcdprintf("TO TRANSACTION");
#else
		ScrPrint(0, 2, 0x01, "PLS USE CONTACT");
		ScrPrint(0, 4, 0x01, "TO TRANSACTION");
#endif
	}

	if(err == ICC_BLOCK)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("CARD LOCKED");
#else
		ScrPrint(0, 2, 0x01, "CARD LOCKED");
#endif
	}

	if(err == EMV_APP_BLOCK)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("APP LOCKED");
#else
		ScrPrint(0, 2, 0x01, "APP LOCKED");
#endif
	}

	PubWaitKey(30);
	return ERR_USERCANCEL;
}
*/


int nAppTransProc_PBOC(unsigned char ucTransPath, unsigned char ucACType)
{
	int nRet = 0;

	int nAidLen = 0;
	int nTempLen = 0;
	unsigned char aucAid[17] = {0};
	unsigned char ucPkIndex = 0;
	unsigned char ucDDAFailFlg = 0;

	appRemovePicc(0x0f);

#ifdef _COM_SIMU
	Clss_SetTLVData_Wave(0x9f37,guc9F37Data,4);
#endif
	switch (ucTransPath)
	{
		// comment by [2/6/2015 jiangjy]
		// 	case CLSS_VISA_MSD:
		//
		// 		//AC_ARQC;
		// 		Clss_nGetTrack1MapData_Pboc(gl_aucTrack1Asc, &gl_nTrack1AscLen);
		// 		Clss_nGetTrack2MapData_Pboc(gl_aucTrack2Asc, &gl_nTrack2AscLen);
		// 		//POSEntryMode = MODE_VISA_MSD;
		// 		nRet = app_OnlineAuthProc_Picc(NULL);
		// 		if (nRet == 0)
		// 		{
		// 			//Inter_DisplayMsg(Msg_APPROVED);
		// 		}
		// 		break;
	case CLSS_VISA_QVSDC:

		//POSEntryMode = MODE_VISA_QVSDC;
		if (ucACType == AC_ARQC)
		{
			nRet = 0;
			break;
		}
		else if (ucACType == AC_TC)
		{
			// comment by [2/6/2015 jiangjy]  CheckExceptionFile support?
			// 			if (nCheckExceptionFile() == -1)
			// 			{
			// 				ucACType = AC_AAC;
			// 			}
			Clss_DelAllRevocList_Pboc();
			Clss_DelAllCAPK_Pboc();
			if (Clss_GetTLVData_Pboc(0x8F, &ucPkIndex, &nTempLen) == 0)
			{
				if (Clss_GetTLVData_Pboc(0x9F06, aucAid, &nTempLen) == 0)
				{
					AppSetClss_capk(ucPkIndex, aucAid,gl_ucAppType);
					AppSetClss_RevLst(ucPkIndex, aucAid,gl_ucAppType);
				}
			}

			nRet = Clss_CardAuth_Pboc(&ucACType, &ucDDAFailFlg);
			if (nRet != 0)
			{
				if (nRet != CLSS_USE_CONTACT)
				{
					nRet =  CLSS_TERMINATE;
				}
			}
			else
			{
				if (ucDDAFailFlg == 1)
				{
					nRet =  CLSS_TERMINATE;
				}
				//else
					//Inter_DisplayMsg(Msg_APPROVED);
			}
		}
		else
		{
			//Inter_DisplayMsg(Msg_DECLINED);
			nRet = EMV_DENIAL;
		}
		break;
	case CLSS_VISA_WAVE2:
		//POSEntryMode = MODE_VISA_WAVE2;
		if (ucACType == AC_ARQC)
		{
			// comment [2/6/2015 jiangjy]
			//	nRet = app_OnlineAuthProc_Picc(NULL);
			nRet =0;
			break;

		}
		else if (ucACType == AC_TC)
		{
			Clss_DelAllRevocList_Pboc();
			Clss_DelAllCAPK_Pboc();
			if (Clss_GetTLVData_Pboc(0x8F, &ucPkIndex, &nTempLen) == 0)
			{
				if (Clss_GetTLVData_Pboc(0x9F06, aucAid, &nTempLen) == 0)
				{
					AppSetClss_capk(ucPkIndex, aucAid,gl_ucAppType);
					AppSetClss_RevLst(ucPkIndex, aucAid,gl_ucAppType);
				}
			}
			// internal authenticate, offline approval
			nRet = Clss_CardAuth_Pboc(&ucACType, &ucDDAFailFlg);

			if (nRet)
			{
				if (nRet == CLSS_WAVE2_TERMINATED)
				{
					nRet =  CLSS_TERMINATE;
				}
				nRet =  CLSS_TERMINATE;
			}
		}
		else
		{
			//Inter_DisplayMsg(Msg_DECLINED);
			nRet = EMV_DENIAL;
		}
		break;
	default:

		nRet = CLSS_TERMINATE;
		break;
	}
	////PiccLight(PICC_LED_RED|PICC_LED_YELLOW|PICC_LED_BLUE, 0);
	return nRet;
}


int nAppTransProc_VISA(unsigned char ucTransPath, unsigned char ucACType)
{
	int nRet = 0;
	//	int nAidLen = 0;
	int nTempLen = 0;
	//	int len = 0;
	unsigned char aucAid[17];
	unsigned char ucPkIndex = 0;
	unsigned char ucDDAFailFlg = 0;
	//	unsigned char buff[100] ={0};
	//	unsigned char ucAppType = APP_TYPE_VISA;
	//	unsigned char ucCVMType = RD_CVM_NO;
	//	unsigned char num=0, j=0;
	Clss_TransParam tTransParm;
	Clss_PreProcInfo tPreProInfo;

	GetClssTxnParam(&tTransParm);
	//	ucAppType = ucAppGetAppType();

	switch (ucTransPath)
	{
	case CLSS_VISA_MSD:
	case CLSS_VISA_MSD_CVN17:
		break;
	case CLSS_VISA_QVSDC:

		appRemovePicc(0x0f);
		nRet = Clss_ProcRestric_Wave();

		if (nRet)
		{
			return nRet;
		}

		if (ucACType == AC_TC && tTransParm.ucTransType != 0x20)//REFUND
		{
			Clss_DelAllRevocList_Wave();
			Clss_DelAllCAPK_Wave();

			if ( Clss_GetTLVData_Wave(0x8F, &ucPkIndex,  &nTempLen) == 0)
			{
				//if (AppConv_GetTLVData(0x9F06, aucAid, &nTempLen) == 0)//get aid
				{
					AppSetClss_capk(ucPkIndex, gl_aucFinalAID,gl_ucAppType);// capk
					AppSetClss_RevLst(ucPkIndex, gl_aucFinalAID,gl_ucAppType);
				}

				nRet = Clss_CardAuth_Wave(&ucACType, &ucDDAFailFlg);//capk

				if (nRet != 0)
				{
					if (nRet != CLSS_USE_CONTACT)
					{
						nRet =  CLSS_TERMINATE;
					}
				}
				else
				{
					if (ucDDAFailFlg == 1)
					{
						nRet =  CLSS_TERMINATE;
					}
					//else
						//ProcApproval_Picc();//display approved
						//Inter_DisplayMsg(Msg_APPROVED);
				}
			}
			else
			{
				if (ucACType == AC_ARQC || ucACType == AC_TC)
				{
					nRet = 0;
				}
				else // (ucACType == AC_AAC)
				{
					nRet = EMV_DENIAL;
				}
			}
		}

		break;
	case CLSS_VISA_WAVE2:
		if (ucACType == AC_TC)
		{
			Clss_DelAllRevocList_Wave();
			Clss_DelAllCAPK_Wave();

			if ( Clss_GetTLVData_Wave(0x8F, &ucPkIndex,  &nTempLen) == 0)
			{
				//if (AppConv_GetTLVData(0x9F06, aucAid, &nTempLen) == 0)
				{
					AppSetClss_capk(ucPkIndex, gl_aucFinalAID,gl_ucAppType);
					AppSetClss_RevLst(ucPkIndex, gl_aucFinalAID,gl_ucAppType);
				}
			}
			// internal authenticate, offline approval
			nRet = Clss_CardAuth_Wave(&ucACType, &ucDDAFailFlg);

			if (nRet)
			{
				return EMV_DENIAL; // 0424-3
			}
		}
		appRemovePicc(0x0f);


		break;
	default:
		//appRemovePicc(PICC_LED_NONE);
		nRet = CLSS_TERMINATE;
		break;
	}

	if (nRet == 0)
	{
		vAppSetTransACType(ucACType);
	}
	return nRet;
}


int  AppConv_GetTLVData(unsigned short Tag, unsigned char *DataOut, int *OutLen)
{
	switch (ucAppGetAppType())
	{
	case KERNTYPE_VIS:
		return (Clss_GetTLVData_Wave(Tag, DataOut, OutLen));
		//	case APP_TYPE_PBOC:
		//		return (Clss_GetTLVData_Pboc(Tag, DataOut, OutLen));
	case KERNTYPE_MC:
		//	return (Clss_GetTLVDataList_MC(Tag, DataOut, OutLen));
	default:
		return CLSS_FAILED;
	}
}


int nAppTransProc_MC(unsigned char ucTransPath, unsigned char *pucACType)
{
	int nRet=0,nTempLen=0;
	unsigned char ucPkIndex,aucAid[17]={0};
	CLSS_TORN_LOG_RECORD atAppTornTransLog[5];//Torn Transaction Log
	int nAppTornLogNum = 0;//number of Tornlog
	int nUpdatedFlg = 0;

	unsigned char ucTmp[20];
	unsigned int iLen;
	int icnt;


	if (ucTransPath == CLSS_MC_MCHIP) // 0x06)
	{
		Clss_DelAllRevocList_MC_MChip();
		Clss_DelAllCAPK_MC_MChip();

		if (Clss_GetTLVDataList_MC((unsigned char*)"\x8F", 1, 1, &ucPkIndex, (uint *)&nTempLen) == 0)
		{
			if (Clss_GetTLVDataList_MC((unsigned char*)"\x4F", 1, sizeof(aucAid), aucAid, (uint *)&nTempLen) == 0)
			{

				AppSetClss_capk(ucPkIndex, aucAid, gl_ucAppType);// CAPK [1/4/2015 jiangjy]
				AppSetClss_RevLst(ucPkIndex, aucAid, gl_ucAppType);
			}
		}

		//nAppGetTornLog_MC(atAppTornTransLog, &nAppTornLogNum);
		if (nAppTornLogNum)
		{
			Clss_SetTornLog_MC_MChip(atAppTornTransLog, nAppTornLogNum);
		}
		nRet = Clss_TransProc_MC_MChip(pucACType);
		memset(atAppTornTransLog, 0, sizeof(atAppTornTransLog));
		nAppTornLogNum = 0;
		nUpdatedFlg = 0;
		Clss_GetTornLog_MC_MChip(atAppTornTransLog, &nAppTornLogNum, &nUpdatedFlg);
		if (nUpdatedFlg)
		{
			//nAppSaveTornLog_MC(atAppTornTransLog, nAppTornLogNum);
		}
	}
	else if (ucTransPath == CLSS_MC_MAG) // 0x05)
	{
		nRet = Clss_TransProc_MC_Mag(pucACType);
//GET TRACK 2 DATA
// 		memset(ucTmp,0,sizeof(ucTmp));
// 		nRet = Clss_GetTLVDataList_MC("\x9F\x6B",2,sizeof(ucTmp), ucTmp, &iLen);
// #ifdef DEBUGfLAG
// 		PubDebugOutput("9F06", ucTmp,iLen,DEVICE_SCR, HEX_MODE);
// #endif

	}
	else
	{
		nRet = CLSS_TERMINATE;
	}

	if (nRet != ICC_CMD_ERR)//3G10-9301(Trx1_CCC_Res) 3G10-9300(Trx2_CCC_Res)
	{
		//Inter_DisplayMsg(Msg_CARD_READ_OK);
		////Inter_DisplayMsg(Msg_REMOVE_CARD);
		appRemovePicc(0x0f);//appRemovePicc(PICC_LED_GREEN);//灯  //PiccLight/////////////////////pls remove card
	}
	if (nRet)
	{
		return nRet;
	}

	vAppSetTransACType(*pucACType);

	return EMV_OK;
}



int AppSetClss_capk(unsigned char index, unsigned char *rid, unsigned char kerId)
{
	int i, iRet;
	unsigned char ucAid[17];
	EMV_CAPK	stEmvCapk;

	unsigned char tmpRid[5];
	unsigned char tmpIndex;
//	unsigned char ucRidLen = 0;
//	ucRidLen = strlen(rid);
//	memset(ucAid, 0, sizeof(ucAid));
//	PubAsc2Bcd(rid, ucRidLen, ucAid);

	for (i=0; i<MAX_KEY_NUM; i++)
	{
		memset(&stEmvCapk, 0, sizeof(EMV_CAPK));
		iRet = EMVGetCAPK(i, &stEmvCapk);

		memcpy(tmpRid,stEmvCapk.RID,5);
		tmpIndex =stEmvCapk.KeyID;

		if(iRet != EMV_OK)
		{
			continue;
		}

		if(memcmp(stEmvCapk.RID, rid, 5) != 0)
		{
			continue;
		}
		if(index != stEmvCapk.KeyID)
		{
			continue;
		}

		else if (kerId == KERNTYPE_VIS)
		{
			Clss_AddCAPK_Wave(&stEmvCapk);
		}
		else if (kerId == KERNTYPE_MC)
		{
			Clss_AddCAPK_MC_MChip(&stEmvCapk);
		}
		else if (kerId == KERNTYPE_PBOC)
		{
			iRet = Clss_AddCAPK_Pboc(&stEmvCapk);
		}
		else
		{
			return ERR_TRAN_FAIL;
		}
	}

	return 0;
}


int AppSetClss_RevLst(unsigned char index, unsigned char *rid, unsigned char kerId)
{
	unsigned char ucAid[10];
	EMV_REVOCLIST tRevocList;

// 	memset(ucAid, 0, sizeof(ucAid));
// 	PubAsc2Bcd(rid, strlen(rid), ucAid);
	memcmp(tRevocList.ucRid, rid, 5);
	tRevocList.ucIndex = index;
	memcpy(tRevocList.ucCertSn, "\x00\x07\x11", 3);

	if (kerId == KERNTYPE_VIS)
	{
		Clss_AddRevocList_Wave(&tRevocList);
	}
	else if (kerId == KERNTYPE_MC)
	{
		Clss_AddRevocList_MC_MChip(&tRevocList);
	}
	else if (kerId == KERNTYPE_PBOC)
	{
		Clss_AddRevocList_Pboc(&tRevocList);
	}

	return 0;
}

static void vAppSetTransACType(unsigned char ucType)
{
	gl_ucACType = ucType;
}

// display message [1/5/2015 jiangjy]
/*
void //Inter_DisplayMsg(unsigned int iMsg)
{
#ifdef _D_SERIES_
	DtScrClrLine(2,7);
#else
	ScrClrLine(2,7);
#endif

	if(iMsg == Msg_CARD_READ_OK)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("CARD READ OK");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("CARD READ OK"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(iMsg == Msg_REMOVE_CARD)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("PLS REMOVE CARD");

#else
		ScrCls();
		DispTransName();
		////PubDispString(_T("PLS REMOVE CARD"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(iMsg == ICC_BLOCK)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("CARD LOCKED");
#else
		ScrCls();
		DispTransName();
		////PubDispString(_T("CARD LOCKED"), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	if(iMsg == Msg_TRY_AGAIN)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("PLS TRY AGAIN");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("PLS TRY AGAIN"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(iMsg == Msg_SEE_PHONE)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("PLS SEE PHONE");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("PLS SEE PHONE"), 4|DISP_LINE_LEFT);

#endif
	}

	if(iMsg == Msg_APPROVED)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("TXN APPROVED");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("TXN APPROVED"), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	if(iMsg == Msg_DECLINED)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("TXN DECLINED");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("TXN DECLINED"), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}


	if(iMsg == Msg_APPROVED_SIGN)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("TXN APPROVED WITH SIG");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("TXN APPROVED WITH SIG"), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	if(iMsg == Msg_OTHER_CARD)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("TRY OTHER CARD");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("TRY OTHER CARD"), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	if(iMsg == Msg_INSERT_CARD)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("PLS INSERT CARD");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("PLS INSERT CARD"), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	if(iMsg == Msg_AUTHORISING)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("AUTHORISING...");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("AUTHORISING..."), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	if(iMsg == Msg_CLEAR_DISPLAY)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("CLEAR...");
#else
		ScrCls();
		//DispTransName();
#endif
	}

	if(iMsg == Msg_TAP_CARD_AGAIN)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("TAP CARD AGAIN");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("TAP CARD AGAIN"), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	if(iMsg == Msg_TAP_CARD_WITH_AMOUNT)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("TAP CARD WITH AMOUNT");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("TAP CARD WITH AMOUNT"), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	if(iMsg == Msg_TAP_CARD_WITH_BALANCE)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("TAP CARD WITH BALANCE");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("TAP CARD WITH BALANCE"), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	if(iMsg == Msg_TRY_ANOTHER_INTERFACE)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("PLS USE CONTACT");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("PLS USE CONTACT "), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	if(iMsg == Msg_END_APPLICATION)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("END APPLICATION");
#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("END APPLICATION"), 4|DISP_LINE_LEFT);
		Beep();
#endif
	}

	//PubWaitKey(30);//delay
	DelayMs(3000);

}

*/
uchar   PiccRemove(uchar mode,uchar cid)
{
	int nRet;

	if(mode != 'R' && mode != 'r' &&
	   mode != 'E' && mode != 'e' &&
	   mode != 'H' && mode != 'h' &&
	   (cid < 0 || cid > 14))
	{
		return PCD_ERR_NOT_ALLOWED_FLAG;
	}

	nRet = OsPiccRemove();

	if(!nRet)
	{
		return 0;
	}
// 	EScrCls();// for tttttt [11/22/2011 zhoujie]
//	 	EScrPrint(0,0,0,"PiccRemove:%d",nRet);
//	 	EGetKey();

	switch(nRet)
	{
	case PCD_ERR_NOT_ALLOWED_FLAG:
		return 0x03;
	case PCD_CHIP_NOT_OPENED:
		return 0x02;
	case PCD_CHIP_CARDEXIST:
		return 0x06;
	default:
		return 0x04;//other
	}
}



int PiccClose(void)
{
	return OsPiccClose();
}

static void appRemovePicc(unsigned char ucLight)
{
	uchar ucRet = 0;

	//if (ucLight)
//	PiccLight(0x0F, 0);
//	PiccLight(ucLight, 1);
//	Beep();
	//Inter_cDisplayMsg(Msg_REMOVE_CARD);

	ucRet = PiccRemove('e', 0);
	while (ucRet)
	{
		OsBeep(0, 200);
		ucRet = PiccRemove('e', 0);
		if ((ucRet == 2) || (ucRet == 3))
		{
			break;
		}
	}
	//DeleyMs(300);
	//PiccLight(0x0F, 0);
	PiccClose();
}


int AppConv_CreateOutCome(int nRet, unsigned char ucACType, CLSS_OUTCOME_DATA *pstOutComeData)
{
	unsigned char aucOutcomeParamSet_MC[8];
	uint unLen=0;
	if(pstOutComeData == NULL)
	{
		return EMV_PARAM_ERR;
	}

	memset(pstOutComeData, 0, sizeof(CLSS_OUTCOME_DATA));
	pstOutComeData->unPathType = ucAppGetTransPath();
	switch (ucAppGetAppType())
	{
	case KERNTYPE_PBOC:
		if (!nRet)
		{
			nRet = Clss_GetCvmType_Pboc(&pstOutComeData->ucCVMType);//  [2/9/2015 jiangjy]
			if (nRet)
			{
				pstOutComeData->ucTransRet = CLSS_DECLINED;
			}
			else
			{
				if(ucACType == AC_TC)
				{
					pstOutComeData->ucTransRet = CLSS_APPROVE;
				}
				else//AC_ARQC
				{
					pstOutComeData->ucTransRet = CLSS_ONLINE_REQUEST;
				}
			}

		}
		else if (nRet == EMV_DENIAL || nRet == CLSS_DECLINE)
		{
			pstOutComeData->ucTransRet = CLSS_DECLINED;
		}
		else if (nRet == CLSS_USE_CONTACT)
		{
			pstOutComeData->ucTransRet = CLSS_TRY_ANOTHER_INTERFACE;
		}
		else
		{
			pstOutComeData->ucTransRet = CLSS_END_APPLICATION;
		}
		break;

	case KERNTYPE_VIS:
		if (!nRet)
		{
			nRet = Clss_GetCvmType_Wave(&pstOutComeData->ucCVMType);//  [1/22/2015 jiangjy]
			if (nRet)
			{
				pstOutComeData->ucTransRet = CLSS_DECLINED;
			}
			else
			{
				if(ucACType == AC_TC)
				{
					pstOutComeData->ucTransRet = CLSS_APPROVE;
				}
				else//AC_ARQC
				{
					pstOutComeData->ucTransRet = CLSS_ONLINE_REQUEST;
				}
			}

		}
		else if (nRet == EMV_DENIAL || nRet == CLSS_DECLINE)
		{
			pstOutComeData->ucTransRet = CLSS_DECLINED;
		}
		else if (nRet == CLSS_USE_CONTACT)
		{
			pstOutComeData->ucTransRet = CLSS_TRY_ANOTHER_INTERFACE;
		}
		else
		{
			pstOutComeData->ucTransRet = CLSS_END_APPLICATION;
		}
		break;
	case KERNTYPE_MC:
		memset(aucOutcomeParamSet_MC, 0, sizeof(aucOutcomeParamSet_MC));
		Clss_GetTLVDataList_MC("\xDF\x81\x29", 3, sizeof(aucOutcomeParamSet_MC), aucOutcomeParamSet_MC, &unLen);
		switch(aucOutcomeParamSet_MC[0]&0xF0)
		{
		case CLSS_OC_APPROVED:
			pstOutComeData->ucTransRet = CLSS_APPROVE;
			break;
		case CLSS_OC_DECLINED:
			pstOutComeData->ucTransRet = CLSS_DECLINED;
			break;
		case CLSS_OC_ONLINE_REQUEST:
			pstOutComeData->ucTransRet = CLSS_ONLINE_REQUEST;
			break;
		case CLSS_OC_TRY_ANOTHER_INTERFACE:
			pstOutComeData->ucTransRet = CLSS_TRY_ANOTHER_INTERFACE;
			break;
		default://CLSS_OC_END_APPLICATION
			pstOutComeData->ucTransRet = CLSS_END_APPLICATION;
			break;
		}
		switch(aucOutcomeParamSet_MC[3]&0xF0)
		{
		case CLSS_OC_NO_CVM:
			pstOutComeData->ucCVMType = CLSS_CVM_NO;
			break;
		case CLSS_OC_OBTAIN_SIGNATURE:
			pstOutComeData->ucCVMType = CLSS_CVM_SIG;
			break;
		case CLSS_OC_ONLINE_PIN:
			pstOutComeData->ucCVMType = CLSS_CVM_ONLINE_PIN;
			break;
		case CLSS_OC_CONFIRM_CODE_VER:
			pstOutComeData->ucCVMType = CLSS_CVM_OFFLINE_PIN;
			break;
		default:
			pstOutComeData->ucCVMType = CLSS_CVM_NO;
			break;
		}
		break;
	default:
		break;
	}
	return nRet;

}

//display
/*void vAppDisplayMsgByRet(int nRet)
{


	switch (nRet)
	{
		//	case EMV_OK:
		//    	Inter_cDisplayMsg(Msg_APPROVED);
		//        break;
	case CLSS_DECLINE:
	case EMV_DENIAL:
		//Inter_DisplayMsg(Msg_DECLINED);
		break;
	case CLSS_USE_CONTACT:
		//Inter_DisplayMsg(Msg_TRY_ANOTHER_INTERFACE);
		break;
	case ICC_RESET_ERR:
	case ICC_CMD_ERR:
		//Inter_DisplayMsg(Msg_TRY_AGAIN);
		break;
	case CLSS_REFER_CONSUMER_DEVICE:
		//		Inter_cDisplayMsg(Msg_TRY_AGAIN);
		//Inter_DisplayMsg(Msg_SEE_PHONE);
		break;
	default:
		//Inter_DisplayMsg(Msg_END_APPLICATION);
		break;
	}
#ifdef _DEBUG
	printf("vAppDisplayMsgByRet Display\n");
#endif
	Beep();
	DelayMs(100);
	Beep();
	DelayMs(1000);
}*/

void vAppInitSchemeId(void)
{
	memset(&gl_stSchemeId,0, sizeof(gl_stSchemeId));
	gl_stSchemeId.ucSupportVISA = 1;
	gl_stSchemeId.ucSupportMC = 1;
	gl_stSchemeId.ucSupportPBOC = 1;


}
/*

int ProcError_Picc(int ret)
{
	if (!ret)
		return 0;

	//PiccLight(0x0f, 0);


#ifdef _D_SERIES_
	DtScrClrLine(2,7);
#else
	ScrClrLine(2,7);
#endif

	if(ret == CLSS_CVMDECLINE)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("CL TXN not permitted");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("CL TXN not permitted"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == ICC_RESET_ERR||ret == ICC_CMD_ERR)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("CARD READ ERROR");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("CARD READ ERROR"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == ICC_BLOCK)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("CARD Blocked");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("CARD Blocked "), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == ICC_BLOCK)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("CARD Blocked");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("CARD Blocked "), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == CLSS_CARD_EXPIRED)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("CARD EXPIRED");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("CARD EXPIRED "), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == EMV_RSP_ERR||ret == EMV_DATA_ERR||ret == CLSS_TERMINATE)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("TXN TERMINATED");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("TXN TERMINATED "), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == CLSS_USE_CONTACT)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("PLS USE CONTACT");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("PLS USE CONTACT "), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == EMV_TIME_OUT)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("TIME OUT");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("TIME OUT "), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == EMV_USER_CANCEL)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("USER CANCEL");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("USER CANCEL "), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == EMV_APP_BLOCK)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("APP BLOCKED");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("APP BLOCKED "), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}
	if(ret == EMV_NOT_ACCEPT)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("NOT ACCEPTTED");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("NOT ACCEPTTED"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == EMV_DENIAL)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("TXN DCLINED");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("TXN DCLINED"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}


	if(ret == NO_TRANS_LOG)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("NO TRANS LOG");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("NO TRANS LOG"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == RECORD_NOTEXIST)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("RECORD NOT EXIST");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("RECORD NOT EXIST"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == RECORD_NOTEXIST)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("RECORD NOT EXIST");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("RECORD NOT EXIST"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == EMV_NO_APP)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("USE CONTACT");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("USE CONTACT"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}

	if(ret == CLSS_PARAM_ERR)
	{
#ifdef _D_SERIES_
		DtScrSetRowFont(0, 2, 0x01);
		Lcdprintf("PARAM ERR");

#else
		ScrCls();
		DispTransName();
		//PubDispString(_T("PARAM ERR"), 4|DISP_LINE_LEFT);
		Beep();

#endif
	}



// 	switch (ret)
// 	{
// 	case CLSS_CVMDECLINE:
// 		ScrPrint(0, 2, 0,"CL TXN not permitted");
// 		break;
// 	case ICC_RESET_ERR:
// 	case ICC_CMD_ERR:
// 		ScrPrint(0, 2, 0,"CARD READ ERROR");
// 		break;
// 	case ICC_BLOCK:
// 		ScrGotoxy(3, 2);
// 		printf("  CARD Blocked  ");
// 		break;
// 	case CLSS_CARD_EXPIRED:
// 		ScrGotoxy(3, 2);
// 		printf("  CARD EXPIRED ");
// 		break;
// 	case EMV_RSP_ERR:
// 	case EMV_DATA_ERR:
// 	case CLSS_TERMINATE:
// 		ScrPrint(0, 2, 0,"TXN TERMINATED");
// 		break;
// 	case CLSS_USE_CONTACT:
// 		ScrPrint(0, 2, 0,"USE CONTACT");
// 		break;
// 	case EMV_TIME_OUT:
// 		ScrGotoxy(3, 2);
// 		printf(" Time  Out ");
// 		break;
// 	case EMV_USER_CANCEL:
// 		ScrGotoxy(3, 2);
// 		printf(" Trans Canceled ");
// 		break;
// 	case EMV_APP_BLOCK:
// 		ScrGotoxy(3, 2);
// 		printf("   App Blocked  ");
// 		break;
// 	case EMV_NOT_ACCEPT:
// 		ScrPrint(0, 2, 0,"NOT ACCEPTED");
// 		break;
// 	case EMV_DENIAL:
//
// 		ScrPrint(0, 2, 0,"DCLINED");
//
// 		break;
// 	case NO_TRANS_LOG:
// 		ScrClrLine(2, 8);
// 		ScrPrint(0, 2, 0x01," No trans log! ");
// 		break;
//
// 	case RECORD_NOTEXIST:
// 		ScrClrLine(2, 8);
// 		ScrPrint(0, 2, 0x01, " All Log Readed!");
// 		break;

// 	case EMV_NO_APP:
// 		ScrClrLine(2, 8);
// 		ScrPrint(0, 2, 0,"USE CONTACT"); // 使用接触式界面交易
// 		break;
// 	case CLSS_PARAM_ERR:
// 		ScrClrLine(2, 8);
// 		ScrPrint(0, 2, 0x01, "PARAM ERR");
// 		break;
// 	default:
// 		ScrClrLine(2, 8);
// 		ScrGotoxy(2, 0);
// 		printf("Unknow Err:%d", ret);
// 		break;
// 	}

	DelayMs(200);
	Beef(0,100);
	appRemovePicc();

	return ret;
}
*/

void vAppInitPaymentData_MC(void)//KS.2 paypass 3.0.1 by zhoujie
{
	memset(gl_aucOutcomeParamSet, 0, sizeof(gl_aucOutcomeParamSet));
	gl_aucOutcomeParamSet[0] = OC_END_APPLICATION;//for s51.13
	gl_aucOutcomeParamSet[1] = OC_NA;//s51.3
	gl_aucOutcomeParamSet[2] = OC_NA;
	gl_aucOutcomeParamSet[3] = OC_NA;
	gl_aucOutcomeParamSet[4] |= 0x10;
	gl_aucOutcomeParamSet[5] = OC_NA;
	gl_aucOutcomeParamSet[6] = 0xFF;

	memset(gl_aucUserInterReqData, 0, sizeof(gl_aucUserInterReqData));
	gl_aucUserInterReqData[0] = MI_NA;
	gl_aucUserInterReqData[1] = MI_NA;
	memcpy(gl_aucUserInterReqData+2, MSG_HOLD_TIME_VALUE, 3);

	memset(gl_aucErrIndication, 0, sizeof(gl_aucErrIndication));
	gl_aucErrIndication[5] = MI_NA;

	vAppInitDiscData_MC();
}

unsigned char ucAppGetTransACType(void)
{
	return gl_ucACType;
}


//**************************************************************
unsigned char *App_GetFirstTLV(unsigned char *dat, unsigned char *datend, unsigned char *pucTag, unsigned int *punLen)//  [1/9/2013 ZhouJie]
{
	uint i, j, unTempDataLen=0;
	unsigned char ucTagLen=0;
	unsigned char *pucDataStart=NULL, *pucDataEnd=NULL, *pucTempTag=NULL;
	if (dat == NULL || datend == NULL || pucTag == NULL || punLen == NULL)
	{
		return NULL;
	}
	pucDataStart = dat;
	pucDataEnd = datend;
	while (pucDataStart < pucDataEnd)
	{
		ucTagLen=0;
		pucTempTag = pucDataStart++;
		ucTagLen++;
		if (pucTempTag[0] == 0x00)
		{
			continue;
		}
		if ((pucTempTag[0] & 0x1F) == 0x1F)
		{
			ucTagLen++;
			while (pucDataStart < pucDataEnd && (*pucDataStart & 0x80))
			{
				pucDataStart++;
				ucTagLen++;
			}
			pucDataStart++;
		}
		if (pucDataStart >= pucDataEnd) // no length // G06-9260[11/24/2011 zhoujie]
		{
			return NULL;
		}
		if (*pucDataStart & 0x80)
		{
			i = (*pucDataStart & 0x7F);
			if (pucDataStart + i > pucDataEnd)
			{
				return NULL;
			}
			pucDataStart++;
			for (j = 0; j < i; j++)
			{
				unTempDataLen <<= 8;
				unTempDataLen += *pucDataStart++;
			}
		}
		else
		{
			unTempDataLen = *pucDataStart++;
		}
		memcpy(pucTag, pucTempTag, ucTagLen);
		*punLen = unTempDataLen;
		return pucDataStart;
	}
	return NULL;
}

unsigned char *App_SearchTLV(int DolList, unsigned char *pucTag, unsigned char *dat, unsigned char *datend, int *nLen)
{
	uint unTempLen;
	unsigned char aucTempTag[4];
	unsigned char *pucValue=NULL;
	unsigned char *pucDataStart, *pucDataEnd;
	if (dat == NULL || datend == NULL || pucTag == NULL)
	{
		return NULL;
	}
	memset(aucTempTag, 0, sizeof(aucTempTag));//for three byte of tag
	pucDataStart = dat;
	pucDataEnd = datend;
	while (pucDataStart < pucDataEnd)
	{
		if (*pucDataStart == 0x00)
		{
			pucDataStart++;
			continue;
		}
		pucValue = App_GetFirstTLV(pucDataStart, pucDataEnd, aucTempTag, &unTempLen);
		if (pucValue == NULL)//can't find a TLV
		{
			return NULL;
		}
		if (!memcmp(aucTempTag, pucTag, ucAppGetTagLen(pucTag))) //successful
		{
			if (nLen != NULL)
			{
				*nLen = unTempLen;
			}
			return pucValue;
		}
		pucDataStart = pucValue;
		if (aucTempTag[0] & 0x20)
		{
			continue;
		}
		if (DolList)
		{
			continue;
		}
		pucDataStart += unTempLen;//continue search next TLV
	}
	return NULL;
}
int nAppSaveTransDataOutput_MC(unsigned char ucMsgFlag)
{
	uint unLen=0, unBuffLen=0;
	unsigned char aucBuff[1024];
	if(ucMsgFlag & CLSS_DATA_OCPS)
	{
		Clss_GetTLVDataList_MC("\xDF\x81\x29", 3, sizeof(gl_aucOutcomeParamSet), gl_aucOutcomeParamSet, &unLen);
	}
	if(ucMsgFlag & CLSS_DATA_UIRD)
	{
		Clss_GetTLVDataList_MC("\xDF\x81\x16", 3, sizeof(gl_aucUserInterReqData), gl_aucUserInterReqData, &unLen);
	}
	if(ucMsgFlag & CLSS_DATA_DISD)
	{
		Clss_GetTLVDataList_MC("\xDF\x81\x15", 3, sizeof(gl_aucErrIndication), gl_aucErrIndication, &unLen);
		memset(aucBuff, 0, sizeof(aucBuff));
		Clss_GetTLVDataList_MC("\xFF\x81\x06", 3, sizeof(aucBuff), aucBuff, &unBuffLen);
		if (App_SearchTLV(0, (unsigned char *)"\x9F\x42", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucAppCurrCodeFlg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucAppCurrCodeFlg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucAppCurrCodeFlg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\xDF\x81\x04", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucBalBeforeGACFlg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucBalBeforeGACFlg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucBalBeforeGACFlg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\xDF\x81\x05", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucBalAfterGACFlg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucBalAfterGACFlg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucBalAfterGACFlg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\xDF\x81\x02", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucDSSum3Flg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucDSSum3Flg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucDSSum3Flg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\xDF\x81\x0B", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucDSSumStFlg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucDSSumStFlg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucDSSumStFlg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\xDF\x81\x15", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				Clss_GetTLVDataList_MC((unsigned char*)"\xDF\x81\x15", 3, sizeof(gl_aucErrIndication), gl_aucErrIndication, &unLen);
				gl_DiscretionayData.ucErrIndicFlg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucErrIndicFlg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucErrIndicFlg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\xDF\x81\x0E", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucPostGACPutDataStFlg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucPostGACPutDataStFlg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucPostGACPutDataStFlg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\xDF\x81\x0F", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucPreGACPutDataStFlg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucPreGACPutDataStFlg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucPreGACPutDataStFlg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\x9F\x6E", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucThirdPartyDataFlg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucThirdPartyDataFlg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucThirdPartyDataFlg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\xFF\x81\x01", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucTornRecordFlg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucTornRecordFlg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucTornRecordFlg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\xDF\x81\x2A", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucDDCardTrack1Flg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucDDCardTrack1Flg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucDDCardTrack1Flg = APP_TAG_NOT_EXIST;
		}
		if (App_SearchTLV(0, (unsigned char *)"\xDF\x81\x2B", aucBuff, aucBuff+unBuffLen, &unLen))
		{
			if (unLen)
			{
				gl_DiscretionayData.ucDDCardTrack2Flg = APP_TAG_EXIST_WITHVAL;
			}
			else
			{
				gl_DiscretionayData.ucDDCardTrack2Flg = APP_TAG_EXIST_NOVAL;
			}
		}
		else
		{
			gl_DiscretionayData.ucDDCardTrack2Flg = APP_TAG_NOT_EXIST;
		}
	}
	return EMV_OK;
}

unsigned char ucAppGetTagLen(unsigned char *pucTag)
{
	unsigned char ucTagLen=0;
	int i;
	unsigned char szTmpTag[4];

	if (pucTag == NULL)
	{
		return 0;
	}

	//	if (pucTag[0] == 0x00)
	//	{
	//		return 0;
	//	}
	memset (szTmpTag, 0, sizeof(szTmpTag));
	for (i=0; i<4; i++)
	{
		if (pucTag[i] != 0x00)
		{
			memcpy(szTmpTag, &pucTag[i], 4-i);
			break;
		}
	}
	ucTagLen = 4-i;

	//	ucTagLen=0;
	//	if ((pucTag[0] & 0x1F) == 0x1F)
	//	{
	//		ucTagLen++;
	//		// add ucTagLen<4, invoid to check tag length over 4 bytes in case that tag variable is not initiated before set value
	//		// for example: pucTag is wrong set as 9F CC CC CC and the bytes ofter pucTag are not 0 [9/12/2014 ZhouJie]
	//		while (ucTagLen<4 && (pucTag[ucTagLen]&0x80))
	//		{
	//			ucTagLen++;
	//		}
	//		ucTagLen++;
	//	}
	//	else
	//	{
	//		ucTagLen=1;
	//	}
	return ucTagLen;
}


int cClssSendTransDataOutput_MC(unsigned char ucMsgFlag)
{
	return nAppSaveTransDataOutput_MC(ucMsgFlag);
}

//set pboc  aid parameter
void InitPbocAid_clss(void)
{
	Clss_PbocAidParam tPbocAidParam = {0};

	tPbocAidParam.ulTermFLmt = 500000;

	Clss_SetPbocAidParam_Pboc(&tPbocAidParam);
}
