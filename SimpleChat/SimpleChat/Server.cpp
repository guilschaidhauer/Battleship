#include "stdafx.h"
#include "Server.h"


Server::Server()
{
}


Server::~Server()
{
}

void Server::initServer()
{
	gameManager.setPlayer1Turn(true);
	ip = sf::IpAddress::getLocalAddress();

	listener1.listen(2000);
	listener1.accept(socket1);

	listener2.listen(2001);
	listener2.accept(socket2);
}

void Server::initPlayersBattleships(vector<Battleship> player1Battleships, vector<Battleship> player2Battleships)
{
	//gameManager.getPlayer1().initBattleships(player1Battleships);
	//gameManager.getPlayer2().initBattleships(player2Battleships);
}

void Server::listenToClient1()
{

}

void Server::listenToClient2()
{

}

void Server::callSendToClient(int clientIndex, string text)
{
	if (clientIndex == 1)
		sendToClient(&socket1, text);
	else if (clientIndex == 2)
		sendToClient(&socket2, text);
}

void Server::sendToClient(sf::TcpSocket * socket, string text)
{
	socket->send(text.c_str(), text.size() + 1);
}

void Server::callSendBattleshipsToClient(int clientIndex, vector<Battleship> battleships)
{
	if (clientIndex == 1)
		setClientBattleships(&socket1, battleships);
	else if (clientIndex == 2)
		setClientBattleships(&socket2, battleships);
}

void Server::setClientBattleships(sf::TcpSocket *socket, vector<Battleship> battleships)
{
	//Figure out a way to set all battleships
	Position position = battleships.at(0).getPositions().at(0);

	sf::Packet packet;
	packet << position.alive << position.x << position.y << -1;

	socket->send(packet);
}

void Server::callSendBattleshipPositionToClient(int clientIndex, Position position, int posIndex)
{
	if (clientIndex == 1)
		sendBattleshipPositionToClient(&socket1, position, posIndex);
	else if (clientIndex == 2)
		sendBattleshipPositionToClient(&socket2, position, posIndex);
}

void Server::sendBattleshipPositionToClient(sf::TcpSocket * socket, Position position, int posIndex)
{
	sf::Packet packet;
	packet << position.alive << position.x << position.y << posIndex;

	socket->send(packet);
}

void Server::addPositionToPlayerBattleships(int clientIndex)
{
	sf::Packet packet;

	sf::TcpSocket *playerSocket;

	if (clientIndex == 1)
		playerSocket = &socket1;
	else 
		playerSocket = &socket2;

	playerSocket->receive(packet);

	Position position;
	int index;
	if (packet >> position.alive >> position.x >> position.y >> index)
	{
		if (clientIndex == 1)
			gameManager.getPlayer1()->addPositionToBattleships(index, position);
		else 
			gameManager.getPlayer2()->addPositionToBattleships(index, position);
	}
}

void Server::waitForBattleships()
{
	for (int i = 0; i < 10; i++)
	{
		addPositionToPlayerBattleships(1);
		addPositionToPlayerBattleships(2);
	}
}



void Server::waitForMissile()
{
	sf::Packet packet;
	sf::TcpSocket *playerSocket;

	if (gameManager.getPlayer1Turn())
		playerSocket = &socket1;
	else
		playerSocket = &socket2;

	playerSocket->receive(packet);

	Position position;
	if (packet >> position.alive >> position.x >> position.y)
	{
		if (gameManager.checkMissileHit(position))
		{
			position.alive = true;
			std::cout << "missile hit a spaceship" << endl;
		}
		else
		{
			position.alive = false;
			std::cout << "missile did not hit a spaceship" << endl;
		}

		sendMissileResponse(position);
	}
}

void Server::sendMissileResponse(Position position)
{
	sf::Packet packet;
	packet << position.alive << position.x << position.y;
	if (gameManager.getPlayer1Turn())
		socket1.send(packet);
	else
		socket2.send(packet);
}
