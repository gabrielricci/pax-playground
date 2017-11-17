
/*****************************************************/
/* ClssApi_Wave_prolin.h                               */
/* Define the Application Program Interface          */
/* of VISA payWave for PAX prolin POS terminals     	 */
/*****************************************************/
#ifndef _CL_WAVE_PROLIN_H
#define _CL_WAVE_PROLIN_H

#include "CL_common.h"

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

//密钥和应用管理函数定义
int  Clss_CoreInit_Wave(void);
int Clss_ReadVerInfo_Wave(char *paucVer);
int  Clss_GetTLVData_Wave(ushort Tag, uchar *DtOut, int *nDtLen);
int  Clss_SetTLVData_Wave(ushort usTag, uchar *pucDtIn, int nDtLen);

//密钥和应用管理函数定义
int  Clss_AddCAPK_Wave(EMV_CAPK  *capk );
int  Clss_GetCAPK_Wave(int Index, EMV_CAPK  *capk );
int  Clss_DelCAPK_Wave(unsigned char KeyID, unsigned char *RID);
void Clss_DelAllCAPK_Wave(void);

// added by liuxl 20070710 因无法由应用程序绕过证书列表问题而增加接口
int  Clss_AddRevocList_Wave(EMV_REVOCLIST *pRevocList);
int  Clss_DelRevocList_Wave(unsigned char ucIndex, unsigned char *pucRID);
void Clss_DelAllRevocList_Wave(void);


// 参数操作相关
int Clss_SetReaderParam_Wave(Clss_ReaderParam *ptParam);
int Clss_GetReaderParam_Wave(Clss_ReaderParam *ptParam);

void Clss_SetRdSchemeInfo_Wave(uchar ucNum, Clss_SchemeID_Info *pInfo);
void Clss_GetSchemeInfo_Wave(uchar *pNum, Clss_SchemeID_Info *pInfo);

// 自定义参数操作相关
int  Clss_SetEMVUnknowTLVData_Wave(unsigned short usTag, unsigned char *pDatIn, int nLen);

//交易处理函数定义
int Clss_SetVisaAidParam_Wave(Clss_VisaAidParam *ptParam);
int Clss_SetTransData_Wave(Clss_TransParam *ptTransParam, Clss_PreProcInterInfo *ptInfo);
int Clss_SetFinalSelectData_Wave(uchar *paucDtIn, int nDtLen);
int Clss_Proctrans_Wave(uchar *pucTransPath, uchar *pucACType);


// only for qVSDC and wave2
int Clss_CardAuth_Wave(uchar *pucACType, uchar *pucDDAFailFlg);
void Clss_SetDDAOmitFlag_Wave(unsigned char ucOmitFlag);
// all schemeid
int Clss_GetCvmType_Wave(uchar *pucType);

//Issuer Update Processing
int Clss_IssuerAuth_Wave (uchar *pIAuthData, int nIAuthDataLen);
int Clss_IssScriptProc_Wave(unsigned char *pucScript, int nScriptLen);
// MSD track data
int Clss_nGetTrack1MapData_Wave(uchar *pTrackData, int *pLen); // for MSD and wave2
int Clss_nGetTrack2MapData_Wave(uchar *pTrackData, int *pLen); // only for MSD

int Clss_ProcRestric_Wave(void);
unsigned char Clss_GetMSDType_Wave(unsigned char *ucMSDType);
uchar cPiccIsoCommand_Wave(uchar cid,APDU_SEND *ApduSend,APDU_RESP *ApduRecv);
int cClssGetUnknowTLVData_Wave(unsigned short usTag, unsigned char *pucData, int nLen);

#endif
