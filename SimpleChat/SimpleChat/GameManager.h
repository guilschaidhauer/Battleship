#pragma once
#include <iostream>
#include "Player.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	Player* getPlayer1() { return &player1; };
	Player* getPlayer2() { return &player2; };

	void handleMissileReceived(Position pos);
	bool checkMissileHit(Position pos);
	void flipTurn() { player1Turn = !player1Turn; };
	void setPlayer1Turn(bool status) { player1Turn = status; };
	bool getPlayer1Turn() { return player1Turn; };

private:
	Player player1, player2;
	bool player1Turn;
};

