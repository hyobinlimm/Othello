#include "GameApp.h"
#include <conio.h>
#include "HBEngine.h"
#include "othello.h"
#include "Scene.h"

GameApp::GameApp()
{
	m_pEngine = nullptr;

	m_PosX = 4;
	m_PosY = 4;

	m_Player = PLAYER_2;
}

GameApp::~GameApp()
{
}

void GameApp::Initialize()
{
	// 엔진 생성 및 초기화 
	m_pEngine = new HBEngine();

	if (m_pEngine->Initialize() == FALSE) return;

	Initialize_HBEngine(m_pEngine);

	m_GameState = eGameState::TitlesScene;

	// 커서 위치 설정
	m_pEngine->LHSetCursor(100, TRUE);

	// 스크린 사이즈 변경
	set_screen_size(60, 60);

	// 보드 초기화
	Initialize_Board();
}

void GameApp::Loop()
{
	// 게임 시작 및 종료
	while (true)
	{
		FiniteStatemachine();
	}
}

void GameApp::FiniteStatemachine()
{
	switch (m_GameState)
	{
	case eGameState::TitlesScene:
		{
			start_title_1(); // 타이틀 화면 그리기

			m_KeyInput = KeyInput();

			if (m_KeyInput != KEY_NONE)
			{
				m_GameState = eGameState::InGame;

				system("cls"); // 타이틀 화면을 지우자. 
			}

		} break;
	case eGameState::InGame:
		{
			// 게임 루프시작
			m_KeyInput = KeyInput();

			switch (m_KeyInput)
			{
			case KEY_LEFTMOVE:
				{
					if (CheckRange(m_PosX - 1, m_PosY))
					{
						m_PosX -= 1;
					}
				} break;
			case KEY_RIGHTMOVE:
				{
					if (CheckRange(m_PosX + 1, m_PosY))
					{
						m_PosX += 1;
					}
				} break;
			case KEY_DOWNMOVE:
				{
					if (CheckRange(m_PosX, m_PosY + 1))
					{
						m_PosY += 1;
					}
				} break;
			case KEY_UPMOVE:
				{
					if (CheckRange(m_PosX, m_PosY - 1))
					{
						m_PosY -= 1;
					}
				} break;
			case KEY_ON_DOT:
				{
					SwitchTurn(m_PosX, m_PosY, &m_Player);
				} break;
			case KEY_NONE:
				break;
			}

			Sleep(100);

			// 보드 그리기 
			DrawBoard();

			// UI 그리기  
			DrawUI(&m_Player);

			m_pEngine->LHLocateCursor(m_PosX * 2, m_PosY);

			if (FullCheckCount(&m_Player))
			{
				m_GameState = eGameState::Win;
			}

		} break;
	case eGameState::Win:
		{
			Win();

			sleep(6000);

			// 보드 초기화 
			Initialize_Board();

			m_GameState = eGameState::TitlesScene;
		} break;
	}
}
