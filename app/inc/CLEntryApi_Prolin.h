/*****************************************************/
/* CLEntryApi_prolin.h                                          */
/* Define the Application Program Interface          */
/* of Entry Point for PAX prolin POS terminals     		     */
/*****************************************************/


#ifndef _ENTRY_LIB_PROLIN_H
#define _ENTRY_LIB_PROLIN_H

#include "Cl_common.h"
#include "emvlib_Prolin.h"
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


// 20090721
int  Clss_ReadVerInfo_Entry(char *paucVer);
int Clss_CoreInit_Entry(void);

int Clss_SetPreProcInfo_Entry(Clss_PreProcInfo *ptPreProcInfoIn);
int Clss_PreTransProc_Entry(Clss_TransParam *pstTransParam);
int Clss_AppSlt_Entry(int Slot, int ReadLogFlag);
int Clss_FinalSelect_Entry(uchar *pucKernType, uchar *pucDtOut, int *pnDtLen);
int Clss_GetPreProcInterFlg_Entry(Clss_PreProcInterInfo *ptInfo);
//add for paypass 3.0.1 [1/22/2013 ZhouJie]
int Clss_GetErrorCode_Entry(int *pnErrorCode);
int Clss_SetMCVersion_Entry(uchar ucVer);

// 20090721 
void Clss_DelAllAidList_Entry(void);
int Clss_DelAidList_Entry(uchar  *pucAID, uchar  ucAidLen);
int Clss_AddAidList_Entry(uchar *pucAID, uchar ucAidLen, uchar ucSelFlg, uchar ucKernType);
int Clss_DelCurCandApp_Entry(void);

int Clss_GetFinalSelectData_Entry(uchar *paucDtOut, int *pnDtLen);

// 20090902
void Clss_DelAllPreProcInfo(void);
int Clss_DelPreProcInfo_Entry(uchar  *pucAID, uchar  ucAidLen);

unsigned char cPiccIsoCommand_Entry(uchar cid, APDU_SEND *ApduSend, APDU_RESP *ApduRecv);

//wanmin add on 2012.3.27
int clss_AppSelect_Entry_UnlockApp(Clss_TransParam *ptTransParam, ClssTmAidList *ptTermAid);
#endif
