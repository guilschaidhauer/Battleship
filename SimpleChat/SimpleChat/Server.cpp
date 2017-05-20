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
