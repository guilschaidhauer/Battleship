#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <Windows.h>
#include <cstdlib>

using namespace std;

class Client
{
public:
	Client();
	~Client();

	void initClient(int port);
	void receiveFromServer();

private:
	sf::TcpSocket socket;
	sf::IpAddress ip;

	char buffer[2000];
	size_t received;
};

