#pragma once
#include "Player.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	Player* getPlayer1() { return &player1; };
	Player* getPlayer2() { return &player2; };

private:
	Player player1, player2;
};

