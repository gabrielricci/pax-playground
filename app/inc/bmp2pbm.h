
/****************************************************************************
��ʽת����bmp to pbm 

�ٸ��������(����)���޹�˾

�޸���ʷ��
1.0.0 2011��7��26��    �μ���

****************************************************************************/

#ifndef _BMP2PBM_H
#define _BMP2PBM_H

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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int Bmp2Pbm(char *pszAppFileName, char *pszOutFileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* _UTIL_H */
