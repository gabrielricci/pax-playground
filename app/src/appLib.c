/*------------------------------------------------------------
* FileName: appLib.c
* Author: jiangjy
* Date: 2015-03-21
------------------------------------------------------------*/
#include <appLib.h>


// 比较2磁道信息和PAN是否一致(For ICC)
// Check whether track2 (from ICC) and PAN (from ICC) are same.
int MatchTrack2AndPan(unsigned char *pszTrack2, unsigned char *pszPan)
{
	int		i;
	unsigned char	szTemp[19+1];

	for(i=0; i<19 && pszTrack2[i]!='\0'; i++)
	{
		if( pszTrack2[i]=='=' )
		{
			break;
		}
		szTemp[i] = pszTrack2[i];
	}
	szTemp[i] = 0;

	if( strcmp((char *)szTemp, (char *)pszPan)==0 )
	{
		return 1;
	}

	return 0;
}

