#ifndef _PIN_PAD_H
#define _PIN_PAD_H

#define PP_OPEN         0x01       
#define PP_CLOSE        0x00
#define PP_GREEN        0x01
#define PP_RED          0x02
#define PP_YELLOW	    0x04

uchar PPBaudRate(long Baudrate);

// Set Pininput Time //精度为0.01秒
uchar PPSetTimeout(unsigned short timeout);

// beep
uchar PPBeep(void);

/*
  OperMode=O_OPEN  打开PINPAD指示灯
  OperMode=O_CLOSE 关闭PINPAD指示灯
  Index          可以为以下值的任意组合:
       RED       红灯
       GREEN     绿灯
       YELLOW    黄灯
*/
uchar PPLight(uchar OperMode, uchar Index);

/*
  OperMode=O_OPEN  打开PINPAD语音提示
  OperMode=O_CLOSE 关闭PINPAD语音提示
*/
uchar PPInvoice(uchar OperMode);

/*
  OperMode=O_OPEN  打开PINPAD按键音
  OperMode=O_CLOSE 关闭PINPAD按键音
*/
uchar PPKbmute(uchar OperMode);


/*
  OperMode=O_OPEN  打开背光
  OperMode=O_CLOSE 关闭背光
*/
uchar PPBackLight(uchar OperMode);

//Input for reserve
/*
  函数功能  输入字符串
  str       输入的字符串
  min       可输入最小长度
  max       可输入最大长度
  mode=0    明码输入
  mode=1    密码输入,显示'*'
*/
uchar PPInput(uchar *str, uchar min, uchar max, uchar mode);

// LCD Ctrl
// Clear screen
uchar PPScrCls(void);


/*
  函数功能  清除行
  line      行(0 - 1)
*/
uchar PPScrClrLine(uchar line);


/*
  函数功能  显示字符串(汉字和英文）
  line      行(0 - 1)
  col       列(0 - 113)
  str       显示的字符串
*/
uchar PPScrPrint(uchar line, uchar col, uchar *str);

/*
  函数功能  显示点阵
  line      行(0 - 1)
  col       列(0 - 121)
  Data      显示数据(数据格式参考协议)
  len       显示数据长度
*/
uchar PPScrWrData(uchar line, uchar col, uchar *Data, uchar len);


/*key magage
  所有的密钥索引范围： 1 - 100
*/
/*
  函数功能  写入主密钥密钥
  KeyID     密钥索引(主密钥区）
  Key       密钥值
  mode=0x01 DES密钥
  mode=0x03 3DES或SEED密钥(16bytes des 密钥)
  mode=0x07	3DES或SEED密钥(24bytes des 密钥)
*/
uchar PPWriteMKey(uchar KeyID, uchar mode, uchar *Key);


/*
  函数功能  写入DES(3DES)计算的密钥
  KeyID     密钥索引(DES密钥区）
  Key       密钥值
  mode=0x01 DES密钥
  mode=0x03 3DES或SEED密钥(16bytes des 密钥)
  mode=0x07	3DES或SEED密钥(24bytes des 密钥)
*/
uchar PPWriteDKey(uchar DESKeyID, uchar mode, uchar *Key);


/*
  函数功能  写入工作密钥
  MKeyID    用该索引指定的主密钥对Key进行mode方式的计算后再写入,MKeyID=0,不进行任何计算,直接写入
  WKeyID    写入的地址(工作密钥区)
  Key       密钥
  Mode=0x01  DES加密
	Mode=0x81  DES解密 
	该模式下，MKeyID与WKeyID均为单DES密钥。
	Mode=0x03  3DES加密
	Mode=0x83  3DES解密
	该模式下，MKeyID为3DES－16字节密钥， WKeyID为8字节密钥。
	Mode=0x07  3DES加密
	Mode=0x87  3DES解密
	该模式下，MKeyID为3DES－24字节密钥， WKeyID为8字节密钥。
	
	Mode=0x31  3DES加密
	Mode=0xb1  3DES解密
	该模式下，MKeyID为3DES－16字节密钥, WKeyID为8字节密钥。
	Mode=0x33  3DES加密
	Mode=0xb3  3DES解密
	该模式下，MKeyID与WKeyID均为3DES－16字节密钥，并采用EBC三重加解密方式。
	
	Mode=0x71  3DES加密
	Mode=0xf1  3DES解密
	该模式下，MKeyID为3DES－24字节密钥, WKeyID为8字节密钥。
	Mode=0x73  3DES加密
	Mode=0xf3  3DES解密
	该模式下，MKeyID为3DES－24字节密钥, WKeyID为3DES－16字节密钥，并采用EBC三重加解密方式。
	Mode=0x77  3DES加密
	Mode=0xf7  3DES解密
	该模式下，MKeyID与WKeyID均为3DES－24字节密钥，并采用EBC三重加解密方式。

*/
uchar PPWriteWKey(uchar MKeyID, uchar WKeyID, uchar mode, uchar *Key);


/*
  函数功能  分散密钥
  KeyID     需进行分散的密钥,高位为1（即0x80)指定的工作密钥区密钥，否则为主密钥区密钥
  WKeyID1   分散数据(工作密钥区)
  WKeyID2   分散结果(工作密钥区)
  Mode=0x01  DES加密
	Mode=0x81  DES解密 
	该模式下，MKeyID与WKeyID1、WKeyID2均为单DES密钥。
	Mode=0x03  3DES加密
	Mode=0x83  3DES解密
	该模式下，MKeyID为3DES－16字节密钥, WKeyID1、WKeyID2为8字节密钥。
	Mode=0x07  3DES加密
	Mode=0x87  3DES解密
	该模式下，MKeyID为3DES－24字节密钥, WKeyID1、WKeyID2为8字节密钥。
	
	Mode=0x31  3DES加密
	Mode=0xb1  3DES解密
	该模式下，MKeyID为3DES－16字节密钥, WKeyID1、WKeyID2为8字节密钥。
	Mode=0x33  3DES加密
	Mode=0xb3  3DES解密
	该模式下，MKeyID与WKeyID1、WKeyID2均为3DES－16字节密钥，并采用EBC三重加解密方式。
	
	Mode=0x71  3DES加密
	Mode=0xf1  3DES解密
	该模式下，MKeyID为3DES－24字节密钥, WKeyID1、WKeyID2为8字节密钥。
	Mode=0x73  3DES加密
	Mode=0xf3  3DES解密
	该模式下，MKeyID为3DES－24字节密钥, WKeyID1、WKeyID2为3DES－16字节密钥，并采用EBC三重加解密方式。
	Mode=0x77  3DES加密
	Mode=0xf7  3DES解密
	该模式下，MKeyID与WKeyID1、WKeyID2均为3DES－24字节密钥，并采用EBC三重加解密方式。

*/
uchar PPDeriveKey(uchar MKeyID, uchar WKeyID1, uchar WKeyID2, uchar mode);


/*
  函数功能     读取ANSI9.8加密的密码
  TwiceInput=1 PINPAD语音提示输入2次
  TwiceInput=0 PINPAD语音提示输入1次
  PinKeyID     计算PIN的工作密钥(工作密钥区)
  min          密码可输入最小长度
  max          密码可输入最大长度
  pin          加密的密码输出
  cardno       经过移位处理的16位卡号（ASCII码）
*/
uchar PPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *cardno, uchar *pin,uchar TwiceInput);

/*
  函数功能     读取ANSI9.8加密的密码
  TwiceInput=1 PINPAD语音提示输入2次
  TwiceInput=0 PINPAD语音提示输入1次
  PinKeyID     计算PIN的工作密钥(工作密钥区)
  min          密码可输入最小长度
  max          密码可输入最大长度
  pin          加密的密码输出
  cardno       经过移位处理的16位卡号（ASCII码）
  mode:
  mode=0x01		des加密
  mode=0x03		16字节3des加密
  mode=0x07 	24字节3des加密		
  
	
*/
uchar Ex_PPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *cardno, uchar *pin,uchar TwiceInput,uchar mode);


/*
  函数功能  DES(3DES)计算
  DESKeyID  计算DES的密钥(DES密钥区)
  datain    输入数据
  macout    输出数据
  mode=0x01  DES加密   [密钥为8字节]
	mode=0x81  DES解密   [密钥为8字节]
	mode=0x03  3DES加密  [密钥为16字节]
	mode=0x83  3DES解密　[密钥为16字节]
	mode=0x07  3DES加密　[密钥为24字节]
	mode=0x87  3DES解密　[密钥为24字节]

*/
uchar PPDes(uchar DESKeyID, uchar mode, uchar *datain, uchar *dataout);
/*
  函数功能  MAC计算
  flag=0    算法1
  flag=1    算法2
  WKeyID    计算MAC的工作密钥(工作密钥区)
  inLen     计算MAC的数据长度
  datain    输入数据
  macout    输出数据(即MAC)
  mode=0x01 DES加密
  mode=0x03 3DES加密(16bytes Des)
  mode=0x07 3DES加密(24bytes Des)
*/
uchar PPMac(uchar WKeyID,uchar mode, uchar *datain, ushort inLen, uchar *macout,uchar flag);

uchar PPVerInfo(uchar *ver);

uchar PPUpdLogo(uchar line, uchar *Data);





//以下为HONG KONG EPS 专用


/*注释：
开始序号：显示内容的开始序号；
结束序号：显示内容的结束序号；
行数：显示内容的总行数；
显示内容：为[行数][16BYTE]的数组。多页显示时用F2，F3翻页。
返回序号：返回的序号，在开始序号、结束序号之间。
当显示金额时，开始序号=0x3d（取消）;结束序号=0x3e（确认）。
可以按POS上（取消）退出。
*/
uchar EPSPPDispMenu(uchar StartNo, uchar EndNo, uchar AllLines, uchar detail[][16], uchar *item);


/*
min：字符串最小长度(为0时，按ENTER键返回，且输入长度Len=0)。
max：字符串最大长度(小于等于16字节)。
Mode：0X00:小数点后有两位小数；
       非0:小数点后有三位小数；
*/
//uchar EPSPPAmount(uchar min, uchar max, uchar mode,uchar *Amt);
uchar EPSPPAmount(uchar point_mode,uchar prompt_mode, uchar min, uchar max,uchar *Amt);

void ConvertEPSPPAmount(uchar point_mode,uchar max_len,uchar *Amt);

/*
由POS产生临时KEY，上传主机，取回密文TMK1和临时加密参考值，由临时KEY对密文TMK1解密，得TMK，
明文TMK应符合奇校验；TMK对"\x12\x34\x56\x78\x90\x12\x34\x56"加密，加密后的结果应与加密参考
值相等；之后将明文TMK写入WkeyID位置。
Len=26.
输入内容b为：WkeyID1(1byte)+临时KEY(8bytes)+密文TMK1(8bytes)+加密参考值(8bytes)。
*/
uchar EPSPPLoadTMK(uchar WkeyID1, uchar *TmpKey, uchar *KTMK, uchar *ReferCode);


/*
用WkeyID1处的TMK对主机下传的新的密文解密，明文应符合奇校验，之后将明文结果写入WkeyID2位置。
（更新TMK：主机下传新的密文TMK2，用明文TMK1对TMK2解密，明文TMK2应符合奇校验，之后将明文TMK2
写入WkeyID2位置，当WkeyID1与WkeyID2相等时，即覆盖明文TMK1。
下载TPK：主机下传新的密文TPK，用WkeyID1处的TMK对其解密，明文TPK应符合奇校验，之后将明文TPK
写入WkeyID2位置。）
Len=11.
输入内容b为：密文WkeyID1(1byte) +WkeyID2(1byte)+TMK2(8bytes)。
*/
uchar EPSPPLoadKey(uchar WKeyID1,uchar WKeyID2, uchar *KKEY);


/*
POS送入8bytes Random, 其应符合奇校验，用WkeyID1处的TMK对其加密，返回其结果。
Len=10.
输入内容b为： WKeyID(1byte) + 明文Random(8bytes)
返回：密文Random(8bytes)
*/
uchar EPSPPGetMAC1(uchar WKeyID, uchar *Random);



/*
POS送入8bytes pin_block,新的ISN,新的TPK,原TPK Wkey_ID2，TMK Wkey_ID1，PINPAD用
原的TPK对pin_block按X3.92解密得到明文PIN，再将新TPK用TMK解密，明文TPK应符合奇校验，
新TPK替换原TPK，之后用明文TPK，ISN对PIN按X3.92加密，返回新的pin_block.
Len=25.
输入内容b为： Wkey_ID1+Wkey_ID2+pin_block(8byte)+ISN(6byte)+ new_TPK(8bytes)
返回：新的pin_block(8bytes)。
*/
uchar EPSPPUpdateTPK(uchar WKeyID1,uchar WKeyID2, uchar *pin_block, uchar *ISN, uchar *new_TPK);


/*
用WkeyID1处的TMK对主机下传的新的密文解密，明文应符合奇校验，
对输入的数据做MAC运算，其结果与输入的MAC运算值相等(前4byte)，之后将明文结果MAC2写入WkeyID2位置。
Len=rsp_Len+20.
输入内容b为：WkeyID1(1byte) +WkeyID2(1byte)+rsp_len(1byte)+MAC2密文+(8bytes)+rsp_data(rsp_len)+rsp_MAC(8byte)。
*/
uchar EPSPPSetMAC2(uchar WKeyID1,uchar WKeyID2,uchar *MAC2, uchar *rsp_data, uchar rsp_data_len, uchar *rsp_MAC);


uchar EPSPPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *ISN, uchar *pin);


/*
用原TMK对"\x12\x34\x56\x78\x90\x12\x34\x56"加密，并返回结果。
Len=2.
输入内容b为：WkeyID1(1byte)。
返回内容：加密结果（8byte）.
*/
uchar EPSPPVerifyTMK(uchar WKeyID, uchar *VerifyData);

/*
  函数功能     读取加密的密码[深银联自定义的算法:密码右对齐，用‘F'填充，格式“FFFFFFFFFFPPPPPP”]
  PinKeyID     计算PIN的工作密钥(工作密钥区)
  min          密码可输入最小长度[<=6, <=max]
  max          密码可输入最大长度[<=6]
  bPwdLen	   输出的PIN长度
  pin          输出的密码[密文]
*/
uchar SYLPPGetPwd(uchar PinKeyID, uchar min, uchar max, uchar *bPinLen,uchar *pin_block);

/*
函数功能		用MKeyID指定的主密钥对new_WKey进行mode方式的加解密，得到新密钥，用此密钥对
				"\x00\x00\x00\x00\x00\x00\x00\x00"作DES加密，将结果与密钥校验数据比较，如果
				相同则将新密钥写入WkeyID指定的工作密钥区。
MKeyID			存储主密钥的索引
WKeyID			发散后的密钥存储索引
new_WKey    	用来验证和发散的密钥[8Bytes，密文]
KeyCv	   	   	密钥校验数据[8Bytes]。
mode         	加解密模式，具体定义如下：
      				0x01  DES加密
      				0x81  DES解密
*/
uchar SYLPPVerifyDerive(uchar MKeyID, uchar WKeyID, uchar *new_WKey, uchar *KeyCv, uchar mode);

/*
函数功能		先用PinKeyID指定的工作密钥对密文pin_block进行解密，然后用MKeyID指定的主密钥对
				new_TPK进行mode方式的加解密，得到新密钥，用此密钥对明文pin_block加密。
MKeyID			存放主密钥[TMK]的索引
PinKeyID		存放PIN密钥[TPK]的索引
pin_block		PIN加密数据[8Bytes，密文]
new_TPK    		新的PIN密钥[8Bytes，密文]
mode         	加解密模式，具体定义如下：
      				0x01  DES加密
      				0x81  DES解密
*/
uchar SYLPPCalcPinBlock(uchar MKeyID,uchar PinKeyID,uchar *pin_block,uchar *new_TPK,uchar mode);


/*
函数功能		得到IC卡的明文密码输入，Pos,PP20传输的过程中采用随机数加密
	min          密码可输入最小长度
  max          密码可输入最大长度
  pin_block    明文密码输出
*/
uchar PPEmvGetPwd(uchar min, uchar max,uchar *pin_block);


/*
  函数功能     读取ANSI9.8加密的密码(16字节3des加密的PIN)
  TwiceInput=1 PINPAD语音提示输入2次
  TwiceInput=0 PINPAD语音提示输入1次
  PinKeyID     计算PIN的工作密钥(工作密钥区)
  min          密码可输入最小长度
  max          密码可输入最大长度
  pin          加密的密码输出
  cardno       经过移位处理的16位卡号（ASCII码）
  mode:
  mode=0x31		16字节3des加密
   
*/
uchar PPGetPwd_3Des(uchar PinKeyID, uchar mode,uchar min, uchar max, uchar *cardno, uchar *pin, uchar TwiceInput);


uchar PPQuickBeepTest(void);
uchar PPCancel(void);

#endif
