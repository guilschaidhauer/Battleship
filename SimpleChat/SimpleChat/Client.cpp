#include "stdafx.h"
#include "Client.h"


Client::Client()
{
}


Client::~Client()
{
}

void Client::initClient(int port)
{
	ip = sf::IpAddress::getLocalAddress();
	socket.connect(ip, port);
}

void Client::receiveFromServer()
{
	socket.receive(buffer, sizeof(buffer), received);
	cout << buffer << endl;
}

void Client::receivePositionFromServerAndAddToPlayerBattleship()
{
	sf::Packet packet;
	socket.receive(packet);

	Position position;
	int index;
	if (packet >> position.alive >> position.x >> position.y >> index)
	{
		addPositionToPlayerBattleships(index, position);
	}
}

void Client::addPositionToPlayerBattleships(int index, Position newPosition)
{
	if (index >= 0)
	{

	}
	else
	{
		Battleship newBattleship;
		newBattleship.addPosition(newPosition);
		player->getBattleships()->push_back(newBattleship);
	}
}
