/*****************************************************/
/* Cl_Common.h                                       */
/* Define the common macros and structurese          */
/* of Contactless application for all PAX Readers    */
/* Created by Liuxl at July 30, 2009                 */
/* Release Date : Nov 11, 2014                       */
/*****************************************************/

#ifndef _CLSS_COMMON_H
#define _CLSS_COMMON_H
  

#ifndef ushort 
#define ushort unsigned short
#endif
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef ulong
#define ulong unsigned long
#endif
#ifndef uint
#define uint unsigned int
#endif


#define CLSS_TXNTYPE_GOODS_SERVICE 0x00    
#define CLSS_TXNTYPE_CASH    0x01
#define CLSS_TXNTYPE_CASHBACK 0x09
#define CLSS_TXNTYPE_REFUND   0x20

// EMV_LIB δ����
#ifndef _EMV_LIB_H 

#define  MAX_REVOCLIST_NUM    30      // EMVCOҪ��ÿ��RID����֧��30�������б�  
#define MAX_APP_NUM       32         //EMV��Ӧ���б����ɴ洢��Ӧ����  

#define PART_MATCH        0x00       //Ӧ��ѡ��ƥ���־(����ƥ��)  
#define FULL_MATCH        0x01       //Ӧ��ѡ��ƥ���־(��ȫƥ��) 

#define EMV_CASH          0x01       //��������(�ֽ�) 
#define EMV_GOODS	      0x02       //��������(����) 
#define EMV_SERVICE       0x04       //��������(����) 
#define EMV_CASHBACK      0x08       //��������(����) 
#define EMV_INQUIRY		  0x10		 //��������(��ѯ) 
#define EMV_TRANSFER	  0x20		 //��������(ת��) 
#define EMV_PAYMENT		  0x40		 //��������(֧��) 
#define EMV_ADMIN		  0x80		 //��������(����) 
#define EMV_CASHDEPOSIT   0x90       //��������(���)


////���׿ⷵ��ֵ
//#define EMV_OK             0         //�ɹ�
//#define ICC_RESET_ERR     -1         //IC����λʧ��
//#define ICC_CMD_ERR       -2         //IC����ʧ��
//#define ICC_BLOCK         -3         //IC������
//
//#define EMV_RSP_ERR       -4         //IC���������
//#define EMV_APP_BLOCK     -5         //Ӧ������
//#define EMV_NO_APP        -6         //��Ƭ��û��EMVӦ��
//#define EMV_USER_CANCEL   -7         //�û�ȡ����ǰ��������
//#define EMV_TIME_OUT      -8         //�û�������ʱ
//#define EMV_DATA_ERR      -9         //��Ƭ���ݴ���
//#define EMV_NOT_ACCEPT    -10        //���ײ�����
//#define EMV_DENIAL        -11        //���ױ��ܾ�
//#define EMV_KEY_EXP       -12        //��Կ����
//
////�ص��������������������붨��
//#define EMV_NO_PINPAD     -13        //û��������̻���̲�����
//#define EMV_NO_PASSWORD   -14        //û��������û���������������
//#define EMV_SUM_ERR       -15        //��֤������ԿУ��ʹ���
//#define EMV_NOT_FOUND     -16        //û���ҵ�ָ�������ݻ�Ԫ��
//#define EMV_NO_DATA       -17        //ָ��������Ԫ��û������
//#define EMV_OVERFLOW      -18        //�ڴ����
//
////��������־
//#define NO_TRANS_LOG      -19
//#define RECORD_NOTEXIST   -20
//#define LOGITEM_NOTEXIST  -21
//#define ICC_RSP_6985      -22        // GPO/GAC�п�Ƭ����6985, ��Ӧ�þ����Ƿ�fallback
//
//#define EMV_PARAM_ERR     -30        // 20081008 liuxl

//typedef struct {
//	unsigned char RID[5];            //Ӧ��ע�������ID
//	unsigned char KeyID;             //��Կ����
//	unsigned char HashInd;           //HASH�㷨��־,SM�㷨ʱ��ֵΪ07
//	unsigned char ArithInd;          //RSA�㷨��־��SM�㷨ʱ��ֵΪ04
//	unsigned char ModulLen;          //ģ���ȣ�SM�㷨ʱ��Ϊ��Կ����
//	unsigned char Modul[248];        //ģ��SM�㷨ʱ��Ϊ��Կֵ
//	unsigned char ExponentLen;       //ָ�����ȣ�SM�㷨ʱ��Ϊ0�������������
//	unsigned char Exponent[3];       //ָ����SM�㷨ʱ��Ϊ0�������������
//	unsigned char ExpDate[3];        //��Ч��(YYMMDD)
//	unsigned char CheckSum[20];      //��ԿУ��ͣ�SM�㷨ʱ��Ϊ0�������������
//}EMV_CAPK;
// added by liuxl 20070710 ����ͨ���򵥵�ɾ����Ӧ�Ĺ�Կ��������������֤������
// �������ʹ�øù�Կ�����������й�Կ֤���޷�ʹ�� 
//typedef  struct
//{
//	unsigned char   ucRid[5];  		        // RID
//	unsigned char   ucIndex;   		        // CA��Կ����
//	unsigned char   ucCertSn[3];            // ������֤�����к�
//}EMV_REVOCLIST;

//	CLSS_TRANSPARAM ������ز����ṹ
//typedef struct CLSS_TRANSPARAM
//{
//    ulong  ulAmntAuth;     // ��Ȩ���(ulong)
//    ulong  ulAmntOther;    // �������(ulong)
//    ulong  ulTransNo;      // �������м�����(4 BYTE)
//	uchar  ucTransType;    // ��������'9C'
//	uchar  aucTransDate[4]; // �������� YYMMDD
//	uchar  aucTransTime[4]; // ����ʱ�� HHMMSS
//}Clss_TransParam;

//typedef struct{
//	unsigned char AppName[33];       //Not used at present ����Ӧ��������'\x00'��β���ַ���
//	unsigned char AID[17];           //Not used at present Ӧ�ñ�־
//	unsigned char AidLen;            //Not used at present AID�ĳ���
//    unsigned char SelFlag;           //Not used at present ѡ���־(PART_MATCH ����ƥ��  FULL_MATCH ȫƥ��)
//	unsigned char Priority;          //Not used at present ���ȼ���־
//	unsigned char TargetPer;         //Ŀ��ٷֱ���
//	unsigned char MaxTargetPer;      //���Ŀ��ٷֱ���
//	unsigned char FloorLimitCheck;   //Not used at present �Ƿ�������޶�
//	unsigned char RandTransSel;      //�Ƿ�����������ѡ��
//	unsigned char VelocityCheck;     //Not used at present �Ƿ����Ƶ�ȼ��
//    unsigned long FloorLimit;        //����޶�
//	unsigned long Threshold;         //��ֵ
//	unsigned char TACDenial[6];      //�ն���Ϊ����(�ܾ�)
//	unsigned char TACOnline[6];      //�ն���Ϊ����(����)
//	unsigned char TACDefault[6];     //�ն���Ϊ����(ȱʡ)
//    unsigned char AcquierId[6];      //Not used at present �յ��б�־
//	unsigned char dDOL[256];         //Not used at present �ն�ȱʡDDOL
//	unsigned char tDOL[256];         //�ն�ȱʡTDOL
//	unsigned char Version[3];        //Ӧ�ð汾
//	unsigned char RiskManData[10];   //Not used at present ���չ�������
//}EMV_APPLIST;

#endif

#define RD_CVM_NO            0x00//no CVM
#define RD_CVM_SIG           0x10//signature
#define RD_CVM_ONLINE_PIN    0x11//online PIN
#define RD_CVM_OFFLINE_PIN   0x12//offline PIN
#define RD_CVM_CONSUMER_DEVICE   0x1F//Refer to consumer device
// �� amount > contactless cvm limit ʱ����Ҫִ�к���CVM��ʽ��
#define RD_CVM_REQ_SIG 0x01
#define RD_CVM_REQ_ONLINE_PIN 0x02
//return code for CLSS kernel
#define CLSS_USE_CONTACT  -23    // ����ʹ������������н���
#define EMV_FILE_ERR      -24
#define CLSS_TERMINATE    -25    // Ӧ��ֹ����       -25 
#define CLSS_FAILED       -26    // ����ʧ�� 20081217 
#define CLSS_DECLINE      -27   
#define CLSS_PARAM_ERR            -30 // -26 // ��EMV �ں��еĲ���������Ϊ-30
#define CLSS_WAVE2_OVERSEA        -31  // 20090418 for visa wave2 trans
#define CLSS_WAVE2_TERMINATED     -32 // 20090421 for wave2 DDA response TLV format error
#define CLSS_WAVE2_US_CARD        -33  // 20090418 for visa wave2 trans
#define CLSS_WAVE3_INS_CARD       -34 // 20090427 FOR VISA L3
#define CLSS_RESELECT_APP         -35
#define CLSS_CARD_EXPIRED         -36 // liuxl 20091104 for qPBOC spec updated

#define EMV_NO_APP_PPSE_ERR       -37

#define CLSS_USE_VSDC             -38 // FOR CLSS PBOC [1/12/2010 yingl]
#define CLSS_CVMDECLINE           -39 // CVM result in decline for AE [1/11/2012 zhoujie]
#define CLSS_REFER_CONSUMER_DEVICE  -40 //CMD response 6986

#define REFER_APPROVE     0x01       //�ο�������(ѡ����׼) 
#define REFER_DENIAL      0x02       //�ο�������(ѡ��ܾ�) 
#define ONLINE_APPROVE    0x00       //����������(������׼) 
#define ONLINE_FAILED     0x01       //����������(����ʧ��)	 
#define ONLINE_REFER      0x02       //����������(�����ο�) 
#define ONLINE_DENIAL     0x03       //����������(�����ܾ�) 
#define ONLINE_ABORT      0x04       //����PBOC(������ֹ) 

#define CLSS_DECLINED               0x00
#define CLSS_APPROVE                0x01
#define CLSS_ONLINE_REQUEST         0x02
#define CLSS_TYR_ANOTHER_INTERFACE  0x03
#define CLSS_END_APPLICATIION       0x04

 
#define KERNTYPE_DEF 0
#define KERNTYPE_JCB 1	
#define KERNTYPE_MC  2
#define KERNTYPE_VIS 3
#define KERNTYPE_PBOC  4	
#define KERNTYPE_AE 5
#define KERNTYPE_ZIP 6
#define KERNTYPE_FLASH 7
#define KERNTYPE_EFT   8		
#define KERNTYPE_RFU 0xFF

#define AC_AAC      0x00
#define AC_TC       0x01
#define AC_ARQC     0x02

//Outcome Parameter Set
#define CLSS_OC_APPROVED                  0x10
#define CLSS_OC_DECLINED                  0x20
#define CLSS_OC_ONLINE_REQUEST            0x30
#define CLSS_OC_END_APPLICATION           0x40
#define CLSS_OC_SELECT_NEXT               0x50
#define CLSS_OC_TRY_ANOTHER_INTERFACE     0x60
#define CLSS_OC_TRY_AGAIN                 0x70
#define CLSS_OC_NA                        0xF0
#define CLSS_OC_A                         0x00
#define CLSS_OC_B                         0x10
#define CLSS_OC_C                         0x20
#define CLSS_OC_D                         0x30
#define CLSS_OC_NO_CVM                    0x00
#define CLSS_OC_OBTAIN_SIGNATURE          0x10
#define CLSS_OC_ONLINE_PIN                0x20
#define CLSS_OC_CONFIRM_CODE_VER          0x30


// ÿ��AID��Ӧ�Ĳ���,���ڽ���Ԥ����
typedef struct CLSS_PREPROCINFO
{
	ulong ulTermFLmt;
	ulong ulRdClssTxnLmt;
	ulong ulRdCVMLmt;
	ulong ulRdClssFLmt;

	uchar aucAID[17];       
	uchar ucAidLen; 
	
	uchar ucKernType; 

	// payWave
	uchar  ucCrypto17Flg;
	uchar   ucZeroAmtNoAllowed;      
	uchar   ucStatusCheckFlg;    
    uchar   aucReaderTTQ[5];     
	
	// common
	uchar ucTermFLmtFlg; 
	uchar ucRdClssTxnLmtFlg; 
	uchar ucRdCVMLmtFlg;   
	uchar ucRdClssFLmtFlg; 	 
	
	uchar aucRFU[2];
}Clss_PreProcInfo;

//pre-processing result flag in kernel
typedef struct CLSS_PREPROC_INTER_FLAG_INFO
{
	uchar aucAID[17];       
	uchar ucAidLen; 
	
	//	uchar ucKernType; // ���������AID ����ѭ���ں�����
	
	// payWave
	uchar   ucZeroAmtFlg;       // 0-���׽��!=0; 1-���׽��=0
	uchar   ucStatusCheckFlg;    // �������Ƿ�֧��״̬���
    uchar   aucReaderTTQ[5];      // �ն˽������ܣ�����VISA/PBOC�У�tag =9F66
	uchar   ucCLAppNotAllowed; // 1-��AID���ܽ��зǽӴ�����
	
	// common
	uchar ucTermFLmtExceed; 
	uchar ucRdCLTxnLmtExceed; 
	uchar ucRdCVMLmtExceed;  
	uchar ucRdCLFLmtExceed;  
	
	uchar ucTermFLmtFlg;
	uchar aucTermFLmt[5];
	
	uchar aucRFU[2];
}Clss_PreProcInterInfo;


#define CLSS_MAX_AIDLIST_NUM  32
#define CLSS_MAX_KEY_NUM       7     //EMV����֤���Ĺ�Կ�����ɴ洢�Ĺ�Կ��

#define CLSS_PATH_NORMAL  0
#define CLSS_VISA_MSD     1   // scheme_visa_msd_20
#define CLSS_VISA_QVSDC   2   // scheme_visa_wave3
#define CLSS_VISA_VSDC    3   // scheme_visa_full_vsdc
#define CLSS_VISA_CONTACT 4   

#define CLSS_MC_MAG       5
#define CLSS_MC_MCHIP     6

#define CLSS_VISA_WAVE2   7   // SCHEME_VISA_WAVE_2
#define CLSS_JCB_WAVE2    8   // SCHEME_JCB_WAVE_2
#define CLSS_VISA_MSD_CVN17    9
#define CLSS_VISA_MSD_LEGACY   10
//JCB: MCHIP[EMV];MAG;LEGACY Mode
#define CLSS_JCB_MAG            11
#define CLSS_JCB_EMV            12
#define CLSS_JCB_LEGACY         13
//DPAS: EMV;MAG;ZIP Mode
#define CLSS_DPAS_MAG           14
#define CLSS_DPAS_EMV           15
#define CLSS_DPAS_ZIP           16

// visa
#define SCHEME_VISA_WAVE_2     0x16  // SUPPORTED
#define SCHEME_VISA_WAVE_3     0x17  // SUPPORTED
#define SCHEME_VISA_MSD_20     0x18  // SUPPORTED
#define SCHEME_JCB_WAVE_1      0x60
#define SCHEME_JCB_WAVE_2      0x61
#define SCHEME_JCB_WAVE_3      0x62 

typedef struct  
{
	unsigned long   ulReferCurrCon;       // �ο����Ҵ���ͽ��״����ת��ϵ��(���׻��ҶԲο����ҵĻ���*1000)
	
	unsigned short  usMchLocLen;          // �̻����Ƽ�λ��������ĳ���
	unsigned char   aucMchNameLoc[257];   // �̻����Ƽ�λ��(1-256 �ֽ�)
	unsigned char   aucMerchCatCode[2];   // �̻�������'9F15'(2�ֽ�) 
	unsigned char   aucMerchantID[15];    // �̻���ʶ(15�ֽ�) 
	
	unsigned char AcquierId[6];       //�յ��б�־
	
    unsigned char   aucTmID[8];           // �ն˱�ʶ(�ն˺�) 
	unsigned char   ucTmType;             // �ն�����
	unsigned char   aucTmCap[3];          // �ն�����
    unsigned char   aucTmCapAd[5];        // �ն˸�������
	
    unsigned char   aucTmCntrCode [2];     // �ն˹��Ҵ���
	unsigned char   aucTmTransCur[2];      // �ն˽��׻��Ҵ���'5F2A'(2�ֽ�) 
	unsigned char   ucTmTransCurExp;       // �ն˽��׻���ָ��'5F36'(1�ֽ�)
	
	unsigned char   aucTmRefCurCode[2];    // �ն˽��ײο����Ҵ���'9F3C'(2�ֽ�)
	unsigned char   ucTmRefCurExp;	       // �ն˽��ײο�����ָ��'9F3D'(1�ֽ�)
	
	unsigned char   aucRFU[3];
}Clss_ReaderParam;



//	ClssTmAidList AID�б�ṹ
typedef struct
{
	uchar  ucAidLen;     
	uchar  aucAID[17];   // 20090609 liuxl ����һ�ֽ� 
	uchar  ucSelFlg;	 // ����ѡ���ʶ(1-����ƥ�䣬0-��ȫƥ��)
	uchar  ucKernType; // 20090609 liuxl ����
}ClssTmAidList;
 

typedef struct VISA_AID_PARAM
{
    ulong ulTermFLmt;        //����޶� - ͬEMV����޶�

	uchar ucDomesticOnly; // 01(default):only supports domestic cl transaction
	// 00 or not present: supports international cl transaction
	
	uchar ucCvmReqNum;
	uchar aucCvmReq[5];// whether a CVM is required when the amount is higher than the Contactless CVM Required Limit.
	// 01-Signature 02-Online PIN	
	
	uchar ucEnDDAVerNo; // Ĭ��=0, ������֧�����а汾��DDA��Ƭ���ѻ�����,
	// 01-��֧��dda�汾Ϊ'01'�Ŀ�Ƭ�����ѻ�����	 
	 
}Clss_VisaAidParam;

typedef struct  
{
	uchar ucSchemeID;
	uchar ucSupportFlg; // supported; 0-not support
	// 20090511
	uchar ucRFU[2];
}Clss_SchemeID_Info;


// PBOC
typedef struct PBOC_AID_PARAM
{
    ulong ulTermFLmt;        //����޶� - ͬEMV����޶�
    uchar aucRFU[4];	  
	
}Clss_PbocAidParam;

// mastercard
typedef struct  MC_AID_PARAM
{
    unsigned long FloorLimit;        //����޶� - ͬEMV����޶�
	unsigned long Threshold;         //��ֵ 

	unsigned short usUDOLLen;       
	unsigned char uDOL[256];        //�ն�ȱʡUDOL

	unsigned char TargetPer;         //Ŀ��ٷֱ��� 
	unsigned char MaxTargetPer;      //���Ŀ��ٷֱ��� 
 	unsigned char FloorLimitCheck;   //�Ƿ�������޶�
	unsigned char RandTransSel;      //�Ƿ�����������ѡ��
	unsigned char VelocityCheck;     //�Ƿ����Ƶ�ȼ��
 	unsigned char TACDenial[6];      //�ն���Ϊ����(�ܾ�)
	unsigned char TACOnline[6];      //�ն���Ϊ����(����)
	unsigned char TACDefault[6];     //�ն���Ϊ����(ȱʡ) 
    unsigned char AcquierId[6];      //�յ��б�־
	unsigned char dDOL[256];         //�ն�ȱʡDDOL  
	unsigned char tDOL[256];         //�ն�ȱʡTDOL 
	unsigned char Version[3];        //Ӧ�ð汾��
	unsigned char ForceOnline;       //�̻�ǿ������(1 ��ʾ������������) 
	unsigned char MagAvn[3];         //����Ӧ�ð汾��

	uchar ucMagSupportFlg; // 1-������֧��MagStripe
	unsigned char ucRFU;
}Clss_MCAidParam; 


#endif
