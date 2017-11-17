
#include "Global.h"

#define _S_SERIES_

#ifndef _P60_S1_





#define MAX_CMD_LEN  300
#define MAX_RSP_LEN  100

#define COMM_ERR     0xff
#define NO_PIN       0x0a
#define NO_AMT       0x0b
#define USER_CANCEL  0x06

#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint
#define uint unsigned short
#endif
#ifndef ulong
#define ulong unsigned long
#endif

static uchar ucRet,ucLen, CmdStr[MAX_CMD_LEN], RespStr[MAX_RSP_LEN];
static uint  uiLen;
static uchar MatrixDot[245];

static uchar SendRecv(uchar *CmdStr,uint SendLen, uchar *RespStr, uint *RecvLen,uint TimeOut);

extern uchar s_GetMatrixDot(uchar *str,uchar *MatrixDot,unsigned int *len, uchar flag);
//extern void ExPortClose(void);

#if defined(_P78_) || defined(_P80_) || defined(_P90_) || defined(_S_SERIES_) || defined(_P58_) || defined(_SP30_)
unsigned char ExPortOpen(int channel, long BaudRate, char Parity, char DataBit)
{
//	return(PortOpen(3, "9600,8,e,1"));
	return(OsPortOpen(3, "115200,8,n,1"));
//	return(PortOpen(3, "9600,8,n,1"));

}
unsigned char ExPortRecv(unsigned char *ch, unsigned short ms)
{
	int iRet;

	iRet = OsPortRecv(3, ch, 1, ms);

	if(iRet > 0) //if(iRet >= 0)
		return 0;
	else
		return 1;
}
void ExPortSend(unsigned char ch)
{
	OsPortSend(3, &ch, 1);
	return;
}
#else
extern unsigned char ExPortOpen(int channel, long BaudRate, char Parity, char DataBit);
extern unsigned char ExPortRecv(unsigned char *ch, unsigned short ms);
extern void ExPortSend(unsigned char ch);
#endif


static uchar RecvResp(uchar Head, uchar *Resp, uint *RecvLen,uint TimeOut)
{
	uint i;
	uchar key;

	Resp[0]=0;
	while(1)
	{
		if(TimeOut)
		{
            if(!ExPortRecv(Resp,(ushort)TimeOut)) break;
            return COMM_ERR;
        }
		else
		{
			ExPortRecv(Resp,100);
			/*if(!kbhit())
			{
				key=getkey();
				if(key==KEYCANCEL) return USER_CANCEL;
				//if(key==KEYENTER) return NO_PIN;
			}*/
		 return NO_PIN;
        }
		if(Resp[0]==Head || Resp[0] == Head -0x80) break;
	}

	if(ExPortRecv(Resp+1,100))return COMM_ERR;
	for(i=0;i<Resp[1]+1;i++)
	{
		if(ExPortRecv(Resp+i+2,100))  return COMM_ERR;
	}
	*RecvLen = Resp[1]+3;
	return 0;
}

static void GetEDC(uchar *str, uint len)
{
	uint i;

	str[len]=str[0];
	for(i=1;i<len;i++)
		str[len] ^= str[i];
}

uchar SendRecv(uchar *CmdStr,uint SendLen, uchar *RespStr, uint *RecvLen,uint TimeOut)
{
    uchar EDC;
    uint i,j;

    if(ExPortOpen(0, 9600L, 'E', 8)) return COMM_ERR;
	GetEDC(CmdStr,SendLen);
	for(j=0;j<3;j++)
	{
    	for(i=0;i<SendLen+1;i++) ExPortSend(CmdStr[i]);
		ucRet=RecvResp(CmdStr[0],RespStr,RecvLen,TimeOut);
		//OsLog(LOG_DEBUG,"SendRecv j:%d ucRet:%x\n", j, ucRet);
		if (ucRet)
		{
			if(ucRet ==COMM_ERR)	continue;
			return ucRet;
		}
		EDC=RespStr[*RecvLen-1];
		GetEDC(RespStr,(uint)(*RecvLen-1));
		if(RespStr[*RecvLen-1] ==EDC)	break;
//		if(RespStr[0]==CmdStr[0] || RespStr[2]!=0x01) break;
		OsSleep(10);
	}
	//OsLog(LOG_DEBUG,"COMM_ERR receLen:%d  j:%d ucRet:%x\n",*RecvLen, j, ucRet);
	if(j >=3)		return COMM_ERR;
    //ExPortClose();
	return 0;
}

uchar PPCancel(void)
{
	uchar ucRet;

	memcpy(CmdStr,"\x8e\x00",2);
	ucRet=SendRecv(CmdStr, 2, RespStr, &uiLen,800);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x8e)	return 0;
	return RespStr[2];
}
/*
uchar PPSetTimeout(uchar timeout)
{
	memcpy(CmdStr,"\x93\x02\x33\x00",4);
	CmdStr[3] =timeout;
	ucRet=SendRecv(CmdStr, 4, RespStr, &uiLen,300);
	if(ucRet) return ucRet;
	if(RespStr[0] == 0x93){
		return 0;
	}
	return RespStr[2];
}*/

uchar PPSetTimeout(unsigned short timeout)
{
	if(timeout==0)
		return 0xff;
	memcpy(CmdStr,"\x96\x04",2);
	CmdStr[2] =0;
	CmdStr[3] =0;
	CmdStr[5]	=(uchar)(timeout&0xff);
	CmdStr[4] =(uchar)(timeout>>8);
	ucRet=SendRecv(CmdStr, 6, RespStr, &uiLen,300);
	if(ucRet) return ucRet;
	if(RespStr[0] == 0x96){
		return 0;
	}
	return RespStr[2];
}
/*
uchar PPBaudRate(long Baudrate)
{
	memcpy(CmdStr,"\x93\x02\x34\x40",4);
	if(Baudrate==19200) CmdStr[3]=0x01;
	else if(Baudrate==28800) CmdStr[3]=0x02;
	else if(Baudrate==57600) CmdStr[3]=0x03;
	else return COMM_ERR;
	ucRet=SendRecv(CmdStr, 4, RespStr, &uiLen,300);
	if(ucRet) return ucRet;
	if(RespStr[0] == 0x93){
		BaudSet=Baudrate;
		return 0;
	}
	return RespStr[2];
}
*/

uchar PPBeep(void)
{
	memcpy(CmdStr,"\x8D\x02\x40\x40",4);
	ucRet=SendRecv(CmdStr, 4, RespStr, &uiLen,1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x8d)	return 0;
	return RespStr[2];
}

uchar PPLight(uchar OperMode, uchar Index)
{
	memcpy(CmdStr,"\x8D\x02\x00\x00",4);
	if((Index & PP_RED) == PP_RED)
		CmdStr[2] |= 0x08;
	if((Index & PP_GREEN) == PP_GREEN)
		CmdStr[2] |= 0x20;
	if((Index & PP_YELLOW) == PP_YELLOW)
		CmdStr[2] |= 0x10;
	if(OperMode == PP_OPEN)
	{
		if((Index & PP_RED) == PP_RED)
			CmdStr[3] |= 0x08;
		if((Index & PP_GREEN) == PP_GREEN)
			CmdStr[3] |= 0x20;
		if((Index & PP_YELLOW) == PP_YELLOW)
			CmdStr[3] |= 0x10;
	}
	ucRet=SendRecv(CmdStr, 4, RespStr, &uiLen,1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x8d)	return 0;
	return RespStr[2];
}

uchar PPInvoice(uchar OperMode)
{
	memcpy(CmdStr,"\x8D\x02\x02\x00",4);
	if(OperMode == PP_OPEN)
		CmdStr[3] |= 0x02;
	ucRet=SendRecv(CmdStr, 4, RespStr, &uiLen,1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x8d)	return 0;
	return RespStr[2];
}


uchar PPKbmute(uchar OperMode)
{
	memcpy(CmdStr,"\x8D\x02\x01\x00",4);
	if(OperMode == PP_OPEN)
		CmdStr[3] |= 0x01;
	ucRet=SendRecv(CmdStr, 4, RespStr, &uiLen,1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x8d)	return 0;
	return RespStr[2];
}

uchar PPBackLight(uchar OperMode)
{
	memcpy(CmdStr,"\x8D\x02\x80\x00",4);
	if(OperMode == PP_OPEN)
		CmdStr[3] |= 0x80;
	ucRet=SendRecv(CmdStr, 4, RespStr, &uiLen,1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x8d)	return 0;
	return RespStr[2];
}


uchar PPInput(uchar *str, uchar min, uchar max, uchar mode)
{
	uchar ucRet;

	if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
	CmdStr[0]=0x8C;
	CmdStr[1]=0x03;
	CmdStr[2]=min;
	CmdStr[3]=max;
	CmdStr[4]=mode;
	ucRet=SendRecv(CmdStr, 5, RespStr, &uiLen, 0);
	if(ucRet)
	{
		//if(ucRet==USER_CANCEL || ucRet==NO_PIN) PPCancel();
		if(ucRet==USER_CANCEL) PPCancel();
		return ucRet;
	}
	if(RespStr[0] == 0x8C)
	{
		memcpy(str,RespStr+2,RespStr[1]);
		str[RespStr[1]]=0;
		return 0;
	}
	return RespStr[2];
}

uchar PPScrPrint(uchar line, uchar col, uchar *str)
{
	unsigned int j;
	uint  i, len,mode;
	uchar flag, bDotLen;
	uchar TmpBuf[33], sDotBuf[130];


	if(line>=2 || col>121) return 0x04;
	len=strlen((char *)str);
	if(len*8>122-col) len=(122-col)/8;
	mode=0;
	flag=0;
	for(i=0;i<len;i++)
	{
		if(str[i]>0x80)
		{
			 mode=1;
			 flag ^= 1;
		}
	}
	if(flag)	len--;
	if(!len)	return 0;
	if(mode)
	{
		bDotLen =0;
		memset(sDotBuf, 0, sizeof(sDotBuf));
		for(i=0;i<len;i++)
		{
			//s_GetMatrixDot(str+i,TmpBuf,&j,1);

			if (j<6||j>32)
			{
				if(str[i]>0x80)
				{
					memcpy(MatrixDot+i*8,TmpBuf,16);
					memcpy(MatrixDot+len*8+i*8,TmpBuf+16,16);
					i ++;
					bDotLen +=16;
				}
				else
				{
			 		memcpy(MatrixDot+i*8,TmpBuf,8);
			 		memcpy(MatrixDot+len*8+i*8,TmpBuf+8,8);
					bDotLen +=8;
				}
			}
			else
			{
				memcpy(MatrixDot+bDotLen, TmpBuf, j/2);
				memcpy(sDotBuf+bDotLen, TmpBuf+j/2, j/2);
				bDotLen +=j/2;

				if(str[i]>0x80)			i++;
				if (i==(len-1))
					memcpy(MatrixDot+bDotLen, sDotBuf, bDotLen);
			}
		}
		return PPScrWrData(line,col, MatrixDot, (uchar)(bDotLen*2));
	}
	else
	{
		ucLen=strlen((char *)str);
		CmdStr[0]=0x8A;
		CmdStr[1]=ucLen+2;
        CmdStr[2]=col;
        CmdStr[3]=line;
		memcpy(CmdStr+4,str,ucLen);
		ucRet=SendRecv(CmdStr, (uint)(ucLen+4), RespStr, &uiLen,(uint)1000);
		if(ucRet)				return ucRet;
		if(RespStr[0] == 0x8A)	return 0;
		return RespStr[2];
	}
}

uchar PPScrCls(void)
{
	memcpy(CmdStr,"\x88\x00",2);
	ucRet=SendRecv(CmdStr, 2, RespStr, &uiLen,50);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x88)	return 0;
	return RespStr[2];
}

uchar PPScrClrLine(uchar Line)
{
	memcpy(CmdStr,"\x89\x01\0x00",3);
	CmdStr[2]=Line;
	ucRet=SendRecv(CmdStr, 3, RespStr, &uiLen,1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x89)	return 0;
	return RespStr[2];
}

uchar PPScrWrData(uchar line, uchar col, uchar *Data, uchar len)
{
	if (len >=252)	return 1;
	CmdStr[0]=0x8B;
	CmdStr[1]=len+2;
    CmdStr[2]=col;
    CmdStr[3]=line;
    memcpy(CmdStr+4,Data,len);
	ucRet=SendRecv(CmdStr, (uint)(len+4), RespStr, &uiLen,(uint)1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x8b)	return 0;
	return RespStr[2];
}


static uchar WriteMDKey(uchar KeyID, uchar mode, uchar *Key)
{
	CmdStr[0]=0x80;
	if(mode==0x01)						CmdStr[1]=10;
	else if(mode==0x03)				CmdStr[1]=18;
	else 											CmdStr[1]=26;
	CmdStr[2]=KeyID;
	CmdStr[3]=mode;
	memcpy(CmdStr+4,Key,CmdStr[1]-2);
	ucRet=SendRecv(CmdStr, (uint)(CmdStr[1]+2), RespStr, &uiLen,(uint)1000);
	if(ucRet) return ucRet;
	if(RespStr[0] ==0x80)	return 0;
	return RespStr[2];
}

uchar PPWriteMKey(uchar KeyID, uchar mode, uchar *Key)
{
	if(KeyID>100||KeyID==0)
		return 0x02;
	else
		return WriteMDKey(KeyID,mode,Key);
}

uchar PPWriteDKey(uchar KeyID, uchar mode, uchar *Key)
{
	if(KeyID>100||KeyID==0)
		return 0x02;
	else
		return WriteMDKey((uchar)(KeyID | 0x80),mode,Key);
}

uchar PPWriteWKey(uchar MKeyID, uchar WKeyID, uchar mode, uchar *Key)
{
	uchar TmpLen;
	CmdStr[0]=0x81;
	switch (mode)
	{
	case 0x01:
	case 0x81:
	case 0x03:
	case 0x83:
	case 0x07:
	case 0x87:
	case 0x31:
	case 0xb1:	
	case 0x71:
	case 0xf1:	CmdStr[1]=11;	TmpLen=8;	break;
	case 0x33:
	case 0xb3:	
	case 0x73:
	case 0xf3:	CmdStr[1]=19;	TmpLen=16;	break;
	default:	CmdStr[1]=27;	TmpLen=24;	break;
	}
	CmdStr[2]=MKeyID;
	CmdStr[3]=WKeyID;
	CmdStr[4]=mode;
	memcpy(CmdStr+5,Key,TmpLen);
	ucRet=SendRecv(CmdStr, (uint)(CmdStr[1]+2), RespStr, &uiLen,1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x81)	return 0;
	return RespStr[2];
}

uchar PPDeriveKey(uchar MKeyID, uchar WKeyID1, uchar WKeyID2, uchar mode)
{
	CmdStr[0]=0x82;
	CmdStr[1]=4;
	CmdStr[2]=MKeyID;
	CmdStr[3]=WKeyID1;
	CmdStr[4]=WKeyID2;
	CmdStr[5]=mode;
	ucRet=SendRecv(CmdStr, 6, RespStr, &uiLen,1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x82)	return 0;
	return RespStr[2];
}

uchar PPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *cardno, uchar *pin, uchar TwiceInput)
{
	uchar ucRet;

	if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
	if(!TwiceInput) CmdStr[0] = 0x83;
	else			CmdStr[0] = 0x84;
	CmdStr[1] = 19;
	CmdStr[2] = PinKeyID;
	CmdStr[3] = min;
	CmdStr[4] = max;
	memcpy(CmdStr+5,cardno,16);
	ucRet=SendRecv(CmdStr, 21, RespStr, &uiLen, 0);
	if(ucRet)
	{
		//if(ucRet==USER_CANCEL || ucRet==NO_PIN) PPCancel();
		if(ucRet==USER_CANCEL) PPCancel();
		return ucRet;
	}
	if( (!TwiceInput && RespStr[0]==0x83) || (TwiceInput && RespStr[0]==0x84) )
	{
		if(RespStr[1] == 0x00) return NO_PIN;
		memcpy(pin,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}

uchar Ex_PPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *cardno, uchar *pin, uchar TwiceInput, uchar mode)
{
	uchar ucRet;
	uchar i;
	if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
	if(mode==0x31||mode==0x71||mode==0x02)
		return 0x03;
	if(!TwiceInput) CmdStr[0] = 0x83;
	else			CmdStr[0] = 0x84;
	//if(max>0x06)
	//	return 0x05;
	//if(strlen(cardno)!=16)
	//	return 0x05;
	for(i=0;i<16;i++)
		{
			if(cardno[i]>0x39||cardno[i]<0x30)
			 cardno[i]=0x0;
		}
	CmdStr[1] = 22;
	CmdStr[2]	= 0xff;
	CmdStr[3]	=0xff;
	
	if(mode==0x02)
		return 0x03;
	if(mode==0x01)	CmdStr[4]=0x01;
	else if(mode==0x03)	CmdStr[4]=0x31;
	else if(mode==0x07)	CmdStr[4]=0x71;
	else CmdStr[4]=mode;
	CmdStr[5] = PinKeyID;
	CmdStr[6] = min;
	CmdStr[7] = max;
	memcpy(CmdStr+8,cardno,16);
	ucRet=SendRecv(CmdStr, 24, RespStr, &uiLen, 0);
	if(ucRet)
	{
		//if(ucRet==USER_CANCEL || ucRet==NO_PIN) PPCancel();
		if(ucRet==USER_CANCEL) PPCancel();
		return ucRet;
	}
	if( (!TwiceInput && RespStr[0]==0x83) || (TwiceInput && RespStr[0]==0x84) )
	{
		if(RespStr[1] == 0x00) return NO_PIN;
		memcpy(pin,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}


uchar PPDes(uchar DESKeyID, uchar mode, uchar *datain, uchar *dataout)
{
	CmdStr[0] = 0x87;
	CmdStr[1] = 10;
	CmdStr[2] = DESKeyID;
	CmdStr[3] = mode;
	memcpy(CmdStr+4,datain,8);
	ucRet=SendRecv(CmdStr, 12, RespStr, &uiLen, 1000);
	if(ucRet) return ucRet;
	if(RespStr[0] == 0x87)
	{
		memcpy(dataout,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}

uchar PPMac(uchar WKeyID,uchar mode, uchar *datain, ushort inLen, uchar *macout,uchar flag)
{
	unsigned char i,j,temp, bBlockNum, bSendLen, bLastLen, bBlockSize;
	unsigned char temp1[8];
	unsigned short nCount;
	unsigned long TimeOut;
	unsigned int k,l;

	if (WKeyID >100)	return 2;
	if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
	if(flag==0x00)	CmdStr[0] = 0x85;
	else		CmdStr[0] = 0x86;
	if(flag==0x00)
	{
			if (inLen >=252)
			{
				bBlockNum =inLen/240;
				bLastLen =inLen%240;
				if (bLastLen >0)	bBlockNum ++;
				else				bLastLen =240;
				bBlockSize =240;
			}
			else
			{
				bBlockNum =1;
				bBlockSize = (uchar)inLen;
			}
			TimeOut=1000;
			if(flag==0)
			{
				TimeOut +=bBlockSize*3;
				if(mode==3)		TimeOut +=bBlockSize*5;
				if(mode==7)		TimeOut +=bBlockSize*7;
			}
			CmdStr[1] = bBlockSize+2;
			CmdStr[2] = WKeyID;
			CmdStr[3] = mode;
			memcpy(CmdStr+4,datain,bBlockSize);
			bSendLen =bBlockSize;
			nCount =0;
			for(i=0; i<bBlockNum; i++)
			{
				ucRet=SendRecv(CmdStr, (uint)(bSendLen+4), RespStr, &uiLen, (uint)TimeOut);
				if(ucRet)	return ucRet;
				if(RespStr[0] !=CmdStr[0])		return RespStr[2];

				if (i ==(bBlockNum-1))			break;
				if (i ==(bBlockNum-2))			bSendLen =bLastLen;
				else							bSendLen =bBlockSize;
				memset(temp1,0x0,8);
				nCount +=bBlockSize;				
				if((0<bSendLen)&&(bSendLen<8))
					{
						temp=bSendLen%240;
						for(j=0;j<temp;j++)
						temp1[j]=datain[nCount+j]^RespStr[2+j];
						for(j=0;j<8-temp;j++)
						temp1[j+temp]=RespStr[2+j+temp];
						bSendLen+=(8-temp);
						memcpy(CmdStr+4, temp1, 8);	
					}
				else
					{
						for(j=0;j<8;j++)
						temp1[j]=datain[nCount+j]^RespStr[2+j];
						memcpy(CmdStr+4, temp1, 8);	
						memcpy(CmdStr+12, &datain[nCount+8], bSendLen-8);
					}
				CmdStr[1] = bSendLen+2;
				CmdStr[2] = WKeyID;
			}
	}
	else
	{
		memset(temp1,0x0,8);
		k=inLen/8;
		temp=inLen%8;
		if(temp)
			{
				memset(&datain[inLen],0x00,8-temp);
				inLen+=temp;
				k++;
			}
		TimeOut=1000;
		for(l=0;l<k;l++)
		{
				for(j=0;j<8;j++)
				temp1[j] ^= datain[j+8*l];
		}
		memcpy(CmdStr+4, temp1, 8);
		CmdStr[1] = 10;
		CmdStr[2] = WKeyID;
		CmdStr[3] = mode;
		ucRet=SendRecv(CmdStr, 12, RespStr, &uiLen, (uint)TimeOut);
				if(ucRet)	return ucRet;
	}
	if(RespStr[0] ==CmdStr[0])
	{
		memcpy(macout,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}

uchar PPVerInfo(uchar *ver)
{
	memcpy(CmdStr,"\x90\x00",2);
	ucRet=SendRecv(CmdStr, 2, RespStr, &uiLen,800);
	if(ucRet) return ucRet;
	if(RespStr[0] == 0x90)
	{
		memcpy(ver,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}

uchar PPUpdLogo(uchar line, uchar *Data)
{
	if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
	memcpy(CmdStr,"\x8f\xf5",2);
	CmdStr[2]=line;
	memcpy(CmdStr+3,Data,0xf4);
	ucRet=SendRecv(CmdStr,0xf7,RespStr,&uiLen,1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x8f)	return 0;
	return RespStr[2];
}

/*注释：
开始序号：显示内容的开始序号；
结束序号：显示内容的结束序号；
行数：显示内容的总行数；
显示内容：为[行数][16BYTE]的数组。多页显示时用F2，F3翻页。
返回序号：返回的序号，在开始序号、结束序号之间。
当显示金额时，开始序号=0x3d（取消）;结束序号=0x3e（确认）。
可以按POS上（取消）退出。
*/
uchar EPSPPDispMenu(uchar StartNo, uchar EndNo, uchar AllLines, uchar detail[][16], uchar *item)
{
    uchar i,len,temp_len,begin_ptr,ucRet;

    if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
    memset(CmdStr,0,180);
	CmdStr[0] = 0x91;
	CmdStr[2] = StartNo;
	CmdStr[3] = EndNo;
	CmdStr[4] = AllLines;
	begin_ptr=5;
	len=0;
	for(i=0;i<AllLines;i++)
	{
		temp_len=strlen((char *)detail[i]);  // len of the detail
		len=len+temp_len+1;
		if (len >=251)		return 1;
		memcpy(CmdStr+begin_ptr,&detail[i][0],temp_len);
		begin_ptr=begin_ptr+temp_len+1;
    }
    CmdStr[1] =len+3;
    ucRet=SendRecv(CmdStr, (uint)(len+5), RespStr, &uiLen,(uint) 0);
	if(ucRet)
	{
		//if(ucRet==USER_CANCEL || ucRet==NO_PIN) PPCancel();
		if(ucRet==USER_CANCEL) PPCancel();
		return ucRet;
	}
	if(RespStr[0] == 0x91)
	{
		memcpy(item,RespStr+2,uiLen-3);
		return 0;
	}
	return RespStr[2];
}

void ConvertEPSPPAmount(uchar point_mode,uchar max_len,uchar *Amt)
{
	uchar Dec_Num=0,Dec_place=0;
	uchar Amt_Str[16]={0};

	switch (point_mode)
	{
	case 0x02://no decimal
		Dec_Num = 0;
		break;
	case 0x03://one decimal
		Dec_Num = 1;
		break;
	case 0x00://two decimal
		Dec_Num = 2;
		break;
	case 0x01://three decimal
		Dec_Num = 3;
		break;
	default:
		Amt_Str[0]=0;
		return;
	}

	Dec_place=(max_len-Dec_Num)-1;
	memcpy(Amt_Str,Amt,(max_len-Dec_Num-1));
	memcpy(&Amt_Str[Dec_place],&Amt[Dec_place+1],Dec_Num);
	Amt_Str[max_len]=0;
	strcpy(Amt,Amt_Str);
	Amt[max_len]=0;
	return;
}

/*
min：字符串最小长度(为0时，按ENTER键返回，且输入长度Len=0)。
max：字符串最大长度(小于等于16字节)。
Mode：0X00:小数点后有两位小数；
       非0:小数点后有三位小数；
*/
uchar EPSPPAmount(uchar point_mode,uchar prompt_mode, uchar min, uchar max,uchar *Amt)
{
	uchar ucRet;

	if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
	CmdStr[0] = 0x92;
	CmdStr[1] = 4;
	CmdStr[2] = min;
	CmdStr[3] = max;
	CmdStr[4] = point_mode;
	CmdStr[5] = prompt_mode;

	ucRet=SendRecv(CmdStr, 6, RespStr, &uiLen, 0);
	if(ucRet)
	{
		if(ucRet==USER_CANCEL) PPCancel();
		return ucRet;
	}
	if(RespStr[0] == 0x92)
	{
//		if(RespStr[1] ==0x00 && min>0) return NO_AMT;
		memcpy(Amt,RespStr+2,uiLen-3);
		Amt[uiLen-3]=0;
		return 0;
	}
	return RespStr[2];
}

/*
由POS产生临时KEY，上传主机，取回密文TMK1和临时加密参考值，由临时KEY对密文TMK1解密，得TMK，
明文TMK应符合奇校验；TMK对"\x12\x34\x56\x78\x90\x12\x34\x56"加密，加密后的结果应与加密参考
值相等；之后将明文TMK写入WkeyID位置。
Len=26.
输入内容b为：WkeyID1(1byte)+临时KEY(8bytes)+密文TMK1(8bytes)+加密参考值(8bytes)。
*/
uchar EPSPPLoadTMK(uchar WKeyID1, uchar *TmpKey, uchar *KTMK, uchar *ReferCode)
{
	memcpy(CmdStr,"\x94\x02\x01",3);
	CmdStr[1]=26;
	CmdStr[3]=WKeyID1;
	memcpy(CmdStr+4,TmpKey,8);
	memcpy(CmdStr+12,KTMK,8);
	memcpy(CmdStr+20,ReferCode,8);
	ucRet=SendRecv(CmdStr, 28, RespStr, &uiLen,1500);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x94)	return 0;
	return RespStr[2];
}


/*
用WkeyID1处的TMK对主机下传的新的密文解密，明文应符合奇校验，之后将明文结果写入WkeyID2位置。
（更新TMK：主机下传新的密文TMK2，用明文TMK1对TMK2解密，明文TMK2应符合奇校验，之后将明文TMK2
写入WkeyID2位置，当WkeyID1与WkeyID2相等时，即覆盖明文TMK1。
下载TPK：主机下传新的密文TPK，用WkeyID1处的TMK对其解密，明文TPK应符合奇校验，之后将明文TPK
写入WkeyID2位置。）
Len=11.
输入内容b为：密文WkeyID1(1byte) +WkeyID2(1byte)+TMK2(8bytes)。
*/
uchar EPSPPLoadKey(uchar WKeyID1,uchar WKeyID2, uchar *KKEY)
{
	memcpy(CmdStr,"\x94\x02\x02",3);
	CmdStr[1]=11;
	CmdStr[3]=WKeyID1;
	CmdStr[4]=WKeyID2;
	memcpy(CmdStr+5,KKEY,8);
	ucRet=SendRecv(CmdStr, 13, RespStr, &uiLen,1000);
	if(ucRet)				return ucRet;
	if(RespStr[0] == 0x94)	return 0;
	return RespStr[2];
}

/*
POS送入8bytes Random, 其应符合奇校验，用WkeyID1处的TMK对其加密，返回其结果。
Len=10.
输入内容b为： WKeyID(1byte) + 明文Random(8bytes)
返回：密文Random(8bytes)
*/
uchar EPSPPGetMAC1(uchar WKeyID, uchar *Random)
{
	memcpy(CmdStr,"\x94\x02\x03",3);
	CmdStr[1]=10;
	CmdStr[3]=WKeyID;
	memcpy(CmdStr+4,Random,8);
	ucRet=SendRecv(CmdStr, 12, RespStr, &uiLen,1000);
	if(ucRet)		return ucRet;
	if(RespStr[0] == 0x94)
	{
		memcpy(Random,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}

/*
POS送入8bytes pin_block,新的ISN,新的TPK,原TPK Wkey_ID2，TMK Wkey_ID1，PINPAD用
原的TPK对pin_block按X3.92解密得到明文PIN，再将新TPK用TMK解密，明文TPK应符合奇校验，
新TPK替换原TPK，之后用明文TPK，ISN对PIN按X3.92加密，返回新的pin_block.
Len=25.
输入内容b为： Wkey_ID1+Wkey_ID2+pin_block(8byte)+ISN(6byte)+ new_TPK(8bytes)
返回：新的pin_block(8bytes)。
*/
uchar EPSPPUpdateTPK(uchar WKeyID1,uchar WKeyID2, uchar *pin_block, uchar *ISN, uchar *new_TPK)
{
	memcpy(CmdStr,"\x94\x02\x04",3);
	CmdStr[1]=25;
	CmdStr[3]=WKeyID1;
	CmdStr[4]=WKeyID2;
	memcpy(CmdStr+5,pin_block,8);
	memcpy(CmdStr+13,ISN,6);
	memcpy(CmdStr+19,new_TPK,8);
	ucRet=SendRecv(CmdStr, 27, RespStr, &uiLen,1500);
	if(ucRet)		return ucRet;
	if(RespStr[0] == 0x94)
	{
		memcpy(pin_block,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}


/*
用WkeyID1处的TMK对主机下传的新的密文解密，明文应符合奇校验，
对输入的数据做MAC运算，其结果与输入的MAC运算值相等(前4byte)，之后将明文结果MAC2写入WkeyID2位置。
Len=rsp_Len+20.
输入内容b为：WkeyID1(1byte) +WkeyID2(1byte)+rsp_len(1byte)+MAC2密文+(8bytes)+rsp_data(rsp_len)+rsp_MAC(8byte)。
*/

uchar EPSPPSetMAC2(uchar WKeyID1,uchar WKeyID2,uchar *MAC2, uchar *rsp_data, uchar rsp_data_len, uchar *rsp_MAC)
{
	unsigned short TimeOut;

	if (rsp_data_len >233)	return 1;
	if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
	TimeOut=rsp_data_len*4+1000;
	memcpy(CmdStr,"\x94\x02\x05",3);
	CmdStr[1]=rsp_data_len+20;
	CmdStr[3]=WKeyID1;
	CmdStr[4]=WKeyID2;
	CmdStr[5]=rsp_data_len;
	memcpy(CmdStr+6,MAC2,8);
	memcpy(CmdStr+14,rsp_data,rsp_data_len);
	memcpy(CmdStr+14+rsp_data_len,rsp_MAC,8);

	ucRet=SendRecv(CmdStr,(uint)(rsp_data_len+22), RespStr, &uiLen,TimeOut);
	if(ucRet) return ucRet;
	if(RespStr[0] == 0x94) return 0;
	return RespStr[2];
}

/*
用原TMK对"\x12\x34\x56\x78\x90\x12\x34\x56"加密，并返回结果。
Len=2.
输入内容b为：WkeyID1(1byte)。
返回内容：加密结果（8byte）.
*/
uchar EPSPPVerifyTMK(uchar WKeyID, uchar *VerifyData)
{
	memcpy(CmdStr,"\x94\x02\x06",3);
	CmdStr[3]=WKeyID;
	ucRet=SendRecv(CmdStr, 4, RespStr, &uiLen,1000);
	if(ucRet) return ucRet;
	if(RespStr[0] == 0x94)
	{
		memcpy(VerifyData,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}

uchar EPSPPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *ISN, uchar *pin)
{
	uchar ucRet;

	if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
	CmdStr[0] = 0x83;
	CmdStr[1] = 12;
	CmdStr[2] = 0xff;
	CmdStr[3] = 0xff;
	CmdStr[4] = 0x02;
	CmdStr[5] = PinKeyID;
	CmdStr[6] = min;
	CmdStr[7] = max;
	memcpy(CmdStr+8,ISN,6);
	ucRet=SendRecv(CmdStr, 14, RespStr, &uiLen, 0);
	if(ucRet)
	{
		//if(ucRet==USER_CANCEL || ucRet==NO_PIN) PPCancel();
		if(ucRet==USER_CANCEL) PPCancel();
		return ucRet;
	}
	if(RespStr[0] == 0x83)
	{
		if(RespStr[1] == 0x00) return NO_PIN;
		memcpy(pin,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}
/* 深银联使用的输PIN函数 */
uchar SYLPPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *bPinLen,uchar *pin_block)
{
	uchar ucRet;

	if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
	CmdStr[0] = 0x83;
	CmdStr[1] = 12;
	CmdStr[2] = 0xff;
	CmdStr[3] = 0xff;
	CmdStr[4] = 0x03;
	CmdStr[5] = PinKeyID;
	CmdStr[6] = min;
	CmdStr[7] = max;
	memcpy(CmdStr+8,"123456",6);
	ucRet=SendRecv(CmdStr, 14, RespStr, &uiLen, 0);
	if(ucRet)
	{
		if(ucRet==USER_CANCEL) PPCancel();
		return ucRet;
	}
	if(RespStr[0] == 0x83)
	{
		*bPinLen =RespStr[2];
		memcpy(pin_block,RespStr+3,8);
		return 0;
	}
	return RespStr[2];
}
uchar SYLPPVerifyDerive(uchar MKeyID, uchar WKeyID, uchar *new_WKey, uchar *KeyCv, uchar mode)
{
	uchar ucRet;

	if (mode!=0x01 && mode!=0x81)	return 0x03;
	if(PPScrClrLine(20)==COMM_ERR) return COMM_ERR;
	CmdStr[0] = 0x95;
	CmdStr[1] = 19;
	CmdStr[2] = MKeyID;
	CmdStr[3] = WKeyID;
	memcpy(CmdStr+4, new_WKey, 8);
	memcpy(CmdStr+12, KeyCv, 8);
	CmdStr[20] = mode;
	ucRet=SendRecv(CmdStr, 21, RespStr, &uiLen, 1000);
	if(ucRet)		return ucRet;
	if(RespStr[0] == 0x95)		return 0;
	return RespStr[2];
}

uchar SYLPPCalcPinBlock(uchar MKeyID,uchar PinKeyID,uchar *pin_block,uchar *new_TPK,uchar mode)
{
	uchar ucRet;

	if (mode!=0x01 && mode!=0x81)	return 0x03;

	memcpy(CmdStr,"\x94\x02\x07",3);
	CmdStr[1]=20;
	CmdStr[3]=MKeyID;
	CmdStr[4]=PinKeyID;
	memcpy(CmdStr+5,pin_block,8);
	memcpy(CmdStr+13,new_TPK,8);
	CmdStr[21] = mode;
	ucRet=SendRecv(CmdStr, 22, RespStr, &uiLen,1500);
	if(ucRet)		return ucRet;
	if(RespStr[0] == 0x94)
	{
		memcpy(pin_block,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}

uchar PPEmvGetPwd(uchar min, uchar max,uchar *pin_block)
{
	uchar ucRet;
	uchar i,tempWkey[8];
	uchar cardno[16];
	uchar En_pinblock[8];
	uchar Clear_EmvPin[8];
	
	if(min<4|| max>12) 
			return 0x05;
	memset(cardno,0x0,16);
	for(i=0;i<8;i++)
		tempWkey[i]=rand()&0xff;
	PPWriteWKey(0,100,0x01,tempWkey);	
	ucRet=PPGetPwd(100, min, max,cardno,En_pinblock, 0);
	OsDES(En_pinblock, Clear_EmvPin,tempWkey, 8, 0);
	Clear_EmvPin[0]|=0x20;
	memcpy(pin_block,Clear_EmvPin,8);
	return ucRet;
	
}

uchar PPGetPwd_3Des(uchar PinKeyID, uchar mode,uchar min, uchar max, uchar *cardno, uchar *pin, uchar TwiceInput)
{
	uchar ucRet;
	
	if(PPScrClrLine(20)==COMM_ERR) 
		return COMM_ERR;
	if(mode==0x03||mode==0x71||mode==0x02)
		return 0x03;
	if(!TwiceInput) CmdStr[0] = 0x83;
	else			CmdStr[0] = 0x84;
	CmdStr[1] = 22;
	CmdStr[2] = 0xff;
	CmdStr[3] = 0xff;
	CmdStr[4] = mode;	
	CmdStr[5] = PinKeyID;
	CmdStr[6] = min;
	CmdStr[7] = max;
	memcpy(CmdStr+8,cardno,16);
	ucRet=SendRecv(CmdStr, 24, RespStr, &uiLen, 10000);
	if(ucRet)
	{
		//if(ucRet==USER_CANCEL || ucRet==NO_PIN) PPCancel();
		if(ucRet==USER_CANCEL) PPCancel();
		return ucRet;
	}

	int i;
	for(i=0;i<uiLen;i++)
		OsLog(LOG_ERROR,"SendRecv  %d:%d\n",i, RespStr[i]);

	if( (!TwiceInput && RespStr[0]==0x83) || (TwiceInput && RespStr[0]==0x84) )
	{
		if(RespStr[1] == 0x00) 
			return NO_PIN;
		memcpy(pin,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}

uchar PPQuickBeepTest(void)
{
/*
memcpy(CmdStr,"\x8D\x02\x40\x40",4);
ucRet=SendRecv(CmdStr, 4, RespStr, &uiLen,300); //ddq
if(ucRet)				return ucRet;
if(RespStr[0] == 0x8d)	return 0;
return RespStr[2];
	*/
	return 0;	
}


#endif		// _P60_S1_

