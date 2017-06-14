#pragma once
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
	Player* getPlayer() { return player; }
	void addPositionToPlayerBattleships(int index, Position newPosition);

	//void callSendBattleshipsToServer(int clientIndex, vector<Battleship> battleships);
	//void setClientBattleships(sf::TcpSocket *socket, vector<Battleship> battleships);

	void callSendBattleshipPositionToServer(Position position, int posIndex);
	//void sendBattleshipPositionToClient(sf::TcpSocket *socket, Position position, int posIndex);

	//=================================================
	void sendBattleshipsToServer();

private:
	sf::TcpSocket socket;
	sf::IpAddress ip;

	char buffer[2000];
	size_t received;

	Player *player;
};

