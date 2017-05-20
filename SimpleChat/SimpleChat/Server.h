#pragma once
#include "GameManager.h"
#include "Client.h"

class Server
{
public:
	Server();
	~Server();

	void initServer();

	void listenToClient1();
	void listenToClient2();

	void sendToClient1();
	void sendToClient2();

private:
	GameManager gameManager;
	Client client1, client2;

	sf::IpAddress ip;;
	sf::TcpSocket socket1, socket2;

	sf::TcpListener listener1, listener2;

	char buffer[2000];
	size_t received;
};

