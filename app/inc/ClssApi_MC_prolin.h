/*****************************************************/
/* ClssApi_MC.h                                      */
/* Define the Application Program Interface          */
/* of MasterCard PayPass for all PAX Readers         */
/* Created by Zhou Jie at 2013.1.1                 */
/*****************************************************/

#ifndef _CLSS_LIB_MC_H
#define _CLSS_LIB_MC_H

#include "Cl_common.h"
 
#ifndef _APDU_DEFINE
#define _APDU_DEFINE
typedef struct
{
    uchar       Command[4];
    ushort      Lc;
    uchar       DataIn[512];
    ushort      Le;
}APDU_SEND;

typedef struct
{
    ushort      LenOut;
    uchar       DataOut[512];
    uchar       SWA;
    uchar       SWB;
}APDU_RESP;
#endif



// the source of data elements
#define CLSS_SRC_ICC   0  //ICC
#define CLSS_SRC_TM    1  //terminal
#define CLSS_SRC_ISS   2  //issuer

#define CLSS_DATA_OCPS 0x01//Outcome Parameter Set
#define CLSS_DATA_DISD 0x02//Discretionary Data
#define CLSS_DATA_UIRD 0x04//User Interface Request Data

typedef struct{
	uchar aucTag[4];       //three bytes are being used now, byte 4 is for future use
	uchar ucTagLen;        //the length of aucTag
	uchar ucSource;        //the source of tag, value can be SRC_TM£¬SRC_ICC£¬SRC_ISS
    uchar *pucData;	       //the value of tag.
	uint unDataLen;        //the length of data.
	uint unMaxDataLen;     //the max length of data
}TLV_ELEMENT_MC;

typedef struct{
	uchar aucPAN[10];
	uchar ucPANLen;
	uchar ucPANSeqFlg;
	uchar ucPANSeq;	
	uchar aucTornData[1024];//the Torn Record start with tag 'FF8101', length and value
	uint unTornDataLen;	//the length of aucTornRecord
}CLSS_TORN_LOG_RECORD;//the format of Torn Record saved in kernel

// Proprietary Tag of parameter defined by this kernel
#define CLSS_PARAM_TIMER    0x01

// Parameter processing
int  Clss_CoreInit_MC(uchar ucDESupportFlag);
int  Clss_ReadVerInfo_MC(char *paucVer); 
int  Clss_AddProprietaryData_MC(TLV_ELEMENT_MC *ptTLV_Element);
int  Clss_SetTLVDataList_MC(uchar *pucTLVDatas, uint unDataLen);
int  Clss_GetTLVDataList_MC(uchar *pucTagList, uchar ucTagListLen, uint unExpectDataLen, uchar *pucDataOut, uint *punActualDataOutLen);
int  Clss_SetTagPresent_MC(uchar *pucTag, uchar ucPresent);
int  Clss_SetParam_MC(uchar *paucTLVParam, uint unParamLen);
//CAPK
int  Clss_AddCAPK_MC_MChip(EMV_CAPK  *ptCAPKey);
void Clss_DelAllCAPK_MC_MChip(void);

// Revocation list
int  Clss_AddRevocList_MC_MChip(EMV_REVOCLIST *ptRevocList);
int  Clss_DelRevocList_MC_MChip(unsigned char ucIndex, unsigned char *pucRID);
void Clss_DelAllRevocList_MC_MChip(void);

 //Transaction processing
int Clss_SetFinalSelectData_MC(uchar *pucRspIn, int nRspLen);
int Clss_InitiateApp_MC(void); 
int Clss_ReadData_MC(uchar *pucPathTypeOut);
// For Mag-Stripe only
int Clss_TransProc_MC_Mag(uchar *pucACTypeOut);
//end
// For MCHIP only
int Clss_TransProc_MC_MChip(uchar *pucACTypeOut);
int Clss_CleanTornLog_MC_MChip(uchar *pucCurrentDateTime, int nDateTimeLen, uchar ucCleanAllFlg);
int Clss_SetTornLog_MC_MChip(CLSS_TORN_LOG_RECORD *ptTornLog, int nTornNum);
int Clss_GetTornLog_MC_MChip(CLSS_TORN_LOG_RECORD *ptTornLog, int *pnTornNum, int *pnUpdatedFlg);
//end


//call back function
typedef int  (*LCP_SENDDEKDATA_MC)(uchar*, uint);
int Clss_SetCBFun_SendDEKData_MC(LCP_SENDDEKDATA_MC cClssSendDEKData);

typedef int  (*LCP_RECEIVEDETDATA_MC)(uchar*, uint*, uchar*);
int Clss_SetCBFun_ReceiveDETData_MC(LCP_RECEIVEDETDATA_MC cClssReceiveDETData);

typedef int  (*LCP_ADDAPDUTOTRANSLOG_MC)(APDU_SEND*, APDU_RESP*);
int Clss_SetCBFun_AddAPDUToTransLog_MC(LCP_ADDAPDUTOTRANSLOG_MC cClssAddAPDUToTransLog);

typedef int  (*LCP_SENDTRANSDATAOUTPUT_MC)(uchar);
int Clss_SetCBFun_SendTransDataOutput_MC(LCP_SENDTRANSDATAOUTPUT_MC cClssSendTransDataOutput);

//Debug interface
int  Clss_GetDebugInfo_MC(int nExpAssistInfoLen, uchar *paucAssistInfo, int *pnErrorCode);

#endif


