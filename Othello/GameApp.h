#pragma once

class HBEngine;

enum class eGameState
{
	TitlesScene,
	InGame,
	GameOver,
	Win,
};

class GameApp
{
public:
	GameApp();
	~GameApp();

public:
	void Initialize();
	void Loop();

	void FiniteStatemachine();

private:

	HBEngine* m_pEngine;

	eGameState m_GameState;

	int m_PosX;
	int m_PosY;

	int m_KeyInput = 0;

	int m_Player;
};

