#pragma once

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <windows.h>
#include "new_conio.h"

class  HBEngine
{
public:
	HBEngine();
	~HBEngine();

private:
	// 핸들은 전역으로 쓸 것이다. 이 파일 범위를 벗어나지 않는다.
	HANDLE g_hConsole;

public:
	BOOL Initialize();

	void LHFillScreen(int ch);
	void LHFillScreenEx(const char* pStr);
	void LHLocateCursor(int x, int y);
	void LHPutChar(int x, int y, char ch);
	void LHPutAttrChar(int x, int y, char ch, WORD bg, WORD fg);
	void LHPutAttrCharEx(int x, int y, const char* pStr, WORD bg, WORD fg);
	void LHSetAttrChar(WORD bg, WORD fg);
	void LHSetCursor(DWORD dwSize, BOOL bVisible);
};

#endif // __CONSOLE_H__e
