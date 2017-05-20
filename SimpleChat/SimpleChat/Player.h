#pragma once
#include "Battleship.h"

class Player
{
public:
	Player();
	~Player();

	void initBattleships(vector<Battleship> newBattleships);

	vector<Battleship>* getBattleships() { return &battleships; };

private:
	vector<Battleship> battleships;
};

