/*------------------------------------------------------------
* FileName: main.c
* Author: LuX
* Date: 2013-04-10
* Example of how to use Prolin OSAL API. It can only run on Prolin OS 2.4 or higher.
*
*	 						Warning
* This code is for reference only. I do not guarantee the correctness, safety, efficiency or completeness.
* I only test the code on the S800 POS and am not sure it can runs well on SXXX like S300, S900.
* You are welcome to send an e-mail to me(lux@paxsz.com) if you have any questions.
------------------------------------------------------------*/
#include "osal.h"
#include "debugger.h"
#include "font.h"
#include "header.h"
#include "lcd.h"
#include "keyboard.h"
#include "ui_base.h"
#include "emvlib_Prolin.h"
#include "signal.h"
#include "pub.h"
#include "Global.h"
#include <pthread.h>

//#include "emvdata.h"


#define LOGIC_KEY_ENTER  16
#define LOGIC_KEY_CANCEL 17
#define LOGIC_KEY_CLEAN  18

#define LOGIC_KEY_MENU   25
#define LOGIC_KEY_DOWN   20
#define LOGIC_KEY_UP     19
#define LOGIC_KEY_F1     23


#define XML_GETCARD    "getcard.xml"
#define XML_GETCARD_2D "getcard_2d.xml"
#define TI   10
#define DISP_TIMEOUT 3
#define TIMEOUT  -2
#define FAIL     -1
#define SUCC	 0
#define QUIT     -3
#define PLS_INSERT_CARD  9001
#define PLS_SWIPE_CARD   9002

#define TRACK1LEN   		79          //һ�ŵ�����
#define TRACK2LEN   		37          //���ŵ�����
#define TRACK3LEN   		104         //���ŵ�����

#define NO_READCARD    0
#define SWIPE   1
#define TAP   2
#define INSERT   3
//��Ƶ�ļ�
#define  AUDIO_CONFIRM      "./res/Please_confirm_account.wav"
#define  AUDIO_ENTER_AMT     "./res/Please_enter_amount.wav"
#define  AUDIO_ENTER_PIN    "./res/Please_enter_pin.wav"
#define  AUDIO_TRANS_SUCC   "./res/Transactions_success.wav"
//����ǩ���ļ�
#define SIGN_FILE         "./res/sign.bmp"

//����ʱ�䣨��λ��s��
#define TIME_SCREENSAVER  10*1000
#define TIME_SCREENSAVER_ENTRY  30*1000


enum PDK_CARD_RET					//����ģ��(-2800~-2999)
{
    CARD_ERR_BASE                   =   (-2800),             //���󷵻ػ���
    CARD_ERR_CHANNEL_NUM            =   CARD_ERR_BASE - 1,          //����ͨ���ļ�ʧ��
    CARD_ERR_PARA_NULL              =   CARD_ERR_BASE - 2,          //���Ϊ��
    CARD_ERR_INVAL_PARAM            =   CARD_ERR_BASE - 3,          //��������
    CARD_ERR_NO_SWIPE_CARD          =   CARD_ERR_BASE - 4,          //ûˢ��
    CARD_MAG_ERR_READ               =   CARD_ERR_BASE - 5,          //��������
    CARD_ICC_ERR_CLOSE              =   CARD_ERR_BASE - 6,          //�ر�ʧ��
    CARD_ICC_ERR_PULL_OUT           =   CARD_ERR_BASE - 7,          //���γ�(����SAM����,����SAM����δ���,��᷵��0X33�������)
    CARD_ICC_ERR_SLOT               =   CARD_ERR_BASE - 8,          //ͨ���Ŵ���
    CARD_ICC_ERR_COMM_TIMEOUT       =   CARD_ERR_BASE - 9,          //ͨ�ų�ʱ
    CARD_ICC_ERR_PARITY             =   CARD_ERR_BASE - 10,         //У�����
    CARD_ICC_ERR_NO_RESET           =   CARD_ERR_BASE - 11,         //û�и�λ��Ƭ
    CARD_ICC_ERR_PROTOCAL           =   CARD_ERR_BASE - 12,         //Э�����
    CARD_ICC_ERR_SEND_TOO_LONG      =   CARD_ERR_BASE - 13,         //��������̫��
    CARD_ICC_ERR_OPERATION          =   CARD_ERR_BASE - 14,         //IC����ʧ��
    CARD_ICC_ERR_NO_CARD            =   CARD_ERR_BASE - 15,         //�޿�����
    CARD_PICC_ERR_OPEN              =   CARD_ERR_BASE - 16,         //�ǽӴ�ʧ��
    CARD_PICC_ERR_DETECT            =   CARD_ERR_BASE - 17,         //̽��ʧ��
    CARD_PICC_ERR_COMMAND           =   CARD_ERR_BASE - 18,         //��Ƭ����APDU��ʽ������ʧ��
    CARD_PICC_ERR_REMOVE            =   CARD_ERR_BASE - 19,         //�ǽ��Ƴ�ʧ��
    CARD_PICC_ERR_M1_AUTH           =   CARD_ERR_BASE - 20,         //M1��Ȩʧ��
    CARD_PICC_ERR_M1_READ           =   CARD_ERR_BASE - 21,         //��ȡM1��ָ���������ʧ��
    CARD_PICC_ERR_M1_WRITE          =   CARD_ERR_BASE - 22,         //дM1��ָ���������ʧ��
    CARD_PICC_ERR_M1_OPERATION      =   CARD_ERR_BASE - 23,         //��M1����ָ�����ݿ����ʧ��
    CARD_ERR_READ_REAL_GLOBAL_VAR   =   CARD_ERR_BASE - 24,         //��ȡ������ȫ��������ʧ��
    CARD_ERR_WRITE_REAL_GLOBAL_VAR  =   CARD_ERR_BASE - 25,         //д������ȫ��������ʧ��

};

#define  AUDIO_CONFIRM        "./res/Please_confirm_account.wav"
#define  AUDIO_ENTER_AMOUNT   "./res/Please_enter_amount.wav"
#define  AUDIO_ENTER_PIN      "./res/Please_enter_pin.wav"
#define  AUDIO_TRANS_SUCC     "./res/Transactions_success.wav"
#define EMV_OK             0
// UI Prompt ID definitions
#define CLSS_USE_ONECARD	0X19
#define CLSS_COLLISION_DETECTED  -50

int EntryType = 0;
char szAudioFile[64];
int szAudioFlag = 0;
int iModule2dFlag = 0;
int gFallbackFlag = 0;

char szPosType[65] = {0};

static int PageExecute(char *psOutData, char *psSetTxtData,char *psSetPromptData, int TimeOut);


static int SignFlowOnPad(void)
{
	int		iRet=0;
	int 	iLoop=0;
//	long    lSigFileLen;
	int		iTry = 0, iMaxTry = 3;
	uchar	ucRet=0;
//	uchar	ucKey;
	uchar	ucTempBuf[100]={0}, ucSigToPrn[20000]={0};
//	uchar	ucTempBuf1[9], ucTempBuf2[9];

// ��չ����Ҫ��sign pad ������Ĭ��Ϊ115200,8,N,1
//	if(SignPadInited==0)
//	{
//		SignPad_Init(PORT_PINPAD,9600);
//
//		//change 9600 to 115200
//		SignPad_change_BaudRate(PORT_PINPAD);
//
//		SignPadInited = 1;
//	}
//
//	SignPad_Init(PORT_PINPAD,115200);

	OsPortClose(PORT_PINPAD);

	//LOGI("OsPortClose~~!!\n\n\n\n");

	for (iLoop=0;iLoop<1000;iLoop++)
	{
		;
	}

	iRet = OsPortOpen(PORT_PINPAD, "115200,8,n,1");

	//LOGI("OsPortOpen %d~~!!\n\n\n\n",iRet);

	while(iTry < iMaxTry)
	{
		iTry++;

		ucRet = SignPad_HandShake(PORT_PINPAD,3);

		if(ucRet != 1)
		{
			if(iTry < iMaxTry)
			{
				continue;
				//return -2;
			}
			else
			{
				SignPad_Close(PORT_PINPAD);
				return -2;
			}
		}

		//LOGI("\n\n\n++++++ HandShake OK! ++++++\n\n\n");

		ucRet = SignPad_SignStart(PORT_PINPAD, "PAX12345", 30);

		iRet = SignPad_SignEnd(PORT_PINPAD);
		SignPad_Close(PORT_PINPAD);

		if(ucRet != 1)
		{
			//LOGI("SignPad_SignEnd SIGN FAILED");
			return -3;
		}
		else
		{
			break;
		}
	}

	return 0;
}

void AuidoOut(char * pszAudioName)
{
	int fd, ret = 0;
	char *buff;
	int len;
	struct stat state;


	stat(pszAudioName, &state);
	len = state.st_size;
	buff = (char *) malloc(len * sizeof(char));
	fd = open(pszAudioName, O_RDONLY);
	if(fd<0) printf("Open File Fail\n");
	ret = read(fd, buff, len);
	ret = OsPlayWave(buff, len, 3, 0);
	if(ret != RET_OK) printf("PlayWave Fail\n");

	close(fd);
	free(buff);
}

static void *thread_fn(void *arg)
{
	while(1)
	{
		if(!szAudioFlag)
		{
	//		sleep(1);
			continue;
		}
		AuidoOut(szAudioFile);
		szAudioFlag = 0;

	}
	return ((void *)0);
}

void MainBcd2Asc(const char *psIn, int iLen, char *psOut)
{
	static const char sHexToChar[17] = "0123456789ABCDEF";
	int iCnt;

	if ((psIn == NULL) || (psOut == NULL))
	{
		return;
	}

	for (iCnt = 0; iCnt < iLen; iCnt++)
	{
		psOut[2*iCnt]   = sHexToChar[(psIn[iCnt] /10)];
		psOut[2*iCnt + 1] = sHexToChar[(psIn[iCnt] %10)];
	}
}

int PdkSys_GetTime(char *pszDateTime)
{
          char sDatetime[15];
          char szYearBuf[5];
          time_t tnow;
          struct tm* mytm;
          *pszDateTime = 0x00;
          if(NULL == pszDateTime)
                     return  -1;

          memset(sDatetime,0,sizeof(sDatetime));
          time(&tnow);
          mytm = localtime(&tnow);
          sDatetime[3] = mytm->tm_hour;
          sDatetime[4] = mytm->tm_min;
          sDatetime[5] = mytm->tm_sec;

          memcpy(pszDateTime, "20", 2);
          MainBcd2Asc(sDatetime, 6, pszDateTime+2);
          printf("psztime = %s",pszDateTime);
          memset(szYearBuf, 0, sizeof(szYearBuf));
          memcpy(szYearBuf, pszDateTime, 4);
          if (atoi(szYearBuf) >= 2050)
          {
              memcpy(pszDateTime, "19", 2);
          }
          pszDateTime[14] = 0;
          return 0;
}

int PdkParam_GetValue()
{
	return 0;
}


//������Ƶ�ļ�
int AudioPro(char * pszFileName)
{
	int fd, ret = 0;
	char *buff;
	int len;
	struct stat state;

	stat(pszFileName, &state);
	len = state.st_size;
	buff = (char *) malloc(len * sizeof(char));
	fd = open(pszFileName, O_RDONLY);
	if(fd<0)
	{
		printf("PlayWave Fail\n");
		return FAIL;
	}
	ret = read(fd, buff, len);
	ret = OsPlayWave(buff, len, 3, 0);
	if(ret != RET_OK)
	{
		printf("PlayWave Fail\n");
		return FAIL;
	}

	close(fd);
	free(buff);
	return SUCC;
}

//CLSS CARD PROCESS
int ClssCardDetect()
{
	int iRet;
	OsLog(LOG_DEBUG,"enter ClssCardDetect!");

DETECTCARD:
     while(1)
     {
 	   iRet = ClssDetectCard();
 	   OsLog(LOG_DEBUG,"ClssDetectCard:%d",iRet);

 		if ( iRet != EMV_OK )
 		{
 			if (CLSS_COLLISION_DETECTED == iRet)
 			{
 				//cUIPrompt(CLSS_USE_ONECARD);
 				 continue;
 			}
 			return iRet;
 		}
 		else
 		{
 			return 0;
 		};//

    }//end of while (1)
}


//�忨����
int Insert_Card_Proc(char *pszCardNo)
{
	int i, iRet, iLen = 0;
	char ucTempBuf[256]={0}, ucCardBuf[256]={0};

	iRet = GetICCData();
	printf("EMVGetTLVData2222 [%d]",iRet);
	OsLog(LOG_DEBUG,"GetICCData [iRet]= %d\n", iRet);
	if(iRet == 0)
	{
		memset(ucTempBuf, 0, sizeof(ucTempBuf));
		OsLog(LOG_DEBUG, "EMVGetTLVData");
		iRet = EMVGetTLVData(0x57, (uchar *)ucTempBuf, &iLen);
		OsLog(LOG_DEBUG,"EMVGetTLVData ret[%d]len[%d]", iRet, iLen);
		if(iRet == 0)
		{
			memset(ucCardBuf, 0, sizeof(ucCardBuf));
			OsLog(LOG_DEBUG,"PubBcd2Asc");
			PubBcd2Asc0(ucTempBuf, iLen, ucCardBuf);
			OsLog(LOG_DEBUG,"card [%d][%s]", iLen, ucCardBuf);
			iLen += iLen;
			for(i=0; i<iLen; i++)
			{
				if(ucCardBuf[i] == 'D' || ucCardBuf[i] == 'F')
				{

					memset(pszCardNo, 0, sizeof(pszCardNo));
					memcpy(pszCardNo, ucCardBuf, i);
					OsLog(LOG_DEBUG,"card [%d][%s]", iLen, pszCardNo);
					break;
				}
			}
			if (i < iLen)
			{
				return 0;
			}
		}
	}

	return -1;
}


int IcCommand(char *pszCardNo, int *piReadCardType)
{
	int i, iRet, iLen;
	char ucTempBuf[40]={0}, ucCardBuf[40]={0};
	int option, ret;
	uchar atr[30];
	memset(atr, 0, sizeof(atr));

	if(ret = OsIccOpen(0x00))
	{
		return 1;
	}

	//option = GetOption(VOLTAGE_5V, PPS, RESET_SPEED_9600, STANDARD_EMV);
	ret = OsIccInit(ICC_USER_SLOT,0,atr);
	OsLog(LOG_DEBUG,"OsIccInit ret = %d",ret);
	if(ret != 0 && ret == ERR_SCI_HW_NOCARD)
	{
		return 2;
	}

	if(ret = OsIccDetect(0x00))
	{
		EntryType = 3;
		return 1;
	}

//	memset(pszCardNo, 0, sizeof(pszCardNo));
//	strcpy(pszCardNo, "************1234");

	iRet = Insert_Card_Proc(pszCardNo);
	OsLog(LOG_DEBUG,"Insert_Card_Proc ret = %d",iRet);
	return 0;
}


/*****************************************************
���ܣ�     ���ַ���תΪ���ģʽ.
���������
    psStr    �����ִ�
�������:
    psAmtStr ����λС�������ִ�
���أ�     void
*****************************************************/
void Asc2Amt(const uchar *psStr, uchar *psAmtStr)
{
    uchar i = 0;
    uchar szTmp[25] = {0};
    uchar szTail[16] = {0};
    uchar ucLen = 0;

    if( NULL == psStr || NULL == psAmtStr)
    {
        return;
    }

    for(i=0;i<strlen((char *)psStr);i++)
    {
        if(psStr[i] != '0')
            break;
    }
    strcpy((char *)szTmp, (char *)(psStr+i));
    /*l_to_dec(szTmp);*/
    memcpy(szTail, "0.00", 4);
    szTail[4] = 0;

    ucLen = strlen((char*)szTmp);
    if (ucLen == 0)
    {
        memcpy(szTmp, szTail, 4);
        szTmp[4] = 0;
    }
    else if (ucLen == 1)
    {
        szTail[3] = szTmp[0];
        memcpy(szTmp, szTail, 4);
        szTmp[4] = 0;
    }
    else if (ucLen == 2)
    {
        szTail[2] = szTmp[0];
        szTail[3] = szTmp[1];
        memcpy(szTmp, szTail, 4);
        szTmp[4] = 0;
    }
    else
    {
        memcpy(szTail, szTmp, ucLen - 2);
        szTail[ucLen - 2] = '.';
        memcpy(szTail + ucLen - 1, szTmp + ucLen - 2, 2);
        szTail[ucLen + 1] = 0;
        memcpy(szTmp, szTail, ucLen + 1);
        szTmp[ucLen + 1] = 0;
    }

    strcpy((char *)psAmtStr, (char *)szTmp);

    return;
}

//========================================== �ǽ�   ===================================================
//=================================================================================================
/**************************************************************************************************
��������:  ��ָ����ģʽ��ѰPICC��Ƭ;�ѵ���Ƭ��,����ѡ�в�����

�������:
		uiChannel----ͨ����
		ucMode------�ѿ���ʽ
�������:
		pcCardType---�ѵ��Ŀ�����ָ�룬����ΪNULL
		psSerialInfo---��ſ�Ƭ���кŵĻ�����ָ�룬��Ƭ��Ψһ��ʶ
		pcCID--------��ſ�Ƭ�߼�ͨ���ŵĻ�����ָ��,��ΪNULL
��  ��  ֵ:
��        ע:
��  ��  ��:
����ʱ��:
***************************************************************************************************/
int PdkPicc_Detect(uint uiChannel,uchar ucMode,char *pcCardType,char *pszSerialInfo,char *pcCID)
{
    int iRet  = -1;
    int iRetA = -1;
	int iRetB = -1;
    unsigned char sSAK[10];
    unsigned char sResponse[12];

    if ( NULL == pszSerialInfo )
    {
    	   return CARD_ERR_PARA_NULL;
    }

    iRet = OsPiccOpen();
    if ( 0 != iRet )
    {
    	return CARD_PICC_ERR_OPEN;
    }

    if ( NULL == pcCID )
    {
        iRet = OsPiccPoll( pcCardType, sResponse );
    }
    else
    {
    	iRet = OsPiccPoll( pcCardType, (unsigned char *)pcCID );
    }

    if ( iRet )
    {
    	return CARD_PICC_ERR_DETECT;
    }

    iRetA = OsPiccAntiSel( 'A', (uchar *)pszSerialInfo, 0, sSAK );
    if ( iRetA )
    {
        iRetB = OsPiccAntiSel( 'B', (uchar *)pszSerialInfo, 0, sSAK );
        if ( iRetB )
        {
            return CARD_PICC_ERR_DETECT;
        }
    }
    return SUCC;
}

uchar IsChipCardSvcCode(uchar *pszTrack2)
{
	char	*pszSeperator;

	if( *pszTrack2==0 )
	{
		return FALSE;
	}

	pszSeperator = strchr((char *)pszTrack2, '=');
	if( pszSeperator==NULL )
	{
		return FALSE;
	}
	if( (pszSeperator[5]=='2') || (pszSeperator[5]=='6') )
	{
		return TRUE;
	}

	return FALSE;
}

//���ſ�
int PdkMag_Read(char *pszTk1, char *pszTk2, char *pszTk3)
{
    int iRet;
    ST_MSR_DATA stTrack1;
    ST_MSR_DATA stTrack2;
    ST_MSR_DATA stTrack3;


    if ( NULL == pszTk1 && NULL == pszTk2 && NULL == pszTk3 )
    {
        iRet = OsMsrSwiped();
        if ( 1 == iRet )
        {
            return SUCC;                 //swipe card action
        }
        else if ( 0 == iRet )
        {
            return CARD_ERR_NO_SWIPE_CARD;      //no swipe card action
        }
        return CARD_MAG_ERR_READ;
    }

	memset( &stTrack1,0,sizeof(ST_MSR_DATA) );
	memset( &stTrack2,0,sizeof(ST_MSR_DATA) );
	memset( &stTrack3,0,sizeof(ST_MSR_DATA) );

    if ( 0 == OsMsrSwiped() )
    {
        return CARD_ERR_NO_SWIPE_CARD;      //no swipe card action
    }

	iRet = OsMsrRead(&stTrack1,&stTrack2,&stTrack3);
	if ( 0 == iRet )
	{
		if ( 0 == stTrack1.Status )
		{
	    	if ( NULL != pszTk1 )
            {
		    	 memcpy(pszTk1,stTrack1.TrackData,stTrack1.DataLen);
            }
		}
			else if ( NULL != pszTk1 )
			{
				pszTk1[0] = '\0';
			}

			if ( 0 == stTrack2.Status )
			{
			    if ( NULL != pszTk2 )
                {
				    memcpy(pszTk2,stTrack2.TrackData,stTrack2.DataLen);
                }
			}
			else if ( NULL != pszTk2 )
			{
				pszTk2[0] = '\0';
			}

			if ( 0 == stTrack3.Status )
			{
			    if ( NULL != pszTk3 )
                {
				    memcpy(pszTk3,stTrack3.TrackData,stTrack3.DataLen);
                }
			}
			else if ( NULL != pszTk3 )
			{
				pszTk3[0] = '\0';
			}

			//add by richard to check icc card
			if(IsChipCardSvcCode((uchar *)pszTk2) && !gFallbackFlag)
			{
				return PLS_INSERT_CARD;
			}

			return 0;
	}
    return CARD_MAG_ERR_READ;

}

/***************************************************************************************************
��������:  �Ӵŵ���Ϣ��ȡ������
�������:               pszTk2-----�ŵ�2��Ϣ
                        pszTk3-----�ŵ�3��Ϣ
��������:
                        pszCardNo----��ȡ���Ŀ���
����ֵ:

��           ע:
������:
����ʱ��:
***************************************************************************************************/
int Card_GetPan(char * pszTk2, char * pszTk3, char *pszCardNo)
{

    int i = 0;

    char szTrack2Asc[TRACK2LEN+1];
    char szTrack3Asc[TRACK3LEN+1];

    if (pszTk2 && pszTk3 == NULL)
    {
        return FAIL;
    }

    memset(szTrack2Asc, 0x00, sizeof(szTrack2Asc));
    memset(szTrack3Asc, 0x00, sizeof(szTrack3Asc));

    memcpy(szTrack2Asc,pszTk2,TRACK2LEN);
    memcpy(szTrack3Asc,pszTk3,TRACK3LEN);

    if(strlen((char *)szTrack2Asc) != 0)
    {
        while (szTrack2Asc[i] != '=' && szTrack2Asc[i] != 'D')
        {
            i++;
            if(i > 19)
            {
                return FAIL;
            }
        }
        if( i < 13 || i > 19)
        {
            return FAIL;
        }
        memcpy(pszCardNo, szTrack2Asc, i);
        pszCardNo[i] = 0;
        return SUCC;
    }
    else if(strlen((char *)szTrack3Asc)!= 0)
    {
        while(szTrack3Asc[i] != '=' && szTrack3Asc[i] != 'D')
        {
            i ++;
            if(i > 21)
            {
                return FAIL;
            }
        }
        if( i < 15 || i > 21)
        {
            return FAIL;
        }
        memcpy(pszCardNo,szTrack3Asc+2,i-2);
        pszCardNo[i-2]=0;
        return SUCC;
    }

    return FAIL;
}



static int _PdkUI_Process(unsigned int uiChannel, int iTimeout, unsigned char *psDataOut,
                          unsigned int uiDataSize, int *puiLenOut)
{
    int iRet = 0;

    iRet = PdkUI_Process(uiChannel, iTimeout, psDataOut, uiDataSize, puiLenOut);
    if(iRet == 0 && iTimeout != DISP_TIMEOUT*1000 && iTimeout != 0 )
    {
        iRet = TIMEOUT;
    }
    else if (iRet == 0)
    {
        iRet = SUCC;
    }
    else if(iRet < 0)
    {
        iRet = FAIL;
    }
    return iRet;
}


static int PageExecute(char *psOutData, char *psSetTxtData,
		char *psSetPromptData, int TimeOut)
{
    int iRet = 0;
    int iOutLen = 0;
    char sOut[1024] = {0};


    if(psSetTxtData)
    {
    	iRet = PdkUI_SetAttribute(0,"txt","value",psSetTxtData);
    	if(iRet < 0)
    	{
    		return FAIL;
    	}
    }

    if(psSetPromptData)
    {
    	iRet = PdkUI_SetAttribute(0,"prompt","value",psSetPromptData);
    	if(iRet < 0)
    	{
    		return FAIL;
    	}
    }
    iRet = _PdkUI_Process(0, TimeOut * 1000, sOut, sizeof(sOut), &iOutLen);

    if (iRet == 1)
    {
    	//add by richard to trans the shortcut key
    	if(LOGIC_KEY_MENU == sOut[0])
    	{
    		return LOGIC_KEY_ENTER;
    	}
    	else if(LOGIC_KEY_DOWN == sOut[0])
    	{
    		return LOGIC_KEY_CLEAN;
    	}

    	if(0 == strcmp(szPosType, "s800"))
    	{
    		if(LOGIC_KEY_F1 == sOut[0])
    		{
    			return LOGIC_KEY_CANCEL;
    		}
    	}
    	else if(LOGIC_KEY_UP == sOut[0])
    	{
    		return LOGIC_KEY_CANCEL;
    	}

        return sOut[0];
    }
    if (iRet < 0)
    {
        return iRet;
    }

    if(psOutData && iRet == 2)
    {
        memcpy(psOutData, sOut, iOutLen);
    }

    return iRet;
}


void GlobalInit()
{

	swipted = 0;
	EntryType = 0;

	memset(cExpDate, 0, sizeof(cExpDate));
	OsLog(LOG_INFO, "before InitLiveApps ");
	//init
	Clss_CoreInit_Entry();
	Clss_CoreInit_Wave();
	Clss_CoreInit_MC(0);
	Clss_CoreInit_Pboc();
	InitLiveApps();//without setting emv param
}

void AppInit(void)
{
	OsLogSetTag("debug");
	signal(SIGILL,    OsSaveCrashReport);
	signal(SIGABRT,   OsSaveCrashReport);
	signal(SIGBUS,    OsSaveCrashReport);
	signal(SIGFPE,    OsSaveCrashReport);
	signal(SIGSEGV,   OsSaveCrashReport);
	signal(SIGSTKFLT, OsSaveCrashReport);
	signal(SIGPIPE,   OsSaveCrashReport);


	EMVTransInit();
	GlobalInit();

	ClssTransInit();

	PdkUI_Init();
	PdkUI_Load(0,"SYS_STATUSBAR.xml");

	OsMsrOpen();
	return ;
}

//ȷ�Ͽ���ҳ��
int ConfirmCardNoPage(char *pszDataOut)
{
		int iRet = 0;
		int iFlag = 1;
		char szDataOut[1024] = {0};

		PdkUI_Flush(0);
		PdkUI_Load(0,"disp_cardno.xml");
		while(1)
		{
			iRet = PageExecute(szDataOut,pszDataOut,NULL,0);
			if(iRet < 0)
			{
				return FAIL;
			}
			OsLog(LOG_DEBUG,"confirm card No KEy = %d\n",iRet);
			if((iRet == LOGIC_KEY_ENTER) || (iRet == LOGIC_KEY_CANCEL))
			{
				OsLog(LOG_DEBUG,"confirm card No KEy1 = %d\n",iRet);
				break;
			}
		}
		return iRet;
}

//������ҳ��
int InputAmountPage(char *pszAmount)
{
		int iRet = 0;
		char szDataOut[1024] = {0};
	    ST_TIMER stTime;
	    int iTextlen = 0;
	    char sztextBuf[10+1] = {0};
	    char szdispBuf[10+1] = "0.00";

		iRet = OsTimerSet(&stTime, 30000);

		PdkUI_Flush(0);
		PdkUI_Load(0,"amt.xml");

		//modified by richard 20150915 use textbox to replace editbox, let key works(MENU F1 ...)
		while(OsTimerCheck(&stTime) > 0)
		{

			iRet = PageExecute(szDataOut,szdispBuf,NULL,0);

			if(iRet >= 48 && iRet <= 57)
			{
				if(iTextlen < 9)
				{
					sztextBuf[iTextlen++] = iRet;
				}
			}
			else if(iRet == LOGIC_KEY_CLEAN)
			{
				if(iTextlen > 0)
				{
					sztextBuf[(iTextlen--) - 1] = 0;
				}
			}
			else if(((iRet == LOGIC_KEY_ENTER)&&(0 != strcmp(szdispBuf, "0.00"))) || (iRet == LOGIC_KEY_CANCEL))
			{
				break;
			}

			memset(szdispBuf, 0, sizeof(szdispBuf));
			if(iTextlen == 0)
			{
				sprintf(szdispBuf, "0.00");
			}
			else if(iTextlen == 1)
			{
				sprintf(szdispBuf, "0.0%c",sztextBuf[0]);
				//OsLog(LOG_DEBUG,"1.PdkUI_Process szdispBuf:%s sztextBuf:%s %d",szdispBuf, sztextBuf, iTextlen);
			}
			else if(iTextlen == 2)
			{
				snprintf(szdispBuf, 5, "0.%s",sztextBuf);
				//OsLog(LOG_DEBUG,"2.PdkUI_Process szdispBuf:%s sztextBuf:%s %d",szdispBuf,  sztextBuf, iTextlen);
			}
			else
			{

				snprintf(szdispBuf, iTextlen-1, "%s",sztextBuf);
				szdispBuf[iTextlen-2]= '.';
				//OsLog(LOG_DEBUG,"3.PdkUI_Process szdispBuf:%s sztextBuf:%s %d",szdispBuf,  sztextBuf, iTextlen);
				snprintf(szdispBuf+iTextlen-1, 3, "%s",sztextBuf+iTextlen-2);
				//OsLog(LOG_DEBUG,"4.PdkUI_Process szdispBuf:%s sztextBuf:%s %d",szdispBuf,  sztextBuf, iTextlen);
			}
		}

		Asc2Amt(sztextBuf,pszAmount);
		return iRet;
}

int InputPinPage(char *pzCardNo, char *pzAmount)
{
	//��������ҳ��
	int iRet = 0;
	unsigned char ucRet;
    int iLoop = 0;
    int iTextlen = 0;
    ST_TIMER stTime;
	char szDataOut[1024] = {0};
    char sztextBuf[10+1] = {0};
    char szPin[8] = {0};

	uchar	ucPinKeyID = 1;

	iRet = OsTimerSet(&stTime, 30000);

	PdkUI_Flush(0);

	OsLog(LOG_DEBUG,"InputPinPage  szPosType:%s\n", szPosType);
	if(0 == strcmp(szPosType, "s800"))
	{
		OsPortClose(PORT_PINPAD);
		for (iLoop=0;iLoop<1000;iLoop++)
		{
			;
		}

		iRet = OsPortOpen(PORT_PINPAD, "115200,8,n,1");

		//use ucRet to check if S200 is connected modified by richard 20151013
		ucRet = PPScrCls();
		OsLog(LOG_DEBUG,"PPScrCls  ucRet:%x\n", ucRet);

		if(0 == ucRet)
		{
			PdkUI_Load(0,"getpin2.xml");
			iRet = PageExecute(szDataOut,NULL,NULL,0);
			PPScrPrint(0, 0, pzAmount);

			ucRet = PPGetPwd_3Des(ucPinKeyID, 0x31, 4, 6, pzCardNo, szPin, 0);
			OsLog(LOG_DEBUG,"PPGetPwd_3Des  ucRet:%x\n", ucRet);

			OsPortClose(PORT_PINPAD);
			OsSleep(50);

			return LOGIC_KEY_ENTER;
		}
		OsPortClose(PORT_PINPAD);
	}

	PdkUI_Load(0,"getpin.xml");
	//modified by richard 20150915 use textbox to replace editbox, let key works(MENU F1 ...)
	while(OsTimerCheck(&stTime) > 0)
	{
		iRet = PageExecute(szDataOut,sztextBuf,NULL,0);

		if(iRet >= 48 && iRet <= 57)
		{
			if(++iTextlen > 6)
			{
				iTextlen = 6;
			}
		}
		else if(iRet == LOGIC_KEY_CLEAN)
		{
				iTextlen = 0;
		}
		else if(((iRet == LOGIC_KEY_ENTER)&&(iTextlen >= 4)) || (iRet == LOGIC_KEY_CANCEL))
		{
			break;
		}

		memset(sztextBuf, 0, sizeof(sztextBuf));
		for(iRet=0; iRet<iTextlen; iRet++)
		{
			sztextBuf[iRet] = '*';
		}
		//OsLog(LOG_DEBUG,"PdkUI_Process szDataOut:%s  iRet:%d",szDataOut, iRet);
	}

	return iRet;
}

//��ͼƬҳ��
int ProcessPage(char * pszXmlName)
{
	int iRet = 0;
	char szDataOut[1024] = {0};

	PdkUI_Flush(0);
	PdkUI_Load(0,pszXmlName);

	iRet = PageExecute(szDataOut,NULL,NULL,1);
	OsLog(LOG_DEBUG,"ProcessPage iRet:%d", iRet);
	if(iRet < 0 && iRet != TIMEOUT)
	{
		return iRet;
	}
	return TIMEOUT;
}

int CompletePage()
{
	int iRet = 0;
	char szDataOut[1024] = {0};

	PdkUI_Flush(0);
	PdkUI_Load(0,"complete.xml");

	iRet = PageExecute(szDataOut,NULL,NULL,10);
	OsLog(LOG_DEBUG,"ProcessPage iRet:%d", iRet);
	if(iRet < 0 && iRet != TIMEOUT)
	{
		return iRet;
	}
	return iRet;
}

int OutOfPaperPage()
{
	int iRet = 0;
	char szDataOut[1024] = {0};

	PdkUI_Flush(0);
	PdkUI_Load(0,"nopaper.xml");
	OsBeep(2, 200);

	while(1)
	{
		iRet = PageExecute(szDataOut,NULL,NULL,0);

		if(LOGIC_KEY_ENTER == iRet || LOGIC_KEY_CANCEL == iRet || (iRet < 0 && iRet != TIMEOUT))
		{
			return iRet;
		}

	}
}

//ǩ����ҳ��
int SignPage(char *pszCardNo,char *pszAmount,int *piSignFlag)
{
	int iRet = 0;
	unsigned char ucRet = 0;
	char szDataOut[1024] = {0};

	*piSignFlag = 0;

	PdkUI_Flush(0);
	PdkUI_Load(0,"signatureboard.xml");

	if(strcmp(szPosType, "s800") == 0)
	{
		if(PPScrCls())
			return SUCC;  //s200 not connected

		PageExecute(szDataOut,NULL,NULL,0);
		iRet = SignFlowOnPad();

		OsLog(LOG_DEBUG,"OsRegGetValue :%s %d",szPosType, iRet);
		if(0 == iRet)
		{
			*piSignFlag = 1;
		}

		return SUCC;
	}

	while(1)
	{
		iRet = PageExecute(szDataOut,pszCardNo,pszAmount,100);

		OsLog(LOG_DEBUG,"PageExecute iRet:%d", iRet);
		if(iRet == LOGIC_KEY_CLEAN)
		{
			iRet = PdkUI_RequestAction(0, "sign", "ClearSign", "true");
			OsLog(LOG_DEBUG,"PdkUI_RequestAction iRet:%d", iRet);
			continue;
		}
		if(iRet == LOGIC_KEY_CANCEL)
		{
			*piSignFlag = 0;
			break;
		}
		if(iRet == LOGIC_KEY_ENTER)
		{
			*piSignFlag = 1;
			PdkUI_SetAttribute(0, "sign", "SnapShotName", SIGN_FILE);
			PdkUI_RequestAction(0, "sign", "SnapScreen", "true");
			break;
		}
		if(iRet < 0)
		{
			return FAIL;
		}
	}
	return SUCC;
}
//����ҳ��
int ScreenSaverPage()
{
	int iRet = 0;
	char szDataOut[1024] = {0};

	while(1)
	{
		PdkUI_Flush(0);
		PdkUI_Load(0,"ScreenSaver1.xml");
		iRet = PageExecute(szDataOut,NULL,NULL,TIME_SCREENSAVER/1000);
		OsLog(LOG_DEBUG,"SCREENSAVER1 iret = %d\n",iRet);
		if(iRet > 0)
		{
			return SUCC;
		}
		if(iRet < 0 && iRet != TIMEOUT)
		{
			return FAIL;
		}

		PdkUI_Flush(0);
		PdkUI_Load(0,"ScreenSaver2.xml");
		iRet = PageExecute(szDataOut,NULL,NULL,TIME_SCREENSAVER/1000);
		OsLog(LOG_DEBUG,"SCREENSAVER2 iret = %d\n",iRet);
		if(iRet > 0)
		{
			return SUCC;
		}
		if(iRet < 0 && iRet != TIMEOUT)
		{
			return FAIL;
		}

		PdkUI_Flush(0);
		PdkUI_Load(0,"ScreenSaver3.xml");
		iRet = PageExecute(szDataOut,NULL,NULL,TIME_SCREENSAVER/1000);
		OsLog(LOG_DEBUG,"SCREENSAVER3 iret = %d\n",iRet);
		if(iRet > 0)
		{
			return SUCC;
		}
		if(iRet < 0 && iRet != TIMEOUT)
		{
			return FAIL;
		}

		PdkUI_Flush(0);
		PdkUI_Load(0,"ScreenSaver4.xml");
		iRet = PageExecute(szDataOut,NULL,NULL,TIME_SCREENSAVER/1000);
		OsLog(LOG_DEBUG,"SCREENSAVER4 iret = %d\n",iRet);
		if(iRet > 0)
		{
			return SUCC;
		}
		if(iRet < 0 && iRet != TIMEOUT)
		{
			return FAIL;
		}
	}

    return SUCC;
}

int SwipeCardProcess(char * pszCardNo,int *piReadCardType)
{
	int iRet = 0;
	char szTk1[79]  = {0};
    char szTk2[40] = {0};
    char szTk3[107] = {0};
	char szDataOut[1024] = {0};
	static ST_TIMER  stDispTimer = {0};

	iRet = PdkMag_Read(szTk1, szTk2, szTk3);
	if(iRet < 0 && iRet != CARD_ERR_NO_SWIPE_CARD )
	{
		OsLog(LOG_DEBUG,"PdkMag_Read err iret = %d\n",iRet);
		return FAIL;
	}

	if(iRet == SUCC)
	{
		iRet = Card_GetPan(szTk2,szTk3,pszCardNo);
		if(iRet != SUCC)
		{
			return iRet;
		}
		*piReadCardType = SWIPE;
		return SUCC;
	}
	else if(iRet == PLS_INSERT_CARD)
	{
		//display picture for 3s
		PdkUI_Flush(0);
		PdkUI_Load(0,"insert_card.xml");

		OsTimerSet(&stDispTimer, 3000);
		while(OsTimerCheck(&stDispTimer) > 0)
		{
			iRet = PageExecute(szDataOut,NULL,NULL,0);
			OsLog(LOG_DEBUG,"PLS_INSERT_CARD iret = %d\n",iRet);

			if(!OsIccDetect(0x00)) return FAIL;
		}

		PdkUI_Flush(0);
		if(iModule2dFlag)
		{
			PdkUI_Load(0,XML_GETCARD_2D);
		}
		else
		{
			PdkUI_Load(0,XML_GETCARD);
		}
		PageExecute(szDataOut,NULL,NULL,0);
	}

	return FAIL;
}

int InsertCardProcess(char * pszCardNo,int *piReadCardType)
{
	int iRet = 0;
	char szDataOut[1024] = {0};

	iRet = IcCommand(pszCardNo, piReadCardType);

	if(gFallbackFlag)
	{
		PdkUI_Flush(0);
		PdkUI_Load(0,"swipe_card.xml");
		while(1)
		{
			iRet = PageExecute(szDataOut,NULL,NULL,0);

			if(iRet == LOGIC_KEY_CANCEL)
			{
				gFallbackFlag = 0;
				PdkUI_Flush(0);
				if(iModule2dFlag)
				{
					PdkUI_Load(0,XML_GETCARD_2D);
				}
				else
				{
					PdkUI_Load(0,XML_GETCARD);
				}
				PageExecute(szDataOut,NULL,NULL,0);
				return FAIL;
			}

			iRet = SwipeCardProcess(pszCardNo, piReadCardType);
			if(iRet == SUCC)
			{
				gFallbackFlag = 0;
				return SUCC;
			}
		}
	}

	OsLog(LOG_DEBUG,"IcCommand iret = %d\n",iRet);
	if(iRet < 0)
	{
		return iRet;
	}
	if(iRet == SUCC)
	{
		*piReadCardType = INSERT;
		return SUCC;
	}

	return FAIL;
}


//��Ƭ����
int CardProcess(char * pszCardNo,int *piReadCardType)
{
	int iRet = 0;
	static ST_TIMER  stDetectTimer = {0};

	memset(pszCardNo, 0, sizeof(pszCardNo));
	strcpy(pszCardNo, "************1234");

	//��������
	while(1)
	{
		//ˢ��
		iRet = SwipeCardProcess(pszCardNo, piReadCardType);
		if(iRet == SUCC)
		{
			OsPiccClose();
			return SUCC;
		}

		//�忨
		iRet = InsertCardProcess(pszCardNo, piReadCardType);
		if(iRet == SUCC)
		{
			OsPiccClose();
			return SUCC;
		}

		//����Ŀ��¼�
		if((0 == strcmp(szPosType, "s920")) || (0 == strcmp(szPosType, "s300")))
		{
			if(OsTimerCheck(&stDetectTimer) <= 0)
			{
				iRet = ClssCardDetect();
				OsTimerSet(&stDetectTimer, 1000);
			}
		}
		else
		{
			iRet = ClssCardDetect();
		}

		//OsLog(LOG_DEBUG,"ClssCardDetect iret = %d\n",iRet);
		if(iRet != SUCC)
		{
			OsPiccClose();
			return iRet;
		}
		if(iRet == SUCC)
		{
			OsSleep(800);
			if(OsMsrSwiped())
			{
				continue;
			}

			//�Ŀ�����������
			iRet = ClssProcTrans(pszCardNo);
			if(iRet != SUCC)
			{
				//return iRet;
			}
			*piReadCardType = TAP;
			OsMsrReset();
			break;
		}
	}

	OsPiccClose();
	return SUCC;
}

void removeCard()
{
	if(RET_OK == OsIccDetect(0))
	{
		PdkUI_Flush(0);
		ProcessPage("remove_card.xml");
		while(1)
		{
			OsBeep(1, 100);
			OsSleep(400);
			if(RET_OK != OsIccDetect(0))
			{
				PdkUI_Flush(0);
				break;
			}
		}
	}
}

//ɨ�����
int scanProcess(char *pszAmount)
{
    int iRet = 0;
    int scanFlag = 0;
    char szReadBuf[3000] = {0};
	char szDataOut[1024] = {0};

	//OsLogSetTag("scan");

	OsPiccClose();
	iRet = OsScanRead(szReadBuf, 3000, 5000);
	if(0 < iRet)
	{
		scanFlag = 1;
		PdkUI_Flush(0);
		PdkUI_Load(0,"scan_success.xml");
	}
	else
	{
		PdkUI_Flush(0);
		PdkUI_Load(0,"scan_failed.xml");
	}

	OsScanClose();
	if(0 == scanFlag)
	{
		iRet = PageExecute(szDataOut,NULL,NULL,10);
		if((iRet == LOGIC_KEY_CANCEL) || (iRet == LOGIC_KEY_ENTER))
		{
			return FAIL;
		}
	}
	else
	{
		iRet = PageExecute(szDataOut,NULL,NULL,10);
		if(iRet == LOGIC_KEY_ENTER)
		{
			PdkUI_Flush(0);
			PdkUI_Load(0,"scan_result.xml");

			iRet = PageExecute(szDataOut,NULL,NULL,0);
			OsLog(LOG_DEBUG,"PageExecute iRet:%d",iRet);

			/*if(iRet == LOGIC_KEY_CANCEL)
			{
				return 0;
			}
			else if(iRet == LOGIC_KEY_ENTER)
			{
				strcpy(pszAmount, "15.00");
				return 1;
			}*/
			strcpy(pszAmount, "15.00");
			return SUCC;
		}
		else if(iRet == LOGIC_KEY_CANCEL)
		{
			PdkUI_Flush(0);
			return FAIL;
		}
	}

	return FAIL;
}


int main()
{
//	start_debug();
	int iRet = 0;
	char szDataOut[1024] = {0};

    char szCardNo[32] = {0};
    char szAmount[32] = {0};
    int iSignFlag = 0;
    int iLoadFlag = 0;
    int iScanFlag = 0;
    int iReadCardType = 0;
    ST_TIMER stTime;
    static pthread_t ntid;
    char szRegValue[65];


    OsRegSetValue("persist.sys.backlighttime","0");
    iRet = OsRegGetValue("ro.fac.mach", szPosType);
    if(iRet <= 0)
    	strcpy(szPosType, "s900");

    iRet = OsRegGetValue("ro.fac.barcode", szRegValue);
    if(iRet > 0)
    	iModule2dFlag = 1;

    AppInit();
    pthread_create(&ntid,NULL,thread_fn,NULL);

	//add by richard 20150902  boot animation
	PdkUI_Load(0,"boot_animation.xml");
	iRet = OsTimerSet(&stTime, 12000);
	if(iRet < 0)
	{
		return FAIL;
	}

	while(OsTimerCheck(&stTime) > 0)
	{
		PageExecute(szDataOut,NULL,NULL,0);

	}
	PdkUI_Flush(0);

    iRet = OsTimerSet(&stTime, TIME_SCREENSAVER_ENTRY);
    if(iRet < 0)
    {
    	return FAIL;
    }

    //ˢ��ҳ��
    iLoadFlag = 1;
	while(1)
	{
		//OsSleep(1000);
		if(iLoadFlag == 1)
		{
			if(iModule2dFlag)
			{
				PdkUI_Load(0,XML_GETCARD_2D);
			}
			else
			{
				PdkUI_Load(0,XML_GETCARD);
			}
		}
		OsLog(LOG_DEBUG,"OsTimerCheck iret = %d %s\n",OsTimerCheck(&stTime), szPosType);
		if(OsTimerCheck(&stTime) <= 0)
		{
			iRet = ScreenSaverPage();
			if(iRet == SUCC)
			{
				OsTimerSet(&stTime, TIME_SCREENSAVER_ENTRY);
				OsMsrReset();
				OsLog(LOG_DEBUG,"OsTimerCheck enter iret = %d\n",OsTimerCheck(&stTime));
				PdkUI_Flush(0);
				iLoadFlag = 1;
				continue;
			}
			break;
		}
		iRet = PageExecute(szDataOut,NULL, NULL,0);
		OsLog(LOG_DEBUG,"HOM PAGE iret = %d\n",iRet);
		if(iRet == LOGIC_KEY_CANCEL)
		{
			//�˳�ҳ��
			if(iScanFlag)
			{
				OsTimerSet(&stTime, TIME_SCREENSAVER_ENTRY);
				memset(szAmount, 0, sizeof(szAmount));
				PdkUI_Flush(0);
				iLoadFlag = 1;
				iScanFlag =0;
				continue;
			}

			PdkUI_Flush(0);
			PdkUI_Load(0,"exit.xml");
			iRet = PageExecute(szDataOut,NULL,NULL,100);
			if(iRet == LOGIC_KEY_ENTER)
			{
				return QUIT;
			}
			if(iRet == LOGIC_KEY_CANCEL)
			{
				OsTimerSet(&stTime, TIME_SCREENSAVER_ENTRY);
				PdkUI_Flush(0);
				iLoadFlag = 1;
				continue;
			}
			if(iRet < 0)
			{
				return FAIL;
			}
		}
		else if(iRet == LOGIC_KEY_ENTER)   //add by richard  20150811
		{
			if(iModule2dFlag)
			{
				PdkUI_Flush(0);
				ProcessPage("scanning.xml");

				iRet = OsScanOpen();
				if(RET_OK == iRet)
				{
					if(SUCC == scanProcess(szAmount))
					{
						iScanFlag = 1;
					}
					else
					{
						iLoadFlag = 1;
					}
					OsTimerSet(&stTime, TIME_SCREENSAVER_ENTRY);
				}
			}
			continue;
		}
		if(iRet < 0)
		{
			return FAIL;
		}
		//��������
		iRet =  CardProcess(szCardNo,&iReadCardType);
		//OsLog(LOG_ERROR,"CardProcess iret = %d  iReadCardType:%d\n",iRet, iReadCardType);
		if(iRet == SUCC)
		{
			szAudioFlag = 1;
			strcpy(szAudioFile,AUDIO_CONFIRM);

			memcpy(szCardNo+6,"******",6);
			iRet = ConfirmCardNoPage(szCardNo); // ȷ�Ͽ���ҳ��
			OsLog(LOG_ERROR,"ConfirmCardNoPage iret = %d\n",iRet);
			if(iRet == LOGIC_KEY_CANCEL)
			{
				iScanFlag = 0;
				OsTimerSet(&stTime, TIME_SCREENSAVER_ENTRY);
				PdkUI_Flush(0);
				iLoadFlag = 1;
				removeCard();
				continue;
			}

			if(0 == iScanFlag)
			{
				//������ҳ��
			//	pthread_create(&ntid,NULL,thread_fn,NULL);
				iRet = InputAmountPage(szAmount);
				OsLog(LOG_ERROR,"InputAmountPage iret = %d\n",iRet);
				if(iRet >= 0 && iRet != LOGIC_KEY_ENTER)
				{
					OsTimerSet(&stTime, TIME_SCREENSAVER_ENTRY);
					PdkUI_Flush(0);
					iLoadFlag = 1;
				//	pthread_cancel(ntid);
					removeCard();
					continue;
				}
				if(iRet < 0)
				{
					return FAIL;
				}
			}
			iScanFlag = 0;

			//��������ҳ��
		//	pthread_cancel(ntid);
			szAudioFlag = 1;
			strcpy(szAudioFile,AUDIO_ENTER_PIN);
		//	pthread_create(&ntid,NULL,thread_fn,NULL);
			iRet = InputPinPage(szCardNo, szAmount);
			OsLog(LOG_ERROR,"InputPinPage iret = %d\n",iRet);
			if(iRet >= 0 && iRet != LOGIC_KEY_ENTER)
			{
				OsTimerSet(&stTime, TIME_SCREENSAVER_ENTRY);
				PdkUI_Flush(0);
				iLoadFlag = 1;
			//	pthread_cancel(ntid);
				removeCard();
				continue;
			}
			if(iRet < 0)
			{
				return FAIL;
			}

			//���̴������
			iRet = ProcessPage("processing.xml");
			if(iRet < 0 && iRet != TIMEOUT)
			{
				return FAIL;
			}
			//ǩ�������
			iRet = SignPage(szCardNo,szAmount,&iSignFlag);
			OsLog(LOG_ERROR,"InputPinPage iret = %d\n",iRet);
			/*if(iRet != SUCC)
			{
				return FAIL;
			}*/
			//��ӡ����  �򿪳ɹ����ӡ������
			if(OsPrnOpen(PRN_REAL,NULL) == RET_OK)
			{
				//��ʼ��ӡ
				while(1)
				{
					iRet = ProcessPage("print.xml");
					if(iRet < 0 && iRet != TIMEOUT)
					{
						return FAIL;
					}

					iRet = Print_Receipt(szAmount,szCardNo,iSignFlag,iReadCardType);
					OsLog(LOG_DEBUG,"Print_Receipt = %d\n",iRet);
					if(iRet == ERR_PRN_PAPEROUT)
					{
						iRet = OutOfPaperPage();
						//iRet = ProcessPage("nopaper.xml");
						if(iRet == LOGIC_KEY_CANCEL)
						{
							break;
						}

						if(iRet < 0 && iRet != TIMEOUT )
						{
							return FAIL;
						}
					}
					else
					{
						break;
					}
				}
			}

			//�������ҳ��
		//	pthread_cancel(ntid);
			szAudioFlag = 1;
			strcpy(szAudioFile,AUDIO_TRANS_SUCC);
		//	pthread_create(&ntid,NULL,thread_fn,NULL);
			iRet = CompletePage();
			if(iRet > 0 || iRet == TIMEOUT)
			{
				OsTimerSet(&stTime, TIME_SCREENSAVER_ENTRY);
				PdkUI_Flush(0);
				iLoadFlag = 1;
			//	pthread_cancel(ntid);
				removeCard();
				continue;
			}
			if(iRet < 0 && iRet != TIMEOUT)
			{
				return FAIL;
			}
			break;
		}
		iLoadFlag = 0;
	}

	OsMsrClose();
	OsIccClose(-1);

	return 0;
}
