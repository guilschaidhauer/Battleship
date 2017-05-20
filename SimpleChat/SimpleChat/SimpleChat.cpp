#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Server.h"
#include "Client.h"
#include "GameManager.h"

using namespace std;

Server server;
Client client1, client2;

GameManager gameManager;

vector<Battleship> battleships1;
vector<Battleship> battleships2;

void drawMatrixFromBattleships(vector<Battleship> battleships)
{
	char matrix[10][10];

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			matrix[j][i] = '#';
		}
	}

	for (int i = 0; i < battleships.size(); i++)
	{
		vector<Position> positions = battleships.at(i).getPositions();

		for (int j = 0; j < positions.size(); j++)
		{
			matrix[positions.at(j).x][positions.at(j).y] = 'X';
		}
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cout << matrix[j][i] << " ";
		}
		cout << endl;
	}
}

//Não usar essa função. Usar interface para criar as battleships a setar elas.
void initPlayers()
{
	Battleship battleship1, battleship2;
	Position position;

	position.x = 5;
	position.y = 0;
	position.alive = true;
	
	battleship1.addPosition(position);
	battleships1.push_back(battleship1);

	position.x = 6;
	position.y = 5;
	position.alive = true;

	battleship2.addPosition(position);
	battleships2.push_back(battleship2);

	server.initPlayersBattleships(battleships1, battleships2);

	//drawMatrixFromBattleships(battleships1);
}

int main()
{
	string connectionType;

	initPlayers();

	cout << "Type in:" << endl;
	getline(cin, connectionType);

	if (connectionType == "s")
	{
		server.initServer();
		server.callSendToClient(1, "Hello Client 1");
		server.callSendToClient(2, "Hello Client 2");

		server.callSendBattleshipsToClient(1, battleships1);
		server.callSendBattleshipsToClient(2, battleships2);
	}
	else if (connectionType == "1")
	{
		client1.initClient(2000);
		client1.receiveFromServer();
		client1.setPlayer(gameManager.getPlayer1());
		client1.receivePositionFromServerAndAddToPlayerBattleship();
		drawMatrixFromBattleships(*gameManager.getPlayer1()->getBattleships());
	}
	else if (connectionType == "2")
	{
		client2.initClient(2001);
		client2.receiveFromServer();
		client2.setPlayer(gameManager.getPlayer2());
		client2.receivePositionFromServerAndAddToPlayerBattleship();
		drawMatrixFromBattleships(*gameManager.getPlayer2()->getBattleships());
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