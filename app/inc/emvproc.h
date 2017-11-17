/*
 ============================================================================
 Name        : emvproc.h
 Author      : Rex
 Version     : 1.0.0
 Copyright   : PAX Computer Technology(Shenzhen) CO., LTD
 Description : EMV transaction
 History     :
 1.0.0 Nov 2, 2012    Rex
 ============================================================================
 */

#ifndef _EMVPBOC_H
#define _EMVPBOC_H


#define _LINUX_TERM
#include "emvlib.h"
//#include "CL_common.h"
//#include "CLEntryApi.h"
//#include "CLssApi_PBOC.h"


#ifndef uchar
	typedef unsigned char	uchar;
#endif

#ifndef ushort
	typedef unsigned short	ushort;
#endif

#ifndef uint
	typedef unsigned int	uint;
#endif

#ifndef ulong
	typedef unsigned long	ulong;
#endif

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

// ICC SLOT
#define ICC_USER		0

// file
#define CAPKFILE	"./data/CAPK.LOG"
#define APPFILE		"./data/APP.LOG"
#define CAPKINFO	"./data/CAPKINFO"
#define AIDINFO		"./data/AIDINFO"

#ifdef __cplusplus
extern "C"
{
#endif

int OsPiccDetect(unsigned char Mode, unsigned char *CardType, unsigned char *SerialInfo, unsigned char *CID, unsigned char *Other);

int EMVTransInit(void);

int GetICCData(void);

int _SaveAppFile(EMV_APPLIST *pstApp, char* szFileName);
int SaveAIDFile(EMV_APPLIST *pstApp);

int _AddAppIntoEmvLib(char* szFileName);
int AddAppIntoEmvLib(void);

int SaveCAPKFile(EMV_CAPK  *pstCAPK, char* szFileName);

int AddCAPKIntoEmvLib(char* szFileName);

void SetEmvParameter(void);

void InitTestApps(void);

#ifdef __cplusplus
};
#endif
#endif /* _EMVPBOC_H */

