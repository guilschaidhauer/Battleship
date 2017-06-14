#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager()
{
	player1Turn = true;
}


GameManager::~GameManager()
{
}

void GameManager::handleMissileReceived(Position pos)
{
	if (checkMissileHit(pos))
		std::cout << "missile hit a spaceship" << endl;
}

bool GameManager::checkMissileHit(Position pos)
{
	vector<Battleship> *battleships;

	if (player1Turn)
		battleships = player2.getBattleships();
	else
		battleships = player1.getBattleships();

	for (int i = 0; i < battleships->size(); i++)
	{
		if (battleships->at(i).getPositions().at(0).x == pos.x && battleships->at(i).getPositions().at(0).y == pos.y)
		{
			return true;
		}
	}

	return false;
}
