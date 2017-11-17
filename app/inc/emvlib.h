/*****************************************************/
/* Emvlib.h                                          */
/* Define the Application Program Interface          */
/* of EMV2 for all PAX POS terminals     		     */
/* Created by ZengYun at 20/01/2005                  */
/*****************************************************/


#ifndef _EMV_LIB_H
#define _EMV_LIB_H

//#include "CL_common.h"
#include "emvlib_Prolin.h"

#ifndef uchar
typedef  unsigned char uchar;
#endif

#ifndef ushort
typedef  unsigned short ushort;
#endif

#ifndef ulong
typedef  unsigned long ulong;
#endif
#define CLSS_SLOT      0xff
#define CONTACT_SLOT   0x00

//不能通过箄1�7单的删除对应的公钥数据来操作回收证书数据
// 否则会令使用该公钥的其他发卡行公钥证书无法使甄1�7
#define  MAX_REVOCLIST_NUM    30      // EMVCO要求每个RID必须支持30个回收列衄1�7

#define MAX_APP_NUM       32         //EMV库应用列表最多可存储的应用数
#define MAX_APP_ITEMS     17         //甄1�7修改丄1�7
#ifndef _LINUX_TERM
#define MAX_KEY_NUM       64         //monitor平台下EMV库认证中心公钥表朄1�7多可存储的公钥数
#else
#define MAX_KEY_NUM       6          //proline平台下EMV库认证中心公钥表朄1�7多可存储的公钄1�7
#endif

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
#define EMV_CASHDEPOSIT   0x90		 //交易类型(存款)


//交易处理函数返回码定乄1�7
#define EMV_OK             0         //成功

#define ICC_RESET_ERR     -1         //IC卡复位失贄1�7
#define ICC_CMD_ERR       -2         //IC命令失败
#define ICC_BLOCK         -3         //IC卡锁卄1�7
   
#define EMV_RSP_ERR       -4         //IC返回码错评1�7
#define EMV_APP_BLOCK     -5         //应用已锁
#define EMV_NO_APP        -6         //卡片里没有EMV应用
#define EMV_USER_CANCEL   -7         //用户取消当前操作或交昄1�7
#define EMV_TIME_OUT      -8         //用户操作超时
#define EMV_DATA_ERR      -9         //卡片数据错误
#define EMV_NOT_ACCEPT    -10        //交易不接叄1�7
#define EMV_DENIAL        -11        //交易被拒组1�7
#define EMV_KEY_EXP       -12        //密钥过期

//回调函数或其他函数返回码定义
#define EMV_NO_PINPAD     -13        //没有密码键盘或键盘不可用
#define EMV_NO_PASSWORD   -14        //没有密码或用户忽略了密码输入
#define EMV_SUM_ERR       -15        //认证中心密钥校验和错评1�7
#define EMV_NOT_FOUND     -16        //没有找到指定的数据或元素
#define EMV_NO_DATA       -17        //指定的数据元素没有数捄1�7
#define EMV_OVERFLOW      -18        //内存溢出

//读交易日忄1�7
#define NO_TRANS_LOG      -19
#define RECORD_NOTEXIST   -20
#define LOGITEM_NOTEXIST  -21

#define ICC_RSP_6985      -22        // GAC中卡片回逄1�785, 由应用决定是否fallback
#define EMV_PARAM_ERR     -30

//因PED接口更新而增加返回�1�7�定乄1�7
#define EMV_PED_TIMEOUT   0x01   // PED接口返回 PED_RET_ERR_INPUT_TIMEOUT
#define EMV_PED_WAIT      0x02   // PED接口返回 PED_RET_ERR_WAIT_INTERVAL
#define EMV_PED_FAIL      0x03   // PED接口返回其他错误


#define REFER_APPROVE     0x01       //参�1�7�返回码(选择批准)
#define REFER_DENIAL      0x02       //参�1�7�返回码(选择拒绝)

#define ONLINE_APPROVE    0x00       //联机返回砄1�7(联机批准)
#define ONLINE_FAILED     0x01       //联机返回砄1�7(联机失败)
#define ONLINE_REFER      0x02       //联机返回砄1�7(联机参�1�7�1�7)
#define ONLINE_DENIAL     0x03       //联机返回砄1�7(联机拒绝)
#define ONLINE_ABORT      0x04       //兼容PBOC(交易终止)


#define AC_AAC       0x00
#define AC_TC        0x01
#define AC_ARQC      0x02
#define AC_AAC_HOST  0x03 //used for VisaOnLine


#ifdef WIN32
#pragma warning(disable:4103)
#pragma pack(1)//设定丄1�7�节对齐
// #else
// 	__attribute__((__packed__))
#endif

typedef struct{
	unsigned char AppName[33];       //本地应用名，仄1�7'\x00'结尾的字符串
	unsigned char AID[17];           //应用标志
	unsigned char AidLen;            //AID的长庄1�7
    unsigned char SelFlag;           //选择标志(PART_MATCH 部分匹配  FULL_MATCH 全匹酄1�7)
	unsigned char Priority;          //优先级标忄1�7
	unsigned char TargetPer;         //目标百分比数
	unsigned char MaxTargetPer;      //朄1�7大目标百分比敄1�7
 	unsigned char FloorLimitCheck;   //是否棄1�7查最低限预1�7
	unsigned char RandTransSel;      //是否进行随机交易选择
	unsigned char VelocityCheck;     //是否进行频度棄1�7浄1�7
    unsigned long FloorLimit;        //朄1�7低限预1�7
	unsigned long Threshold;         //阄1�7倄1�7
 	unsigned char TACDenial[6];      //终端行为代码(拒绝)
	unsigned char TACOnline[6];      //终端行为代码(联机)
	unsigned char TACDefault[6];     //终端行为代码(缺省)
    unsigned char AcquierId[6];      //收单行标忄1�7
	unsigned char dDOL[256];         //终端缺省DDOL
	unsigned char tDOL[256];         //终端缺省TDOL
	unsigned char Version[3];        //应用版本
	unsigned char RiskManData[10];   //风险管理数据
}EMV_APPLIST;

typedef struct{
	unsigned char aucAppPreName[17];  //应用优先选择名称，以'\0'结尾
	unsigned char aucAppLabel[17];  //应用标签，以'\0'结尾
	unsigned char aucIssDiscrData[244]; //tag 'BF0C'数据＄1�7�字节的长度字节+'BF0C'朄1�7处1�72个字节，仄1�7'\0'结尾
	                                    //tag '73'数据＄1�7�字节的长度字节+'73'朄1�7处1�72个字节，仄1�7'\0'结尾
	unsigned char aucAID[17]; //卡片AID，以'\0'结尾
	unsigned char ucAidLen; //卡片AID长度
}APPLABEL_LIST;

typedef struct {
	unsigned char RID[5];            //应用注册服务商ID
	unsigned char KeyID;             //密钥索引
	unsigned char HashInd;           //HASH算法标志
	unsigned char ArithInd;          //RSA算法标志
	unsigned char ModulLen;          //模长庄1�7
	unsigned char Modul[248];        //樄1�7
	unsigned char ExponentLen;       //指数长度
	unsigned char Exponent[3];       //指数
	unsigned char ExpDate[3];        //有效朄1�7(YYMMDD)
	unsigned char CheckSum[20];      //密钥校验咄1�7
}EMV_CAPK;

typedef struct{
	unsigned char MerchName[256];    //商户各1�7
	unsigned char MerchCateCode[2];  //商户类别砄1�7(没要求可不设罄1�7)
	unsigned char MerchId[15];       //商户标志(商户叄1�7)
	unsigned char TermId[8];         //终端标志(POS叄1�7)
	unsigned char TerminalType;      //终端类型
	unsigned char Capability[3];     //终端性能
	unsigned char ExCapability[5];   //终端扩展性能
	unsigned char TransCurrExp;      //交易货币代码指数
	unsigned char ReferCurrExp;      //参�1�7�货币指敄1�7
	unsigned char ReferCurrCode[2];  //参�1�7�货币代砄1�7
	unsigned char CountryCode[2];    //终端国家代码
	unsigned char TransCurrCode[2];  //交易货币代码
	unsigned long ReferCurrCon;      //参�1�7�货币代码和交易代码的转换系敄1�7(交易货币对参考货币的汇率*1000)
	unsigned char TransType;         //当前交易类型(EMV_CASH EMV_GOODS EMV_SERVICE EMV_GOODS&EMV_CASHBACK EMV_SERVICE&EMV_CASHBACK)
	unsigned char ForceOnline;       //商户强制联机(1 表示总是联机交易)
	unsigned char GetDataPIN;        //密码棄1�7测前是否读重试次敄1�7
	unsigned char SurportPSESel;     //是否支持PSE选择方式
}EMV_PARAM;

// 不能通过箄1�7单的删除对应的公钥数据来操作回收证书数据
// 否则会令使用该公钥的其他发卡行公钥证书无法使甄1�7
typedef  struct 
{
	unsigned char   ucRid[5];  		        // RID
	unsigned char   ucIndex;   		        // CA公钥索引
	unsigned char   ucCertSn[3];            // 发卡行证书序列号
}EMV_REVOCLIST;


typedef struct  
{
	unsigned char ucUseTermAIPFlg;   //是否基于卡片AIP进行风险管理,0-用卡片AIP,1-用终端AIP,默认丄1�7
	unsigned char aucTermAIP[2];    //终端是否强制进行风险管理，byte1-bit4丄1�7�强制进行风险管理；byte1-bit4丄1�7�不进行风险管理。默认两个字节全丄1�7�1�7
	unsigned char ucBypassAllFlg;         //whether bypass all other pin when one pin has been bypassed 1-Yes, 0-No
}EMV_EXTMPARAM;

// MCK 配置参数结构
typedef struct 
{
	unsigned char  ucBypassPin; //0-不支持，1－支持，默认支持
	unsigned char  ucBatchCapture;  // 0-online data capture, 1-batch capture
	void *pvoid;
}EMV_MCKPARAM;

// for PBOC 小额支付
// 设置终端电子现金相关参数
typedef struct 
{
	unsigned char ucECTSIFlg;   // TSI存在? 1-存在 电子现金终端支持指示噄1�7 （EC Terminal Support Indicator＄1�7
	unsigned char ucECTSIVal;   // 电子现金终端支持指示噄1�7 = 1,支持
	unsigned char ucECTTLFlg;   // TTL存在? 1-存在 电子现金终端交易限额（EC Terminal Transaction Limit＄1�7
	unsigned long ulECTTLVal;   // 电子现金终端交易限额
}EMV_TMECPARAM;

typedef struct{
    int MaxLen;
    unsigned short Tag;
    unsigned short Attr;
	unsigned short usTemplate[2];// 扄1�7属的模版，没有则丄1�7
	unsigned char ucSource;// 三个取�1�7�EMV_SRC_TM，EMV_SRC_ICC，EMV_SRC_ISS
}ELEMENT_ATTR;


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

#ifdef _LINUX_TERM
typedef struct
{
     unsigned int  modlen;           //PIN加密公钥模数镄1�7
     unsigned char mod[256];        //PIN加密公钥模数
     unsigned char exp[4];          //PIN加密公钥指数
     unsigned char iccrandomlen;    //从卡行取得的随机数长
     unsigned char iccrandom[8];    //从卡行取得的随机敄1�7
}RSA_PINKEY;


// PEDAPI 错误码起始�1�7�1�7
#define PED_RET_ERR_START -300
// PED函数返回正确
#define PED_RET_OK 0
// 没输入PIN
#define PED_RET_ERR_NO_PIN_INPUT   (PED_RET_ERR_START-5) //
// 取消输入PIN
#define PED_RET_ERR_INPUT_CANCEL   (PED_RET_ERR_START-6) //
// 函数调用小于朄1�7小间隔时闄1�7
#define PED_RET_ERR_WAIT_INTERVAL  (PED_RET_ERR_START-7) //

// 输入PIN超时
#define PED_RET_ERR_INPUT_TIMEOUT  (PED_RET_ERR_START-15) //
// IC卡不存在
#define PED_RET_ERR_NO_ICC          (PED_RET_ERR_START-16)//
// IC卡未初始匄1�7
#define PED_RET_ERR_ICC_NO_INIT    (PED_RET_ERR_START-17) //
//wanmin add on 2012.4.02 end
#endif

//密钥和应用管理函数定乄1�7
int  EMVCoreInit(void);
void EMVGetParameter(EMV_PARAM *tParam);
void EMVSetParameter(EMV_PARAM *tParam);
int  EMVGetTLVData(unsigned short Tag, unsigned char *DataOut, int *OutLen);
int  EMVSetTLVData(unsigned short Tag, unsigned char *DataIn, int DataLen);
int  EMVGetScriptResult(unsigned char *Result, int *RetLen);

//密钥和应用管理函数定乄1�7
int  EMVAddApp(EMV_APPLIST *App);
int  EMVGetApp(int Index, EMV_APPLIST *App);
int  EMVDelApp(unsigned char *AID, int AidLen);
int  EMVModFinalAppPara(EMV_APPLIST *ptEMV_APP);
int  EMVGetFinalAppPara(EMV_APPLIST *ptEMV_APP);
int  EMVGetLabelList(APPLABEL_LIST  *ptAppLabel, int *pnAppNum); //add by nt 20091218

int  EMVAddCAPK(EMV_CAPK  *capk );
int  EMVGetCAPK(int Index, EMV_CAPK  *capk );
int  EMVDelCAPK(unsigned char KeyID, unsigned char *RID);
int  EMVCheckCAPK(unsigned char *KeyID, unsigned char *RID);

// 因无法由应用程序绕过证书列表问题而增加接叄1�7
int  EMVAddRevocList(EMV_REVOCLIST *pRevocList);
int  EMVDelRevocList(unsigned char ucIndex, unsigned char *pucRID);
void EMVDelAllRevocList(void);

//交易处理函数定义
int  EMVAppSelect(int Slot,unsigned long TransNo);
int  EMVReadAppData(void);
int  EMVCardAuth(void);
int  EMVStartTrans(unsigned long ulAuthAmt, unsigned long ulCashBackAmt, unsigned char *pACType); //for Visa Online
int  EMVCompleteTrans(int nCommuStatus, unsigned char *paucScript, int *pnScriptLen, unsigned char *pACType); //for Visa Online

//读日志时调用
//增加参数 ucFlg, 用于确定是否加入锁定的应甄1�7
int  EMVAppSelectForLog(int Slot, unsigned char ucFlg);
int  EMVAddIccTag(ELEMENT_ATTR tEleAttr[], int nAddNum);

//回调函数定义
int  cEMVWaitAppSel(int TryCnt, EMV_APPLIST List[], int AppNum);  
int  cEMVInputAmount(unsigned long *AuthAmt, unsigned long *CashBackAmt);
int  cEMVGetHolderPwd(int TryFlag, int RemainCnt, unsigned char *pin);
int  cEMVReferProc(void);
int  cEMVUnknowTLVData(unsigned short Tag, unsigned char *dat, int len);
void cEMVAdviceProc(void);
void cEMVVerifyPINOK(void);
int  cEMVSetParam(void); //因应用需求，增加返回值处琄1�7
// 标准非接PBOC回调，接触式可将此回调函数实现为空函敄1�7
unsigned char  cEMVPiccIsoCommand(unsigned char cid,APDU_SEND *ApduSend,APDU_RESP *ApduRecv);

//证件认证(PBOC2.0)
int  cCertVerify(void);

//读交易日忄1�7(PBOC2.0)
int ReadLogRecord(int RecordNo);
int GetLogItem(unsigned short Tag, unsigned char *TagData, int *TagLen);

// 获取参数标志,如打印签名标忄1�7
int EMVGetParamFlag(unsigned char ucParam, int *pnFlg);

// 读电子现金余额时调用
int EMVGetCardECBalance(unsigned long *plBalance);
int EMVSetTmECParam(EMV_TMECPARAM *tParam);

// 设置MCK配置中的参数，用于将MCK配置设置传给内核
int EMVSetMCKParam(EMV_MCKPARAM *pMCKParam);
int EMVGetMCKParam(EMV_MCKPARAM *pMCKParam);// 获取内核的MCK参数

// 获取EMV 库发布版本号及发布日期信恄1�7
int EMVReadVerInfo(char *paucVer);

//增加设置是否支持advice的标忄1�7
void EMVSetConfigFlag(int nConfigflag);

// 初始化EMV数据元�1�7�（内核设置默认值）
void EMVInitTLVData(void);

#ifndef CLSS_PBOC
#define CLSS_PBOC
// add for contactless pboc
typedef struct
{
    unsigned long  ulAmntAuth;     // Authorize amount(unsigned long), for cash back, the amount required to include the amount of ulAmntOther.
    unsigned long  ulAmntOther;    // Other amount(unsigned long)
    unsigned long  ulTransNo;      // Transaction Sequence Counter(4 BYTE)
	unsigned char  ucTransType;     // Transaction type'9C', 0-Consumer/Services 1-Cash/Cash back
	unsigned char  aucTransDate[4]; // Transaction Date YYMMDD
	unsigned char  aucTransTime[4]; // Transaction time HHMMSS
}Clss_TransParam;
#endif

// 标准非接PBOC专用接口
// 设置非接PBOC朄1�7终�1�7�择以及GPO数据到内核中
int EMVSwitchClss(Clss_TransParam *ptTransParam,unsigned char *pucSelData, int nSelLen, unsigned char *pucGPOData, int nGPOLen);

// 设置大金额到内核
int EMVSetAmount(unsigned char * szAuthAmount, unsigned char * szCashBackAmount);

#ifndef _LINUX_TERM
int  EMVCoreVersion(void);

int  EMVProcTrans(void);

//设置(关闭)EMV－CORE的调试状怄1�7
void EMVSetDebug(int EnableFlag);
void EMVGetICCStatus(unsigned char *SWA, unsigned char *SWB);
int  cEMVOnlineProc(unsigned char *RspCode, unsigned char *AuthCode, int *AuthCodeLen, unsigned char *IAuthData, int *IAuthDataLen, unsigned char *Script, int *ScriptLen);

// 设置是否使用PCI 脱机PIN操作接口
int EMVSetPCIModeParam(unsigned char ucPciMode, unsigned char *pucExpectPinLen,unsigned long ulTimeoutMs);

// 清除EMV 库中保存的交易日志信息（用于终端风险管理中的朄1�7低限额检查功能）
int EMVClearTransLog(void);
#else
//ic卡类操作
uchar cEMVIccIsoCommand(uchar ucslot, APDU_SEND *tApduSend, APDU_RESP *tApduRecv);	//IC卡操作函数�1�7�该函数支持IC卡�1�7�用接口协议(T=0及T=1)〄1�7

//明文、密文PIN认证
int cEMVPedVerifyPlainPin(uchar ucIccSlot, uchar *pucExpPinLenIn, uchar *ucIccRespOut, uchar ucMode,ulong ulTimeoutMs);
int cEMVPedVerifyCipherPin(uchar ucIccSlot, uchar *pExpPinLenIn, RSA_PINKEY *tRsaPinKeyIn, uchar *pucIccRespOut, uchar ucMode,ulong ulTimeoutMs);

#endif

#endif
