#pragma once

#include <stdio.h>
#include "othello.h"

int G_board[MAP_HEIGHT_][MAP_WIDTH_];
int Black_Count = 0;
int White_Count = 0;

HBEngine* g_pCGEngine;

void Initialize_HBEngine(HBEngine* pCGEngine)
{
	g_pCGEngine = pCGEngine;
}

void Initialize_Board()
{
	for (int y = 0; y < MAP_HEIGHT_; y++)
	{
		for (int x = 0; x < MAP_WIDTH_; x++)
		{
			if (x == 0 || x == 9 || y == 0 || y == 9)
			{
				G_board[y][x] = WALL_;
			}
			else if ((x == 4 && y == 4) || (x == 5 && y == 5))
			{
				G_board[y][x] = PLAYER_1;
			}
			else if ((x == 5 && y == 4) || (x == 4 && y == 5))
			{
				G_board[y][x] = PLAYER_2;
			}
			else
			{
				G_board[y][x] = EMPTY_;
			}
		}
		printf("\n");
	}
}

void DrawBoard()
{
	for (int y = 0; y < MAP_HEIGHT_; y++)
	{
		for (int x = 0; x < MAP_WIDTH_; x++)
		{
			if (G_board[y][x] == 0)
			{
				g_pCGEngine->LHPutAttrCharEx(x * 2, y, "��", BG_WHITE, FG_BLACK);
			}
			else if (G_board[y][x] == -1)
			{
				g_pCGEngine->LHPutAttrCharEx(x * 2, y, "��", BG_PURPLE, FG_RED);
			}
			else if (G_board[y][x] == 3)
			{
				g_pCGEngine->LHPutAttrCharEx(x * 2, y, "��", BG_YELLOW, FG_BLACK);
			}
			else if (G_board[y][x] == 1)
			{
				g_pCGEngine->LHPutAttrCharEx(x * 2, y, "��", BG_WHITE, FG_YELLOW);
			}
			else if (G_board[y][x] == 2)
			{
				g_pCGEngine->LHPutAttrCharEx(x * 2, y, "��", BG_WHITE, FG_LBLUE);
			}
		}
		printf("\n");
	}
}

void DrawUI(int* pTurn)
{
	if (*pTurn == PLAYER_1)
	{
		g_pCGEngine->LHPutAttrCharEx(24, 4, "�� ����", BG_BLACK, FG_WHITE);
	}
	if (*pTurn == PLAYER_2)
	{
		g_pCGEngine->LHPutAttrCharEx(24, 4, "�� ����", BG_BLACK, FG_WHITE);
	}

	// ���忡 ���� ���� ��
	g_pCGEngine->LHLocateCursor(24, 6);
	printf("�� : %02d ", White_Count);
	printf(" �� : %02d ", Black_Count);

	// UI ���� ���
	g_pCGEngine->LHPutAttrCharEx(0, 11, "�̵� Ű : �� �� �� �� / ���� : space", BG_BLACK, FG_WHITE);
}

// Ű �Է� ��
int KeyInput()
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		return KEY_LEFTMOVE;
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		return KEY_RIGHTMOVE;
	}

	if (GetAsyncKeyState(VK_UP))
	{
		return KEY_UPMOVE;
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		return KEY_DOWNMOVE;
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		return KEY_ON_DOT;
	}

	return KEY_NONE;
}

// Ŀ�� ���� üũ
bool CheckRange(int x, int y)
{
	if (x < 1 || x > 8 || y < 1 || y > 8)
	{
		return false;
	}
	return true;
}

// �� �� ������ ���� �ְ� ����
void SwitchTurn(int cursorPosX, int cursorPosY, int* pTurn)
{
	if (*pTurn == PLAYER_1)
	{
		if (CheckPos(cursorPosX, cursorPosY, pTurn, true)) // �ֺ� 8ĭ ����, ���� �� �ִ��� ������ �˻�. 
		{
			G_board[cursorPosY][cursorPosX] = *pTurn;	// ���� �� �ڸ��� �� ����

			*pTurn = PLAYER_2; // �浹 ����		
		};
	}
	else
	{
		if (CheckPos(cursorPosX, cursorPosY, pTurn, true))		// �ֺ� 8ĭ ����, ���� �� �ִ��� ������ �˻�.
		{
			G_board[cursorPosY][cursorPosX] = *pTurn;	// ���� �� �ڸ��� �� ����

			*pTurn = PLAYER_1;
		}
	}
}

// �ֺ� 8�ڸ� üũ, ���� �� �� �ִ� �ڸ����� �˻�
bool CheckPos(int cursorPosX, int cursorPosY, int* pTurn, bool isFlip)
{
	// ���� Ŀ�� ��ġ�� �ֺ��� player�� �ٸ� �� ���� �ִ��� �˻�
	for (int y = cursorPosY - 1; y <= cursorPosY + 1; y++)
	{
		for (int x = cursorPosX - 1; x <= cursorPosX + 1; x++)
		{
			if (G_board[y][x] != *pTurn && G_board[y][x] != EMPTY_ && G_board[y][x] != USABLE) // �ٸ��� �� �߰�
			{
				if (CheckFlip(cursorPosX, cursorPosY, pTurn, isFlip))	// 8�������� üũ �� ������
				{
					return true;
				}
			}
		}
	}

	return false;
}

// ������ �� �ִ� ���� 8���� üũ�Ѵ�. 
bool CheckFlip(int cursorPosX, int cursorPosY, int* pTurn, bool isFlip)
{
	int max_X = 8;
	int max_Y = 8;

	bool isFind = false; // �ѹ��̶� true�� ������ �������� ����. 

	// 8���� +1�� �ִ� ���� ���� ���ų� ������̸� �˻�X

	// ������ �˻�
	if (G_board[cursorPosY][cursorPosX + 1] != *pTurn
		&& G_board[cursorPosY][cursorPosX + 1] != EMPTY_
		&& G_board[cursorPosY][cursorPosX + 1] != USABLE)
	{
		for (int i = cursorPosX + 1; i <= max_X; i++)
		{
			if (G_board[cursorPosY][i] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // ������ ���� ���� �Ŷ�� �����. 

				for (int j = i; cursorPosX < j; j--)
				{
					G_board[cursorPosY][j] = *pTurn; // �� ������. 
				}
				break;
			}
			else if (G_board[cursorPosY][i] == EMPTY_ || G_board[cursorPosY][i] == WALL_ || G_board[cursorPosY][i] == USABLE)
			{
				break; // �ƹ��͵� ���� ����.
			}
		}
	}

	// ���� �˻�
	if (G_board[cursorPosY][cursorPosX - 1] != *pTurn
		&& G_board[cursorPosY][cursorPosX - 1] != EMPTY_
		&& G_board[cursorPosY][cursorPosX - 1] != USABLE)
	{
		for (int i = cursorPosX - 1; 1 <= i; i--)
		{
			if (G_board[cursorPosY][i] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // ������ ���� ���� �Ŷ�� �����. 

				for (int j = i; j < cursorPosX; j++)
				{
					G_board[cursorPosY][j] = *pTurn;
				}
				break;
			}
			else if (G_board[cursorPosY][i] == EMPTY_ || G_board[cursorPosY][i] == WALL_ || G_board[cursorPosY][i] == USABLE)
			{
				break; // �ƹ��͵� ���� ����.
			}
		}
	}

	// ���� �˻�
	if (G_board[cursorPosY - 1][cursorPosX] != *pTurn
		&& G_board[cursorPosY - 1][cursorPosX] != EMPTY_
		&& G_board[cursorPosY - 1][cursorPosX] != USABLE)
	{
		for (int i = cursorPosY - 1; 1 <= i; i--)
		{
			if (G_board[i][cursorPosX] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // ������ ���� ���� �Ŷ�� �����. 

				for (int j = i; j < cursorPosY; j++)
				{
					G_board[j][cursorPosX] = *pTurn;
				}
				break;
			}
			else if (G_board[i][cursorPosX] == EMPTY_ || G_board[i][cursorPosX] == WALL_ || G_board[i][cursorPosX] == USABLE)
			{
				break; // �ƹ��͵� ���� ����.
			}
		}
	}

	// �Ʒ��� �˻�
	if (G_board[cursorPosY + 1][cursorPosX] != *pTurn
		&& G_board[cursorPosY + 1][cursorPosX] != EMPTY_
		&& G_board[cursorPosY + 1][cursorPosX] != USABLE)
	{
		for (int i = cursorPosY + 1; i <= max_Y; i++)
		{
			if (G_board[i][cursorPosX] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // ������ ���� ���� �Ŷ�� �����. 

				for (int j = i; cursorPosY < j; j--)
				{
					G_board[j][cursorPosX] = *pTurn;
				}
				break;
			}
			else if (G_board[i][cursorPosX] == EMPTY_ || G_board[i][cursorPosX] == WALL_ || G_board[i][cursorPosX] == USABLE)
			{
				break; // �ƹ��͵� ���� ����.
			}
		}
	}

	// �밢�� ������ ��
	if (G_board[cursorPosY - 1][cursorPosX + 1] != *pTurn
		&& G_board[cursorPosY - 1][cursorPosX + 1] != EMPTY_
		&& G_board[cursorPosY - 1][cursorPosX + 1] != USABLE)
	{
		for (int i = 1; i <= max_X - cursorPosX; i++)
		{
			if (G_board[cursorPosY - i][cursorPosX + i] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // ������ ���� ���� �Ŷ�� �����. 

				for (int k = i; 1 <= k; k--)
				{
					G_board[cursorPosY - k][cursorPosX + k] = *pTurn;
				}
				break;
			}
			else if (G_board[cursorPosY - i][cursorPosX + i] == EMPTY_ || G_board[cursorPosY - i][cursorPosX + i] == WALL_ || G_board[cursorPosY - i][cursorPosX + i] == USABLE)
			{
				break;
			}
		}
	}

	// �밢�� ������ �Ʒ�
	if (G_board[cursorPosY + 1][cursorPosX + 1] != *pTurn
		&& G_board[cursorPosY + 1][cursorPosX + 1] != EMPTY_
		&& G_board[cursorPosY + 1][cursorPosX + 1] != USABLE)
	{
		for (int i = 1; i < max_X - cursorPosX; i++)
		{
			if (G_board[cursorPosY + i][cursorPosX + i] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // ������ ���� ���� �Ŷ�� �����. 

				for (int k = i; 1 <= k; k--)
				{
					G_board[cursorPosY + k][cursorPosX + k] = *pTurn;
				}
				break;
			}
			else if (G_board[cursorPosY + i][cursorPosX + i] == EMPTY_ || G_board[cursorPosY + i][cursorPosX + i] == WALL_ || G_board[cursorPosY + i][cursorPosX + i] == USABLE)
			{
				break;
			}
		}
	}

	// �밢�� ���� ��
	if (G_board[cursorPosY - 1][cursorPosX - 1] != *pTurn
		&& G_board[cursorPosY - 1][cursorPosX - 1] != EMPTY_
		&& G_board[cursorPosY - 1][cursorPosX - 1] != USABLE)
	{
		for (int i = 1; i < cursorPosY; i++)
		{
			if (G_board[cursorPosY - i][cursorPosX - i] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // ������ ���� ���� �Ŷ�� �����. 

				for (int k = i; 1 <= k; k--)
				{
					G_board[cursorPosY - k][cursorPosX - k] = *pTurn;
				}
				break;
			}
			else if (G_board[cursorPosY - i][cursorPosX - i] == EMPTY_ || G_board[cursorPosY - i][cursorPosX - i] == WALL_ || G_board[cursorPosY - i][cursorPosX - i] == USABLE)
			{
				break;
			}
		}
	}

	// �밢�� ���� �Ʒ�
	if (G_board[cursorPosY + 1][cursorPosX - 1] != *pTurn
		&& G_board[cursorPosY + 1][cursorPosX - 1] != EMPTY_
		&& G_board[cursorPosY + 1][cursorPosX - 1] != USABLE)
	{
		for (int i = 1; i < cursorPosX; i++)
		{
			if (G_board[cursorPosY + i][cursorPosX - i] == *pTurn)
			{
				isFind = true;
				if (!isFlip) break; // ������ ���� ���� �Ŷ�� �����. 

				for (int k = i; 1 <= k; k--)
				{
					G_board[cursorPosY + k][cursorPosX - k] = *pTurn;
				}
				break;
			}
			else if (G_board[cursorPosY + i][cursorPosX - i] == EMPTY_ || G_board[cursorPosY + i][cursorPosX - i] == WALL_ || G_board[cursorPosY + i][cursorPosX - i] == USABLE)
			{
				break;
			}
		}
	}

	return isFind;
}

int FindUsableCell(int* pTurn)
{
	int FindCells = 0;

	// ���� usable ���� �����ִٸ� �ʱ�ȭ 
	for (int y = 0; y < MAP_HEIGHT_; y++)
	{
		for (int x = 0; x < MAP_WIDTH_; x++)
		{
			if (G_board[y][x] == USABLE)
			{
				// ���� �� �ִ� �ڸ�
				G_board[y][x] = 0;
			}
		}
	}

	// ��ĭ�̰�, ���� �ٸ� �� ���� ���� ��.
	for (int y = 0; y < MAP_HEIGHT_; y++)
	{
		for (int x = 0; x < MAP_WIDTH_; x++)
		{
			if (G_board[y][x] != EMPTY_ && G_board[y][x] != USABLE) continue;

			if (CheckPos(x, y, pTurn, false))
			{
				// ���� �� �ִ� �ڸ�
				G_board[y][x] = USABLE;
				++FindCells;
			}
		}
	}

	return  FindCells;
}

bool FullCheckCount(int* pTurn)
{
	White_Count = 0;
	Black_Count = 0;

	for (int y = 0; y < MAP_HEIGHT_; y++)
	{
		for (int x = 0; x < MAP_WIDTH_; x++)
		{
			if (G_board[y][x] == 1)
			{
				++White_Count;
			}
			else if (G_board[y][x] == 2)
			{
				++Black_Count;
			}
		}
	}

	// �浹�� �鵹�� ������ ��, ������ �� ĭ ������ ������ ���� ����
	if (Black_Count + White_Count == 64) return true;
	if (Black_Count == 0 || White_Count == 0) return true;

	// �����ڸ��� ������ ���� ����
	if (FindUsableCell(pTurn) == 0) return true;

	return false;
}

void Win()
{
	// ���忡 ���� ���� ��
	g_pCGEngine->LHLocateCursor(24, 6);
	printf("�� : %02d ", White_Count);
	printf(" �� : %02d ", Black_Count);

	if (Black_Count > White_Count)
	{
		g_pCGEngine->LHPutAttrCharEx(4, 9, "�浹�� �¸��Դϴ�! �����մϴ�!", BG_BLACK, FG_WHITE);
	}
	else if (White_Count > Black_Count)
	{
		g_pCGEngine->LHPutAttrCharEx(4, 9, "�鵹�� �¸��Դϴ�! �����մϴ�!", BG_BLACK, FG_WHITE);
	}
	else if (White_Count == Black_Count)
	{
		g_pCGEngine->LHPutAttrCharEx(4, 9, "���º� �Դϴ�! \n", BG_BLACK, FG_WHITE);
	}
}
