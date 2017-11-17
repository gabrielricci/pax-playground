/****************************************************************************
锟斤拷锟斤拷锟角╋拷锟斤拷通讯协锟介：connect to the elec pad

锟劫革拷锟斤拷锟斤拷锟斤拷锟�锟斤拷锟斤拷)锟斤拷锟睫癸拷司

锟睫革拷锟斤拷史锟斤拷
1.0.0 2011锟斤拷10锟斤拷31锟斤拷    锟轿硷拷锟斤拷

****************************************************************************/

#ifndef _SIGN_PAD_H
#define _SIGN_PAD_H

#ifndef NULL
#define NULL ((void *)0)
#endif

//#define RS232_SIGN	0
#define STX				0x02

#ifndef ETX
#define ETX				0X03
#endif

#define SIGN_TIMER			0

#define FILE_SIGNATURE		"./res/sign.bmp"

//#define LEN_MAX_SIGN_DATA	1024*10+32
#define LEN_MAX_SIGN_DATA	1024*10		//锟斤拷锟斤拷锟斤拷签锟斤拷图片锟侥达拷小
#define LEN_MAX_RECV_DATA	1024*10+9	//签锟斤拷图片锟侥达拷小+锟斤拷锟斤拷+锟斤拷锟斤拷锟斤拷锟斤拷锟�

#define SIGNPAD_HANDSHAKE_REQ			0xA0
#define SIGNPAD_HANDSHAKE_REP			0xB0
#define SIGNPAD_UPLOAD_STATUS_REQ		0xA1
#define SIGNPAD_UPLOAD_STATUS_REP		0xB1
#define SIGNPAD_SIGN_REQ				0xA2
#define SIGNPAD_OK_REP					0xB2
#define SIGNPAD_CANCEL_REP				0xD2
#define SIGNPAD_NOK_REP					0xC2
#define SIGNPAD_SIGN_END_REQ			0xA3
#define SIGNPAD_SIGN_END_REP			0xB3
#define SIGNPAD_UPLOAD_START_REQ		0xA4
#define SIGNPAD_UPLOAD_START_REP		0xB4
#define SIGNPAD_UPLOAD_END_REQ			0xA5
#define SIGNPAD_UPLOAD_END_REP			0xB5


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
	void SignPad_Init(int iPortNo,long BaudRateType);
	void SignPad_Close(int iPortNo);
	unsigned char SignPad_HandShake(int iPortNo, int uiTimeOutSec);
	unsigned char SignPad_SignStart(int iPortNo, uchar *szSerial, int uiTimeOutSec);
	unsigned char SignPad_SignEnd(int iPortNo);


	int SignPad_Txd(int iPortNo, unsigned char ucCmdID, unsigned char *psTxdData, unsigned short uiDataLen);
	int SignPad_Rxd(int iPortNo, unsigned char *psRxdData, unsigned int *uiDataLen, unsigned short uiTimeOutSec);

	int PrtDebugInfo(uchar *pInData, int iLen, int type);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* _UTIL_H */
