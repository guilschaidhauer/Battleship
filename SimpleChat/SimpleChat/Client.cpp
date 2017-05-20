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
	// Receive the packet at the other end
	sf::Packet packet;
	socket.receive(packet);

	// Extract the variables contained in the packet
	position position;
	double d;
	if (packet >> position.alive >> position.x >> position.y)
	{
		cout << position.alive << " - " << position.x << " - " << position.y << endl;
	}
}
