#ifndef _AX_SM2_H_
#define _AX_SM2_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned char privkey[32];  /*SM2 ˽Կ*/
    unsigned char pubkey[64];  /*SM2 ��Կ*/
} ST_SM2_KEY;

typedef ST_SM2_KEY  SM2_KEY;

typedef struct
{
    ST_SM2_KEY  key;  /*SM2 ��Կ��*/
    unsigned char *ida;       /*�û���ݱ�ʶID*/
    unsigned int idalen;         /*�û���ݱ�ʶID����*/
} ST_SM2_INFO;

/*********************************    Gen Za   ************************************
*Input:   entla       ENTLa��������ַ                       										*
*         entla_len   ENTLa�����                             										*
*         ida         IDa��������ַ                         										*
*         ida_len     IDa�����                               										*
*         pubkey      ��Կ��������ַ                        										*
*                                                               									*
* ����ֵ: 0 ---- ��ʾ�ɹ�			                            												*
**********************************************************************************/
int Gen_Za(unsigned char *entla,int entla_len,unsigned char *ida,int ida_len,char *pubkey);

/*********************************    Gen SM2 key   *******************************
*Input:   sm2key       sm2 key��������ַ                    										*
*                                                               									*
*����ֵ: 0 ---------- ��ʾ�ɹ�			                        												*
*        ����ֵ ----- ��ʾʧ��                                  										*
**********************************************************************************/
int Gen_SM2_key(SM2_KEY *sm2key);

/*******************************    SM2 Encrypt   *********************************
*Input:   privkey       ˽Կ                                    										*
*         input         ��ǩ������                            										*
*         input_len     ��ǩ�����ݳ���                        										*
*Output:  sig           ���ܺ�����                            										*
*		  sig_len       ���ܺ���ݳ���                          												*
*����ֵ:  0-------------��ʾ�ɹ�			                    													*
*         ����ֵ ------ ��ʾʧ��                               										*
**********************************************************************************/
int SM2_Sign(unsigned char *privkey,unsigned char *input,int input_len,unsigned char *sig, unsigned int *sig_len);

/*******************************    SM2 Decrypt   *********************************
*Input:   pubkey        ��Կ                                    										*
*         input         ��У�����Ϣ                            										*
*         input_len     ��У�����Ϣ����                        										*
*         sig           ��У�����Ϣ��ǩ��                      										*
*		  sig_len       ��У�����Ϣ��ǩ��ĳ���                												*
*����ֵ:  0-------------��ʾ�ɹ�			                    													*
*         ����ֵ ------ ��ʾʧ��                                										*
**********************************************************************************/
int SM2_Verify(unsigned char *pubkey,unsigned char *input,int input_len,const unsigned char *sig, int siglen);

/*******************************    SM2 Pubkey encrypt   **************************
*Input:   privkey       ��Կ                                   										*
*         input         ����ܵ���ݻ������ַ                  										*
*         input_len     ����ܵ���ݳ���                        										*
*         output        �����ݻ������ַ                      										*
*		  output_len    �����ݵĳ���                          												*
*����ֵ:  0-------------��ʾ�ɹ�			                    													*
*         ����ֵ ------ ��ʾʧ��                                										*
**********************************************************************************/
int SM2_PubKey_Encrypt(unsigned char *pubkey,unsigned char *input,int input_len, unsigned char *output, int *output_len);

/*******************************    SM2 Privkey Decrypt   *************************
*Input:   privkey       ˽Կ                                   										*
*         input         ����ܵ���ݻ������ַ                  										*
*         input_len     ����ܵ���ݳ���                        										*
*         output        �����ݻ������ַ                      										*
*		  output_len    �����ݵĳ���                          												*
*����ֵ:  0-------------��ʾ�ɹ�			                    													*
*         ����ֵ ------ ��ʾʧ��                                										*
**********************************************************************************/
int SM2_PrivKey_Decrypt(unsigned char *privkey,unsigned char *intput, int input_len,unsigned char *output,int *output_len);
int SM2GenKeyPair_sw(ST_SM2_KEY  *key );
int SM2Sign_sw (ST_SM2_INFO *info, unsigned char *data, unsigned int datalen,
		unsigned char * sign , unsigned int *signlen);
int SM2Verify_sw(ST_SM2_INFO * info,unsigned char *data,unsigned int datalen,
		unsigned char *sign, unsigned int signlen);
int SM2Recover_sw(ST_SM2_KEY *key ,unsigned char *datain,unsigned int datainlen, 
		unsigned char *dataout, unsigned int *dataoutlen, unsigned int mode );
int SM3_sw(unsigned char *key, unsigned int keylen ,unsigned char *datain,
		unsigned int datainlen,unsigned char *dataout );
int SM4_sw (unsigned char *datain,unsigned int datainlen,unsigned char iv[16],
		unsigned char key[16],unsigned char *dataout,unsigned int mode );

#ifdef __cplusplus
}
#endif

#endif
