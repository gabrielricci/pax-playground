#include "emvlib_Prolin.h"
#if 0
//Callback Functions
int  cEMVWaitAppSel(int TryCnt, EMV_APPLIST List[], int AppNum)
{
	return 0;
}

int  cEMVInputAmount(unsigned long *AuthAmt, unsigned long *CashBackAmt)
{
	return 0;
}
int  cEMVGetHolderPwd(int TryFlag, int RemainCnt, unsigned char *pin)
{
	return 0;
}
int  cEMVUnknowTLVData(unsigned short Tag, unsigned char *dat, int len)
{
	return 0;
}
void cEMVAdviceProc(void)
{
	return ;
}
void cEMVVerifyPINOK(void)
{
	return ;
}
int  cCertVerify(void)
{
	return 0;
}
int  cEMVSetParam(void)
{
	return 0;
}

unsigned char  cEMVPiccIsoCommand(unsigned char cid,APDU_SEND *ApduSend,APDU_RESP *ApduRecv)
{
	return 0;
}

unsigned char cEMVSM2Verify(unsigned char *paucPubkeyIn,unsigned char *paucMsgIn,int nMsglenIn,
							   unsigned char *paucSignIn, int nSignlenIn)
{
	return 0;
}
unsigned char cEMVSM3(unsigned char *paucMsgIn, int nMsglenIn,unsigned char *paucResultOut)
{
	return 0;
}

int cEMVPedVerifyPlainPin(uchar ucIccSlot, uchar *pucExpPinLenIn, uchar *ucIccRespOut, uchar ucMode,ulong ulTimeoutMs)
{
	return 0;
}
int cEMVPedVerifyCipherPin(uchar ucIccSlot, uchar *pExpPinLenIn, RSA_PINKEY *tRsaPinKeyIn, uchar *pucIccRespOut, uchar ucMode,ulong ulTimeoutMs)
{
	return 0;
}

unsigned char  cEMVIccIsoCommand(unsigned char cid,APDU_SEND *tApduSend,APDU_RESP *tApduRecv)
{
	int iRet;
	char szBuff[1024] ={0};
	ST_APDU_REQ ApduReq;
	ST_APDU_RSP ApduRsp;


		memset(&ApduReq, 0, sizeof(ST_APDU_REQ));
		memset(&ApduRsp, 0, sizeof(ST_APDU_RSP));
		memcpy(ApduReq.Cmd, tApduSend->Command, 4);
		memcpy(ApduReq.DataIn, tApduSend->DataIn, sizeof(tApduSend->DataIn));
		ApduReq.LC = tApduSend->Lc;
		ApduReq.LE = tApduSend->Le;
		memcpy(&ApduRsp, tApduRecv, sizeof(APDU_RESP));


		OsLog(LOG_DEBUG,"IN: \t LC=%d LE=%d\n", ApduReq.LC, ApduReq.LE);
		OsLog(LOG_DEBUG,"%02x %02x %02x %02x\n", ApduReq.Cmd[0], ApduReq.Cmd[1], ApduReq.Cmd[2], ApduReq.Cmd[3]);
		OsLog(LOG_DEBUG,"%02x %02x %02x %02x\n", ApduReq.DataIn[0], ApduReq.DataIn[1], ApduReq.DataIn[2], ApduReq.DataIn[3]);
		OsLog(LOG_DEBUG,"%02x %02x %02x %02x\n", ApduReq.DataIn[4], ApduReq.DataIn[5], ApduReq.DataIn[6], ApduReq.DataIn[7]);
		OsLog(LOG_DEBUG,"%02x %02x %02x %02x\n", ApduReq.DataIn[5], ApduReq.DataIn[6], ApduReq.DataIn[7], ApduReq.DataIn[8]);
		OsLog(LOG_DEBUG,"%02x %02x %02x %02x\n", ApduReq.DataIn[9], ApduReq.DataIn[10], ApduReq.DataIn[11], ApduReq.DataIn[12]);
		OsLog(LOG_DEBUG,"%02x %02x %02x %02x\n", ApduReq.DataIn[13], ApduReq.DataIn[14], ApduReq.DataIn[15], ApduReq.DataIn[16]);


		iRet = OsIccExchange(0, 1, &ApduReq, &ApduRsp);
		if(iRet != RET_OK)
		{
			OsLog(LOG_DEBUG,"OsIccExchange return : %d\n", iRet);
			return iRet;
		}

		memcpy(tApduRecv->DataOut, ApduRsp.DataOut, ApduRsp.LenOut);
		tApduRecv->LenOut = ApduRsp.LenOut;
		tApduRecv->SWA = ApduRsp.SWA;
		tApduRecv->SWB = ApduRsp.SWB;

		OsLog(LOG_DEBUG,"OUT: \t SWA=%02x SWB=%02x\n", ApduRsp.SWA, ApduRsp.SWB);

		return EMV_OK;
}
#endif
