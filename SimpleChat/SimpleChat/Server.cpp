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