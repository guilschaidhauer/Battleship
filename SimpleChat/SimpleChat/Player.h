#pragma once
#include <SFML/Network.hpp>
#include "Battleship.h"

class Player
{
public:
	Player();
	~Player();

	void initBattleships(vector<Battleship> newBattleships);

	vector<Battleship>* getBattleships() { return &battleships; };

	void addPositionToBattleships(int index, Position newPosition);

private:
	vector<Battleship> battleships;
};

