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

// EMV_LIB 未包含
#ifndef _EMV_LIB_H 

#define  MAX_REVOCLIST_NUM    30      // EMVCO要求每个RID必须支持30个回收列表  
#define MAX_APP_NUM       32         //EMV库应用列表最多可存储的应用数  

#define PART_MATCH        0x00       //应用选择匹配标志(部分匹配)  
#define FULL_MATCH        0x01       //应用选择匹配标志(完全匹配) 

#define EMV_CASH          0x01       //交易类型(现金) 
#define EMV_GOODS	      0x02       //交易类型(购物) 
#define EMV_SERVICE       0x04       //交易类型(服务) 
#define EMV_CASHBACK      0x08       //交易类型(反现) 
#define EMV_INQUIRY		  0x10		 //交易类型(查询) 
#define EMV_TRANSFER	  0x20		 //交易类型(转账) 
#define EMV_PAYMENT		  0x40		 //交易类型(支付) 
#define EMV_ADMIN		  0x80		 //交易类型(管理) 
#define EMV_CASHDEPOSIT   0x90       //交易类型(存款)


////交易库返回值
//#define EMV_OK             0         //成功
//#define ICC_RESET_ERR     -1         //IC卡复位失败
//#define ICC_CMD_ERR       -2         //IC命令失败
//#define ICC_BLOCK         -3         //IC卡锁卡
//
//#define EMV_RSP_ERR       -4         //IC返回码错误
//#define EMV_APP_BLOCK     -5         //应用已锁
//#define EMV_NO_APP        -6         //卡片里没有EMV应用
//#define EMV_USER_CANCEL   -7         //用户取消当前操作或交易
//#define EMV_TIME_OUT      -8         //用户操作超时
//#define EMV_DATA_ERR      -9         //卡片数据错误
//#define EMV_NOT_ACCEPT    -10        //交易不接受
//#define EMV_DENIAL        -11        //交易被拒绝
//#define EMV_KEY_EXP       -12        //密钥过期
//
////回调函数或其他函数返回码定义
//#define EMV_NO_PINPAD     -13        //没有密码键盘或键盘不可用
//#define EMV_NO_PASSWORD   -14        //没有密码或用户忽略了密码输入
//#define EMV_SUM_ERR       -15        //认证中心密钥校验和错误
//#define EMV_NOT_FOUND     -16        //没有找到指定的数据或元素
//#define EMV_NO_DATA       -17        //指定的数据元素没有数据
//#define EMV_OVERFLOW      -18        //内存溢出
//
////读交易日志
//#define NO_TRANS_LOG      -19
//#define RECORD_NOTEXIST   -20
//#define LOGITEM_NOTEXIST  -21
//#define ICC_RSP_6985      -22        // GPO/GAC中卡片回送6985, 由应用决定是否fallback
//
//#define EMV_PARAM_ERR     -30        // 20081008 liuxl

//typedef struct {
//	unsigned char RID[5];            //应用注册服务商ID
//	unsigned char KeyID;             //密钥索引
//	unsigned char HashInd;           //HASH算法标志,SM算法时，值为07
//	unsigned char ArithInd;          //RSA算法标志，SM算法时，值为04
//	unsigned char ModulLen;          //模长度，SM算法时，为公钥长度
//	unsigned char Modul[248];        //模，SM算法时，为公钥值
//	unsigned char ExponentLen;       //指数长度，SM算法时，为0，不检查此数据项。
//	unsigned char Exponent[3];       //指数，SM算法时，为0，不检查此数据项。
//	unsigned char ExpDate[3];        //有效期(YYMMDD)
//	unsigned char CheckSum[20];      //密钥校验和，SM算法时，为0，不检查此数据项。
//}EMV_CAPK;
// added by liuxl 20070710 不能通过简单的删除对应的公钥数据来操作回收证书数据
// 否则会令使用该公钥的其他发卡行公钥证书无法使用 
//typedef  struct
//{
//	unsigned char   ucRid[5];  		        // RID
//	unsigned char   ucIndex;   		        // CA公钥索引
//	unsigned char   ucCertSn[3];            // 发卡行证书序列号
//}EMV_REVOCLIST;

//	CLSS_TRANSPARAM 交易相关参数结构
//typedef struct CLSS_TRANSPARAM
//{
//    ulong  ulAmntAuth;     // 授权金额(ulong)
//    ulong  ulAmntOther;    // 其他金额(ulong)
//    ulong  ulTransNo;      // 交易序列计数器(4 BYTE)
//	uchar  ucTransType;    // 交易类型'9C'
//	uchar  aucTransDate[4]; // 交易日期 YYMMDD
//	uchar  aucTransTime[4]; // 交易时间 HHMMSS
//}Clss_TransParam;

//typedef struct{
//	unsigned char AppName[33];       //Not used at present 本地应用名，以'\x00'结尾的字符串
//	unsigned char AID[17];           //Not used at present 应用标志
//	unsigned char AidLen;            //Not used at present AID的长度
//    unsigned char SelFlag;           //Not used at present 选择标志(PART_MATCH 部分匹配  FULL_MATCH 全匹配)
//	unsigned char Priority;          //Not used at present 优先级标志
//	unsigned char TargetPer;         //目标百分比数
//	unsigned char MaxTargetPer;      //最大目标百分比数
//	unsigned char FloorLimitCheck;   //Not used at present 是否检查最低限额
//	unsigned char RandTransSel;      //是否进行随机交易选择
//	unsigned char VelocityCheck;     //Not used at present 是否进行频度检测
//    unsigned long FloorLimit;        //最低限额
//	unsigned long Threshold;         //阀值
//	unsigned char TACDenial[6];      //终端行为代码(拒绝)
//	unsigned char TACOnline[6];      //终端行为代码(联机)
//	unsigned char TACDefault[6];     //终端行为代码(缺省)
//    unsigned char AcquierId[6];      //Not used at present 收单行标志
//	unsigned char dDOL[256];         //Not used at present 终端缺省DDOL
//	unsigned char tDOL[256];         //终端缺省TDOL
//	unsigned char Version[3];        //应用版本
//	unsigned char RiskManData[10];   //Not used at present 风险管理数据
//}EMV_APPLIST;

#endif

#define RD_CVM_NO            0x00//no CVM
#define RD_CVM_SIG           0x10//signature
#define RD_CVM_ONLINE_PIN    0x11//online PIN
#define RD_CVM_OFFLINE_PIN   0x12//offline PIN
#define RD_CVM_CONSUMER_DEVICE   0x1F//Refer to consumer device
// 当 amount > contactless cvm limit 时，需要执行何种CVM方式：
#define RD_CVM_REQ_SIG 0x01
#define RD_CVM_REQ_ONLINE_PIN 0x02
//return code for CLSS kernel
#define CLSS_USE_CONTACT  -23    // 必须使用其他界面进行交易
#define EMV_FILE_ERR      -24
#define CLSS_TERMINATE    -25    // 应终止交易       -25 
#define CLSS_FAILED       -26    // 交易失败 20081217 
#define CLSS_DECLINE      -27   
#define CLSS_PARAM_ERR            -30 // -26 // 因EMV 内核中的参数错误定义为-30
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

#define REFER_APPROVE     0x01       //参考返回码(选择批准) 
#define REFER_DENIAL      0x02       //参考返回码(选择拒绝) 
#define ONLINE_APPROVE    0x00       //联机返回码(联机批准) 
#define ONLINE_FAILED     0x01       //联机返回码(联机失败)	 
#define ONLINE_REFER      0x02       //联机返回码(联机参考) 
#define ONLINE_DENIAL     0x03       //联机返回码(联机拒绝) 
#define ONLINE_ABORT      0x04       //兼容PBOC(交易终止) 

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


// 每个AID对应的参数,用于交易预处理
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
	
	//	uchar ucKernType; // 定义后续该AID 所遵循的内核类型
	
	// payWave
	uchar   ucZeroAmtFlg;       // 0-交易金额!=0; 1-交易金额=0
	uchar   ucStatusCheckFlg;    // 读卡器是否支持状态检查
    uchar   aucReaderTTQ[5];      // 终端交易性能，用于VISA/PBOC中，tag =9F66
	uchar   ucCLAppNotAllowed; // 1-该AID不能进行非接触交易
	
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
#define CLSS_MAX_KEY_NUM       7     //EMV库认证中心公钥表最多可存储的公钥数

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
	unsigned long   ulReferCurrCon;       // 参考货币代码和交易代码的转换系数(交易货币对参考货币的汇率*1000)
	
	unsigned short  usMchLocLen;          // 商户名称及位置数据域的长度
	unsigned char   aucMchNameLoc[257];   // 商户名称及位置(1-256 字节)
	unsigned char   aucMerchCatCode[2];   // 商户分类码'9F15'(2字节) 
	unsigned char   aucMerchantID[15];    // 商户标识(15字节) 
	
	unsigned char AcquierId[6];       //收单行标志
	
    unsigned char   aucTmID[8];           // 终端标识(终端号) 
	unsigned char   ucTmType;             // 终端类型
	unsigned char   aucTmCap[3];          // 终端性能
    unsigned char   aucTmCapAd[5];        // 终端附加性能
	
    unsigned char   aucTmCntrCode [2];     // 终端国家代码
	unsigned char   aucTmTransCur[2];      // 终端交易货币代码'5F2A'(2字节) 
	unsigned char   ucTmTransCurExp;       // 终端交易货币指数'5F36'(1字节)
	
	unsigned char   aucTmRefCurCode[2];    // 终端交易参考货币代码'9F3C'(2字节)
	unsigned char   ucTmRefCurExp;	       // 终端交易参考货币指数'9F3D'(1字节)
	
	unsigned char   aucRFU[3];
}Clss_ReaderParam;



//	ClssTmAidList AID列表结构
typedef struct
{
	uchar  ucAidLen;     
	uchar  aucAID[17];   // 20090609 liuxl 增加一字节 
	uchar  ucSelFlg;	 // 部分选择标识(1-部分匹配，0-完全匹配)
	uchar  ucKernType; // 20090609 liuxl 增加
}ClssTmAidList;
 

typedef struct VISA_AID_PARAM
{
    ulong ulTermFLmt;        //最低限额 - 同EMV最低限额

	uchar ucDomesticOnly; // 01(default):only supports domestic cl transaction
	// 00 or not present: supports international cl transaction
	
	uchar ucCvmReqNum;
	uchar aucCvmReq[5];// whether a CVM is required when the amount is higher than the Contactless CVM Required Limit.
	// 01-Signature 02-Online PIN	
	
	uchar ucEnDDAVerNo; // 默认=0, 读卡器支持所有版本的DDA卡片的脱机交易,
	// 01-仅支持dda版本为'01'的卡片进行脱机交易	 
	 
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
    ulong ulTermFLmt;        //最低限额 - 同EMV最低限额
    uchar aucRFU[4];	  
	
}Clss_PbocAidParam;

// mastercard
typedef struct  MC_AID_PARAM
{
    unsigned long FloorLimit;        //最低限额 - 同EMV最低限额
	unsigned long Threshold;         //阈值 

	unsigned short usUDOLLen;       
	unsigned char uDOL[256];        //终端缺省UDOL

	unsigned char TargetPer;         //目标百分比数 
	unsigned char MaxTargetPer;      //最大目标百分比数 
 	unsigned char FloorLimitCheck;   //是否检查最低限额
	unsigned char RandTransSel;      //是否进行随机交易选择
	unsigned char VelocityCheck;     //是否进行频度检测
 	unsigned char TACDenial[6];      //终端行为代码(拒绝)
	unsigned char TACOnline[6];      //终端行为代码(联机)
	unsigned char TACDefault[6];     //终端行为代码(缺省) 
    unsigned char AcquierId[6];      //收单行标志
	unsigned char dDOL[256];         //终端缺省DDOL  
	unsigned char tDOL[256];         //终端缺省TDOL 
	unsigned char Version[3];        //应用版本号
	unsigned char ForceOnline;       //商户强制联机(1 表示总是联机交易) 
	unsigned char MagAvn[3];         //磁条应用版本号

	uchar ucMagSupportFlg; // 1-读卡器支持MagStripe
	unsigned char ucRFU;
}Clss_MCAidParam; 


#endif
