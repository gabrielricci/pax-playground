#ifndef RESOURCE_H
#define RESOURCE_H

#define SCREEN_WIDTH 240
#define SCREEN_HEIGH 320

#define STATUS_HEIGH 20
#define BOTTOM_HEIGH 60

//status pic id
#define IDC_STATUE_BATTERY				90
#define IDC_STATUE_GPRS					91
#define IDC_STATUE_PRINTER				92
#define IDC_STATUE_IC					93
#define IDC_STATUE_BLUETOOTH			94
#define IDC_STATUE_TIME                 95

//Home.c
#define IDC_HOME_BK_PIC					100
#define IDC_HOME_SWIPING_CARD			102
#define IDC_HOME_WIFI					103
#define IDC_HOME_TOOLS					104
#define IDC_HOME_DETAILS				105
#define IDC_HOME_LOGO					106
#define IDC_UPDATA_TIME_ID              107
#define IDC_HOME_SCREEN_SAVER_TIMER		108

//input_amount.c
#define IDC_INPUT_AMONUT_BK             201
#define IDC_INPUT_AMONUT_PIC            202
#define IDC_INPUT_AMOUNT_EDIT           203
#define IDC_INPUT_AMOUNT_FORWARD        204
#define IDC_INPUT_AMOUNT_BACKWARD       205
#define IDC_INPUT_AMOUNT_TIMEOUT_ID     206

//swipe_card.c
#define IDC_SWIPE_CARD_BK				301
#define IDC_SWIPE_CARD_LOGO				302
#define IDC_SWIPE_CARD_PIC				303
#define IDC_SWIPE_CARD_BACKWARD			304
#define IDC_SWIPE_CARD_TIMEOUT_ID		305
#define IDC_SWIPE_CARD_TIMER        	306
#define IDC_SWIPE_CARD_SCREEN_TIMER		315

#define IDC_SWIPE_CARD_RED				307
#define IDC_SWIPE_CARD_RED_EMPTY		308
#define IDC_SWIPE_CARD_YELLOW			309
#define IDC_SWIPE_CARD_YELLOW_EMPTY		310
#define IDC_SWIPE_CARD_BLUE				311
#define IDC_SWIPE_CARD_BLUE_EMPTY		312
#define IDC_SWIPE_CARD_GREEN        	313
#define IDC_SWIPE_CARD_GREEN_EMPTY      314

//confirm_account.c
#define IDC_CONFIRM_ACCOUNT_BK          401
#define IDC_CONFIRM_ACCOUNT_LOGO		402
#define IDC_CONFIRM_ACCOUNT_PIC         403
#define IDC_CONFIRM_ACCOUNT		        404
#define IDC_CONFIRM_ACCOUNT_FORWARD     405
#define IDC_CONFIRM_ACCOUNT_BACKWARD    406
#define IDC_CONFIRM_ACCOUNT_TIMEOUT_ID  407
//input_pin.c
#define IDC_INPUT_PIN_BK				501
#define IDC_INPUT_PIN_PIC				502
#define IDC_INPUT_PIN_EDIT				503
#define IDC_INPUT_PIN_LOGO				504
#define IDC_INPUT_PIN_FORWARD			505
#define IDC_INPUT_PIN_BACKWARD			506
#define IDC_INPUT_PIN_TIMEOUT_ID		507
//pin_error.c
#define IDC_PIN_ERROR_BK				601
#define IDC_PIN_ERROR_LOGO              605
#define IDC_PIN_ERROR_PIC				602
#define IDC_PIN_ERROR_BACKWARD			603
#define IDC_PIN_ERROR_TIMEOUT_ID		604
//processing.c
#define IDC_PROCESSING_BK				701
#define IDC_PROCESSING_PIC				702
#define IDC_PROCESSING_LOGO             703
#define IDC_PROCESSING_TIMEOUT_ID		704
//printing.c
#define IDC_PRINTING_BK					801
#define IDC_PRINTING_LOGO               802
#define IDC_PRINTING_PIC				803
#define IDC_PRINTING_TIMER				804
#define IDC_PRINTING_FAILE_TIMER		805
//out_off_paper.c
#define IDC_OUT_OFF_PAPER_BK			901
#define IDC_OUT_OFF_PAPER_LOGO          902
#define IDC_OUT_OFF_PAPER_PIC		    903
#define IDC_OUT_OFF_PAPER_BACKWARD		904
#define IDC_OUT_OFF_PAPER_FORWARD		905
#define IDC_OUT_OFF_PAPER_TIMEOUT_ID	906
//complete.c
#define IDC_COMPLETE_BK				    1001
#define IDC_COMPLETE_PIC				1002
#define IDC_COMPLETE_LOGO               1003
#define IDC_COMPLETE_FORWARD            1004
#define IDC_COMPLETE_TIMEOUT_ID		    1005
//signature.c
#define IDC_SIGNATURE_BK          1101
#define IDC_SIGNATURE_BUTTON1     1102
#define IDC_SIGNATURE_BUTTON2     1103
#define IDC_SIGNATURE_BUTTON3     1104
#define IDC_SIGNATURE_DRAW_BOARD  1105
#define IDC_SIGNATURE_ACCOUNT     1106
#define IDC_SIGNATURE_AMOUNT      1107
#define IDC_SIGNATURE_AUTH        1108
#define IDC_SIGNATURE_TIP		  1109
#define IDC_SIGNATURE_CLEAN_TIMER 1110
//menu.c
#define IDC_MENU_BK_PIC					143
#define IDC_MENU_PIC					144
#define IDC_MENU_LEFT					145
#define IDC_MENU_UP						146
#define IDC_MENU_DOWN					147
#define IDC_MENU_RIGHT					148
#define IDC_MENU_1						149
#define IDC_MENU_2						150
#define IDC_MENU_3						151
#define IDC_MENU_4						152
#define IDC_MENU_TIMEOUT				153
//system_test.c
#define IDC_SYSTEM_TEST_BK				154
#define IDC_SYSTEM_TEST_PIC				155
#define IDC_SYSTEM_TEST_TIMEOUT_ID		156
//system_test.c
#define IDC_DOWNLOADING_BK				157
#define IDC_DOWNLOADING_PIC				158
//tools.c
#define IDC_TOOLS_BK					159
#define IDC_TOOLS_PIC					160
#define IDC_TOOLS_TIME_PIC				161
#define IDC_TOOLS_UPLOADLOG_PIC			162
//upload_log.c
#define IDC_UPLOADLOG_BK                163
#define IDC_UPLOADLOG_PIC               164
#define IDC_UPLOADLOG_LIST              165
//voice_set.c
#define IDC_VOICE_SET_BK                166
#define IDC_VOICE_SET_PIC               167
#define IDC_VOICE_SET_TRACEBAR          168
//time_set.c
#define IDC_TIMESET_EDIT1     200
#define IDC_TIMESET_EDIT2     201
#define IDC_TIMESET_EDIT3     202
#define IDC_TIMESET_EDIT4     203
#define IDC_TIMESET_EDIT5     204
#define IDC_TIMESET_STATIC1   205
#define IDC_TIMESET_STATIC2   206
#define IDC_TIMESET_STATIC3   207
#define IDC_TIMESET_STATIC4   208
#define IDC_TIMESET_STATIC5   209

#define IDC_DESKTOP_BACKWARD	210
#define IDC_DESKTOP_FORWARD	211

//ScreenSave1.c
#define ID_SCREEN_SAVER_1_TIMER		1201
#define ID_SCREEN_SAVER_1_PIC		1202

//ScreenSave2.c
#define ID_SCREEN_SAVER_2_TIMER		1301
#define ID_SCREEN_SAVER_2_PIC		1302

//ScreenSave3.c
#define ID_SCREEN_SAVER_3_TIMER		1401
#define ID_SCREEN_SAVER_3_PIC		1402

//ScreenSave4.c
#define ID_SCREEN_SAVER_4_TIMER		1601
#define ID_SCREEN_SAVER_4_PIC		1602

//exit_confirm.c
#define ID_EXIT_CONFIRM_TIMER		1501
#define ID_EXIT_CONFIRM_ENTER       1502
#define ID_EXIT_CONFIRM_CANCEL      1503

#endif 
