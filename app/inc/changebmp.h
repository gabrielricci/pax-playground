/*
 ============================================================================
 Name        : changebmp.h
 Author      : Rex
 Version     : 1.0.0
 Copyright   : PAX Computer Technology(Shenzhen) CO., LTD
 Description : EMV transaction
 History     :
 1.0.0 Nov 06, 2012 Rex
 ============================================================================
 */

#ifndef _CHANGEBMP_H
#define _CHANGEBMP_H

/*************************************************************************************
浠�.BMP涓烘墿灞曞悕鐨勫浘鐗囩敱浠ヤ笅涓変釜閮ㄥ垎缁勬垚:

1.浣嶅浘鏂囦欢澶�(BITMAPFILEHEADER) 鏁版嵁缁撴瀯
2.浣嶅浘淇℃伅(BITMAPINFO)鏁版嵁缁撴瀯
3.浣嶅浘闃靛垪

褰揵iBitCount=1鏃讹紝8涓儚绱犲崰1涓瓧鑺�
褰揵iBitCount=4鏃讹紝2涓儚绱犲崰1涓瓧鑺�
褰揵iBitCount=8鏃讹紝1涓儚绱犲崰1涓瓧鑺�
褰揵iBitCount=24鏃�,1涓儚绱犲崰3涓瓧鑺�
褰揵iBitCount=32鏃�,1涓儚绱犲崰4涓瓧鑺�

娉ㄦ剰:瀛樺湪瀛楄妭瀵归綈鐨勯棶棰�
*************************************************************************************/

typedef struct tagBITMAP_FILE_HEADER
{
	unsigned short 	bfType;	//琛ㄧず鏂囦欢鐨勭被鍨嬶紝璇ュ�煎繀闇�鏄�4D42锛屼篃灏辨槸瀛楃'BM'
	unsigned int	bfSize;	//琛ㄧず璇ヤ綅鍥炬枃浠剁殑澶у皬锛岀敤瀛楄妭涓哄崟浣�
	unsigned short 	bfReserved1;//淇濈暀锛屽繀椤昏缃负0
	unsigned short 	bfReserved2;//淇濈暀锛屽繀椤昏缃负0
	unsigned int 	bfOffBits;	//琛ㄧず浠庢枃浠跺ご寮�濮嬪埌瀹為檯鐨勫浘璞℃暟鎹箣闂寸殑瀛楄妭鐨勫亸绉婚噺銆傝繖涓弬鏁版槸闈炲父鏈夌敤鐨勶紝鍥犱负浣嶅浘淇℃伅澶村拰璋冭壊鏉跨殑闀垮害浼氭牴鎹笉鍚屾儏鍐佃�屽彉鍖栵紝鎵�浠ヤ綘鍙互鐢ㄨ繖涓亸绉诲�艰繀閫熺殑浠庢枃浠朵腑璇诲彇鍒颁綅鏁版嵁
} BITMAP_FILE_HEADER;

#define FILE_HEADER_LEN 14

typedef struct tagBITMAP_INFO_HEADER
{
	unsigned int biSize;		//琛ㄧずBITMAPINFOHEADER缁撴瀯鎵�闇�瑕佺殑瀛楄妭鏁�
	unsigned int biWidth;		//琛ㄧず鍥捐薄鐨勫搴︼紝浠ヨ薄绱犱负鍗曚綅
	unsigned int biHeight;		//琛ㄧず鍥捐薄鐨勯珮搴︼紝浠ヨ薄绱犱负鍗曚綅,骞跺�掑悜姝ｅ悜鐨勬爣蹇�
	unsigned short biPlanes;	//涓虹洰鏍囪澶囪鏄庝綅闈㈡暟锛屽叾鍊煎皢鎬绘槸琚涓�
	unsigned short biBitCount;	//琛ㄧず姣旂壒鏁�/璞＄礌锛屽叾鍊间负1銆���銆�銆佹垨32
	unsigned int biCompression;	//琛ㄧず鍥捐薄鏁版嵁鍘嬬缉鐨勭被鍨�
	unsigned int biSizeImage;	//琛ㄧず鍥捐薄鐨勫ぇ灏忥紝浠ュ瓧鑺備负鍗曚綅銆傚綋鐢˙I_RGB鏍煎紡鏃讹紝鍙缃负0
	unsigned int biXPelsPerMeter;//琛ㄧず姘村钩鍒嗚鲸鐜囷紝鐢ㄨ薄绱�/绫宠〃绀�
	unsigned int biYPelsPerMeter;//琛ㄧず鍨傜洿鍒嗚鲸鐜囷紝鐢ㄨ薄绱�/绫宠〃绀�
	unsigned int biClrUsed;		 //琛ㄧず浣嶅浘瀹為檯浣跨敤鐨勫僵鑹茶〃涓殑棰滆壊绱㈠紩鏁帮紙璁句负0鐨勮瘽锛屽垯璇存槑浣跨敤鎵�鏈夎皟鑹叉澘椤癸級
	unsigned int biClrImportant; //琛ㄧず瀵瑰浘璞℃樉绀烘湁閲嶈褰卞搷鐨勯鑹茬储寮曠殑鏁扮洰锛屽鏋滄槸0锛岃〃绀洪兘閲嶈
} BITMAP_INFO_HEADER;

#define INFO_HEADER_LEN 40

//bimColor[]鏄竴涓鑹茶〃,鐢ㄤ簬璇存槑浣嶅浘涓殑棰滆壊.瀹冩湁鑻ュ共涓〃椤�,姣忎竴琛ㄩ」鏄竴涓猂GBQUAD绫诲瀷鐨勭粨鏋�,瀹氫箟浜嗕竴绉嶉鑹�.RGBQUAD鐨勫畾涔�
//棰滆壊琛ㄧ敤浜庤鏄庝綅鍥句腑鐨勯鑹�,鐪熷僵鑹插浘锛�浣嶇殑BMP锛夊氨涓嶉渶瑕侀鑹茶〃锛屽洜涓轰綅鍥句腑鐨凴GB鍊煎氨浠ｈ〃浜嗘瘡涓薄绱犵殑棰滆壊銆備絾鏄�浣峳5g6b5浣嶅煙褰╄壊鍥惧儚闇�瑕侀鑹茶〃
typedef struct tagRGBQUAD
{
	unsigned char rgbBlue;	//鍊艰寖鍥翠负0-255
	unsigned char rgbGreen;	//鍊艰寖鍥翠负0-255
	unsigned char rgbRed;	//鍊艰寖鍥翠负0-255
	unsigned char rgbReserved;	//淇濈暀锛屽繀椤讳负0
}RGBQUAD;

#define RGBQUAD_LEN 4

//浣嶅浘淇℃伅鏁版嵁缁撴瀯鍚湁浣嶅浘鏂囦欢鐨勫昂瀵稿拰棰滆壊绛変俊鎭�
typedef struct tagBITMAPINFO
{
	BITMAP_INFO_HEADER bmiHeader;
	RGBQUAD bmiColor[1];
}BITMAP_INFO;

//鏂囦欢淇℃伅澶村拰浣嶅浘淇℃伅缁勬垚浣嶅浘鏂囦欢锛孊ITMAPFILE缁撴瀯瀹氫箟濡備笅
typedef struct tagBITMAP
{
	BITMAP_FILE_HEADER bfHeader;
	BITMAP_INFO biInfo;
}BITMAP_FILE;


#ifdef __cplusplus
extern "C"
{
#endif

	int Change24BmpToGrey(char *changefilename, char *greyfilename);

#ifdef __cplusplus
};
#endif
#endif /* _CHANGEBMP_H */

