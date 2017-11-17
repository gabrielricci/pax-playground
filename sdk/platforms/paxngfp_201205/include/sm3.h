#ifndef _SM3_H
#define _SM3_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          Output = SM3( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SM3 checksum result
 */
void sm3( unsigned char *input, int ilen,unsigned char output[32]);


/**
 * \brief          Output = HMAC-SM3( hmac key, input buffer )
 *
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   HMAC-SM3 result
 */
void sm3_hmac( unsigned char *key, int keylen,
                unsigned char *input, int ilen,
                unsigned char output[32] );

int SM3_sw(unsigned char *key, unsigned int keylen ,unsigned char *datain, unsigned int datainlen,unsigned char *dataout );

#ifdef __cplusplus
}
#endif

#endif /* sm3.h */
