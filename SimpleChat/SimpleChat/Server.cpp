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

void Server::sendToClient1()
{
	string text = "Hello client 1";
	socket1.send(text.c_str(), text.size() + 1);
}

void Server::sendToClient2()
{
	string text = "Hello client 2";
	socket2.send(text.c_str(), text.size() + 1);
}
