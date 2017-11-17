#ifndef _GLOBLE_H_
#define _GLOBLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <osal.h>
#include <xui.h>

#include <pthread.h>//For Multi-Thread

#include "emvlib_Prolin.h"
#include "CLEntryApi_Prolin.h"
#include "ClssApi_MC_prolin.h"
#include "ClssApi_Wave_prolin.h"
#include "ClssApi_PBOC_Prolin.h"
#include "CL_common.h"
#include "ProcTrans.h"
#include "cpinpad.h"

#define OK    1
#define NOK   0

#define DEBUG

// 错误代码(具体定义待定!!!!!)
#define ERR_BASE			0x10000
#define ERR_PINPAD			(ERR_BASE+0x01)
#define ERR_NO_TELNO		(ERR_BASE+0x03)
#define ERR_SWIPECARD		(ERR_BASE+0x05)
#define ERR_USERCANCEL		(ERR_BASE+0x06)
#define ERR_TRAN_FAIL		(ERR_BASE+0x07)
#define ERR_UNSUPPORT_CARD	(ERR_BASE+0x08)
#define ERR_SEL_ACQ			(ERR_BASE+0x09)
#define ERR_HOST_REJ		(ERR_BASE+0x0A)

#define ERR_FILEOPER		(ERR_BASE+99)
// #define ERR_NOT_EMV_CARD	(ERR_BASE+100)
#define ERR_NEED_INSERT		(ERR_BASE+101)
#define ERR_NEED_FALLBACK	(ERR_BASE+102)
#define ERR_NEED_SWIPE		(ERR_BASE+103)
#define ERR_EXIT_APP		(ERR_BASE+990)
#define ERR_NO_DISP			(ERR_BASE+999)
//Add by Grace 2015/03/19
#define ERR_NEED_RETRY		(ERR_BASE+103)



#define EMV_OK             0
// UI Prompt ID definitions
#define CLSS_USE_ONECARD	0X19
#define CLSS_COLLISION_DETECTED  -50


//PICC LED显示控制宏
#define PICC_LED_RED    0x01
#define PICC_LED_GREEN  0x02
#define PICC_LED_YELLOW 0x04
#define PICC_LED_BLUE   0x08
#define PICC_LED_NONE   0xFF



//Icon Status Globle Variables
extern int icon_IC_Status;
extern int icon_Prn_Status;
extern int icon_Battery_Status;

extern int icon_IC_Status_Last;
extern int icon_Prn_Status_Last;
extern int icon_Battery_Status_Last;


extern int swipted;
extern int EntryType;

extern unsigned char cAccount[20];
extern unsigned char cExpDate[8];
extern uchar		szTrack2[40+1];
extern unsigned char szTrack3[108];
extern Clss_TransParam ClssTransParam;

extern XuiWindow *canvasConfirmAccount;









#endif
