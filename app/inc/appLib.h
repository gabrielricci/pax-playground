/*------------------------------------------------------------
* FileName: appLib.h
* Author: jiangjy
* Date: 2015-03-21
------------------------------------------------------------*/
#ifndef APPLIB_H
#define APPLIB_H

#endif
//Similar with function PubOne2Two(), and add '\0' at the end of object string
void PubBcd2Asc0(unsigned char *psIn, unsigned int uiLength, unsigned char *pszOut);

//Convert BIN string to readable HEX string, which have double length of BIN string. 0x12AB-->"12AB"
void PubBcd2Asc(unsigned char *psIn, unsigned int uiLength, unsigned char *psOut);

// �Ƚ�2�ŵ���Ϣ��PAN�Ƿ�һ��(For ICC)
// Check whether track2 (from ICC) and PAN (from ICC) are same.
int MatchTrack2AndPan(unsigned char *pszTrack2, unsigned char *pszPan);

//ɾ���ַ����ұ��������ض��ַ�
//Delete the specified continuous characters on the right of the string
void PubTrimTailChars(unsigned char *pszString, unsigned char ucRemoveChar);
