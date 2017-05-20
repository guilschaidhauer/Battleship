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
