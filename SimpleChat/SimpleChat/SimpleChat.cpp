#include "stdafx.h"
#include "SFML/Graphics.hpp"
#include "Server.h"
#include "Client.h"
#include "GameManager.h"

using namespace std;

Server server;
Client client1, client2;

GameManager *gameManager;

vector<Battleship> battleships1;
vector<Battleship> battleships2;


//SFML stuff

int height = 900, width = 800, xSize = 32, ySize = 32;
sf::RenderWindow sfmlWindow(sf::VideoMode(width, height, 32), "XXXX - Redes 2017/1");

sf::Texture spaceshipTexture, explosionTexture, spaceTexture, bgTexture, gameOverTexture, creditsTexture;
sf::Sprite spaceshipSprite, explosionSprite, spaceSprite, menuSprite, bgSprite, gOverSprite, creditsSprite;

float timerTransition, transitionDelay = 0.3f;
sf::Clock myClock;

sf::Font MyFont;

void loadTextures() {
	spaceshipTexture.loadFromFile("images\\spaceship.png");
	spaceTexture.loadFromFile("images\\space.png");
	explosionTexture.loadFromFile("images\\explosion.png");
	bgTexture.loadFromFile("images\\bg.png");

}


int offset = 5, initialX = 150, initialY = 20;

void drawMatrix(vector<Battleship> *battleships) {
	
	char matrix[10][10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			matrix[j][i] = '#';
		}
	}
	for (int i = 0; i < battleships->size(); i++)
	{
		vector<Position> positions = battleships->at(i).getPositions();

		for (int j = 0; j < positions.size(); j++)
		{
			matrix[positions.at(j).x][positions.at(j).y] = 'X';
		}
	}


	int x = 0, y = 0;
	sf::Sprite currentSprite;
	for (int i = 0; i < 10; i++)
	{
		currentSprite = spaceSprite;
		for (int j = 0; j < 10; j++)
		{
			x = initialX + (j * 40) + (offset * j);
			y = initialY + (i * 35) + offset;
			spaceSprite.setPosition(x, y);
			sfmlWindow.draw(spaceSprite);
			if (matrix[j][i] != '#') {
				switch (matrix[j][i])
				{
				case 'X': {
					currentSprite = spaceshipSprite;
				}
						break;
				case 2: {
					currentSprite = explosionSprite;
				}
						break;
				}
				x += 5;
				currentSprite.setPosition(x, y);
				sfmlWindow.draw(currentSprite);
			}
		}
	}
}

void keyboardFunc(sf::Event event) {
	switch (event.key.code)
	{
	case sf::Keyboard::Escape:
		sfmlWindow.close();
		break;
	}

}

void mouseFunc(sf::Event event) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		int mouseX = sf::Mouse::getPosition(sfmlWindow).x;
		int mouseY = sf::Mouse::getPosition(sfmlWindow).y;


	}
}

void gameLoop() {

	sfmlWindow.draw(bgSprite);

	sf::Text P1("Player 1", MyFont, 30);

	sf::Text P2("Player 2", MyFont, 30);

	P1.setPosition(10, 0);
	P2.setPosition(10,420);
	sfmlWindow.draw(P1);
	sfmlWindow.draw(P2);

	initialY = 30;
	drawMatrix(gameManager->getPlayer1()->getBattleships());
	initialY = 450;
	drawMatrix(gameManager->getPlayer2()->getBattleships());
	

}

void drawMatrixFromBattleships(vector<Battleship> *battleships)
{
	char matrix[10][10];

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			matrix[j][i] = '#';
		}
	}

	for (int i = 0; i < battleships->size(); i++)
	{
		vector<Position> positions = battleships->at(i).getPositions();

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

	position.x = 5;
	position.y = 1;
	position.alive = true;
	battleship1.addPosition(position);

	position.x = 4;
	position.y = 1;
	position.alive = true;
	battleship1.addPosition(position);

	battleships1.push_back(battleship1);

	///=============================

	position.x = 6;
	position.y = 5;
	position.alive = true;
	battleship2.addPosition(position);

	position.x = 6;
	position.y = 4;
	position.alive = true;
	battleship2.addPosition(position);

	battleships2.push_back(battleship2);


	server.initPlayersBattleships(battleships1, battleships2);

	//drawMatrixFromBattleships(battleships1);
}

int main()
{
	sfmlWindow.setVisible(false);
	string connectionType;

	initPlayers();

	cout << "Type in:" << endl;
	getline(cin, connectionType);

	if (connectionType == "s")
	{
		gameManager = server.getGameManager();
		server.initServer();
		server.callSendToClient(1, "Hello Client 1");
		server.callSendToClient(2, "Hello Client 2");

		server.addPositionToPlayerBattleships(1);
		server.addPositionToPlayerBattleships(2);
		server.addPositionToPlayerBattleships(1);
		server.addPositionToPlayerBattleships(2);
		server.addPositionToPlayerBattleships(1);

		cout << endl << "Client 1:" << endl;
		drawMatrixFromBattleships(gameManager->getPlayer1()->getBattleships());
		cout << endl << "Client 2:" << endl;
		drawMatrixFromBattleships(gameManager->getPlayer2()->getBattleships());		
	}
	else if (connectionType == "1")
	{
		client1.initClient(2000);
		client1.receiveFromServer();
		client1.setPlayer(gameManager->getPlayer1());
		client1.callSendBattleshipPositionToServer(battleships1.at(0).getPositions().at(0), -1);
		client1.callSendBattleshipPositionToServer(battleships1.at(0).getPositions().at(1), 0);
		client1.callSendBattleshipPositionToServer(battleships1.at(0).getPositions().at(2), 0);
	}
	else if (connectionType == "2")
	{
		client2.initClient(2001);
		client2.receiveFromServer();
		client2.setPlayer(gameManager->getPlayer2());
		client2.callSendBattleshipPositionToServer(battleships2.at(0).getPositions().at(0), -1);
		client2.callSendBattleshipPositionToServer(battleships2.at(0).getPositions().at(1), 0);
	}

	//SFML stuff
	if (connectionType == "s") {
		sfmlWindow.setVisible(true);
		if (!MyFont.loadFromFile("fonts\\DimboRegular.ttf"))
		{
			// Error...
		}
		/** Prepare the window */
		sfmlWindow.setFramerateLimit(60);


		/** Prepare textures */
		loadTextures();
		//	menuSprite.setTexture(menuTexture);
		bgSprite.setTexture(bgTexture);
		spaceSprite.setTexture(spaceTexture);
		spaceshipSprite.setTexture(spaceshipTexture);
		explosionSprite.setTexture(explosionTexture);
		//gOverSprite.setTexture(gameOverTexture);
		//creditsSprite.setTexture(creditsTexture);

		int current = 1;
		while (sfmlWindow.isOpen())
		{
			sf::Event event;
			while (sfmlWindow.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					sfmlWindow.close();
					break;
				case sf::Event::KeyPressed:
					keyboardFunc(event);
					break;
				default:
					break;
				}
				if (event.type == sf::Event::Closed)
					sfmlWindow.close();
			}
			if (sf::Event::MouseButtonPressed)
				mouseFunc(event);

			switch (current)
			{
			case 0:
			{

			}
			break;
			case 1:
			{
				sfmlWindow.clear(sf::Color::Black);
				gameLoop();
			}
			break;
			case 2:
			{

			}
			break;
			case 3:
			{
				sfmlWindow.draw(creditsSprite);

			}
			break;
			}

			sfmlWindow.display();
		}
	}

	system("pause");
	return 0;
}