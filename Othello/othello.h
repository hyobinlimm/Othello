#pragma once

#include "HBEngine.h"

#define MAP_HEIGHT_  10
#define MAP_WIDTH_   10

#define WALL_ -1
#define EMPTY_ 0
#define USABLE 3
#define PLAYER_1 1		// 흰돌
#define PLAYER_2 2		// 검은돌

// 키 입력 정의
#define KEY_LEFTMOVE 101
#define KEY_RIGHTMOVE 102
#define KEY_DOWNMOVE 103
#define KEY_UPMOVE 104
#define KEY_ON_DOT 105
#define KEY_NONE 106

extern int G_board[MAP_HEIGHT_][MAP_WIDTH_];

//그래픽스 엔진을 초기화 한다
void Initialize_HBEngine(HBEngine* pCGEngine);

// 보드 구현
void Initialize_Board();

// 보드 그리기
void DrawBoard();

// UI 그리기
void DrawUI(int* pTurn);

// 키 입력 값
int KeyInput();

// 커서 범위 체크
bool CheckRange(int x, int y);

// space를 누르면 흑백 바뀜
void SwitchTurn(int cursorPosX, int cursorPosY, int* pTurn);

// 빈공간에 놓을 수 있는지 체크
bool CheckPos(int cursorPosX, int cursorPosY, int* pTurn, bool isFlip);

// 8방향으로 돌을 찾아서, 뒤집는다.
bool CheckFlip(int cursorPosX, int cursorPosY, int* pTurn, bool isFlip);

// 보드에 놓을 수 있는 자리 검색
int FindUsableCell(int* pTurn);

// 보드가 흑/백 돌의 개수 
bool FullCheckCount(int* pTurn);

// 승부판결 출력
void Win();