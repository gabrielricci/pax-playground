/*--------------------------------------------------------------------------
 * OSAL (Operating System Abstract Layer) is unified API for POS application
 * development.
 *
 * Author: zhengrc@paxsz.com
 * Date: 2011 Dec 6th
 * Version: 1.2
 *
 * $Id: osal.h 5013 2016-12-05 01:36:14Z liuchuan $
 *--------------------------------------------------------------------------*/
#pragma once
#ifndef _OSAL_H_
#define _OSAL_H_

#ifndef __KERNEL__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef uchar
#define uchar	unsigned char
#endif
#ifndef ushort
#define ushort	unsigned short
#endif
#ifndef ulong
#define ulong	unsigned long
#endif

/*--------------------------------------------------------------------------
 * Common return codes
 *--------------------------------------------------------------------------*/
#define RET_OK				   0	/* Succeed */
#define ERR_INVALID_HANDLE		(-1000)	/* Handle error */
#define ERR_TIME_OUT			(-1001)	/* Overtime */
#define ERR_APP_NOT_EXIST		(-1002)	/* The app doesn't exist */
#define ERR_INVALID_PARAM		(-1003)	/* Error parameter */
#define ERR_DEV_NOT_EXIST		(-1004)	/* The device doesn't exist */
#define ERR_DEV_BUSY			(-1005)	/* The device is in use */
#define ERR_DEV_NOT_OPEN		(-1006)	/* The device isn't open */
#define ERR_ACCESS_DENY			(-1007)	/* No permission */
#define ERR_FONT_NOT_EXIST		(-1008)	/* The font doesn't exist */
#define ERR_FILE_FORMAT			(-1009)	/* File format error */
#define ERR_USER_CANCEL			(-1010)	/* User cancel */
#define ERR_NO_PORT			(-1011)	/* No usable port */
#define ERR_NOT_CONNECT			(-1012)	/* No connection */
#define ERR_MEM_FAULT			(-1013)	/* Memory error */
#define ERR_SYS_BAD			(-1014)	/* System error */
#define ERR_SYS_NOT_SUPPORT		(-1015)	/* System doesn't support */
#define ERR_STR_LEN			(-1016)	/* String is too long */
#define ERR_TOO_MANY_HANDLE		(-1017)	/* Handle is too much */
#define ERR_APP_MODE			(-1018)	/* Mode error */
#define ERR_FILE_NOT_EXIST		(-1019)	/* The file doesn't exist */
#define ERR_TS_DISABLE			(-1020)	/* Touchscreen can't be used */
#define ERR_FONT_CODE			(-1021)	/* Coding error */
#define ERR_VERSION_TOO_LOW		(-1022)	/* Version is too low */
#define ERR_BATTERY_ABSENT		(-1023)	/* Battery does not exist */
#define ERR_BATTERY_VOLTAGE_TOO_LOW	(-1024) /* Battery voltage is too low */

#define ERR_BASE_ABSENT			(-1025)	/* Base does not exist */
#define ERR_BASE_COMM			(-1026)	/* communication failed with Base*/

/*--------------------------------------------------------------------------
 * System functions
 *--------------------------------------------------------------------------*/
#define ERR_FILE_NOT_FOUND		(-2201)	/* The file can't be found */
#define ERR_VERIFY_SIGN_FAIL		(-2204)	/* Signature verification error */
#define ERR_NO_SPACE			(-2205)	/* No space */
#define ERR_NEED_ADMIN			(-2207)	/* Need higher permissions */
#define ERR_PUK_NOT_EXIST		(-2208)	/* The PUK doesn't exist */

/*--------------------------------------------------------------------------
 * Encryption & Decryption
 *--------------------------------------------------------------------------*/
#define ERR_DATA_TOO_BIG		(-2400)	/* Encrypted data of RSA is too long */
#define ERR_GEN_RANDOM			(-2401)	/* Failed to generate random numbers */
#define ERR_GEN_FAIL			(-2402)	/* Failed to generate RSA key */

#define SHA_TYPE_1			1
#define SHA_TYPE_224			224
#define SHA_TYPE_256			256
#define SHA_TYPE_384			384
#define SHA_TYPE_512			512

/*--------------------------------------------------------------------------
 * PED management
 *--------------------------------------------------------------------------*/
#define ERR_PED_NO_KEY			(-3801)	/* The key doesn't exist */
#define ERR_PED_KEY_IDX_ERR		(-3802)	/* The key index isn't in the range */
#define ERR_PED_DERIVE_ERR		(-3803)	/* The key hierarchy is error */
#define ERR_PED_CHECK_KEY_FAIL		(-3804)	/* The key verification has failed */
#define ERR_PED_NO_PIN_INPUT		(-3805)	/* No PIN input */
#define ERR_PED_PIN_INPUT_CANCEL	(-3806)	/* Cancel the input PIN */
#define ERR_PED_WAIT_INTERVAL		(-3807)	/* Function call is less than the minimum time interval */
#define ERR_PED_KCV_MODE_ERR		(-3808)	/* KCV mode error */
#define ERR_PED_KEY_TAG_ERR		(-3809)	/* Key label error */
#define ERR_PED_KEY_TYPE_ERR		(-3810)	/* Key type isn't support */
#define ERR_PED_PIN_LEN_ERR		(-3811)	/* The PIN length error */
#define ERR_PED_DSTKEY_IDX_ERR		(-3812)	/* Target key index error */
#define ERR_PED_SRCKEY_IDX_ERR		(-3813)	/* Bad key source index */
#define ERR_PED_KEY_LEN_ERR		(-3814)	/* Bad key length error */
#define ERR_PED_INPUT_PIN_TIMEOUT	(-3815)	/* Enter the PIN overtime */
#define ERR_PED_NO_ICC			(-3816)	/* IC card doesn't exist */
#define ERR_PED_ICC_INIT_ERR		(-3817)	/* IC card initialization failed */
#define ERR_PED_GROUP_IDX_ERR		(-3818)	/* The index of DUPKT group is error */
#define ERR_PED_TAMPERED		(-3819)	/* Ped is locked */
#define ERR_PED_NO_MORE_BUF		(-3820)	/* No free buffer */
#define ERR_PED_NORMAL_ERR		(-3821)	/* PED normal error */
#define ERR_PED_NEED_ADMIN		(-3822)	/* Need higher permissioni */
#define ERR_PED_DUKPT_KSN_OVERFLOW	(-3823)	/* DUKPT has overflowed */
#define ERR_PED_KCV_CHECK_FAIL		(-3824)	/* KCV check failed */
#define ERR_PED_SRCKEY_TYPE_ERR		(-3825)	/* The type of source key ID doesn't match the source key */
#define ERR_PED_UNSPT_CMD		(-3826)	/* Command isn't supported */
#define ERR_PED_ADMIN_ERR		(-3827)	/* System failed to get the sensitive data */
#define ERR_PED_DOWNLOAD_INACTIVE	(-3828)	/* The download function of PED isn't activated */
#define ERR_PED_KCV_ODD_CHECK_FAIL	(-3829)	/* KCV odd parity failed */
#define ERR_PED_PED_DATA_RW_FAIL	(-3830)	/* Read PED data failed */
#define ERR_PED_ICC_CMD_ERR		(-3831)	/* IC card operation error */

#define ERR_PED_DUKPT_NEED_INC_KSN	(-3832)	/* DUKPT KSN need plus one */
#define ERR_PED_DUKPT_NO_KCV		(-3833)	/* DUKPT havn't KCV */
#define ERR_PED_NO_FREE_FLASH		(-3834)	/* The space of PED isn't enough */
#define ERR_PED_INPUT_CLEAR		(-3835)	/* The user presses the clear key to exit the PIN input */
#define ERR_PED_INPUT_BYPASS_BYFUNCTION	(-3836)	/* The user presses the FN/ATM4 key to cancel the entered PIN */
#define ERR_PED_NO_PIN_MODE		(-3837)	/* The input mode of the PIN hasn't been set */
#define ERR_PED_DATA_MAC_ERR		(-3838)	/* Mac data parity error */
#define ERR_PED_DATA_CRC_ERR		(-3839)	/* CRC data parity error */
/* The key value doesn't meet requirements of work, or already exists */
#define ERR_PED_KEY_VALUE_INVALID	(-3840)

/* MACROs for PED key type */
#define PED_TLK                0x01    /* Loading key */
#define PED_TMK                0x02    /* Master key */
#define PED_TPK                0x03    /* PIN key */
#define PED_TAK                0x04    /* MAC key */
#define PED_TDK                0x05    /* Data key */
#define PED_TRK                0x07    /* MSR key */    
#define PED_TIK                0x10    /* DUKPT key */
#define PED_TAESK              0x20    /* AES key */

#define PED_SM2_PVT_KEY        0x30    /* SM2 private key */
#define PED_SM2_PUB_KEY        0x31    /* SM2 public key */
#define PED_SM4_TMK            0x32    /* SM4 master key */
#define PED_SM4_TPK            0x33    /* SM4 PIN key */
#define PED_SM4_TAK            0x34    /* SM4 MAC key */
#define PED_SM4_TDK            0x35    /* SM4 Data Key */

#define PED_ENCRYPT			1
#define PED_DECRYPT			0
#define TRK_FIXED			0
#define TRK_DUKPT			1

#define PED_ASTERISK_ALIGN_LEFT		0
#define PED_ASTERISK_ALIGN_CENTER	1
#define PED_ASTERISK_ALIGN_RIGHT	2

/* SM */
#define SM2_PVK_LEN         32      /* SM2 private key length */
#define SM2_PUK_LEN         64      /* SM2 public key length */
#define SM2_SIG_LEN         64      /* SM2 signature length */
#define SM2_UID_MAX_LEN     512     /* SM2 UserID max length */
#define SM2_DATA_MAX_LEN    1024    /* SM2 Sign/Verify max length */
#define SM3_CS_LEN          32      /* SM3 checksum length */
#define SM4_IV_LEN          16      /* SM4 init-vector length */
#define SM4_PB_LEN          16      /* SM4 PIN block length */


/* RGB(_r, _g, _b) */
#ifndef RGB
#define RGB(_r, _g, _b) ((ushort)((((ushort)((_r) >> 3)) << 11) | \
    (((ushort)((_g) >> 2)) << 5) | ((uchar)((_b) >> 3))))
#endif

#ifndef RGB_R
#define RGB_R(rgb) ((uchar)(((rgb) >> 11) << 3))
#endif

#ifndef RGB_G
#define RGB_G(rgb) ((uchar)((rgb) >> 5) << 2)
#endif

#ifndef RGB_B
#define RGB_B(rgb) ((uchar)((rgb) << 3))
#endif

/*--------------------------------------------------------------------------
 * Printer
 *--------------------------------------------------------------------------*/
#define ERR_PRN_BUSY			(-3701)	/* Printer is busy */
#define ERR_PRN_PAPEROUT		(-3702)	/* Printer out of paper */
#define ERR_PRN_WRONG_PACKAGE		(-3703)	/* Data format error */
#define ERR_PRN_OVERHEAT		(-3704)	/* Printer over heat */
#define ERR_PRN_OUTOFMEMORY		(-3705)	/* Print data is too large */
#define ERR_PRN_OVERVOLTAGE		(-3706)	/* Over-high voltage */

#define REAL_PRN_NAME			"/dev/printer"
#define REAL_PRN_WIDTH			384
#define VIRTUAL_PRN_WIDTH		384
#define PRN_WIDTH_MAX			600
#define PRN_HEIGHT_MAX			5000
#define BIGGEST_FONT			64
#define PAGE_LINE			200
#define DEFAULTFONT			"paxfont.ttf"

enum PrinterType { PRN_REAL, PRN_BMP, PRN_PNG, PRN_PDF };
enum PrinterMode { PRNMODE_HOR, PRNMODE_VERT };

/*--------------------------------------------------------------------------
 * Magnetic card
 *--------------------------------------------------------------------------*/
#define ERR_MSR_FAILED		(-2701)	/* Operation error */
#define ERR_MSR_HEADERR		(-2702)	/* Starting character is not found */
#define ERR_MSR_ENDERR		(-2703)	/* Terminator is not found */
#define ERR_MSR_LRCERR		(-2704)	/* LRC parity error */
#define ERR_MSR_PARERR		(-2705)	/* Wrong one bit of the track data */
#define ERR_MSR_NOT_SWIPED	(-2706)	/* Not swiped */
#define ERR_MSR_NO_DATA		(-2707)
#define ERR_MSR_END_ZEROERR	(-2708)
#define ERR_MSR_PED_DECRYPTERR	(-2709)	/* Ped decryption failed */

/*--------------------------------------------------------------------------
 * IC card
 *--------------------------------------------------------------------------*/
#define ICC_USER_SLOT		0
#define ICC_SAM1_SLOT		2
#define ICC_SAM2_SLOT		3
#define ICC_SAM3_SLOT		4
#define ICC_SAM4_SLOT		5

/* Option parameter in OsIccInit */
#define ICC_VCC_5000mV		3
#define ICC_VCC_3000mV		2
#define ICC_VCC_1800mV		1
#define ICC_PPS_SUPPORT		4

#define ICC_SPD_1X		0
#define ICC_SPD_2X		8
#define ICC_SPD_4X		16
#define ICC_ISO_SPEC		32
#define ICC_EMV_SPEC		0

/* CtrlFlag parameter in OsIccExchange */
#define ICC_AUTO_GET_RSP	1
#define ICC_NON_AUTO_RSP	0

#define ERR_SCI_HW_NOCARD	(-2800)	/* no card */
#define ERR_SCI_HW_STEP		(-2801)	/* exchange when no init, warm reset when no active */
#define ERR_SCI_HW_PARITY	(-2802)	/* parity error */
#define ERR_SCI_HW_TIMEOUT	(-2803)	/* time out */
#define ERR_SCI_HW_TCK		(-2804)	/* TCK error */
#define ERR_SCI_ATR_TS		(-2810)	/* TS error */
#define ERR_SCI_ATR_TA1		(-2811)	/* TA1 error */
#define ERR_SCI_ATR_TD1		(-2812)	/* TD1 error */
#define ERR_SCI_ATR_TA2		(-2813)	/* TA2 error */
#define ERR_SCI_ATR_TB1		(-2814)	/* TB1 error */
#define ERR_SCI_ATR_TB2		(-2815)	/* TB2 error */
#define ERR_SCI_ATR_TC2		(-2816)	/* TC2 error */
#define ERR_SCI_ATR_TD2		(-2817)	/* TD2 error */
#define ERR_SCI_ATR_TA3		(-2818)	/* TA3 error */
#define ERR_SCI_ATR_TB3		(-2819)	/* TB3 error */
#define ERR_SCI_ATR_TC3		(-2820)	/* TC3 error */
#define ERR_SCI_T_ORDER		(-2821)	/* protocol is not T0 or T1 */
#define ERR_SCI_PPS_PPSS	(-2830)	/* PPSS error in PPS */
#define ERR_SCI_PPS_PPS0	(-2831)	/* PPS0 error in PPS */
#define ERR_SCI_PPS_PCK		(-2832)	/* PCK error in PPS */
#define ERR_SCI_T0_PARAM	(-2840)	/* data in transmitting is too long in T0 */
#define ERR_SCI_T0_REPEAT	(-2841)	/* too many character repetition in T0 */
#define ERR_SCI_T0_PROB		(-2842)	/* procedure byte error in T0 */
#define ERR_SCI_T1_PARAM	(-2850)	/* data in transmitting is too long in T1 */
#define ERR_SCI_T1_BWT		(-2851)	/* BWT exceed in T1 */
#define ERR_SCI_T1_CWT		(-2852)	/* CWT exceed in T1 */
#define ERR_SCI_T1_BREP		(-2853)	/* too many block repetition in T1 */
#define ERR_SCI_T1_LRC		(-2854)	/* LRC error in T1 */
#define ERR_SCI_T1_NAD		(-2855)	/* NAD error in T1 */
#define ERR_SCI_T1_LEN		(-2856)	/* LEN error in T1 */
#define ERR_SCI_T1_PCB		(-2857)	/* PCB error in T1 */
#define ERR_SCI_T1_SRC		(-2858)	/* SRC error in T1 */
#define ERR_SCI_T1_SRL		(-2859)	/* SRL error in T1 */
#define ERR_SCI_T1_SRA		(-2860)	/* SRA error in T1 */
#define ERR_SCI_PARAM		(-2880)	/* a NULL pointer */

#define SCI_T0_WARNING		1
#define SCI_IDLE		0x00
#define SCI_COLD_RESET		0x01
#define SCI_WARM_RESET		0x02
#define SCI_ACTIVATION		0x03

/*--------------------------------------------------------------------------
 * RF card
 *--------------------------------------------------------------------------*/

/* RF card types */
#define ISO14443_A		0
#define ISO14443_B		1
#define JISX6319_4		2
#define MIFARE_STANDARD		3

/* Error codes */
#define PCD_ERR_BASE_NUMBER	(-2900)
#define PCD_ERR_PAR_FLAG	(-1 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_CRC_FLAG	(-2 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_WTO_FLAG	(-3 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_COLL_FLAG	(-4 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_ECD_FLAG	(-5 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_EMD_FLAG	(-6 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_COM_FLAG	(-7 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_AUT_FLAG	(-8 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_TRANSMIT_FLAG	(-9 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_PROTOCOL_FLAG	(-10 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_PARAMFILE_FLAG	(-11 + PCD_ERR_BASE_NUMBER)
#define PCD_ERR_USER_CANCEL	(-12 + PCD_ERR_BASE_NUMBER)	/* user cancel */
#define PCD_ERR_CARRIER_OBTAIN_FLAG (-13 + PCD_ERR_BASE_NUMBER)	/* carrier not obtained */
#define PCD_ERR_CONFIG_FLAG	(-14 + PCD_ERR_BASE_NUMBER)	/* set config error */

/* PCD error codes */
#define PCD_STATE_ERR_BASE_NUMBER	(-2950)
#define PCD_ERR_NOT_ALLOWED_FLAG	(-1 + PCD_STATE_ERR_BASE_NUMBER)
#define PCD_CHIP_ABNORMAL		(-2 + PCD_STATE_ERR_BASE_NUMBER)
#define PCD_CHIP_NOT_OPENED		(-3 + PCD_STATE_ERR_BASE_NUMBER)
#define PCD_CHIP_CARDEXIST		(-4 + PCD_STATE_ERR_BASE_NUMBER)
#define PCD_ERR_NOT_IDLE_FLAG		(-5 + PCD_STATE_ERR_BASE_NUMBER)
#define PCD_ERR_NOT_POLLING_FLAG	(-6 + PCD_STATE_ERR_BASE_NUMBER)
#define PCD_ERR_NOT_WAKEUP_FLAG		(-7 + PCD_STATE_ERR_BASE_NUMBER)
#define PCD_ERR_NOT_ACTIVE_FLAG		(-8 + PCD_STATE_ERR_BASE_NUMBER)

/*--------------------------------------------------------------------------
 * Modem
 *--------------------------------------------------------------------------*/
#define ERR_MDM_TXOVER		(-3100)	/* The send buffer is full */
/* The hardware of NGFP S800 has no side telephone port, and also no such return value */
#define ERR_MDM_BYPASS_BUSY	(-3101)
/* Telephone line is not properly connected, or parallel line is occupied */
#define ERR_MDM_LINE_BUSY	(-3102)
/* Carrier wave of telephone lost. (Built synchronization chain failure) */
#define ERR_MDM_NO_CARRIER	(-3103)
#define ERR_MDM_NO_ANSWER	(-3104)	/* No response for dialing */
#define ERR_MDM_CALLEE_BUSY	(-3105)	/* Line busy */
#define ERR_MDM_NO_LINE		(-3106)	/* Telephone line is not connected (line voltage is 0) */
#define ERR_MDM_CMD_BUF_FULL	(-3108)	/* The excommand buffer is full */
#define ERR_MDM_CMD_TOO_LONG	(-3109)	/* Command of excommand is too long, exceeded 100 */
#define ERR_MDM_CMD_NOT_SUPPORT	(-3110)	/* Excommand does not support the command */
#define ERR_MDM_CANCEL_CONNECT	(-3211)	/* Received the cancel button in the dial-up process */
#define ERR_MDM_INIT		(-3217)	/* Modem init fail */

#define MODEM_PRE_DIAL			0
#define MODEM_DIAL			1
#define MODEM_WAIT_CALL			2

#define MODEM_COMM_SYNC			0
#define MODEM_COMM_ASYNC		1

#define MODEM_CODE_DTMF			0
#define MODEM_CODE_PULSE1		1
#define MODEM_CODE_PULSE2		2
#define MODEM_CONNECTED			0
#define MODEM_SEND_POOL_FULL		1
#define MODEM_HAVE_DIALED		6
#define MODEM_RECV_POOL_HAVE_DATA	8
#define MODEM_RECVDATA_SEND_IS_FULL	9
#define MODEM_CONNECTING		10
#define MODEM_IDLE			11

#define IP6_STATELESS_ADDR_AUTOCONFIG 0
#define IP6_DHCP6_STATEFULL_ADDR_CONFIG 1
#define IP6_DHCP6_STATELESS_ADDR_CONFIG 2


/*--------------------------------------------------------------------------
 * IP communication
 *--------------------------------------------------------------------------*/
#define ERR_NET_SERVER_BAD	(-3301)	/* IP server error */
/* IP server is busy; it can only provide sevice for 5 applications at a time */
#define ERR_NET_SERVER_BUSY	(-3302)
#define ERR_NET_NO_ROUTE	(-3305)	/* Set no route */
/* Connection is full; the application can set up to 20 connections at a time */
#define ERR_NET_FULL		(-3306)
/* Network interface link is unavailable (The link has not set up or
   there is no corresponding device) */
#define ERR_NET_IF		(-3307)
/* TCP or UDP session connection has been broken and is unavailable */
#define ERR_NET_SESS_BROKEN	(-3308)
#define ERR_NET_PASSWD		(-3309)	/* Password is incorrect */
#define ERR_NET_LOGOUT		(-3310)	/* Application logout initiatively */
#define ERR_NET_INIT		(-3311)	/* Initialization failed */
#define ERR_NET_DHCP_DISCOVER	(-3312)	/* Has not found DHCP Server */
#define ERR_NET_DHCP_OFFER	(-3313)	/* DHCP cannot be assigned to the IP address */
#define ERR_NET_DHCP_START	(-3314)	/* DHCP not started */
#define ERR_NET_DNS		(-3315)	/* DNS cannot analyse the corresponding domain */
#define ERR_NET_SERV_USING	(-3316)	/* The port specified by the server is in use */
#define ERR_NET_NODNServer	(-3317)	/* Has not configured the domain name server */
#define ERR_NET_LINKDOWN	(-3318)	/* Link is broken by the Server */
#define ERR_NET_CONN		(-3319)	/* Can not connect to the specified Server */
#define ERR_NET_TIMEOUT		(-3320)	/* Connection timeout */
#define ERR_NET_PPP		(-3321)	/* PPP disconnects */
#define ERR_NET_SERV		(-3322)	/* PPPoE server not found */
#define ERR_NET_AGAIN		(-3323)	/* The requested resource doesn't exist */
#define ERR_NET_AUTH		(-3324)	/* The RADIUS Server refused the modem connection */
#define ERR_PPP_SERV_NOTREADY	(-3325)	/* The window server not ready for ppp_direct */
#define ERR_NET_PACKET		(-3326)
#define ERR_NET_ICMPV6_UNREACH	(-3327)
#define ERR_NET_ICMPV6_PKT_TOOBIG	(-3328)
#define ERR_NET_ICMPV6_TIME_EXCEED	(-3329)
#define ERR_NET_ICMPV6_PARAMPROB	(-3330)
#define ERR_NET_DHCPV6_DNS              (-3331)
#define ERR_NET_DHCP6_SOLICITE          (-3335)
#define ERR_NET_DHCP6_START             (-3336)
#define ERR_NET_DHCP6_REQUEST           (-3337)
#define ERR_NET_DHCP6_INFORMATION       (-3338)
#define ERR_NET_DHCP6_VALID_LIFE        (-3339)
#define ERR_NET_DHCP6_NOADDRSAVAIL      (-3340)
#define ERR_NET_DHCP6_DUPLICATE_ADDR    (-3341)

#define ERR_MODE_NOT_SUPPORT	(-3350)
#define ERR_WIFI_POWER_OFF	(-3351)
#define ERR_NOT_FOUND_AP	(-3352)
#define ERR_AUTH_SEC_MODE	(-3353)
#define ERR_WIFI_BAD_SIGNAL	(-3354)
#define ERR_PIN_FAIL		(-3355)		/* AP pin checksum error */
#define ERR_WPS_PBC_OVERLAP	(-3356)		/* WPS overlap detected in PBC mode */
#define ERR_WPS_FAIL 		(-3357)		/* WPS registration failed after M2/M2D */
#define ERR_WPS_TIMEOUT		(-3358)		/* WPS config over 2min,time out */
#define ERR_EAP_ID			(-3359)

#define ERR_ROUTE_EXIST     (-3360)
#define ERR_ROUTE_NONEXIST  (-3361)
#define RET_CONNECTING		1

#define P_LINUX			0	/* ppp_direct server runs Linux */
#define P_WINDOWS		1	/* ppp_direct server runs Windows */

/*--------------------------------------------------------------------------
 * SYSTEM MANAGER
 *--------------------------------------------------------------------------*/
#define FILE_TYPE_APP		1	/* Application */
#define FILE_TYPE_APP_PARAM	2	/* Application parameter */
#define FILE_TYPE_SYS_LIB	3	/* Dynamic system library */
#define FILE_TYPE_PUB_KEY	4	/* User public key */
#define FILE_TYPE_AUP		5	/* Application upgrade kit */

#define FILE_TYPE_KERNEL	6	/* Firmware kernel, "firmware-kernel" */
#define FILE_TYPE_RAMDISK	7	/* Firmware ramdisk, "firmware-ramdisk" */
#define FILE_TYPE_BASE		8	/* Firmware base, "firmware-base" */
#define FILE_TYPE_FWP		9	/* Firmware pkg, "*.fwp" */

#define PUK_TYPE_M		0	/* Vendor public key */
#define PUK_TYPE_US_PUK		1	/* User signature certificate public key */
#define PUK_TYPE_USER		2	/* User public key*/
#define PUK_TYPE_USER0   PUK_TYPE_USER /* User public key, PUK_TYPE_USER0 is departed*/
#define PUK_TYPE_US_PUK_SE	3	/* Second user signature certificate public key */


#define TYPE_OS_VER		3	/* Operatiing system version */
#define TYPE_OSAL_VER		4	/* API Library version */
#define TYPE_DRIVER_VER		5	/* Driver version */
#define TYPE_PED_VER		6	/* Built-in PED version */
#define TYPE_MSR_VER		7	/* MSR version */
#define TYPE_ICC_VER		8	/* ICC Reader version */
#define TYPE_PCD_VER		9	/* PCD Reader version */
#define TYPE_EMVL1_VER		10	/* EMV Level1 version */
#define TYPE_PRINTER_VER	11	/* Printer version */
#define TYPE_MODEM_VER		12	/* Modem version */
#define TYPE_ETH_VER		13	/* Netcard version */
#define TYPE_GPRS_VER		14	/* GPRS version */
#define TYPE_CDMA_VER		15	/* CDMA version */
#define TYPE_TD_VER		16	/* TD version */
#define TYPE_WIFI_VER		17	/* Wi-Fi version */
#define TYPE_BT_VER		18	/* Bluetooth version */

#define RF_TYPE_RC531		1
#define RF_TYPE_PN512		2

/*--------------------------------------------------------------------------
 * Wi-Fi
 *--------------------------------------------------------------------------*/
/* pare cipher */
#define PARE_CIPHERS_NONE	0x00000000	/* No security */
#define PARE_CIPHERS_WEP64	0x00000001	/* Means 40 bit key with concatenated 24 bit initialization vector */
#define PARE_CIPHERS_WEP128	0x00000002	/* Means 104 bit key with 24 bit initialization vector */
#define PARE_CIPHERS_WEPX	0x00000004	/* Not sure of WEP key bits */
#define PARE_CIPHERS_CCMP	0x00000010	/* AES in Counter mode with CBC-MAC */
#define PARE_CIPHERS_TKIP	0x00000020	/* Temporal Key Integrity Protocol */

#define MAX_BSSID_LEN		20
#define MAX_ESSID_LEN		33
#define MAX_key_LEN		40

/*--------------------------------------------------------------------------
 * GPRS/CDMA
 *--------------------------------------------------------------------------*/
#define ERR_WL_POWER_ONING	(-3501)	/* Module powering on, please wait */
#define ERR_WL_POWER_OFF	(-3502)	/* Module powering off */
/* Has not called OsWlInit () to initialize successfully and cannot work normally */
#define ERR_WL_NOT_INIT		(-3503)
#define ERR_WL_NEEDPIN		(-3504)	/* SIM card needs PIN */
#define ERR_WL_RSPERR		(-3505)	/* Module response error */
#define ERR_WL_NORSP		(-3506)	/* Module no response */
#define ERR_WL_NEEDPUK		(-3507)	/* SIM card needs PUK */
#define ERR_WL_WRONG_PIN	(-3508)	/* PIN of SIM card error */
#define ERR_WL_NOSIM		(-3509)	/* SIM card not inserted */
#define ERR_WL_NOREG		(-3510)	/* Cannot register on the GPRS network */
#define ERR_WL_AUTO_RST		(-3511)	/* Module reset automatically */
#define ERR_WL_BUF		(-3512)	/* Module memory error */
#define ERR_WL_GET_SIGNAL	(-3513)	/* Getting the signal, please wait for 3s */
#define ERR_WL_NOTYPE		(-3514)	/* Module cannot be recognized */
#define ERR_WL_PPP_ONLINE	(-3515)	/* PPP is online, and it cannot be sleeping */
#define ERR_WL_ERR_BUSY		(-3516)	/* Module is busy */
#define ERR_WL_SLEEP_ONING	(-3517)	/* Module is in sleeping */
#define ERR_WL_SLEEP_FAIL	(-3518)	/* Sleeping failed */
#define ERR_WL_SIM_FAILURE	(-3519)	/* SIM card operation failed */

#define PPP_ALG_PAP		0x01	/* PAP authentication algorithm */
#define PPP_ALG_CHAP		0x02	/* PAP authentication algorithm */
#define PPP_ALG_MSCHAPV1	0x04	/* MSCHAPV1 authentication algorithm */
#define PPP_ALG_MSCHAPV2	0x08	/* MSCHAPV2 authentication algorithm */
#define PPP_ALG_ALL		0xFF	/* All authentication algorithm */

/*--------------------------------------------------------------------------
 * Power
 *--------------------------------------------------------------------------*/
#define BATTERY_LEVEL_0		0	/* Power  0~20% */
#define BATTERY_LEVEL_1		1	/* Power 20~40% */
#define BATTERY_LEVEL_2		2	/* Power 40~60% */
#define BATTERY_LEVEL_3		3	/* Power 60~80% */
#define BATTERY_LEVEL_4		4	/* Power 80~100% */
#define BATTERY_LEVEL_CHARGE	5	/* Battery is being charged */
#define BATTERY_LEVEL_COMPLETE	6	/* Battery charge complete */
#define BATTERY_LEVEL_ABSENT	7	/* Battery is absent */

typedef enum  {
	PM_FORBID_SLEEP,
	PM_ALLOW_SLEEP,
	PM_FORBID_SCREENSAVER,
	PM_ALLOW_SCREENSAVER,
	PM_FORBID_POWEROFF,
	PM_ALLOW_POWEROFF,
	PM_REQ_MAX
} PM_REQ_T;

typedef enum {
	PM_MSG_NO_EVENT,
	PM_MSG_ENTER_SLEEP,
	PM_MSG_EXIT_SLEEP,
	PM_MSG_ENTER_SCREENSAVER,
	PM_MSG_EXIT_SCREENSAVER,
	PM_MSG_ENTER_POWEROFF,
	PM_MSG_MAX
} PM_MSG_T;


/*--------------------------------------------------------------------------
 * USB
 *--------------------------------------------------------------------------*/
#define USB_ERR_NOT_OPEN	(-3403)	/* The channel isn't open */
#define USB_ERR_BUF		(-3404)	/* Send buffer error */
#define USB_ERR_NOT_FREE	(-3405)	/* No ports are available */
/* The device isn't enumerate and configuration process is completed */
#define USB_ERR_NO_CONF		(-3411)
/* The device is disconnected from the host */
#define USB_ERR_DISCONN		(-3412)
#define USB_ERR_MEM_SYSTEM	(-3413)	/* Memory exception */
#define USB_ERR_BUSY		(-3414)	/* USB busy */
/* Applicatioin of system resources failed */
#define USB_ERR_RC_SYSTEM	(-3415)
#define USB_ERR_DEV_ABSENT	(-3416)	/* USB device doesn't exist */
/* USB communication in an invalid state */
#define USB_ERR_INVALID		(-3417)

/*--------------------------------------------------------------------------
 * Other macros
 *--------------------------------------------------------------------------*/
#define NET_LINK_ETH		1	/* Ethernet */
#define NET_LINK_ETH1		2	/* USB Ethernet */
#define NET_LINK_WL		3	/* GPRS, CDMA, TDSCDMA */
#define NET_LINK_WIFI		4	/* WiFi */
#define NET_LINK_PPPOE		5	/* ADSL */
#define NET_LINK_MODEM		6	/* Modem PPP */
#define NET_LINK_PPPDIRECT	7	/* ppp_direct */
#define NET_LINK_TAP		8	/* tap vlan */

#define ETH_AUTO		1	/* Auto-negotiaton mode */
#define ETH_10MHD		2	/* 10M half-duplex mode */
#define ETH_10MFD		3	/* 10M full-duplex mode */
#define ETH_100MHD		4	/* 100M half-duplex mode */
#define ETH_100MFD		5	/* 100M full-duplex mode */

#define NET_DOING		1

#define PPP_LOGINING		1
#define PPP_LOGOUTING		2

#define POWER_OFF		0	/* Power off */
#define POWER_ON		1	/* Power on */
#define SLEEP_IN		1	/* Sleep */
#define SLEEP_OUT		0	/* Wake up */

#define CMD_LCPTIME		0
#define CMD_IPCPTIME		1
#define CMD_PAPCHAPTIME		2

#define SEL_SIM0		0
#define SEL_SIM1		1

/* Logical port defines for serial port */
#define PORT_COM1		0
#define PORT_COM2		1
#define PORT_COM3		2
#define PORT_PINPAD		3	/* External PinPad */
#define PORT_MODEM		4	/* Modem module */
#define PORT_WIFI		5	/* WiFi module */
#define PORT_RS485		6	/* RS485 */
#define PORT_RF24G		7	/* RF 2.4G reader module */
#define PORT_WL			8	/* Wireless, inc. GPRS, CDMA, etc. */
#define PORT_BT			9	/* Bluetooth */
#define PORT_USBDEV		11	/* USB port device */
#define PORT_USBHOST		12	/* USB port host */

#define PORT_BASE_DEV		15	/* base usb dev */
#define PORT_BASE_HOST		16	/* base usb host */
#define PORT_BASE_A			17	/* base uart a */
#define PORT_BASE_B			18	/* base uart b */


/* System message defines for magnetic and IC cards */
#define SIGMAG			50
#define SIGICC			51

/* Key values */
#define KEY1			2	/* 1 */
#define KEY2			3	/* 2 */
#define KEY3			4	/* 3 */
#define KEY4			5	/* 4 */
#define KEY5			6	/* 5 */
#define KEY6			7	/* 6 */
#define KEY7			8	/* 7 */
#define KEY8			9	/* 8 */
#define KEY9			10	/* 9 */
#define KEY0			11	/* 0 */

#define KEYCANCEL		223	/* Cancel */
#define KEYCLEAR		14	/* Clear */
#define KEYENTER		28	/* Enter */
#define KEYALPHA		69	/* Alpha */

#define KEYF1			59
#define KEYF2			60
#define KEYF3			61
#define KEYF4			62

#define KEYFUNC			102
#define KEYUP			103
#define KEYDOWN			108

#define KEYPOWER		116	/* Power key in D200 */

#define KEYMENU			139

/* Font style */
#define FONT_STYLE_NONE		0x00000000
#define FONT_STYLE_BOLD		0x00000001
#define FONT_STYLE_ITALIC	0x00000002

/*--------------------------------------------------------------------------
 * GPS 
 *--------------------------------------------------------------------------*/
#define GPS_NAVIGATING		1

#define GPS_MODE        1  //normal GPS
#define AGPS_MODE       2  //AGPS

/*--------------------------------------------------------------------------
 * Data structures
 *--------------------------------------------------------------------------*/
typedef struct {
	int ModulusLen;			/* Module length (bits) */
	unsigned char Modulus[512];	/* Module; if the length of modulus
					   <512 bytes, store from right,
					   add 0x00 on the left */
	int ExponentLen;		/* Exponent Length (bits) */
	unsigned char Exponent[512];	/* When exponent <512 bytes，add 0x00
					   on the left */
	unsigned char KeyInfo[128];	/* RSA key information */
} ST_RSA_KEY;

typedef struct {
	int ModulusLen;			/* Module length (bits) */
	unsigned char Modulus[256];	/* Module of PIN public key */
	unsigned char Exponent[4];	/* Exponent of PIN public key */
	int IccRandomLen;		/* Length of random data from IC card */
	unsigned char IccRandom[8];	/* Random data from IC card */
} ST_RSA_PINKEY;

typedef struct {
	uchar ucKeyVarMode;		/* varinfo mode */
	uchar aucKeyVarIn[128];		/* varinfo of the key */
} ST_KEY_VAR_INFO;

typedef struct {
	char FileName[64];		/* Font file name */
	char FontName[64];		/* Font name */
} FT_FONT;

typedef struct {
	unsigned char Left;		/* Font offset left from the baseline */
	unsigned char Top;		/* Font top offset from the baseline */
	unsigned char Width;		/* Font width */
	unsigned char Height;		/* Font height */
	unsigned char Dot[3072];	/* Valid font data */
} FT_DOT;

typedef struct {
	unsigned char TrackData[256];	/* Track data buffer */
	int DataLen;			/* Track data length */
	/* Track data status, status of 0 indicate read track data successed,
	   other value indicate failed */
	int Status;
} ST_MSR_DATA;

/* IC card device control block */
typedef struct sci_dcb_t {
	unsigned int voltage;		/* operation condition: voltage */

	/* frequency adjust integer, default value is 372 */
	unsigned int fi;
	/* speed adjust integer, default value is 1 */
	unsigned int di;

	unsigned int conv;		/* logical converse direction */
	unsigned int protocol;		/* T=0 or T=1 */

	unsigned char option_clock;		/* stop clock options */
	unsigned char option_voltage;		/* voltage options */
	unsigned char option_spu;

	/* these members are appended, you must notice */
	unsigned int spec;
	unsigned int nego;			/* support PPS protocol */

	/* the guard time between characters, only for T=0 */
	unsigned int cgt;
	/* block guard time, default value is 22, only for T=1 */
	unsigned int bgt;

	/* RESET signal maintain LOW level clock cycles, default is 42500 */
	unsigned int rstt;

	unsigned int wtt;	/* TS wait time, default value is ??? */

	/* allowed maximum of ATR duration, only used in EMV mode (spec=0) */
	unsigned int twt;
	unsigned int wwt;	/* T=0, work wait time, default is 0x0A */

	/* character repetition (in T=0), maximum is 6 */
	unsigned int tpar_retry;	/* send repeat time on parity error */
	unsigned int rpar_retry;	/* recv repeat time on parity error */

	unsigned int bwt;	/* T=1, block wait time, default is 0x00 */
	unsigned int cwt;	/* T=1, character wait time, default is 0x05 */

	unsigned char repeat;

	/* the maxium frame size of ICC, default value is 32 */
	unsigned int fsc;
	unsigned int fsd;

	unsigned char sci_last_ipcb;
	unsigned char icc_last_ipcb;

	unsigned char sci_last_pcb;
	unsigned char icc_last_pcb;

	/* reset status: cold reset, warm reset, or activation */
	unsigned int status;
} ST_SCI_DCB;

/* IC card ATR */
typedef struct sci_atr_t {
	unsigned char ts;
	unsigned char t0;
	unsigned char ta_flag;
	unsigned char tb_flag;
	unsigned char tc_flag;
	unsigned char td_flag;
	unsigned char ta[8];
	unsigned char tb[8];
	unsigned char tc[8];
	unsigned char td[8];
	unsigned char hbytes[15];
	unsigned char tck;
} ST_SCI_ATR;

/* IC card request APDU */
typedef struct {
	unsigned char Cmd[4];
	int LC;
	unsigned char DataIn[512];
	int LE;
} ST_APDU_REQ;

/* IC card response APDU */
typedef struct {
	int LenOut;
	unsigned char DataOut[512];
	unsigned char SWA;
	unsigned char SWB;
} ST_APDU_RSP;

struct PCD_PARAM_ST {
	unsigned int uiProtocolCheckEn;
	unsigned int uiFSC;
	unsigned int uiFWT;
	unsigned int uiSFGT;

	unsigned int uiTypeAConduct;
	unsigned int uiReserved;
	unsigned int uiTypeARxThreshold;
	unsigned int uiTypeAGain;

	unsigned int uiTypeBConduct;
	unsigned int uiTypeBModulDepth;
	unsigned int uiTypeBRxThreshold;
	unsigned int uiTypeBGain;

	unsigned int uiFelicaConduct;
	unsigned int uiFelicaModulDepth;
	unsigned int uiFelicaRxThreshold;
	unsigned int uiFelicaGain;

	unsigned int uiRFU[60];
};

typedef struct pcd_user_t {
	unsigned char wait_retry_limit_w;
	unsigned int wait_retry_limit_val;
	unsigned char check_cancel_key_w;
	unsigned char check_cancel_key_val;
	int (*check_cancel_key_function)(void);
	unsigned char os_picc_transfer_set_w; /* Only in OsPiccTransfer takes effect*/
	unsigned char os_picc_transfer_set_val;/* Only in OsPiccTransfer takes effect*/
	unsigned char anti_interference_flag;
	unsigned char reserved[55];
} PCD_USER_ST;

typedef struct {
	int CallMode;
	int CommMode;
	int CodeType;
	int CodeDuration;
	int CodeSpacing;
	int DetectLineVoltage;
	int DetectDialTone;
	int DialToneTimeout;
	int CommaPauseTime;
	char ConnectRate[20];
	char ConnectFormat[20];
	int ConnectTimeout;
	int DialTimes;
	int IdleTimeout;
	int Pppom;
	int Reserved[9];
} ST_MODEM_SETUP;

typedef struct {
	char Id[64];
	char Name[64];
	char Bin[64];
	char Artwork[64];
	char Desc[64];
	char Vender[32];
	char Version[32];
} ST_APP_INFO;

typedef struct {
	char Name[64];
	char Version[32];
} ST_OPT_INFO;

typedef struct {
	int Year;
	int Month;
	int Day;
	int Hour;
	int Minute;
	int Second;
	int DayOfWeek;
} ST_TIME;

typedef struct {
	unsigned long Start;
	unsigned long Stop;
	unsigned long TimeLeft;
} ST_TIMER;

typedef enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR
} LOG_T;

enum WIFI_AUTH_MODE {
	AUTH_NONE_OPEN = 1,
	AUTH_NONE_WEP,
	AUTH_NONE_WEP_SHARED,
	AUTH_IEEE8021X,
	AUTH_WPA_PSK,
	AUTH_WPA_EAP,
	AUTH_WPA_WPA2_PSK,
	AUTH_WPA_WPA2_EAP,
	AUTH_WPA2_PSK,
	AUTH_WPA2_EAP
};

typedef enum{
	WPS_MODE_PBC = 1,		/*WPS config by pushing button */
	WPS_MODE_PIN_CLIENT,		/*WPS config by client's pin */
	WPS_MODE_PIN_AP			/*WPS config by AP's pin */
}WPS_MODE;

enum WIFI_EAP_MODE {
	EAP_MD5 = 1,
	EAP_TLS,
	EAP_PEAP,
	EAP_TTLS
};

typedef struct _WepSecKey {
	char Key[4][40];	/* WEP key data */
	int KeyLen;		/* WEP key data length */
	int Idx;		/* WEP key index [0..3] of actually used key */
} WEP_SEC_KEY;

typedef struct _WpaPskKey {
	char Key[64];		/* PSK-Key data */
	int KeyLen;		/* PSK-Key data length */
} WPA_PSK_KEY;

typedef struct _WpaEapKey {
	int EapType;		/* EAP type */
	char Pwd[132];		/* Password */
	char Id[132];		/* Identity */
	char CaCert[132];	/* Path and filename of CA certificate */
	char CliCert[132];	/* Path and filename of client certificate */
	char PriKey[132];	/* File path to client private key file */
	char PriKeyPwd[132];	/* Password for private key file */
} WPA_EAP_KEY;

typedef struct _WifiApInfo {
	char Essid[33];		/* AP name */
	char Bssid[20];		/* MAC address */
	int Channel;		/* Channel */
	int Mode;		/* Connection mode, 0:Station; 1:IBSS */
	int Rssi;		/* Signal value */
	int AuthMode;		/* Authentication modes */
	int SecMode;		/* Encryption mode, NONE,WEP,TKIP,CCMP */
} ST_WifiApInfo;

typedef struct _WifiApSet {
	char Essid[33];	/* AP name, it can support 32 bytes at most, and ending with '\0' */
	char Bssid[20];	/* MAC address, if there is no any APs with the same ESSID, Bssid can be "\0" */
	int Channel;	/* Channel, 0:Auto set */
	int Mode;	/* Connection mode, 0:Station; 1:IBSS */
	int AuthMode;	/* Authentication modes */
	int SecMode;	/* Encryption mode, NONE,WEP,TKIP,CCMP */
	union KEY_UNION {		/* Key */
		WEP_SEC_KEY WepKey;	/* For wep */
		WPA_PSK_KEY PskKey;	/* For wpa,wpa2-psk authentication */
		WPA_EAP_KEY EapKey;	/* For wpa,wpa2-eap */
	} KeyUnion;
} ST_WifiApSet;

#define WPS_PIN_LEN			8

typedef enum {
	POWER_ADAPTER = 1,
	POWER_USB,
	POWER_BATTERY,
	POWER_WPC
} POWER_TYPE;

#define MAX_SITE_ADDR_SIZE 4
#define MAX_GLOBAL_ADDR_SIZE 8
#define MAX_DNS_ADDR_SIZE 2

struct IPv6Info {
    char  LinkAddr[64];// Link local address
    int   NumSiteAddr;
    char  SiteAddr[MAX_SITE_ADDR_SIZE][64];// Site local address
    int   NumGlobalAddr;
    char  GlobalAddr[MAX_GLOBAL_ADDR_SIZE][64];// Global address
    int   NumDns;
    char  Dns[MAX_DNS_ADDR_SIZE][64]; //DNS server address
};

struct IPv6RouteTable
{
	int RouteNum;
	struct {
		char DestAddr[64]; 
		int DestAddrPrefixLen ;
		char NextHop[64]; 
	} RouteTable[16];
};

struct IPv4RouteTable{
	int RouteNum;/* number of Route*/
	struct{
		char DestAddr[16];/* dest addr*/
		char GenMask[16]; /* gen mask */
		char GateWay[16]; /* Gateway*/
	}RouteTable4[16];
};

typedef struct {
	double       latitude;
	double       longitude;
	double       altitude;
} GPS_LOCATION;

struct DnsAddrInfo{
    int   NumIPv4Addr ;/*numbers of ipv4 addr*/
    char  IPv4Addr[4][16];/*ipv4 addr*/
    int   NumIPv6Addr ; /*numbers of ipv6 addr*/
    char  IPv6Addr[4][64];/*ipv6 addr*/
};

/*==========================================================================
 * APIs
 *==========================================================================*/

/*--------------------------------------------------------------------------
 * Systerm Manager
 *--------------------------------------------------------------------------*/
typedef void (*RUNAPP_CB)(char *appid, char *str, void *data);
int OsRunApp(char *AppId, char **Argv, void *Data, RUNAPP_CB CbOut,
    RUNAPP_CB CbErr);
int OsRegGetValue(const char *Key, char *Value);
int OsRegSetValue(const char *Key, const char *Value);
void OsGetSysVer(int VerType, char *Version);
int OsGetAppInfo(ST_APP_INFO AppInfo[], int InfoCnt);
int OsGetOptInfo(ST_OPT_INFO OptInfo[], int InfoCnt);
int OsInstallFile(const char *Name, const char *FileName, int FileType);
int OsFirmwareGetVersion(char *Version, int Size);
int OsUninstallFile(const char *AppName, int FileType);
int OsFirmwareUpgrade(const char *FwFileName);
int OsVerifySign(const char *FileName, int PUKType);
int OsVerifySignExternal(const char *FileName, const void *SignData,
    int PUKType);
void OsLogSetTag(const char *Tag);
int OsLog(LOG_T Prio, const char *fmt, ...);
int OsSetTime(const ST_TIME *Time);
void OsGetTime(ST_TIME *Time);
unsigned long OsGetTickCount(void);
int OsTimerSet(ST_TIMER *Timer, unsigned long Ms);
unsigned long OsTimerCheck(ST_TIMER *Timer);
void OsBeep(int ToneType, int DurationMs);
void OsSleep(unsigned int Ms);
int OsMount(const char *Source, const char *Target, const char *FileSystemType,
    unsigned long MountFlags, const void *Data);
int OsUmount(const char *Target, int Flags);
int OsCryptFormat(const char *Dev, const char *Pwd, const char *FsType);
int OsCryptMount(const char *Dev, const char *Pwd, const char *Target, const char *FsType);
int OsCryptUmount(const char *Target);
int OsOnBase(void);
int OsBaseOpen(void);
int OsBaseCheck(void);
int OsBaseClose(void);


/*--------------------------------------------------------------------------
 * Font
 *--------------------------------------------------------------------------*/
int OsEnumFont(FT_FONT **FontList);
int OsOpenFont(const char *FileName);
int OsGetFontDot(int Handle, const char *Utf8Code, const int Width,
    const int Height, const int Style, FT_DOT *FtDot);
void OsCloseFont(int Handle);

/*--------------------------------------------------------------------------
 * Code
 *--------------------------------------------------------------------------*/
int OsCodeConvert(const char *FromCharset, const char *ToCharset,
    const char *InBuf, char *OutBuf, unsigned int LenOut);

/*--------------------------------------------------------------------------
 * Uart
 *--------------------------------------------------------------------------*/
int OsPortOpen(int channel, const char *attr);
void OsPortClose(int channel);
int OsPortSend(int channel, const void *sendBuf, unsigned int sendLen);
int OsPortRecv(int channel, void *recvBuf, unsigned int recvLen,
    unsigned int timeoutMs);
int OsPortReset(int Channel);
int OsPortCheckTx(int Channel);

/*--------------------------------------------------------------------------
 * Printer
 *--------------------------------------------------------------------------*/
int OsPrnOpen(unsigned int printertype, const char *targetname);
int OsPrnSetFont(const char *fontname);
int OsPrnSetSize(unsigned int Width, unsigned int Height);
int OsPrnSetDirection(unsigned char Mode);
int OsPrnSetIndent(unsigned int Left, unsigned int Right);
void OsPrnSetSpace(int CharSpace, int LineSpace);
void OsPrnSetGray(int Level);
void OsPrnSelectFontSize(int SingleCodeWidth, int SingleCodeHeight,
    int MultiCodeWidth, int MultiCodeHeight);
void OsPrnSetReversal(int Attr);
void OsPrnFeed(int Pixel);
void OsPrnPrintf(const char *Str, ...);
void OsPrnPutImage(const unsigned char *Logo);
int OsPrnStart(void);
int OsPrnCheck(void);
int OsPrnGetDotLine(void);
void OsPrnReset(void);
void OsPrnClose(void);
void OsPrnClrBuf(void);
int OsPrnSetParam(unsigned int cmd);

/*--------------------------------------------------------------------------
 * Wavplay
 *--------------------------------------------------------------------------*/
int OsPlayWave(const char *Buf, int Len, int Volume, int DurationMs);

/*--------------------------------------------------------------------------
 * Keyboard
 *--------------------------------------------------------------------------*/
void OsKbBacklight(int OnOff);

/*--------------------------------------------------------------------------
 * LCD
 *--------------------------------------------------------------------------*/
void OsScrBrightness(int Brightness);
void OsScrGetSize(int *Width, int *Height);

/*--------------------------------------------------------------------------
 * Ethernet and wireless
 *--------------------------------------------------------------------------*/
int OsNetPing(const char *Addr, int TimeOutMs);
int OsNetPingEx(const char *Addr, int TimeOutMs, int size);
int OsNetPing6(const char *ScrAddr, const char *DestAddr,int TimeOutMs, int size);
int OsNetDns(const char *Name, char *Addr, int TimeOutMs);
int OsNetDnsEx(const char *Name, struct DnsAddrInfo *Addr, int TimeOutMs);
int OsNetSetRoute(int NetLink);
int OsNetGetRoute(void);
int OsNetSetConfig(int NetLink, const char *Addr, const char *Mask,
    const char *Gateway, const char *DNServer);
int OsNetGetConfig(int NetLink, char *Addr, char *Mask, char *Gateway,
    char *DNServer);
int OsNetStartDhcp(int NetLink);
int OsNetStartDhcp6(int NetLink,int state);
int OsNetCheckDhcp6(int NetLink);
int OsNetStopDhcp6(int NetLink);
int OsNetGetRouteAdvertise6(int NetLink,int TimeOutMs,int *RouteMode);
int OsNetSetDns(const char *dns);
int OsNetGetIPv6Addr(int NetLink,struct IPv6Info *AddrInfo);
int OsNetSetIPv6Addr(int NetLink,const char *addr,int PrefixLength);
int OsNetSetRouteTable6(int NetLink, const char *DestAddr,int PrefixLength,const char *NextHop);
int OsNetGetRouteTable6(int NetLink,struct IPv6RouteTable *AddrInfo);
int OsNetSetRouteTable(int NetLink, const char *DestAddr, const char *GenMask, const char *GateWay);
int OsNetDelRouteTable(int NetLink, const char *DestAddr,  const char *GenMask);
int OsNetGetRouteTable(int NetLink, struct IPv4RouteTable *RouteTable);

int OsNetCheckDhcp(int NetLink);
void OsNetStopDhcp(int NetLink);
int OsNetAddArp(int NetLink, const char *Addr, const char MAC[6]);
int OsPppoeLogin(const char *Name, const char *Password, int TimeOutMs);
int OsPppoeCheck(void);
void OsPppoeLogout(void);

int OsWlLock(void);
void OsWlUnLock(void);
int OsWlInit(const char *SimPin);
int OsWlGetSignal(void);
int OsWlSwitchPower(int OnOff);
int OsWlLogin(const char *APN, const char *Name, const char *Password,
    long Auth, int TimeOutMs, int KeepAlive, int ReserParam);
int OsWlLoginEx(const char *DialNum, const char *APN, const char *Name,
    const char *Password, long Auth, int TimeOutMs, int KeepAlive,
    int ReserParam);
int OsWlCheck(void);
int OsWlLogout(void);
int OsWlSwitchSleep(int OnOff);
int OsWlSelSim(int simno);

/*--------------------------------------------------------------------------
 * Crypt
 *--------------------------------------------------------------------------*/
void OsGetRandom(uchar *Random, int RandomLen);
void OsSHA(int Mode, const void *Data, int DataLen, uchar *ShaOut);
void OsDES(const uchar *Input, uchar *Output, const uchar *DesKey, int KeyLen,
    int Mode);
void OsAES(const uchar *Input, uchar *Output, const uchar *AesKey, int KeyLen,
    int Mode);
int OsRSA(const uchar *Modulus, int ModulusLen, const uchar *Exp, int ExpLen,
    const uchar *DataIn, uchar *DataOut);
int OsRSAKeyGen(uchar *Modulus, uchar *PriExp, int ModulusLen,
    const uchar *PubExp);

/*--------------------------------------------------------------------------
 * PED
 *--------------------------------------------------------------------------*/
int OsPedOpen(void);
void OsPedClose(void);
int OsPedGetVer(uchar *PedVer);
int OsPedSetInterval(ulong TpkIntervalMs);
int OsPedVerifyPlainPin(int IccSlot, const char *ExpPinLen, int Mode,
    ulong TimeoutMs, uchar *IccRsp);
int OsPedVerifyCipherPin(int IccSlot, const ST_RSA_PINKEY *RsaPinKey,
    const char *ExpPinLen, int Mode, ulong TimeoutMs, uchar *IccRsp);
int OsPedEraseKeys(void);
int OsPedEraseKey(int KeyType, int KeyIdx);
int OsPedSetFunctionKey(int KeyFlag);
int OsPedCancelPinEntry(void);
int OsPedWriteKey(const uchar *KeyBlock);
int OsPedWriteKeyEx(const uchar *KeyBlock, ST_KEY_VAR_INFO *KeyVarInfo);
int OsPedWriteTIK(const uchar *KeyBlock);
int OsPedWriteKeyVar(int KeyType, int SrcKeyIdx, int DstKeyIdx,
    const char *KeyVar);
int OsPedGetPinBlock(int KeyIdx, const uchar *DataIn, const char *ExpPinLen,
    int Mode, ulong TimeoutMs, uchar *PinBlock);
int OsPedUpdatePinBlock(int UpdataFlag, const uchar *KeyInfo,
    const uchar *DataIn, uchar *PinBlock, int Mode);
int OsPedGetMac(int KeyIdx, const uchar *DataIn, int DataInLen, uchar *Mac,
    int Mode);
int OsPedDes(int KeyIdx, uchar *InitVector, const uchar *DataIn, int DataInLen,
    uchar *DataOut, int Mode);
int OsPedGetKcv(int KeyType, int KeyIdx, int KcvMode, int KcvDataLen,
    uchar *KcvData, uchar *Kcv);
int OsPedDeriveKey(int SrcKeyType, int SrcKeyIdx, int DstKeyType,
    int DstFromKeyIdx, int DstToKeyIdx, int Mode);
int OsPedGetPinDukpt(int GroupIdx, const uchar *DataIn, const char *ExpPinLen,
    int Mode, ulong TimeoutMs, uchar *Ksn, uchar *PinBlock);
int OsPedGetMacDukpt(int GroupIdx, const uchar *DataIn, int DataInLen,
    uchar *Mac, uchar *Ksn, int Mode);
int OsPedDesDukpt(int GroupIdx, int KeyVarType, uchar *InitVector,
    int DataInLen, const uchar *DataIn, uchar *DataOut, uchar *Ksn, int Mode);
int OsPedGetKsnDukpt(int GroupIdx, uchar *Ksn);
int OsPedIncreaseKsnDukpt(int GroupIdx);
int OsPedReadRsaKey(int RsaKeyIdx, ST_RSA_KEY *RsaKey);
int OsPedWriteRsaKey(int RsaKeyIdx, ST_RSA_KEY *RsaKey);
int OsPedRsaRecover(int RsaKeyIdx, int DataInLen, uchar *DataIn, uchar *DataOut,
    uchar *KeyInfo);
int OsPedReadCipherRsaKey(int RsaKeyIdx, uchar *CipherRsaKey);
int OsPedWriteCipherRsaKey(int RsaKeyIdx, int CipherRsaKeyLen,
    uchar *CipherRsaKey);
int OsPedMsrKeyEncDec(int KeyType, uchar *InitVector, int DataInLen,
    uchar *DataIn, uchar *DataOut, int Mode, uchar *MsrKeyKsn);
int OsPedAdminLogon(ulong TimeOutMs);
int OsPedAdminModifyPwd(void);
int OsPedAdminInjectTLK(void);
int OsPedAdminOutputLog(uchar *OutputLog);
void OsPedAdminUartDownload(void);
int OsPedSetAsteriskLayout(int x, int y, int fontSize, int fontColor,
    uchar align);
int OsPedSetPinIconLayout(int X, int Y, int Interval, uchar IconNum,
	char *PinIcon, int PinIconLen, char *PinIconBG, int PinIconBGLen);
int OsPedWriteAesKey(const uchar *KeyBlock);	/* AES function */
int OsPedAes(int KeyIdx, unsigned char *InitVector,
    const unsigned char *DataIn, int DataInLen, unsigned char *DataOut,
    int Mode);
int OsPedGenSM2Pair(uchar *PvtKey, uchar *PubKey, int KeyLenBit);
int OsPedWriteSM2Key(int KeyIdx, int KeyType, uchar *KeyValue);
int OsPedSM2Sign(int PubKeyIdx, int PvtKeyIdx,
        uchar *Uid, int UidLen,
        uchar *Input, int InputLen,
        uchar *Signature);
int OsPedSM2Verify(int PubKeyIdx,
        uchar *Uid, int UidLen,
        uchar *Input, int InputLen,
        const uchar *Signature);
int OsPedSM2Recover(int KeyIdx, uchar *Input, int InputLen,
        uchar *Output, int *OutputLen, int Mode);
int OsPedSM3(uchar *Input, int InputLen, uchar *Output, int Mode);
int OsPedSM4(int KeyIdx, uchar *InitVector, uchar *Input, int InputLen, uchar *Output, int Mode);
int OsPedGetMacSM(int KeyIdx, uchar *InitVector, uchar *Input, int InputLen,
        uchar *MacOut, int Mode);
int OsPedGetPinBlockSM4(int KeyIdx, const char *ExpPinLenIn, uchar *DataIn,
        uchar *PinBlockOut, int Mode, ulong TimeoutMs);

/*--------------------------------------------------------------------------
 * Magnetic card
 *--------------------------------------------------------------------------*/
int OsMsrOpen(void);
void OsMsrClose(void);
int OsMsrSwiped(void);
int OsMsrRead(ST_MSR_DATA *Track1, ST_MSR_DATA *Track2, ST_MSR_DATA *Track3);
int OsMsrReadEx(ST_MSR_DATA *Track1, ST_MSR_DATA *Track2, ST_MSR_DATA *Track3);
void OsMsrReset(void);

/*--------------------------------------------------------------------------
 * IC card
 *--------------------------------------------------------------------------*/
int sci_lock(int id);
int sci_unlock(int id);
int sci_open(int id);
int sci_get_fd(int id);
int sci_get_dcb(int id, struct sci_dcb_t *dcb);
int sci_set_dcb(int id, struct sci_dcb_t *dcb);
int sci_read(int id, unsigned char *pbuf, int length);
int sci_write(int id, unsigned char *pbuf, int length);
int sci_close(int id);
int sci_powerup(int dev_id);
int sci_cold_reset(int id, struct sci_atr_t *pstATR);
int sci_warm_reset(int id, struct sci_atr_t *pstATR);
int sci_powerdown(int id);
int sci_detect(int id);
int iso7816_t0_exchange(int id, ST_APDU_REQ *apdu_req, ST_APDU_RSP *apdu_resp);
int iso7816_t1_exchange(int id, ST_APDU_REQ *apdu_req, ST_APDU_RSP *apdu_resp);
int iso7816_t1_ifsd_request(int id);
int iso7816_ocs(int id, struct sci_atr_t *pstATR);
int iso7816_pps(int, struct sci_atr_t *pstATR, struct sci_dcb_t *dcb);
int emv_atr_parse(const struct sci_atr_t *pstATR, struct sci_dcb_t *dcb);
int iso7816_atr_parse(const struct sci_atr_t *pstATR, struct sci_dcb_t *dcb);

int OsIccOpen(int Slot);
int OsIccDetect(int Slot);
int OsIccInit(int Slot, unsigned long Option, unsigned char *Atr);
int OsIccExchange(int Slot, int CtrlFlag, ST_APDU_REQ *ApduReq,
    ST_APDU_RSP *ApduRsp);
int OsIccClose(int Slot);
int OsIccGetFd(int Slot);


/*--------------------------------------------------------------------------
 * RF card
 *--------------------------------------------------------------------------*/
int OsPiccOpen(void);
int OsPiccResetCarrier(void);
int OsPiccPoll(char *pcPiccType, unsigned char *pucATQx);

int OsPiccAntiSel(const char cPiccType, unsigned char *pucUID,
    const unsigned char ucATQ0 __attribute__((unused)), unsigned char *pucSAK);
int OsPiccActive(const char cPiccType, unsigned char *pucATS);
int OsPiccClose(void);
int OsPiccInitFelica(unsigned char ucSpeed, unsigned char ucModInvert);
int OsPiccRemove(void);
int OsPiccTransfer(const unsigned char *pucTxBuff, int iTxLen,
    unsigned char *pucRxBuff, int *piRxLen);

int OsMifareAuthority(const unsigned char *uid, unsigned char blk_no,
    unsigned char group, const unsigned char *psw);
int OsMifareOperate(unsigned char ucOpCode, unsigned char ucSrcBlkNo,
    unsigned char *pucVal, unsigned char ucDesBlkNo);
int OsPiccSetParam(struct PCD_PARAM_ST *pstParam);
int OsPiccGetParam(struct PCD_PARAM_ST *pstParam);

int OsPiccIsoCommand(int cid __attribute__((unused)), ST_APDU_REQ *ApduReq,
    ST_APDU_RSP *ApduRsp);

int OsPiccSetUserConfig(PCD_USER_ST *pcd_user_config);
int OsPiccGetUserConfig(PCD_USER_ST *pcd_user_config);

/*--------------------------------------------------------------------------
 * Modem
 *--------------------------------------------------------------------------*/
int OsModemSwitchPower(int OnOff);
int OsModemSetPppomStatus(int RunStatus);
int OsModemOpen(void);
int OsModemConnect(const ST_MODEM_SETUP *Setup, const char *TelNo);
int OsModemCheck(void);
int OsModemSend(const void *SendBuf, int SendLen);
int OsModemRecv(void *RecvBuf, int BufSize, int Timeout);
int OsModemExCmd(const char *Cmd, char *Rsp, int *RespLen, int TimeoutMs);
void OsModemHangup(void);
int OsModemGetUart(void);
void OsModemClose(void);

/*--------------------------------------------------------------------------
 * Modem PPP
 *--------------------------------------------------------------------------*/
int OsPppomLogin(const char *Name, const char *Password, long Auth,
    int TimeOutMs);
int OsPppomCheck(void);
int OsPppomLogout(void);

/*--------------------------------------------------------------------------
 * Wi-Fi
 *--------------------------------------------------------------------------*/
int OsWifiOpen(void);
void OsWifiClose(void);
int OsWifiSwitchPower(int Type);
int OsWifiScan(ST_WifiApInfo **Aps);
int OsWifiConnect(const ST_WifiApSet *Ap, int TimeOutMs);
int OsWifiDisconnect(void);
int OsWifiCheck(char *Essid, char *Bssid, int *Rssi);
int OsWifiCmd(const char *Argv[], int Argc, char *Result, int Len);
int OsWifiWpsConnect(WPS_MODE Mode,char *WpsPin);

/*--------------------------------------------------------------------------
 * SLIP
 *--------------------------------------------------------------------------*/
int OsSlipAttachEx(const char *TtyName, const char *Speed,
    const char *LocalIp, const char *PeerIp);
int OsSlipDetachEx(const char *TtyName);

/*--------------------------------------------------------------------------
 * PPP_DIRECT
 *--------------------------------------------------------------------------*/
int OsPppDirectLogin(const char *TtyName, const char *Attr, const char *Name,
    const char *Password, long Auth, int TimeOutMs, int Platform);
int OsPppDirectCheck(void);
int OsPppDirectLogout(void);

/*--------------------------------------------------------------------------
 * Power
 *--------------------------------------------------------------------------*/
int OsCheckBattery(void);
int OsSysSleep(void);
int OsSysSleepEx(int Level);
int OsSysSleepTime(int Time);
int OsCheckPowerSupply(void);
int OsReboot(void);
int OsPowerOff(void);
int OsPmRequest (PM_REQ_T ReqType);
PM_MSG_T OsPmGetEvent (int TimeoutMs);

/*--------------------------------------------------------------------------
 * Crash reporting
 *--------------------------------------------------------------------------*/
void OsSaveCrashReport(int signal);

/*--------------------------------------------------------------------------
 * Barcode
 *--------------------------------------------------------------------------*/
int OsScanOpen(void);
int OsScanRead(char *Buf, int Len, int TimeoutMs);
void OsScanClose(void);

/*--------------------------------------------------------------------------
 * GPS 
 *--------------------------------------------------------------------------*/
int OsGpsOpen(int Mode, const char *Attr);
int OsGpsRead (GPS_LOCATION  *Location);
void OsGpsClose(void);

#ifdef __cplusplus
}
#endif

#endif /* _OSAL_H_ */
