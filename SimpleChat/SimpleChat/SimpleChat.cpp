#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Server.h"
#include "Client.h"

using namespace std;

int main()
{
	string connectionType;

	cout << "Enter 's' to connect as a server, Enter for '1' for client 1 and '2' for client 2" << endl;
	getline(cin, connectionType);

	if (connectionType == "s")
	{
		Server server;
		server.initServer();
		server.callSendToClient(1, "Hello Client 1");
		server.callSendToClient(2, "Hello Client 2");
	}
	else if (connectionType == "1")
	{
		Client client;
		client.initClient(2000);
		client.receiveFromServer();
	}
	else if (connectionType == "2")
	{
		Client client;
		client.initClient(2001);
		client.receiveFromServer();
	}

	/*sf::RenderWindow window(sf::VideoMode(800, 200), "Hello World");

	sf::Text message;

	sf::Font font;
	font.loadFromFile("28 Days Later.ttf");

	message.setFont(font);

	message.setString("Hello world");

	message.setCharacterSize(100);

	message.setFillColor(sf::Color::White);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				// Someone closed the window- bye
				window.close();
		}

		window.clear();

		window.draw(message);

		window.display();
	}*/

	system("pause");
	return 0;
}