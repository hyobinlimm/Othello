#pragma once

#include "HBEngine.h"

#define MAP_HEIGHT_  10
#define MAP_WIDTH_   10

#define WALL_ -1
#define EMPTY_ 0
#define USABLE 3
#define PLAYER_1 1		// ��
#define PLAYER_2 2		// ������

// Ű �Է� ����
#define KEY_LEFTMOVE 101
#define KEY_RIGHTMOVE 102
#define KEY_DOWNMOVE 103
#define KEY_UPMOVE 104
#define KEY_ON_DOT 105
#define KEY_NONE 106

extern int G_board[MAP_HEIGHT_][MAP_WIDTH_];

//�׷��Ƚ� ������ �ʱ�ȭ �Ѵ�
void Initialize_HBEngine(HBEngine* pCGEngine);

// ���� ����
void Initialize_Board();

// ���� �׸���
void DrawBoard();

// UI �׸���
void DrawUI(int* pTurn);

// Ű �Է� ��
int KeyInput();

// Ŀ�� ���� üũ
bool CheckRange(int x, int y);

// space�� ������ ��� �ٲ�
void SwitchTurn(int cursorPosX, int cursorPosY, int* pTurn);

// ������� ���� �� �ִ��� üũ
bool CheckPos(int cursorPosX, int cursorPosY, int* pTurn, bool isFlip);

// 8�������� ���� ã�Ƽ�, �����´�.
bool CheckFlip(int cursorPosX, int cursorPosY, int* pTurn, bool isFlip);

// ���忡 ���� �� �ִ� �ڸ� �˻�
int FindUsableCell(int* pTurn);

// ���尡 ��/�� ���� ���� 
bool FullCheckCount(int* pTurn);

// �º��ǰ� ���
void Win();