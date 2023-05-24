#pragma once

#include <cstdio>
#include "HBEngine.h"

HBEngine::HBEngine()
	: g_hConsole(NULL)
{

}

HBEngine::~HBEngine()
{

}

BOOL HBEngine::Initialize()
{
	// �ְܼ��ӿ� �ڵ� ����
	g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (g_hConsole == INVALID_HANDLE_VALUE)
	{
		printf("Err, INVALID_HANDLE_VALUE\n");

		return FALSE;
	}
	return TRUE;
}

//// ȭ�� ä���
void HBEngine::LHFillScreen(int ch)
{
	int   ix, iy;
	COORD cdPos;

	for (iy = 0; iy < 24; iy++)
	{
		for (ix = 0; ix < 80; ix++)
		{
			cdPos.X = ix;
			cdPos.Y = iy;
			SetConsoleCursorPosition(g_hConsole, cdPos);
			printf("%c", ch);
			//putchar(ch);
		}
	}
}

//// ��ǥ�� ���ڿ� ���
void HBEngine::LHFillScreenEx(const char* pStr)
{
	int   ix, iy;
	COORD cdPos;
	int	  nStrlen = strlen(pStr);

	for (iy = 0; iy < 24; iy++)
	{
		for (ix = 0; ix < 80; ix += nStrlen)
		{
			cdPos.X = ix;
			cdPos.Y = iy;
			SetConsoleCursorPosition(g_hConsole, cdPos);
			printf("%s", pStr);
		}
	}
}

//// Ŀ�� ��ġ ����
void HBEngine::LHLocateCursor(int x, int y)
{
	COORD cdPos = { x, y };

	SetConsoleCursorPosition(g_hConsole, cdPos);
}


//// ��ǥ�� ���� ���
void HBEngine::LHPutChar(int x, int y, char ch)
{
	COORD cdPos = { x, y };

	SetConsoleCursorPosition(g_hConsole, cdPos);
	printf("%c", ch);
	//putchar(ch);
}

//// ��ǥ�� ���� ��� ( �÷��Ӽ�����)
void HBEngine::LHPutAttrChar(int x, int y, char ch, WORD bg, WORD fg)
{
	DWORD dwCharsWritten;
	COORD cdFill = { x, y };
	WORD  wColor = bg | fg;

	FillConsoleOutputAttribute(g_hConsole, wColor, 1, cdFill, &dwCharsWritten);
	FillConsoleOutputCharacter(g_hConsole, ch, 1, cdFill, &dwCharsWritten);
}

//// ��ǥ�� ���ڿ� ��� ( �÷��Ӽ����� )
void HBEngine::LHPutAttrCharEx(int x, int y, const char* pStr, WORD bg, WORD fg)
{
	DWORD dwCharsWritten;
	COORD cdFill = { x, y };
	WORD  wColor = bg | fg;
	int   nStrlen = strlen(pStr);

	FillConsoleOutputAttribute(g_hConsole, wColor, nStrlen, cdFill, &dwCharsWritten);
	WriteConsoleOutputCharacter(g_hConsole, pStr, nStrlen, cdFill, &dwCharsWritten);
}

//// ��ü ���� �÷� �Ӽ� ����
void HBEngine::LHSetAttrChar(WORD bg, WORD fg)
{
	WORD wColor = bg | fg;
	SetConsoleTextAttribute(g_hConsole, wColor);
}

//// Ŀ�� ���� ����
void HBEngine::LHSetCursor(DWORD dwSize, BOOL bVisible)
{
	CONSOLE_CURSOR_INFO curInfo;

	curInfo.bVisible = bVisible;
	curInfo.dwSize = dwSize;
	SetConsoleCursorInfo(g_hConsole, &curInfo);
}
