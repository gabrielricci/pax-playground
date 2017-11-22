/*****************************************************/
/* ClssApi_Pboc.h                                    */
/* Define the Application Program Interface          */
/* of qPBOC for all PAX Readers                      */
/* Created by Liuxl at July 30, 2009                 */
/*****************************************************/

#ifndef _CLSS_QPBOC_PROLIN_H
#define _CLSS_QPBOC_PROLIN_H

#include "CL_common.h"

#ifndef _APDU_DEFINE
#define _APDU_DEFINE
typedef struct
{
	unsigned char       Command[4];
    ushort      Lc;
    unsigned char       DataIn[512];
    ushort      Le;
}APDU_SEND;

typedef struct
{
    ushort      LenOut;
    unsigned char       DataOut[512];
    unsigned char       SWA;
    unsigned char       SWB;
}APDU_RESP;
#endif

//��Կ��Ӧ�ù���������
int  Clss_CoreInit_Pboc(void);
int Clss_ReadVerInfo_Pboc(char *paucVer);
int  Clss_GetTLVData_Pboc(ushort Tag, unsigned char *DtOut, int *nDtLen);
int  Clss_SetTLVData_Pboc(ushort usTag, unsigned char *pucDtIn, int nDtLen);
 
//��Կ��Ӧ�ù��������� 
int  Clss_AddCAPK_Pboc(EMV_CAPK  *capk );
int  Clss_GetCAPK_Pboc(int Index, EMV_CAPK  *capk );
int  Clss_DelCAPK_Pboc(unsigned char KeyID, unsigned char *RID);
void Clss_DelAllCAPK_Pboc(void);

// added by liuxl 20070710 ���޷���Ӧ�ó����ƹ�֤���б���������ӽӿ�
int  Clss_AddRevocList_Pboc(EMV_REVOCLIST *pRevocList);
int  Clss_DelRevocList_Pboc(unsigned char ucIndex, unsigned char *pucRID);
void Clss_DelAllRevocList_Pboc(void);


// �����������
int Clss_SetReaderParam_Pboc(Clss_ReaderParam *ptParam);
int Clss_GetReaderParam_Pboc(Clss_ReaderParam *ptParam);
int Clss_SetReaderChannel_Pboc(unsigned char ucChannel);// ���÷ǽӶ�����ͨ�� [2/14/2011 zhoujie]

// �Զ�������������
int  Clss_SetEMVUnknowTLVData_Pboc(unsigned short usTag, unsigned char *pDatIn, int nLen);

//���״���������
int Clss_SetPbocAidParam_Pboc(Clss_PbocAidParam *ptParam);
int Clss_SetTransData_Pboc(Clss_TransParam *ptTransParam, Clss_PreProcInterInfo *ptInfo);
int Clss_SetFinalSelectData_Pboc(unsigned char *paucDtIn, int nDtLen);
int Clss_Proctrans_Pboc(unsigned char *pucTransPath, unsigned char *pucACType);
int Clss_GetGPOData_Pboc(unsigned char *pucGPODt, int *nGPODtLen);
int Clss_ReSendLastCmd_Pboc(unsigned char *pucTransPath, unsigned char *pucACType);//  [1/6/2011 zhoujie]

// only for qVSDC and wave2
int Clss_CardAuth_Pboc(unsigned char *pucACType, unsigned char *pucDDAFailFlg);

// all schemeid
int Clss_GetCvmType_Pboc(unsigned char *pucType);
//��GET DATA�����ȡ��Ƭ����
int Clss_GetDataCmd_Pboc(ushort usTag, unsigned char *DtOut, int *nDtLen);
// MSD track data
int Clss_nGetTrack1MapData_Pboc(unsigned char *pTrackData, int *pLen); // for MSD and wave2
int Clss_nGetTrack2MapData_Pboc(unsigned char *pTrackData, int *pLen); // only for MSD

unsigned char cPiccIsoCommand_Pboc(unsigned char cid, APDU_SEND *ApduSend, APDU_RESP *ApduRecv);

// �������Ҫ����GPO�󣬶�Ӧ������֮ǰ�����쳣�ļ���飬���������ؽӿ� [9/8/2011 zhoujie]
int cClssCheckExceptionFile_Pboc(unsigned char *pucPAN, int nPANLen, unsigned char *pucPANSeq);

// ��ͨ����Ӧ�ó�ʼ���ӿ� [2012.3.27 ����]
int Clss_Proctrans_Pboc_UnlockApp(unsigned char *pucTransPath);
//[2012.8.28 funy]����һ���ص�������Ӧ�ô����ն����кš���������Զ�������
int cClssGetUnknowTLVData_Pboc(unsigned short usTag, unsigned char *pucData, int nLen);
//SM2������֤
unsigned char cClssSM2Verify_Pboc(unsigned char *paucPubkeyIn,unsigned char *paucMsgIn,int nMsglenIn,unsigned char *paucSignIn, int nSignlenIn);

#endif
