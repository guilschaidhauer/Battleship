#pragma once
#include "Battleship.h"

class Player
{
public:
	Player();
	~Player();

	void initBattleships(vector<Battleship> newBattleships);

private:
	vector<Battleship> battleships;
};

