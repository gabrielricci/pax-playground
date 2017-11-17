#ifndef __UI_BASE_H__
#define __UI_BASE_H__

/* ui return value */
#define UI_RET_OK                   0  
#define UI_ERR_BASE                 (-3100)
#define UI_ERR_INVALID_PARAM        (UI_ERR_BASE - 1) 
#define UI_ERR_PAGE_USED_UP         (UI_ERR_BASE - 2)
#define UI_ERR_FILE_NOT_EXIST       (UI_ERR_BASE - 3)
#define UI_ERR_INVALID_ELEMENT      (UI_ERR_BASE - 4)
#define UI_ERR_INVALID_ATTRIBUTE    (UI_ERR_BASE - 5)
#define UI_ERR_NO_RESULT            (UI_ERR_BASE - 6)
#define UI_RET_MEM_RUN_OUT          (UI_ERR_BASE - 7)

/* pdk ui function */
int PdkUI_Init(void);
int PdkUI_Load(unsigned int uiChannel, const char *pszXmlFileName);
int PdkUI_SetAttribute(unsigned int uiChannel, const char *pszName, const char *pszAttr, const char *pszValue);
int PdkUI_Process(unsigned int uiChannel, int iTimeout, unsigned char *psDataOut, unsigned int uiDataSize, int *puiLenOut);
int PdkUI_RequestAction(unsigned int uiChannel, const char *pszName, const char *pszAction, char *pszValue);
int PdkUI_Flush(unsigned int uiChannel);
int PdkUI_Free(void);
//¥Ú”°
int Print_Receipt(char * pszAmount,char *pszCardNo,int iSignFlag,int iReadCardType);

#endif /* __UI_BASE_H__ */
