#include "stdafx.h"
#include "SFML/Graphics.hpp"
#include "Server.h"
#include "Client.h"
#include "GameManager.h"

using namespace std;

Server server;
Client client1, client2;
Player player;

GameManager *gameManager;

vector<Battleship> battleships1;
vector<Battleship> battleships2;


//SFML stuff

int height = 900, width = 800, xSize = 32, ySize = 32;
sf::RenderWindow sfmlWindow(sf::VideoMode(width, height, 32), "XXXX - Redes 2017/1");

sf::Texture spaceshipTexture, explosionTexture, spaceTexture, bgTexture, gameOverTexture, creditsTexture, buttonTexture;
sf::Sprite spaceshipSprite, explosionSprite, spaceSprite, menuSprite, bgSprite, gOverSprite, creditsSprite, buttonSprite, serverSprite, client1Sprite, client2Sprite;

float timerTransition, transitionDelay = 0.3f;
sf::Clock myClock;

sf::Font MyFont;
int currentState = 0, connectionType = -1;


void loadTextures() {
	spaceshipTexture.loadFromFile("images\\spaceship.png");
	spaceTexture.loadFromFile("images\\space.png");
	explosionTexture.loadFromFile("images\\explosion.png");
	buttonTexture.loadFromFile("images\\button.png");
	bgTexture.loadFromFile("images\\bg.png");
}

void setSprites() {
	bgSprite.setTexture(bgTexture);
	spaceSprite.setTexture(spaceTexture);
	spaceshipSprite.setTexture(spaceshipTexture);
	explosionSprite.setTexture(explosionTexture);
	buttonSprite.setTexture(buttonTexture);
	serverSprite.setTexture(buttonTexture);
	client1Sprite.setTexture(buttonTexture);
	client2Sprite.setTexture(buttonTexture);
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
		if (currentState == 0 && sfmlWindow.hasFocus()) {
			if (serverSprite.getGlobalBounds().contains(mouseX, mouseY)) {
				cout << "s";
				connectionType = 0;
				currentState = 1;
			}
			else if (client1Sprite.getGlobalBounds().contains(mouseX, mouseY)) {
				cout << "c1";
				connectionType = 1;
				currentState = 1;
			}
			else if (client2Sprite.getGlobalBounds().contains(mouseX, mouseY)) {
				cout << "c2";
				connectionType = 2;
				currentState = 1;
			}

		}

	}
}

void gameLoop(int client) {

	sfmlWindow.draw(bgSprite);

	sf::Text P1("Player 1", MyFont, 30);

	sf::Text P2("Player 2", MyFont, 30);

	P1.setPosition(10, 0);
	P2.setPosition(10,420);
	sfmlWindow.draw(P1);
	sfmlWindow.draw(P2);



	switch (connectionType) {
	case 0: {
		initialY = 30;
		drawMatrix(gameManager->getPlayer1()->getBattleships());
		initialY = 450;
		drawMatrix(gameManager->getPlayer2()->getBattleships());
	}
			break;
	case 1: {
		drawMatrix(client1.getPlayer()->getBattleships());
	}
			break;
	case 2: {
		drawMatrix(client2.getPlayer()->getBattleships());
	}
			break;
	}
	

}

void drawMenu() {
	sfmlWindow.draw(bgSprite);

	sf::Text Title("Battleship", MyFont, 50);
	sf::Text Server("Server", MyFont, 40);
	sf::Text Client1("Client 1", MyFont, 40);
	sf::Text Client2("Client 2", MyFont, 40);
	int x, y;
	y = 10;
	x = width / 2 - Title.getGlobalBounds().width / 2;
	Title.setPosition(x, y);
	Title.setFillColor(sf::Color(255, 0, 0));
	y = height / 2;
	x = width / 2 - 100;
	int ySize = 80, xSize = 200, offset = 10, xText;
	xText = x + (xSize / 4);
	
	Server.setPosition(xText, y + offset);
	serverSprite.setPosition(x, y);
	sfmlWindow.draw(serverSprite);

	y = (y + ySize);
	Client1.setPosition(xText, y + offset);
	client1Sprite.setPosition(x, y);
	sfmlWindow.draw(client1Sprite);

	y = (y + ySize);
	Client2.setPosition(xText, y + offset);
	client2Sprite.setPosition(x, y);
	sfmlWindow.draw(client2Sprite);

	sfmlWindow.draw(Title);
	sfmlWindow.draw(Server);
	sfmlWindow.draw(Client1);
	sfmlWindow.draw(Client2);
}

//END SFML

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
	Battleship battleship1, battleship2, battleship3, battleship4;
	Position position;

	position.x = 5;
	position.y = 0;
	position.alive = true;
	battleship1.addPosition(position);

	battleships1.push_back(battleship1);

	//=============================

	position.x = 6;
	position.y = 5;
	position.alive = true;
	battleship2.addPosition(position);

	battleships2.push_back(battleship2);

	//==============================================
	//==============================================

	position.x = 4;
	position.y = 1;
	position.alive = true;
	battleship3.addPosition(position);
	battleships1.push_back(battleship3);

	//=============================

	position.x = 3;
	position.y = 0;
	position.alive = true;
	battleship4.addPosition(position);
	battleships2.push_back(battleship4);

	//server.initPlayersBattleships(battleships1, battleships2);


	//drawMatrixFromBattleships(battleships1);
}

void syncBattleships() {
	switch (connectionType) {
		case 0: { // server

			for (int i = 0; i < 2; i++)
			{
				server.addPositionToPlayerBattleships(1);
				server.addPositionToPlayerBattleships(2);
			}

			drawMatrixFromBattleships(gameManager->getPlayer1()->getBattleships());
			drawMatrixFromBattleships(gameManager->getPlayer2()->getBattleships());
		}
				break;
		case 1: { //client 1
			client1.setPlayer(&player);

			for (int i = 0; i < battleships1.size(); i++)
			{
				client1.callSendBattleshipPositionToServer(battleships1.at(i).getPositions().at(0), -1);
			}
		}
				break;
		case 2: { // client 2
			client2.setPlayer(&player);

			for (int i = 0; i < battleships2.size(); i++)
			{
				client2.callSendBattleshipPositionToServer(battleships2.at(i).getPositions().at(0), -1);
			}
		}
			break;
	}
}

void doConnectionConfiguration() {
	switch (connectionType) {
		case 0: { // server
			gameManager = server.getGameManager();
			server.initServer();
			server.waitForBattleships();
		}
		break;
		case 1: { //client 1
			client1.setPlayer(&player);
			client1.initClient(2000);
			player.initBattleships(battleships1);
			client1.sendBattleshipsToServer();
		}
		break;
		case 2: { // client 2
			client2.setPlayer(&player);
			client2.initClient(2001);
			player.initBattleships(battleships2);
			client2.sendBattleshipsToServer();
		}
		break;
	}

	//syncBattleships();
}



int main()
{
	//string connectionType;

	initPlayers();
	bool configured = false;
	//cout << "Type in:" << endl;
	//getline(cin, connectionType);

	//if (connectionType == "s")
	//{
	//	gameManager = server.getGameManager();
	//	server.initServer();
	//	server.callSendToClient(1, "Hello Client 1");
	//	server.callSendToClient(2, "Hello Client 2");

	//	server.addPositionToPlayerBattleships(1);
	//	server.addPositionToPlayerBattleships(2);
	//	server.addPositionToPlayerBattleships(1);
	//	server.addPositionToPlayerBattleships(2);
	//	server.addPositionToPlayerBattleships(1);

	//	cout << endl << "Client 1:" << endl;
	//	drawMatrixFromBattleships(gameManager->getPlayer1()->getBattleships());
	//	cout << endl << "Client 2:" << endl;
	//	drawMatrixFromBattleships(gameManager->getPlayer2()->getBattleships());		
	//}
	//else if (connectionType == "1")
	//{
	//	client1.initClient(2000);
	//	client1.receiveFromServer();
	//	client1.setPlayer(gameManager->getPlayer1());
	//	client1.callSendBattleshipPositionToServer(battleships1.at(0).getPositions().at(0), -1);
	//	client1.callSendBattleshipPositionToServer(battleships1.at(0).getPositions().at(1), 0);
	//	client1.callSendBattleshipPositionToServer(battleships1.at(0).getPositions().at(2), 0);
	//}
	//else if (connectionType == "2")
	//{
	//	client2.initClient(2001);
	//	client2.receiveFromServer();
	//	client2.setPlayer(gameManager->getPlayer2());
	//	client2.callSendBattleshipPositionToServer(battleships2.at(0).getPositions().at(0), -1);
	//	client2.callSendBattleshipPositionToServer(battleships2.at(0).getPositions().at(1), 0);
	//}

	//SFML stuff
		
		if (!MyFont.loadFromFile("fonts\\DimboRegular.ttf"))
		{
			// Error...
		}
		/** Prepare the window */
		sfmlWindow.setFramerateLimit(60);


		/** Prepare textures */
		loadTextures();
		setSprites();

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

			switch (currentState)
			{
			case 0:
			{
				drawMenu();
				
			}
			break;
			case 1:
			{
				sfmlWindow.clear(sf::Color::Black);
				if (!configured) {
					configured = true;
					cout << "pre conf" << endl;
					doConnectionConfiguration();
					cout << "pos conf" << endl;
				}
				if (connectionType == 0) {
					gameLoop(connectionType);
				}
				else {
					sfmlWindow.close();
				}
			}
			break;
			case 2:
			{

			}
			break;
			case 3:
			{

			}
			break;
			}

			sfmlWindow.display();
		}
	

	//system("pause");
	return 0;
}