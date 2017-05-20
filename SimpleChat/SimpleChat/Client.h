#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <Windows.h>
#include <cstdlib>
#include "Player.h"

using namespace std;

class Client
{
public:
	Client();
	~Client();

	void initClient(int port);
	void receiveFromServer();
	void receivePositionFromServerAndAddToPlayerBattleship();

	void setPlayer(Player* newPlayer) { player = newPlayer; };

	void addPositionToPlayerBattleships(int index, Position newPosition);

private:
	sf::TcpSocket socket;
	sf::IpAddress ip;

	char buffer[2000];
	size_t received;

	Player *player;
};

