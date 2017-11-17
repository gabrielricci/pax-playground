#include "Global.h"

extern void  s_GetMatrixDot(uchar *str,uchar *MatrixDot,uint *len, uchar flag);
//Icon Status Globle Variables
int icon_IC_Status;
int icon_Prn_Status;
int icon_Battery_Status;

int icon_IC_Status_Last;
int icon_Prn_Status_Last;
int icon_Battery_Status_Last;

int swipted;
int EntryType;

unsigned char cAccount[20];
unsigned char cExpDate[8];
unsigned char	szTrack2[40+1];
unsigned char szTrack3[108];
Clss_TransParam ClssTransParam;

XuiWindow *canvasConfirmAccount;
