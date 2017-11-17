/*------------------------------------------------------------
* FileName: keyboard.h
* Author: LuX
* Date: 2013-02-20
------------------------------------------------------------*/
#ifndef KEYBOARD_H
#define KEYBOARD_H

/**
 * open keyboard
 * [in] filename keyboard file name, such as "/dev/input/event0"
 * return keyboard device(>=0)
 */
int OpenKeyboard(const char* filename);

/**
 * close keyboard
 * return 0 if successful, else return -1
 */
int CloseKeyboard();

/**
 * get key value (>=0), see macro in linux/input.h, such as KEY_1, KEY_CANCEL, etc.
 * return key value
 */
int GetKey();

#endif
