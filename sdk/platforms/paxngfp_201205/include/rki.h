#ifndef _RKI_H
#define _RKI_H

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef ulong
#define	ulong unsigned long
#endif

#ifndef uint
#define	uint unsigned int
#endif

#ifndef ushort
#define	ushort unsigned short
#endif

#define MAX_RSA_MODULUS_BITS 4096
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS + 7) / 8)
#define MAX_RSA_PRIME_BITS ((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN ((MAX_RSA_PRIME_BITS + 7) / 8)
#define MAX_RSA_EXPONENT_LEN MAX_RSA_MODULUS_LEN


#define PEDRKI_DEVAUTH_KEY_IDX 	4
#define PEDRKI_VAK_PUK_IDX 		1
#define PEDRKI_AAK_PUK_IDX 		2
#define PEDRKI_DME_PUK_IDX 		3
#define PEDRKI_DEVENC_KEY_IDX	5
#define PEDRKI_RKIAK_PUK_IDX 	6

#define ERR_RKI_START 			-380
#define ERR_RKI_HASH_CMP 		(ERR_RKI_START - 1)
#define ERR_RKI_DEV_AUTH_FAIL 	(ERR_RKI_START - 2)
#define ERR_RKI_GEN_ATH_FAIL 	(ERR_RKI_START - 3)
#define ERR_RKI_SIG_ERR 		(ERR_RKI_START - 4)
#define ERR_RKI_DEV_KEY_ERR 	(ERR_RKI_START - 5)
#define ERR_RKI_NOT_AUTH		(ERR_RKI_START - 6)
#define ERR_RKI_TLV_UNPACK		(ERR_RKI_START - 7)
#define ERR_RKI_NO_SN			(ERR_RKI_START - 8)
#define ERR_RKI_PCKS1_PACK		(ERR_RKI_START - 9)

#define X509_RET_OK 			0
#define ERR_RKI_X509_START		-400
#define ERR_RKI_X509_ERROR  	(ERR_RKI_X509_START - 1)
#define ERR_RKI_X509_SIGNATURE  (ERR_RKI_X509_START - 2)
#define ERR_RKI_X509_UNKNOW_ANS1	(ERR_RKI_X509_START - 3)
#define ERR_RKI_X509_CERT_REVOKED	(ERR_RKI_X509_START - 4)
#define ERR_RKI_ISSUERNAME_ERR 		(ERR_RKI_X509_START-5)
#define ERR_RKI_SUBNAME_ERR 		(ERR_RKI_X509_START-6)
#define ERR_RKI_VALIDITY_DATE_ERR 	(ERR_RKI_X509_START-7)

enum
{
	/* Device Authentication Public Key */
	PEDRKI_TAG_DA_PUK=0x01,
	/* Device Authentication Private Key */
	PEDRKI_TAG_DA_PVK=0x02,
	/* Vendor Authentication Public Key */
	PEDRKI_TAG_VAK_PUK=0x03,
	/* Vendor Authentication Private Key */
	PEDRKI_TAG_VSK_PUK=0x04,
	/* Acquirer Authentication Public Key */
	PEDRKI_TAG_AAK_PUK=0x05,
	/* Device Message Encryption key Protect Public Key */
	PEDRKI_TAG_DME_PUK=0x06,
	/* Host Signature Token */
	PEDRKI_TAG_AT_H=0x07,
	/* Device Authentication Token */
	PEDRKI_TAG_AT_D=0x08,
	/* Temporary transfer Key */
	PEDRKI_TAG_TK=0x09,
	/* Terminal Master Key */
	PEDRKI_TAG_TMK=0x0a,
	/* SHA1 Checksum */
	PEDRKI_TAG_SHA1=0x0b,
	/* SHA256 Checksum */
	PEDRKI_TAG_SHA256=0x0c,
	/* Encryption Algorithm */
	PEDRKI_TAG_ENC_ALG=0x0d,
	/* Coding Algorithm */
	PEDRKI_TAG_CODE_ALG=0x0e,
	/* Terminal Message Encryption Key */
	PEDRKI_TAG_TMEK=0x0f,
	/* Terminal Message Encryption Key Information */
	PEDRKI_TAG_TMEK_INFO=0x10,
	/* KCV Value */
	PEDRKI_TAG_KCV=0x11,
	/* Key Derive Factor */
	PEDRKI_TAG_DERIVE_VAR=0x12,
	/* Key Derive Algolithm */
	PEDRKI_TAG_DERIVE_ALG=0x13,
	/* Device Authentication Data */
	PEDRKI_TAG_DEV_AUTH_DATA=0x14,
	/* Publick Key Information */
	PEDRKI_TAG_PUK_INFO=0x15,
	/* Device Message Encryption Public Key Mailer */
	PEDRKI_TAG_DME_PUK_MAIL=0x16,
	/* KMS ID */
	PEDRKI_TAG_KMS_ID=0x17,
	/* Terminal Encryption Key */
	PEDRKI_TAG_TEK=0x18,
	/* Authentication Encryption */
	PEDRKI_TAG_AUK=0x19,
	/* TEKAUK CIPHER */
	PEDRKI_TAG_TEKAUK_CIPHER=0x1a,

	//ADD 20150819
	/* Public Key Format */
	PEDRKI_TAG_PUK_FMT=0x1b,
	/* Device Authentication Public Key */
	PEDRKI_TAG_DE_PUK=0x1c,
	/* Device Authentication Private Key */
	PEDRKI_TAG_DE_PVK=0x1d,
	/* LKI Authentication Public Key */
	PEDRKI_TAG_LKIAK_PUK=0X1e,
	/* RKI Authentication Public Key */
	PEDRKI_TAG_RKIAK_PUK=0X1f,
	/* Device ID */
	PEDRKI_TAG_DEV_ID=0X20,
	/* Certificate Revocation List */
	PEDRKI_TAG_CRL=0X21,
	/* Transmission Key Ciphertext */
	PEDRKI_TAG_KTK=0X22,
	/* Key Number */
	PEDRKI_TAG_KEY_CNT=0X23,
	/* TR31 Key block */
	PEDRKI_TAG_TR31_KEYBLK=0X24,
	/* Tag List */
	PEDRKI_TAG_LIST=0X25,
	/* Tag Invalid */
	PEDRKI_TAG_INVALID=0xff,
};

enum
{
	PEDRKI_INFO_STATUS=0X01,
	PEDRKI_INFO_KMS_ID=0X02,
};


enum
{
	PED_CMD_RKI_OPEN,
	PED_CMD_RKI_CLOSE,
	PED_CMD_RKI_GET_DEVID,
	PED_CMD_RKI_READ_PVK,
	PED_CMD_RKI_READ_PUK_TMP,
	PED_CMD_RKI_READ_PUK,
	PED_CMD_RKI_WRITE_PUK,
	PED_CMD_RKI_WRITE_DEV_AUTH_KEY,
	PED_CMD_RKI_GEN_DEV_KEY_TMP,
	PED_CMD_RKI_GEN_DEV_KEY,
	PED_CMD_RKI_GEN_DEV_AUTH_DATA,
	PED_CMD_RKI_GET_DEV_AUTH_DATA,
	PED_CMD_LKI_GET_DEV_AUTH_DATA,
	PED_CMD_RKI_VERIFY_DEV_AUTH_TOKEN,
	PED_CMD_RKI_GEN_HOST_AUTH_DATA,
	PED_CMD_RKI_WRITE_INFO,
	PED_CMD_RKI_READ_INFO,
	PED_CMD_RKI_VERIFY_PUK,
	PED_CMD_RKI_VERIFY_DEV_KEY,
	PED_CMD_RKI_ERASE_KEYS,
	PED_CMD_RKI_INJECT_KEY,
	PED_CMD_RKI_DEV_KEY_STATE,
	
	PED_CMD_RKI_ERROR
};

enum
{
	DEV_KEY_STATE_NONE,
	DEV_KEY_STATE_CERT,
	DEV_KEY_STATE_PUK		
};

typedef struct {
  	unsigned short int bits;                     					/* length in bits of modulus */
  	unsigned char modulus[MAX_RSA_MODULUS_LEN];  	/* modulus */
  	unsigned char publicExponent[MAX_RSA_MODULUS_LEN];     	/* public exponent */  
  	unsigned char exponent[MAX_RSA_MODULUS_LEN]; 			/* private exponent */
  	unsigned char prime[2][MAX_RSA_PRIME_LEN];   		/* prime factors */
  	unsigned char primeExponent[2][MAX_RSA_PRIME_LEN];	/* exponents for CRT */
  	unsigned char coefficient[MAX_RSA_PRIME_LEN];          	/* CRT coefficient */
} R_RSA_PRIVATE_KEY2;

int RkiOpen(void);
void RkiClose(void);
int RkiGetDevId(ushort usDevIdOutLen, uchar *pcDevIdOut);
int RkiReadPvk(uchar ucKeyIdx, uint *puiModBits, uchar *pucModuls,
	uchar *pucPvkExp);
int RkiReadPukTmp(uchar ucKeyIdx, ushort *pusKeyLen, uchar *pucKeyOut);
int RkiReadPuk(uchar ucKeyIdx, ushort *pusKeyLen, uchar *pucKeyOut);
int RkiWritePuk(uchar ucKeyIdx, ushort usKeyLen, uchar *pucKeyIn,
	ushort usCrlLen, uchar *pucCrl);
int RkiWriteDevAuthKey(uchar ucKeyIdx,ushort usDaPukLen, 
	uchar *pucDaPuk, ushort usDaPvkLen, uchar *pucDaPvk);
int RkiGenDevKey(void);
int RkiGenDevKeyTmp(uchar ucKeyIdx, R_RSA_PRIVATE_KEY2 *pstPvkKey);
int RkiGenDevAuthData(uchar ucDevATLen,uchar ucTkLen);
int RkiGetDevAuthData(uchar ucPukIdx, ushort usHostATCipherLen, 
	uchar *pucHostATCipher, ushort *pusDevAuthDataLen, uchar *pucDevAuthData);
int LkiGetDevAuthData(ushort usPukLen, uchar *pucPuk, ushort usCrlLen,
	uchar *pucCrl, ushort *pusDevAuthDataLen, uchar *pucDevAuthData);
int RkiVerifyDevAuthToken(uchar ucDevATLen, uchar *pucDevATCipher);
int RkiGenHostAuthData(uchar ucHostAtFmt, ushort usHostAtLen, uchar *pucHostAtOut);
int RkiWriteInfo(uchar ucItem, ushort usInfoLen, uchar *pucInfoData);
int RkiReadInfo(uchar ucItem, uchar *pucInfoData);
int RkiVerifyPuk(uchar ucKeyIdx, ushort usCrlLen, uchar *pucCrl);
int RkiEraseKeys(void);
int RkiInjectKey(ushort usKeyBlkLen, uchar *pucKeyBlk, uchar ucDstKeyIdx);
int RkiDevKeyState(void);


#endif 

