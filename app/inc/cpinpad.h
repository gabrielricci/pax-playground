#ifndef _PIN_PAD_H
#define _PIN_PAD_H

#define PP_OPEN         0x01       
#define PP_CLOSE        0x00
#define PP_GREEN        0x01
#define PP_RED          0x02
#define PP_YELLOW	    0x04

uchar PPBaudRate(long Baudrate);

// Set Pininput Time //����Ϊ0.01��
uchar PPSetTimeout(unsigned short timeout);

// beep
uchar PPBeep(void);

/*
  OperMode=O_OPEN  ��PINPADָʾ��
  OperMode=O_CLOSE �ر�PINPADָʾ��
  Index          ����Ϊ����ֵ���������:
       RED       ���
       GREEN     �̵�
       YELLOW    �Ƶ�
*/
uchar PPLight(uchar OperMode, uchar Index);

/*
  OperMode=O_OPEN  ��PINPAD������ʾ
  OperMode=O_CLOSE �ر�PINPAD������ʾ
*/
uchar PPInvoice(uchar OperMode);

/*
  OperMode=O_OPEN  ��PINPAD������
  OperMode=O_CLOSE �ر�PINPAD������
*/
uchar PPKbmute(uchar OperMode);


/*
  OperMode=O_OPEN  �򿪱���
  OperMode=O_CLOSE �رձ���
*/
uchar PPBackLight(uchar OperMode);

//Input for reserve
/*
  ��������  �����ַ���
  str       ������ַ���
  min       ��������С����
  max       ��������󳤶�
  mode=0    ��������
  mode=1    ��������,��ʾ'*'
*/
uchar PPInput(uchar *str, uchar min, uchar max, uchar mode);

// LCD Ctrl
// Clear screen
uchar PPScrCls(void);


/*
  ��������  �����
  line      ��(0 - 1)
*/
uchar PPScrClrLine(uchar line);


/*
  ��������  ��ʾ�ַ���(���ֺ�Ӣ�ģ�
  line      ��(0 - 1)
  col       ��(0 - 113)
  str       ��ʾ���ַ���
*/
uchar PPScrPrint(uchar line, uchar col, uchar *str);

/*
  ��������  ��ʾ����
  line      ��(0 - 1)
  col       ��(0 - 121)
  Data      ��ʾ����(���ݸ�ʽ�ο�Э��)
  len       ��ʾ���ݳ���
*/
uchar PPScrWrData(uchar line, uchar col, uchar *Data, uchar len);


/*key magage
  ���е���Կ������Χ�� 1 - 100
*/
/*
  ��������  д������Կ��Կ
  KeyID     ��Կ����(����Կ����
  Key       ��Կֵ
  mode=0x01 DES��Կ
  mode=0x03 3DES��SEED��Կ(16bytes des ��Կ)
  mode=0x07	3DES��SEED��Կ(24bytes des ��Կ)
*/
uchar PPWriteMKey(uchar KeyID, uchar mode, uchar *Key);


/*
  ��������  д��DES(3DES)�������Կ
  KeyID     ��Կ����(DES��Կ����
  Key       ��Կֵ
  mode=0x01 DES��Կ
  mode=0x03 3DES��SEED��Կ(16bytes des ��Կ)
  mode=0x07	3DES��SEED��Կ(24bytes des ��Կ)
*/
uchar PPWriteDKey(uchar DESKeyID, uchar mode, uchar *Key);


/*
  ��������  д�빤����Կ
  MKeyID    �ø�����ָ��������Կ��Key����mode��ʽ�ļ������д��,MKeyID=0,�������κμ���,ֱ��д��
  WKeyID    д��ĵ�ַ(������Կ��)
  Key       ��Կ
  Mode=0x01  DES����
	Mode=0x81  DES���� 
	��ģʽ�£�MKeyID��WKeyID��Ϊ��DES��Կ��
	Mode=0x03  3DES����
	Mode=0x83  3DES����
	��ģʽ�£�MKeyIDΪ3DES��16�ֽ���Կ�� WKeyIDΪ8�ֽ���Կ��
	Mode=0x07  3DES����
	Mode=0x87  3DES����
	��ģʽ�£�MKeyIDΪ3DES��24�ֽ���Կ�� WKeyIDΪ8�ֽ���Կ��
	
	Mode=0x31  3DES����
	Mode=0xb1  3DES����
	��ģʽ�£�MKeyIDΪ3DES��16�ֽ���Կ, WKeyIDΪ8�ֽ���Կ��
	Mode=0x33  3DES����
	Mode=0xb3  3DES����
	��ģʽ�£�MKeyID��WKeyID��Ϊ3DES��16�ֽ���Կ��������EBC���ؼӽ��ܷ�ʽ��
	
	Mode=0x71  3DES����
	Mode=0xf1  3DES����
	��ģʽ�£�MKeyIDΪ3DES��24�ֽ���Կ, WKeyIDΪ8�ֽ���Կ��
	Mode=0x73  3DES����
	Mode=0xf3  3DES����
	��ģʽ�£�MKeyIDΪ3DES��24�ֽ���Կ, WKeyIDΪ3DES��16�ֽ���Կ��������EBC���ؼӽ��ܷ�ʽ��
	Mode=0x77  3DES����
	Mode=0xf7  3DES����
	��ģʽ�£�MKeyID��WKeyID��Ϊ3DES��24�ֽ���Կ��������EBC���ؼӽ��ܷ�ʽ��

*/
uchar PPWriteWKey(uchar MKeyID, uchar WKeyID, uchar mode, uchar *Key);


/*
  ��������  ��ɢ��Կ
  KeyID     ����з�ɢ����Կ,��λΪ1����0x80)ָ���Ĺ�����Կ����Կ������Ϊ����Կ����Կ
  WKeyID1   ��ɢ����(������Կ��)
  WKeyID2   ��ɢ���(������Կ��)
  Mode=0x01  DES����
	Mode=0x81  DES���� 
	��ģʽ�£�MKeyID��WKeyID1��WKeyID2��Ϊ��DES��Կ��
	Mode=0x03  3DES����
	Mode=0x83  3DES����
	��ģʽ�£�MKeyIDΪ3DES��16�ֽ���Կ, WKeyID1��WKeyID2Ϊ8�ֽ���Կ��
	Mode=0x07  3DES����
	Mode=0x87  3DES����
	��ģʽ�£�MKeyIDΪ3DES��24�ֽ���Կ, WKeyID1��WKeyID2Ϊ8�ֽ���Կ��
	
	Mode=0x31  3DES����
	Mode=0xb1  3DES����
	��ģʽ�£�MKeyIDΪ3DES��16�ֽ���Կ, WKeyID1��WKeyID2Ϊ8�ֽ���Կ��
	Mode=0x33  3DES����
	Mode=0xb3  3DES����
	��ģʽ�£�MKeyID��WKeyID1��WKeyID2��Ϊ3DES��16�ֽ���Կ��������EBC���ؼӽ��ܷ�ʽ��
	
	Mode=0x71  3DES����
	Mode=0xf1  3DES����
	��ģʽ�£�MKeyIDΪ3DES��24�ֽ���Կ, WKeyID1��WKeyID2Ϊ8�ֽ���Կ��
	Mode=0x73  3DES����
	Mode=0xf3  3DES����
	��ģʽ�£�MKeyIDΪ3DES��24�ֽ���Կ, WKeyID1��WKeyID2Ϊ3DES��16�ֽ���Կ��������EBC���ؼӽ��ܷ�ʽ��
	Mode=0x77  3DES����
	Mode=0xf7  3DES����
	��ģʽ�£�MKeyID��WKeyID1��WKeyID2��Ϊ3DES��24�ֽ���Կ��������EBC���ؼӽ��ܷ�ʽ��

*/
uchar PPDeriveKey(uchar MKeyID, uchar WKeyID1, uchar WKeyID2, uchar mode);


/*
  ��������     ��ȡANSI9.8���ܵ�����
  TwiceInput=1 PINPAD������ʾ����2��
  TwiceInput=0 PINPAD������ʾ����1��
  PinKeyID     ����PIN�Ĺ�����Կ(������Կ��)
  min          �����������С����
  max          �����������󳤶�
  pin          ���ܵ��������
  cardno       ������λ�����16λ���ţ�ASCII�룩
*/
uchar PPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *cardno, uchar *pin,uchar TwiceInput);

/*
  ��������     ��ȡANSI9.8���ܵ�����
  TwiceInput=1 PINPAD������ʾ����2��
  TwiceInput=0 PINPAD������ʾ����1��
  PinKeyID     ����PIN�Ĺ�����Կ(������Կ��)
  min          �����������С����
  max          �����������󳤶�
  pin          ���ܵ��������
  cardno       ������λ�����16λ���ţ�ASCII�룩
  mode:
  mode=0x01		des����
  mode=0x03		16�ֽ�3des����
  mode=0x07 	24�ֽ�3des����		
  
	
*/
uchar Ex_PPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *cardno, uchar *pin,uchar TwiceInput,uchar mode);


/*
  ��������  DES(3DES)����
  DESKeyID  ����DES����Կ(DES��Կ��)
  datain    ��������
  macout    �������
  mode=0x01  DES����   [��ԿΪ8�ֽ�]
	mode=0x81  DES����   [��ԿΪ8�ֽ�]
	mode=0x03  3DES����  [��ԿΪ16�ֽ�]
	mode=0x83  3DES���ܡ�[��ԿΪ16�ֽ�]
	mode=0x07  3DES���ܡ�[��ԿΪ24�ֽ�]
	mode=0x87  3DES���ܡ�[��ԿΪ24�ֽ�]

*/
uchar PPDes(uchar DESKeyID, uchar mode, uchar *datain, uchar *dataout);
/*
  ��������  MAC����
  flag=0    �㷨1
  flag=1    �㷨2
  WKeyID    ����MAC�Ĺ�����Կ(������Կ��)
  inLen     ����MAC�����ݳ���
  datain    ��������
  macout    �������(��MAC)
  mode=0x01 DES����
  mode=0x03 3DES����(16bytes Des)
  mode=0x07 3DES����(24bytes Des)
*/
uchar PPMac(uchar WKeyID,uchar mode, uchar *datain, ushort inLen, uchar *macout,uchar flag);

uchar PPVerInfo(uchar *ver);

uchar PPUpdLogo(uchar line, uchar *Data);





//����ΪHONG KONG EPS ר��


/*ע�ͣ�
��ʼ��ţ���ʾ���ݵĿ�ʼ��ţ�
������ţ���ʾ���ݵĽ�����ţ�
��������ʾ���ݵ���������
��ʾ���ݣ�Ϊ[����][16BYTE]�����顣��ҳ��ʾʱ��F2��F3��ҳ��
������ţ����ص���ţ��ڿ�ʼ��š��������֮�䡣
����ʾ���ʱ����ʼ���=0x3d��ȡ����;�������=0x3e��ȷ�ϣ���
���԰�POS�ϣ�ȡ�����˳���
*/
uchar EPSPPDispMenu(uchar StartNo, uchar EndNo, uchar AllLines, uchar detail[][16], uchar *item);


/*
min���ַ�����С����(Ϊ0ʱ����ENTER�����أ������볤��Len=0)��
max���ַ�����󳤶�(С�ڵ���16�ֽ�)��
Mode��0X00:С���������λС����
       ��0:С���������λС����
*/
//uchar EPSPPAmount(uchar min, uchar max, uchar mode,uchar *Amt);
uchar EPSPPAmount(uchar point_mode,uchar prompt_mode, uchar min, uchar max,uchar *Amt);

void ConvertEPSPPAmount(uchar point_mode,uchar max_len,uchar *Amt);

/*
��POS������ʱKEY���ϴ�������ȡ������TMK1����ʱ���ܲο�ֵ������ʱKEY������TMK1���ܣ���TMK��
����TMKӦ������У�飻TMK��"\x12\x34\x56\x78\x90\x12\x34\x56"���ܣ����ܺ�Ľ��Ӧ����ܲο�
ֵ��ȣ�֮������TMKд��WkeyIDλ�á�
Len=26.
��������bΪ��WkeyID1(1byte)+��ʱKEY(8bytes)+����TMK1(8bytes)+���ܲο�ֵ(8bytes)��
*/
uchar EPSPPLoadTMK(uchar WkeyID1, uchar *TmpKey, uchar *KTMK, uchar *ReferCode);


/*
��WkeyID1����TMK�������´����µ����Ľ��ܣ�����Ӧ������У�飬֮�����Ľ��д��WkeyID2λ�á�
������TMK�������´��µ�����TMK2��������TMK1��TMK2���ܣ�����TMK2Ӧ������У�飬֮������TMK2
д��WkeyID2λ�ã���WkeyID1��WkeyID2���ʱ������������TMK1��
����TPK�������´��µ�����TPK����WkeyID1����TMK������ܣ�����TPKӦ������У�飬֮������TPK
д��WkeyID2λ�á���
Len=11.
��������bΪ������WkeyID1(1byte) +WkeyID2(1byte)+TMK2(8bytes)��
*/
uchar EPSPPLoadKey(uchar WKeyID1,uchar WKeyID2, uchar *KKEY);


/*
POS����8bytes Random, ��Ӧ������У�飬��WkeyID1����TMK������ܣ�����������
Len=10.
��������bΪ�� WKeyID(1byte) + ����Random(8bytes)
���أ�����Random(8bytes)
*/
uchar EPSPPGetMAC1(uchar WKeyID, uchar *Random);



/*
POS����8bytes pin_block,�µ�ISN,�µ�TPK,ԭTPK Wkey_ID2��TMK Wkey_ID1��PINPAD��
ԭ��TPK��pin_block��X3.92���ܵõ�����PIN���ٽ���TPK��TMK���ܣ�����TPKӦ������У�飬
��TPK�滻ԭTPK��֮��������TPK��ISN��PIN��X3.92���ܣ������µ�pin_block.
Len=25.
��������bΪ�� Wkey_ID1+Wkey_ID2+pin_block(8byte)+ISN(6byte)+ new_TPK(8bytes)
���أ��µ�pin_block(8bytes)��
*/
uchar EPSPPUpdateTPK(uchar WKeyID1,uchar WKeyID2, uchar *pin_block, uchar *ISN, uchar *new_TPK);


/*
��WkeyID1����TMK�������´����µ����Ľ��ܣ�����Ӧ������У�飬
�������������MAC���㣬�����������MAC����ֵ���(ǰ4byte)��֮�����Ľ��MAC2д��WkeyID2λ�á�
Len=rsp_Len+20.
��������bΪ��WkeyID1(1byte) +WkeyID2(1byte)+rsp_len(1byte)+MAC2����+(8bytes)+rsp_data(rsp_len)+rsp_MAC(8byte)��
*/
uchar EPSPPSetMAC2(uchar WKeyID1,uchar WKeyID2,uchar *MAC2, uchar *rsp_data, uchar rsp_data_len, uchar *rsp_MAC);


uchar EPSPPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *ISN, uchar *pin);


/*
��ԭTMK��"\x12\x34\x56\x78\x90\x12\x34\x56"���ܣ������ؽ����
Len=2.
��������bΪ��WkeyID1(1byte)��
�������ݣ����ܽ����8byte��.
*/
uchar EPSPPVerifyTMK(uchar WKeyID, uchar *VerifyData);

/*
  ��������     ��ȡ���ܵ�����[�������Զ�����㷨:�����Ҷ��룬�á�F'��䣬��ʽ��FFFFFFFFFFPPPPPP��]
  PinKeyID     ����PIN�Ĺ�����Կ(������Կ��)
  min          �����������С����[<=6, <=max]
  max          �����������󳤶�[<=6]
  bPwdLen	   �����PIN����
  pin          ���������[����]
*/
uchar SYLPPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *bPinLen,uchar *pin_block);

/*
��������		��MKeyIDָ��������Կ��new_WKey����mode��ʽ�ļӽ��ܣ��õ�����Կ���ô���Կ��
				"\x00\x00\x00\x00\x00\x00\x00\x00"��DES���ܣ����������ԿУ�����ݱȽϣ����
				��ͬ������Կд��WkeyIDָ���Ĺ�����Կ����
MKeyID			�洢����Կ������
WKeyID			��ɢ�����Կ�洢����
new_WKey    	������֤�ͷ�ɢ����Կ[8Bytes������]
KeyCv	   	   	��ԿУ������[8Bytes]��
mode         	�ӽ���ģʽ�����嶨�����£�
      				0x01  DES����
      				0x81  DES����
*/
uchar SYLPPVerifyDerive(uchar MKeyID, uchar WKeyID, uchar *new_WKey, uchar *KeyCv, uchar mode);

/*
��������		����PinKeyIDָ���Ĺ�����Կ������pin_block���н��ܣ�Ȼ����MKeyIDָ��������Կ��
				new_TPK����mode��ʽ�ļӽ��ܣ��õ�����Կ���ô���Կ������pin_block���ܡ�
MKeyID			�������Կ[TMK]������
PinKeyID		���PIN��Կ[TPK]������
pin_block		PIN��������[8Bytes������]
new_TPK    		�µ�PIN��Կ[8Bytes������]
mode         	�ӽ���ģʽ�����嶨�����£�
      				0x01  DES����
      				0x81  DES����
*/
uchar SYLPPCalcPinBlock(uchar MKeyID,uchar PinKeyID,uchar *pin_block,uchar *new_TPK,uchar mode);


/*
��������		�õ�IC���������������룬Pos,PP20����Ĺ����в������������
	min          �����������С����
  max          �����������󳤶�
  pin_block    �����������
*/
uchar PPEmvGetPwd(uchar min, uchar max,uchar *pin_block);


/*
  ��������     ��ȡANSI9.8���ܵ�����(16�ֽ�3des���ܵ�PIN)
  TwiceInput=1 PINPAD������ʾ����2��
  TwiceInput=0 PINPAD������ʾ����1��
  PinKeyID     ����PIN�Ĺ�����Կ(������Կ��)
  min          �����������С����
  max          �����������󳤶�
  pin          ���ܵ��������
  cardno       ������λ�����16λ���ţ�ASCII�룩
  mode:
  mode=0x31		16�ֽ�3des����
   
*/
uchar PPGetPwd_3Des(uchar PinKeyID, uchar mode,uchar min, uchar max, uchar *cardno, uchar *pin, uchar TwiceInput);


uchar PPQuickBeepTest(void);
uchar PPCancel(void);

#endif
