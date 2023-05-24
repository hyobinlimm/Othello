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
				g_pCGEngine->LHPutAttrCharEx(x * 2, y, "□", BG_WHITE, FG_BLACK);
			}
			else if (G_board[y][x] == -1)
			{
				g_pCGEngine->LHPutAttrCharEx(x * 2, y, "◈", BG_PURPLE, FG_RED);
			}
			else if (G_board[y][x] == 3)
			{
				g_pCGEngine->LHPutAttrCharEx(x * 2, y, "□", BG_YELLOW, FG_BLACK);
			}
			else if (G_board[y][x] == 1)
			{
				g_pCGEngine->LHPutAttrCharEx(x * 2, y, "○", BG_WHITE, FG_YELLOW);
			}
			else if (G_board[y][x] == 2)
			{
				g_pCGEngine->LHPutAttrCharEx(x * 2, y, "●", BG_WHITE, FG_LBLUE);
			}
		}
		printf("\n");
	}
}

void DrawUI(int* pTurn)
{
	if (*pTurn == PLAYER_1)
	{
		g_pCGEngine->LHPutAttrCharEx(24, 4, "● 차례", BG_BLACK, FG_WHITE);
	}
	if (*pTurn == PLAYER_2)
	{
		g_pCGEngine->LHPutAttrCharEx(24, 4, "○ 차례", BG_BLACK, FG_WHITE);
	}

	// 보드에 놓인 돌의 수
	g_pCGEngine->LHLocateCursor(24, 6);
	printf("● : %02d ", White_Count);
	printf(" ○ : %02d ", Black_Count);

	// UI 설명 출력
	g_pCGEngine->LHPutAttrCharEx(0, 11, "이동 키 : ← → ↑ ↓ / 놓기 : space", BG_BLACK, FG_WHITE);
}

// 키 입력 값
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

// 커서 범위 체크
bool CheckRange(int x, int y)
{
	if (x < 1 || x > 8 || y < 1 || y > 8)
	{
		return false;
	}
	return true;
}

// 흑 백 번갈아 턴을 주고 받음
void SwitchTurn(int cursorPosX, int cursorPosY, int* pTurn)
{
	if (*pTurn == PLAYER_1)
	{
		if (CheckPos(cursorPosX, cursorPosY, pTurn, true)) // 주변 8칸 조사, 놓을 수 있는지 없는지 검사. 
		{
			G_board[cursorPosY][cursorPosX] = *pTurn;	// 조사 후 자리에 돌 놓기

			*pTurn = PLAYER_2; // 흑돌 차례		
		};
	}
	else
	{
		if (CheckPos(cursorPosX, cursorPosY, pTurn, true))		// 주변 8칸 조사, 놓을 수 있는지 없는지 검사.
		{
			G_board[cursorPosY][cursorPosX] = *pTurn;	// 조사 후 자리에 돌 놓기

			*pTurn = PLAYER_1;
		}
	}
}

// 주변 8자리 체크, 내가 놀 수 있는 자리인지 검사
bool CheckPos(int cursorPosX, int cursorPosY, int* pTurn, bool isFlip)
{
	// 현재 커서 위치의 주변에 player와 다른 색 돌이 있는지 검색
	for (int y = cursorPosY - 1; y <= cursorPosY + 1; y++)
	{
		for (int x = cursorPosX - 1; x <= cursorPosX + 1; x++)
		{
			if (G_board[y][x] != *pTurn && G_board[y][x] != EMPTY_ && G_board[y][x] != USABLE) // 다른색 돌 발견
			{
				if (CheckFlip(cursorPosX, cursorPosY, pTurn, isFlip))	// 8방향으로 체크 및 뒤집기
				{
					return true;
				}
			}
		}
	}

	return false;
}

// 뒤집을 수 있는 돌을 8방향 체크한다. 
bool CheckFlip(int cursorPosX, int cursorPosY, int* pTurn, bool isFlip)
{
	int max_X = 8;
	int max_Y = 8;

	bool isFind = false; // 한번이라도 true가 나오면 뒤집을게 있음. 

	// 8방향 +1에 있는 돌이 나와 같거나 빈공간이면 검사X

	// 오른쪽 검사
	if (G_board[cursorPosY][cursorPosX + 1] != *pTurn
		&& G_board[cursorPosY][cursorPosX + 1] != EMPTY_
		&& G_board[cursorPosY][cursorPosX + 1] != USABLE)
	{
		for (int i = cursorPosX + 1; i <= max_X; i++)
		{
			if (G_board[cursorPosY][i] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // 뒤집어 주지 않을 거라면 멈춘다. 

				for (int j = i; cursorPosX < j; j--)
				{
					G_board[cursorPosY][j] = *pTurn; // 돌 뒤집기. 
				}
				break;
			}
			else if (G_board[cursorPosY][i] == EMPTY_ || G_board[cursorPosY][i] == WALL_ || G_board[cursorPosY][i] == USABLE)
			{
				break; // 아무것도 하지 않음.
			}
		}
	}

	// 왼쪽 검사
	if (G_board[cursorPosY][cursorPosX - 1] != *pTurn
		&& G_board[cursorPosY][cursorPosX - 1] != EMPTY_
		&& G_board[cursorPosY][cursorPosX - 1] != USABLE)
	{
		for (int i = cursorPosX - 1; 1 <= i; i--)
		{
			if (G_board[cursorPosY][i] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // 뒤집어 주지 않을 거라면 멈춘다. 

				for (int j = i; j < cursorPosX; j++)
				{
					G_board[cursorPosY][j] = *pTurn;
				}
				break;
			}
			else if (G_board[cursorPosY][i] == EMPTY_ || G_board[cursorPosY][i] == WALL_ || G_board[cursorPosY][i] == USABLE)
			{
				break; // 아무것도 하지 않음.
			}
		}
	}

	// 위쪽 검사
	if (G_board[cursorPosY - 1][cursorPosX] != *pTurn
		&& G_board[cursorPosY - 1][cursorPosX] != EMPTY_
		&& G_board[cursorPosY - 1][cursorPosX] != USABLE)
	{
		for (int i = cursorPosY - 1; 1 <= i; i--)
		{
			if (G_board[i][cursorPosX] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // 뒤집어 주지 않을 거라면 멈춘다. 

				for (int j = i; j < cursorPosY; j++)
				{
					G_board[j][cursorPosX] = *pTurn;
				}
				break;
			}
			else if (G_board[i][cursorPosX] == EMPTY_ || G_board[i][cursorPosX] == WALL_ || G_board[i][cursorPosX] == USABLE)
			{
				break; // 아무것도 하지 않음.
			}
		}
	}

	// 아래쪽 검사
	if (G_board[cursorPosY + 1][cursorPosX] != *pTurn
		&& G_board[cursorPosY + 1][cursorPosX] != EMPTY_
		&& G_board[cursorPosY + 1][cursorPosX] != USABLE)
	{
		for (int i = cursorPosY + 1; i <= max_Y; i++)
		{
			if (G_board[i][cursorPosX] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // 뒤집어 주지 않을 거라면 멈춘다. 

				for (int j = i; cursorPosY < j; j--)
				{
					G_board[j][cursorPosX] = *pTurn;
				}
				break;
			}
			else if (G_board[i][cursorPosX] == EMPTY_ || G_board[i][cursorPosX] == WALL_ || G_board[i][cursorPosX] == USABLE)
			{
				break; // 아무것도 하지 않음.
			}
		}
	}

	// 대각선 오른쪽 위
	if (G_board[cursorPosY - 1][cursorPosX + 1] != *pTurn
		&& G_board[cursorPosY - 1][cursorPosX + 1] != EMPTY_
		&& G_board[cursorPosY - 1][cursorPosX + 1] != USABLE)
	{
		for (int i = 1; i <= max_X - cursorPosX; i++)
		{
			if (G_board[cursorPosY - i][cursorPosX + i] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // 뒤집어 주지 않을 거라면 멈춘다. 

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

	// 대각선 오른쪽 아래
	if (G_board[cursorPosY + 1][cursorPosX + 1] != *pTurn
		&& G_board[cursorPosY + 1][cursorPosX + 1] != EMPTY_
		&& G_board[cursorPosY + 1][cursorPosX + 1] != USABLE)
	{
		for (int i = 1; i < max_X - cursorPosX; i++)
		{
			if (G_board[cursorPosY + i][cursorPosX + i] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // 뒤집어 주지 않을 거라면 멈춘다. 

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

	// 대각선 왼쪽 위
	if (G_board[cursorPosY - 1][cursorPosX - 1] != *pTurn
		&& G_board[cursorPosY - 1][cursorPosX - 1] != EMPTY_
		&& G_board[cursorPosY - 1][cursorPosX - 1] != USABLE)
	{
		for (int i = 1; i < cursorPosY; i++)
		{
			if (G_board[cursorPosY - i][cursorPosX - i] == *pTurn)
			{
				isFind = true;

				if (!isFlip) break; // 뒤집어 주지 않을 거라면 멈춘다. 

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

	// 대각선 왼쪽 아래
	if (G_board[cursorPosY + 1][cursorPosX - 1] != *pTurn
		&& G_board[cursorPosY + 1][cursorPosX - 1] != EMPTY_
		&& G_board[cursorPosY + 1][cursorPosX - 1] != USABLE)
	{
		for (int i = 1; i < cursorPosX; i++)
		{
			if (G_board[cursorPosY + i][cursorPosX - i] == *pTurn)
			{
				isFind = true;
				if (!isFlip) break; // 뒤집어 주지 않을 거라면 멈춘다. 

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

	// 이전 usable 값이 남아있다면 초기화 
	for (int y = 0; y < MAP_HEIGHT_; y++)
	{
		for (int x = 0; x < MAP_WIDTH_; x++)
		{
			if (G_board[y][x] == USABLE)
			{
				// 놓을 수 있는 자리
				G_board[y][x] = 0;
			}
		}
	}

	// 빈칸이고, 옆에 다른 색 돌이 있을 때.
	for (int y = 0; y < MAP_HEIGHT_; y++)
	{
		for (int x = 0; x < MAP_WIDTH_; x++)
		{
			if (G_board[y][x] != EMPTY_ && G_board[y][x] != USABLE) continue;

			if (CheckPos(x, y, pTurn, false))
			{
				// 놓을 수 있는 자리
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

	// 흑돌과 백돌이 합했을 때, 보드의 총 칸 개수와 같으면 게임 종류
	if (Black_Count + White_Count == 64) return true;
	if (Black_Count == 0 || White_Count == 0) return true;

	// 놓을자리가 없으면 게임 종류
	if (FindUsableCell(pTurn) == 0) return true;

	return false;
}

void Win()
{
	// 보드에 놓인 돌의 수
	g_pCGEngine->LHLocateCursor(24, 6);
	printf("● : %02d ", White_Count);
	printf(" ○ : %02d ", Black_Count);

	if (Black_Count > White_Count)
	{
		g_pCGEngine->LHPutAttrCharEx(4, 9, "흑돌의 승리입니다! 축하합니다!", BG_BLACK, FG_WHITE);
	}
	else if (White_Count > Black_Count)
	{
		g_pCGEngine->LHPutAttrCharEx(4, 9, "백돌의 승리입니다! 축하합니다!", BG_BLACK, FG_WHITE);
	}
	else if (White_Count == Black_Count)
	{
		g_pCGEngine->LHPutAttrCharEx(4, 9, "무승부 입니다! \n", BG_BLACK, FG_WHITE);
	}
}
