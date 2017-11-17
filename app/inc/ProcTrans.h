/*------------------------------------------------------------
* FileName: ProcTrans.h
* Author: jiangjy
* Date: 2015-03-20
------------------------------------------------------------*/
#ifndef PROCTRANS_H
#define PROCTRANS_H



#include "Global.h"

//#include "emvlib_Prolin.h"

#define App_Try_Again -2000

#define APP_TAG_NOT_EXIST           0 //Tag is present
#define APP_TAG_EXIST_WITHVAL       1 //Tag is present and not empty
#define APP_TAG_EXIST_NOVAL         2 //Tag is present but empty

//CLSS_OUTCOME_DATA   iRet
#define CLSS_DECLINED               0x00
#define CLSS_APPROVE                0x01
#define CLSS_ONLINE_REQUEST         0x02
#define CLSS_TRY_ANOTHER_INTERFACE  0x03
#define CLSS_END_APPLICATION       	0x04

//CLSS_OUTCOME_DATA   cvm
#define CLSS_CVM_NO            		0x00//no CVM
#define CLSS_CVM_SIG           		0x10//signature
#define CLSS_CVM_ONLINE_PIN    		0x11//online PIN
#define CLSS_CVM_OFFLINE_PIN   		0x12//offline PIN
#define CLSS_CVM_CONSUMER_DEVICE   	0x1F//Refer to consumer device

//new define for paypass 3.0.1
//Error Indication
#define OK                           0x00
#define L1_TIME_OUT_ERROR            0x01
#define L1_TRANSMISSION_ERROR        0x02
#define L1_PROTOCOL_ERROR            0x03
#define L2_CARD_DATA_MISSING         0x01
#define L2_CAM_FAILED                0x02
#define L2_STATUS_BYTES              0x03
#define L2_PARSING_ERROR             0x04
#define L2_MAX_LIMIT_EXCEEDED        0x05
#define L2_CARD_DATA_ERROR           0x06
#define L2_MAGSTRIPE_NOT_SUPPORTED   0x07
#define L2_NO_PPSE                   0x08
#define L2_PPSE_FAULT                0x09
#define L2_EMPTY_CANDIDATE_LIST      0x0A
#define L2_IDS_READ_ERROR            0x0B
#define L2_IDS_WRITE_ERROR           0x0C
#define L2_IDS_DATA_ERROR            0x0D
#define L2_IDS_NO_MATCHING_AC        0x0E
#define L2_TERMINAL_DATA_ERROR       0x0F
#define L3_TIME_OUT                  0x01
#define L3_STOP                      0x02
#define L3_AMOUNT_NOT_PRESENT        0x03

//Outcome Parameter Set
#define OC_APPROVED                  0x10
#define OC_DECLINED                  0x20
#define OC_ONLINE_REQUEST            0x30
#define OC_END_APPLICATION           0x40
#define OC_SELECT_NEXT               0x50
#define OC_TRY_ANOTHER_INTERFACE     0x60
#define OC_TRY_AGAIN                 0x70
#define OC_NA                        0xF0
#define OC_A                         0x00
#define OC_B                         0x10
#define OC_C                         0x20
#define OC_D                         0x30
#define OC_NO_CVM                    0x00
#define OC_OBTAIN_SIGNATURE          0x10
#define OC_ONLINE_PIN                0x20
#define OC_CONFIRM_CODE_VER          0x30

//User Interface Request Data
#define MI_CARD_READ_OK              0x17
#define MI_TRY_AGAIN                 0x21
#define MI_APPROVED                  0x03
#define MI_APPROVED_SIGN             0x1A
#define MI_DECLINED                  0x07
#define MI_ERROR_OTHER_CARD          0x1C
#define MI_INSERT_CARD               0x1D
#define MI_SEE_PHONE                 0x20
#define MI_AUTHORISING_PLS_WAIT      0x1B
#define MI_CLEAR_DISPLAY             0x1E
#define MI_NA                        0xFF
#define MI_NOT_READY                 0x00
#define MI_IDLE                      0x01
#define MI_READY_TO_READ             0x02
#define MI_PROCESSING                0x03
#define MI_CARD_READ_SUCC            0x04
#define MI_PROC_ERROR                0x05

#define MSG_HOLD_TIME_VALUE "\x00\x00\x13"


// Message [1/5/2015 jiangjy]
#define Msg_CARD_READ_OK          	0
#define Msg_TRY_AGAIN              	1
#define Msg_APPROVED				2
#define Msg_APPROVED_SIGN			3//?
#define Msg_DECLINED				4
#define Msg_OTHER_CARD				5//?
#define Msg_INSERT_CARD 			6//?
#define Msg_SEE_PHONE				7
#define Msg_AUTHORISING 			8//?
#define Msg_CLEAR_DISPLAY 			9
#define Msg_REMOVE_CARD				10
#define Msg_TAP_CARD_AGAIN 			11
#define Msg_TAP_CARD_WITH_AMOUNT	12
#define Msg_TAP_CARD_WITH_BALANCE	13//?
#define Msg_TRY_ANOTHER_INTERFACE	14
#define Msg_END_APPLICATION			15

#define NO_PICCARD              0x03
#define MORE_PICCARD            0x04


//outcome
typedef struct{
	unsigned char ucTransRet;//iProcRet;
	unsigned char ucCVMType;
	unsigned int  unPathType;
}CLSS_OUTCOME_DATA;

//Table 5.9 Discretionary Data for an EMV Mode Transaction
typedef struct{
	unsigned char ucAppCurrCodeFlg;		//Application Currency Code
	unsigned char ucBalAfterGACFlg;		//Balance Read After Gen AC
	unsigned char ucBalBeforeGACFlg;	//Balance Read Before Gen AC
	unsigned char ucDSSum3Flg;			//DS Summary 3
	unsigned char ucDSSumStFlg;			//DS Summary Status
	unsigned char ucErrIndicFlg;		//Error Indication
	unsigned char ucPostGACPutDataStFlg;//Post-Gen AC Put Data Status
	unsigned char ucPreGACPutDataStFlg;	//Pre-Gen AC Put Data Status
	unsigned char ucThirdPartyDataFlg;	//Third Party Data
	unsigned char ucTornRecordFlg;		//Torn Record
	unsigned char ucDDCardTrack1Flg;	//DD Card (Track1)
	unsigned char ucDDCardTrack2Flg;	//DD Card (Track2)
}CLSS_DISC_DATA_MC;////the format of Discretionary Data saved in kernel

typedef struct{
	unsigned char ucSupportVISA;
	unsigned char ucSupportMC;
	unsigned char ucSchemeIdAE;
	unsigned char ucSupportPBOC;// add for pboc [2/9/2015 jiangjy]
	//...
}App_SchemeId;



//*********************************declaration//////////////////


//outcome
void  vAppCreateOutcomeData_MC(int nRet);
int   AppConv_CreateOutCome(int nRet, unsigned char ucACType, CLSS_OUTCOME_DATA *pstOutComeData);
void  vAppInitPaymentData_MC(void);//KS.2 paypass 3.0.1 by zhoujie

unsigned char *App_GetFirstTLV(unsigned char *dat, unsigned char *datend, unsigned char *pucTag, unsigned int *punLen);//  [1/9/2013 ZhouJie]
unsigned char *App_SearchTLV(int DolList, unsigned char *pucTag, unsigned char *dat, unsigned char *datend, int *nLen);
int nAppSaveTransDataOutput_MC(unsigned char ucMsgFlag);
unsigned char ucAppGetTagLen(unsigned char *pucTag);
int cClssSendTransDataOutput_MC(unsigned char ucMsgFlag);


void  vAppInitDiscData_MC(void);
//kernel type
void  vAppSetAppType(unsigned char ucType);
unsigned char ucAppGetAppType(void);
int   nAppSetCurAppType(unsigned char ucKernType);
//path type
void  vAppSetTransPath(unsigned char ucTransPath);
//actype
unsigned char ucAppGetTransACType(void);

unsigned char ucAppGetTransPath(void);
//Final selected AID
void  vSetFinalSelectAID(unsigned char *paucAID, unsigned char ucAIDLen);
//atype
static void vAppSetTransACType(unsigned char ucType);

//Trans param
void SetClssTxnParam(Clss_TransParam *pParam);
void GetClssTxnParam(Clss_TransParam *pParam);
int  nSetDETData(unsigned char *pucTag, unsigned char ucTagLen, unsigned char *pucData, unsigned char ucDataLen);
void PPS_MChip1_ConfigID();
void SetTermParam_MC();
//pboc aid parameter
void InitPbocAid_clss(void);


void vAppInitSchemeId(void);
//data process
void vTwoOne_app(unsigned char *in, unsigned short in_len, unsigned char *out);
void vOneTwo_app(unsigned char *One,unsigned short len,unsigned char *Two);
void vInitPaymentData(void);//KS.2 paypass 3.0.1
void vInitDiscData(void);//Initialize Discretionary Data

//torn log
/*
int nAppCleanTornLog_MC(void);
int nAppGetTornLog_MC(CLSS_TORN_LOG_RECORD *ptTornLog, int *pnTornNum);
int nAppLoadTornLog_MC(void);//read Torn Log from file
int nAppSaveTornLog_MC(CLSS_TORN_LOG_RECORD *ptTornLog, int nTornNum);//save Torn Log in file
*/


int  AppConv_GetTLVData(unsigned short Tag, unsigned char *DataOut, int *OutLen);
//transproc
int nAppTransProc_VISA(unsigned char ucTransPath, unsigned char ucACType);
int nAppTransProc_MC(unsigned char ucTransPath, unsigned char *pucACType);
int nAppTransProc_PBOC(unsigned char ucTransPath, unsigned char ucACType);

int AppSetClss_capk(unsigned char index, unsigned char *rid, unsigned char kerId);
int AppSetClss_RevLst(unsigned char index, unsigned char *rid, unsigned char kerId);
//display
//int  disp_clss_err(int err);
//void Inter_DisplayMsg(unsigned int iMsg);
//void appRemovePicc();
static void appRemovePicc(unsigned char ucLight);
//void vAppDisplayMsgByRet(int nRet);
//add for check whether this should be a MSR trans
//unsigned char ucDetOtherCancelCmd(void);


#endif
