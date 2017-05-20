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
