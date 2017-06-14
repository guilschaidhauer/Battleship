#pragma once
#include "GameManager.h"
#include "Client.h"

using namespace std;

class Server
{
public:
	Server();
	~Server();

	GameManager *getGameManager() { return &gameManager; };

	void initServer();
	void initPlayersBattleships(vector<Battleship> player1Battleships, vector<Battleship> player2Battleships);

	void listenToClient1();
	void listenToClient2();

	void callSendToClient(int clientIndex, string text);
	void sendToClient(sf::TcpSocket *socket, string text);

	void callSendBattleshipsToClient(int clientIndex, vector<Battleship> battleships);
	void setClientBattleships(sf::TcpSocket *socket, vector<Battleship> battleships);

	void callSendBattleshipPositionToClient(int clientIndex, Position position, int posIndex);
	void sendBattleshipPositionToClient(sf::TcpSocket *socket, Position position, int posIndex);

	void addPositionToPlayerBattleships(int clientIndex);

	//==============================
	void waitForBattleships ();

	void waitForMissile();

private:
	GameManager gameManager;

	sf::IpAddress ip;;
	sf::TcpSocket socket1, socket2;

	sf::TcpListener listener1, listener2;

	char buffer[2000];
	size_t received;
};

