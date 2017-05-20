#pragma once
#include "Battleship.h"

class Player
{
public:
	Player();
	~Player();

private:
	vector<Battleship> battleships;
};

