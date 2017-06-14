#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}

void Player::initBattleships(vector<Battleship> newBattleships)
{
	battleships = newBattleships;
}

void Player::addPositionToBattleships(int index, Position position)
{
	/*if (index >= 0)
	{
		battleships.at(battleships.size()-1).addPosition(position);
	}
	else
	{
		Battleship newBattleship;
		newBattleship.addPosition(position);
		battleships.push_back(newBattleship);
	}*/

	Battleship newBattleship;
	newBattleship.addPosition(position);
	battleships.push_back(newBattleship);
}

