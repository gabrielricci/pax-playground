/*
 ============================================================================
 Name        : Device.c
 Author      : PAX
 Version     : 
 Copyright   : PAX Computer Technology(Shenzhen) CO., LTD
 Description : PAX POS Static Library
 ============================================================================
 */

#include "aes.h"
#include "Device.h"
//#include "clss_rfcard_detect.h"

int gl_nCancelKeyFlag=0;
unsigned char gl_ucCancelKeyValue = KEYCANCEL;

#define ENTLA "\x00\x80"
#define IDA "\x31\x32\x33\x34\x35\x36\x37\x38\x31\x32\x33\x34\x35\x36\x37\x38"

int SM2_Verify(unsigned char *pubkey,unsigned char *input,int input_len,const unsigned char *sig, int siglen){return 1;}
int Gen_Za(unsigned char *entla,int entla_len,unsigned char *ida,int ida_len,char *pubkey){return 1;}
void sm3( unsigned char *input, int ilen,unsigned char *output){};


#ifndef _LINUX_TERM // for monitor platform
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int  DEVICE_FOpen(char *filename, unsigned char mode)
{
	int fid = 0;

	fid = open(filename, mode);
	if (fid<0)
	{
		return DEVICE_FILE_ERROR;
	}
	return fid;
}

int  DEVICE_FRead(int fid, unsigned char *dat, int len)
{
	int nRet = 0;
	
	nRet = read(fid, dat, len);
	if (nRet<0)
	{
		return DEVICE_FILE_ERROR;
	}
	return nRet;
}

int  DEVICE_FWrite(int fid, unsigned char *dat, int len)
{
	int nRet = 0;
	
	nRet = write(fid, dat, len);
	if (nRet<0)
	{
		return DEVICE_FILE_ERROR;
	}
	return nRet;
}

int  DEVICE_FClose(int fid)
{
	int nRet = 0;
	
	nRet = close(fid);
	if (nRet<0)
	{
		return DEVICE_FILE_ERROR;
	}
	return DEVICE_FILE_OK;
}

int  DEVICE_FRemove(char *filename)
{
	int nRet = 0;

	nRet = remove(filename);
	if (nRet<0)
	{
		return DEVICE_FILE_ERROR;
	}
	return DEVICE_FILE_OK;
}

void DEVICE_DES(unsigned char *input, unsigned char *output, unsigned char *deskey, int mode)
{
	des(input, output, deskey, mode);
}

int DEVICE_AES(unsigned char *Input, unsigned char *Output, unsigned char *AesKey, int KeyLen, int Mode)
{
	int nRet = 0;

	nRet = AES(Input, Output, AesKey, KeyLen, Mode);
	if (nRet)
	{
		return DEVICE_ARITH_ERROR;
	}
	return DEVICE_ARITH_OK;
}

void DEVICE_Hash(unsigned char* DataIn, unsigned int DataInLen, unsigned char* DataOut)
{
	Hash(DataIn, DataInLen, DataOut);
}
int DEVICE_RSARecover(unsigned char *pbyModule, unsigned int dwModuleLen,
 					  unsigned char *pbyExp, unsigned int dwExpLen,
 					  unsigned char *pbyDataIn, unsigned char *pbyDataOut)
{
	int nRet = 0;

	nRet = RSARecover(pbyModule, dwModuleLen, pbyExp, dwExpLen, pbyDataIn, pbyDataOut);
	if (nRet)
	{
		return DEVICE_ARITH_ERROR;
	}
	return DEVICE_ARITH_OK;
}

int DEVICE_SM2Verify(unsigned char *paucPubkeyIn,unsigned char *paucMsgIn,int nMsglenIn,
							   unsigned char *paucSignIn, int nSignlenIn)
{
	extern int SM2_Verify(unsigned char *pubkey,unsigned char *input,int input_len,const unsigned char *sig, int siglen);
	extern int Gen_Za(unsigned char *entla,int entla_len,unsigned char *ida,int ida_len,char *pubkey);

	int nRet = 0;
	
	Gen_Za(ENTLA, 2, IDA, 16, paucPubkeyIn);
	nRet = SM2_Verify(paucPubkeyIn,paucMsgIn,nMsglenIn,paucSignIn,nSignlenIn);
	if (nRet)
	{
		return DEVICE_ARITH_ERROR;
	}
	return DEVICE_ARITH_OK;
}

void DEVICE_SM3(unsigned char *paucMsgIn, int nMsglenIn,unsigned char *paucResultOut)
{
	extern void sm3( unsigned char *input, int ilen,unsigned char *output);
	sm3(paucMsgIn, nMsglenIn,paucResultOut);
}

unsigned char DEVICE_IccIsoCommand(unsigned char ucslot, APDU_SEND *tApduSend, APDU_RESP *tApduRecv)
{
	return IccIsoCommand(ucslot, tApduSend, tApduRecv);
}

unsigned char	DEVICE_PiccOpen(void)
{
	gl_nCancelKeyFlag = 0;

	return PiccOpen();
}

void vChangeCancelKeyFlag(unsigned char ucContral)
{
	PICC_PARA tpicc_para;

	memset(&tpicc_para, 0, sizeof(tpicc_para));

	if ((ucContral&0x01)==1 && gl_nCancelKeyFlag==0)
	{
		gl_nCancelKeyFlag = 1;

		PiccSetup('r', &tpicc_para);
		tpicc_para.user_control_w = 1;
		tpicc_para.user_control_key_val=gl_ucCancelKeyValue;
		PiccSetup('w', &tpicc_para);
	}
	else if ((ucContral&0x01)==0 && gl_nCancelKeyFlag==1)
	{
		gl_nCancelKeyFlag = 0;

		PiccSetup('r', &tpicc_para);
		tpicc_para.user_control_w = 1;
		tpicc_para.user_control_key_val=0;
		PiccSetup('w', &tpicc_para);
	}
}

unsigned char   DEVICE_PiccDetect(unsigned char Mode,unsigned char *CardType,unsigned char *SerialNo,unsigned char *cid,unsigned char *Other)
{
	int nRet=0;


	nRet = PiccDetect(Mode,CardType,SerialNo,cid,Other);
	if (nRet == 0)
	{
//		vChangeCancelKeyFlag(ucContral);
		return DEVICE_PICC_OK;
	}
	else if (nRet == 0x03)
	{
		return DEVICE_PICC_NO_PICCARD;
	}
	else if (nRet == 0x04)
	{
		return DEVICE_PICC_MORE_PICCARD;
	}
	else if (nRet == 0x13)
	{
		return DEVICE_PICC_CARD_ACTIVE_ERR;
	}
	else if (nRet == 0x17)
	{
		return DEVICE_PICC_CARD_COMMU_ERR;
	}
	return DEVICE_PICC_OTHER_ERR;
}

//ucContral:bit 1 - Enable CANCELKEY or not
unsigned char DEVICE_PiccIsoCommand(unsigned char ucCid, APDU_SEND *ptApduSend, APDU_RESP *ptApduRecv)
{
	unsigned char ucRet=0;

	ucRet = PiccIsoCommand(ucCid,ptApduSend,ptApduRecv);
	if (!ucRet)
	{
		return DEVICE_PICC_OK;
	}

	else if(ucRet == RET_RF_ERR_USER_CANCEL)//test case 3B02-9001 for paypass 3.0.1 by zhoujie
	{
		return DEVICE_PICC_USER_CANCEL;
	}
	else if(ucRet == 0x16)
	{
		return DEVICE_PICC_PROTOCOL_ERROR;
	}
	else if(ucRet == 0x17)
	{
		return DEVICE_PICC_TRANSMIT_ERROR;
	}
	else if(ucRet == 0x15)
	{
		return DEVICE_PICC_TIME_OUT_ERROR;
	}
	else
	{
		return DEVICE_PICC_OTHER_ERR;
	}
}
unsigned char   DEVICE_PiccRemove(unsigned char mode,unsigned char cid)
{
	return PiccRemove(mode,cid);
}
void    DEVICE_PiccClose(void)
{
	PiccClose();
}
void	DEVICE_PiccLight(unsigned char ucLedIndex,unsigned char ucOnOff)
{
	PiccLight(ucLedIndex,ucOnOff);
}

// void PXX_InitSeed(void)
// {
// 	unsigned short i;
//     unsigned char Buf[10];
// 
//     GetTime(Buf);
//     i = (Buf[4] << 8) + Buf[5];
//     srand(i);
// }
// 
// void PXX_GetRand(unsigned char *buf, int len)
// {
//     unsigned short i, j;
// 
//     for (j = 0; j < len; j++)
// 	{
//         i = rand();
//         buf[j++] = (unsigned char)i;
//         if (j < len) buf[j] = (unsigned char)(i >> 8);
//     }
// }
// 
// void SXX_GetRand(unsigned char *buf, int len)
// {
// 	unsigned short i;
// 	unsigned char random[8];
// 	if (buf == NULL)
// 	{
// 		return;
// 	}
// 
// 	i = 0;
// 	while(len-i>=8)
// 	{
// 		PciGetRandom(random);
// 		memcpy(buf+i, random, 8);
// 		i += 8;
// 	}
// 	PciGetRandom(random);
// 	memcpy(buf+i, random, len-i);
// }

void DEVICE_GetRand(unsigned char *buf, int len)
{
	unsigned int timecnt;
	uchar buff[9],sn[32];
	static uchar aucBuf[9],aucBuf1[9];
	static int sncnt=0;
	
	memset(sn,0,sizeof(sn));
	timecnt = GetTimerCount();
	
	if ((buf == NULL)||(len >8))
		return;
	
	memcpy(buff,(char*)(&timecnt),4);
	if(sncnt==0)
	{
		ReadSN(sn);
		sncnt = atoi(sn);
		if(sncnt==0)
		{
			memcpy(&sncnt,&timecnt-4,4);
		}
	}
	memcpy(buff+4,(char*)(&sncnt),4);
	
	des(buff,aucBuf,(uchar *)"\x16\x89\x88\xAA\x66\x58\x51\x8F",1);
	des(aucBuf,aucBuf1,aucBuf,1);
    memcpy(buf,aucBuf1,len);
// 	uchar aucPOSTypeNum[40];
// 
// #ifdef _SXX_TERM
// 	
// 	GetTermInfo(aucPOSTypeNum);
// 	if (aucPOSTypeNum[0] == 0x80)//R50
// 	{
// 		PXX_GetRand(buf, len);
// 	}
// 	else
// 	{
// 		SXX_GetRand(buf, len);
// 	}
// // 	ScrCls();// for test [2/26/2014 ZhouJie]
// // 	ScrPrint(0,0,0,"S:%02x %02x %02x %02x",buf[0],buf[1],buf[2],buf[3]);
// // 	getkey();
// #else
// //	PXX_InitSeed();
// 	PXX_GetRand(buf, len);
// // 	ScrCls();// for test [2/26/2014 ZhouJie]
// // 	ScrPrint(0,0,0,"P:%02x %02x %02x %02x",buf[0],buf[1],buf[2],buf[3]);
// // 	getkey();
// #endif
}

void DEVICE_GetTime(unsigned char *time)
{
	GetTime(time);
}

void DEVICE_ReadSN(unsigned char *SerialNo)
{
	ReadSN(SerialNo);
}

void DEVICE_TimerSet(unsigned char *pucTimerNo, unsigned short usTimeMS)
{
	*pucTimerNo = 0;// this number can be indicated by application [1/14/2014 ZhouJie]
	TimerSet(*pucTimerNo, (unsigned short)(usTimeMS/100));
}

unsigned short DEVICE_TimerCheck(unsigned char TimerNo)
{
	unsigned short usRet=0;

	usRet = TimerCheck(TimerNo)*100;
	return usRet;
}

void DEVICE_DelayMs(unsigned short usTimeMS)
{
	DelayMs(usTimeMS);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else //#ifndef _LINUX_TERM
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_TIMER ST_Timer;
void DEVICE_TimerSet(unsigned char *pucTimerNo, unsigned short usTimeMS)
{
	memset(&ST_Timer, 0, sizeof(ST_Timer));
	OsTimerSet(&ST_Timer, usTimeMS);
	*pucTimerNo = 0;// this number can be indicated by application [1/14/2014 ZhouJie]
}

unsigned short DEVICE_TimerCheck(unsigned char TimerNo)
{
	unsigned long ulRet=0;
	
	ulRet = OsTimerCheck(&ST_Timer);
	return (unsigned short)ulRet;
}

void DEVICE_DelayMs(unsigned short usTimeMS)
{
	usleep(usTimeMS*1000);
}

void DEVICE_GetRand(unsigned char *buf, int len)
{
	OsGetRandom(buf, len);
}

int  DEVICE_FOpen(char *filename, unsigned char mode)
{
	int fid = 0;

//	fid = open(filename, mode);
	if (fid<0)
	{
		return DEVICE_FILE_ERROR;
	}
	return fid;
}

int  DEVICE_FRead(int fid, unsigned char *dat, int len)
{
	int nRet = 0;
	
//	nRet = read(fid, dat, len);
	if (nRet<0)
	{
		return DEVICE_FILE_ERROR;
	}
	return nRet;
}

int  DEVICE_FWrite(int fid, unsigned char *dat, int len)
{
	int nRet = 0;
	
//	nRet = write(fid, dat, len);
	if (nRet<0)
	{
		return DEVICE_FILE_ERROR;
	}
	return nRet;
}

int  DEVICE_FClose(int fid)
{
	int nRet = 0;
	
//	nRet = close(fid);
	if (nRet<0)
	{
		return DEVICE_FILE_ERROR;
	}
	return DEVICE_FILE_OK;
}

int  DEVICE_FRemove(char *filename)
{
	int nRet = 0;

//	nRet = remove(filename);
	if (nRet<0)
	{
		return DEVICE_FILE_ERROR;
	}
	return DEVICE_FILE_OK;
}

void DEVICE_DES(unsigned char *input, unsigned char *output, unsigned char *deskey, int mode)
{
	OsDES(input, output, deskey, 8, mode);
}

int DEVICE_AES(unsigned char *Input, unsigned char *Output, unsigned char *AesKey, int KeyLen, int Mode)
{
	OsAES(Input, Output, AesKey, KeyLen, Mode);
	return DEVICE_ARITH_OK;
}

void DEVICE_Hash(unsigned char* DataIn, unsigned int DataInLen, unsigned char* DataOut)
{
	OsSHA(SHA_TYPE_1, DataIn, DataInLen, DataOut);
}
int DEVICE_RSARecover(unsigned char *pbyModule, unsigned int dwModuleLen,
					  unsigned char *pbyExp, unsigned int dwExpLen,
					  unsigned char *pbyDataIn, unsigned char *pbyDataOut)
{
	int nRet = 0;

	nRet = OsRSA(pbyModule, dwModuleLen, pbyExp, dwExpLen, pbyDataIn, pbyDataOut);
	if (nRet)
	{
		return DEVICE_ARITH_ERROR;
	}
	return DEVICE_ARITH_OK;
}

unsigned char	DEVICE_PiccOpen(void)
{
	gl_nCancelKeyFlag = 0;

	return OsPiccOpen();
}

void vChangeCancelKeyFlag(unsigned char ucContral)
{
	PCD_USER_ST pcd_user_config;

	memset(&pcd_user_config, 0, sizeof(PCD_USER_ST));

	OsPiccGetUserConfig(&pcd_user_config);
	if ((ucContral&0x01)==1 && gl_nCancelKeyFlag==0)
	{
		gl_nCancelKeyFlag = 1;
		pcd_user_config.check_cancel_key_w = 1;
		pcd_user_config.check_cancel_key_val = 1;
	}
	else if ((ucContral&0x01)==0 && gl_nCancelKeyFlag==1)
	{
		gl_nCancelKeyFlag = 0;
		pcd_user_config.check_cancel_key_w = 1;
		pcd_user_config.check_cancel_key_val = 0;
	}
	OsPiccSetUserConfig(&pcd_user_config);
}

unsigned char   DEVICE_PiccDetect(unsigned char Mode,unsigned char *CardType,unsigned char *SerialNo,unsigned char *cid,unsigned char *Other)
{
	int nRet=0;
	unsigned char atqx[13];
	unsigned char aucRats[254];
	char cMyCardType;

	nRet = OsPiccPoll(&cMyCardType, atqx);

	if(nRet == PCD_ERR_WTO_FLAG)
	{
		return DEVICE_PICC_NO_PICCARD;
	}
	else if(nRet == PCD_ERR_COLL_FLAG)
	{
		return DEVICE_PICC_MORE_PICCARD;
	}
	else if(nRet)
	{
		return DEVICE_PICC_OTHER_ERR;
	}

	nRet = OsPiccAntiSel(cMyCardType, SerialNo, atqx[0], Other);
	if(nRet)
	{
		return DEVICE_PICC_CARD_COMMU_ERR;
	}
	nRet = OsPiccActive(cMyCardType, aucRats);

	if(nRet)
	{
		return DEVICE_PICC_CARD_ACTIVE_ERR;
	}
	return 0;
}

//ucContral:bit 1 - Enable CANCELKEY or not
unsigned char DEVICE_PiccIsoCommand(unsigned char ucCid, APDU_SEND *ptApduSend, APDU_RESP *ptApduRecv)
{
	unsigned char ucRet=0;
	ST_APDU_REQ ST_ApduSend;
	ST_APDU_RSP ST_ApduRecv;
	int k;

	memset(&ST_ApduSend, 0, sizeof(ST_APDU_REQ));
	memset(&ST_ApduRecv, 0, sizeof(ST_APDU_RSP));

	memcpy(ST_ApduSend.Cmd, ptApduSend->Command, 4);
	ST_ApduSend.LC = ptApduSend->Lc;
	ST_ApduSend.LE = ptApduSend->Le;
	memcpy(ST_ApduSend.DataIn, ptApduSend->DataIn, 512);

	//memcpy(&ST_ApduSend, ptApduSend, sizeof(ST_APDU_REQ));

//#ifdef _DEBUG
//	//int k;
//	printf("DEVICE_PiccIsoCommand ST_ApduSend ");
//	for(k=0; k<4; k++)
//		printf("%02x ", ST_ApduSend.Cmd[k]);
//	printf("\n");
//	printf("DEVICE_PiccIsoCommand LC = %x\n", ST_ApduSend.LC);
//	printf("DEVICE_PiccIsoCommand LE = %x\n", ST_ApduSend.LE);
//	printf("DEVICE_PiccIsoCommand Data = ");
//	for(k=0; k<4; k++)
//		printf("%02x ", ST_ApduSend.DataIn[k]);
//	printf("\n");
//#endif
	ucRet = OsPiccIsoCommand(ucCid,&ST_ApduSend,&ST_ApduRecv);



	memcpy( ptApduRecv->DataOut, ST_ApduRecv.DataOut,512);
	ptApduRecv->SWA = ST_ApduRecv.SWA;
	ptApduRecv->SWB = ST_ApduRecv.SWB;
	ptApduRecv->LenOut = ST_ApduRecv.LenOut;

//#ifdef _DEBUG
//	//int k;
//	printf("OsPiccIsoCommand SWA = %x\n", ptApduRecv->SWA);
//	printf("OsPiccIsoCommand SWb = %x\n", ptApduRecv->SWB);
//	printf("OsPiccIsoCommand LenOut = %x\n", ptApduRecv->LenOut);
//	printf("OsPiccIsoCommand Data = ");
//	for(k=0; k<ptApduRecv->LenOut; k++)
//		printf("%02x ", ptApduRecv->DataOut[k]);
//	printf("\n");
//#endif

	//memcpy(ptApduRecv, &ST_ApduRecv, sizeof(ST_APDU_RSP));
	if (!ucRet)
	{
		return DEVICE_PICC_OK;
	}

	else if(ucRet == PCD_ERR_USER_CANCEL)//test case 3B02-9001 for paypass 3.0.1 by zhoujie
	{
		return DEVICE_PICC_USER_CANCEL;
	}
	else if(ucRet == PCD_ERR_PROTOCOL_FLAG)
	{
		return DEVICE_PICC_PROTOCOL_ERROR;
	}
	else if(ucRet == PCD_ERR_TRANSMIT_FLAG)
	{
		return DEVICE_PICC_TRANSMIT_ERROR;
	}
	else if(ucRet == PCD_ERR_WTO_FLAG)//time over
	{
		return DEVICE_PICC_TIME_OUT_ERROR;
	}
	else
	{
		return DEVICE_PICC_OTHER_ERR;
	}
}
unsigned char   DEVICE_PiccRemove(unsigned char mode,unsigned char cid)
{
	return OsPiccRemove();
}
void    DEVICE_PiccClose(void)
{
	OsPiccClose();
}
void	DEVICE_PiccLight(unsigned char ucLedIndex,unsigned char ucOnOff)
{
    return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //#ifndef _LINUX_TERM

int  DEVICE_SetCancelKey(unsigned char ucKeyValue)
{
	gl_ucCancelKeyValue = ucKeyValue;
	return 0;
}

//the Byte 1 of pucParam indicates that support cancelkey in piccisocommand or not
unsigned char	DEVICE_SetControlParam (unsigned char *pucParam)
{
	if (pucParam==NULL)
	{
		return -1;
	}
	if ((pucParam[0] & 0x01)==1)
	{
		vChangeCancelKeyFlag(0x01);
	}
	else
	{
		vChangeCancelKeyFlag(0x00);
	}
	return 0;
}

