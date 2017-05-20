#pragma once
#include "GameManager.h"
#include "Client.h"

class Server
{
public:
	Server();
	~Server();

private:
	GameManager gameManager;
	Client client1, client2;
};

