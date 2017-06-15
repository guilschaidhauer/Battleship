#include "stdafx.h"
#include "SFML/Graphics.hpp"
#include "Server.h"
#include "Client.h"
#include "GameManager.h"
#include <sstream>
using namespace std;

Server server;
Client client1, client2;
Player player;

GameManager *gameManager;

vector<Battleship> battleships1;
vector<Battleship> battleships2;

//SFML stuff

//Add useless comment

int height = 900, width = 800, xSize = 32, ySize = 32;
sf::RenderWindow sfmlWindow(sf::VideoMode(width, height, 32), "XXXX - Redes 2017/1");

sf::Texture spaceshipTexture, explosionTexture, spaceTexture, bgTexture, gameOverTexture, creditsTexture, buttonTexture;
sf::Sprite spaceshipSprite, explosionSprite, spaceSprite, menuSprite, bgSprite, gOverSprite, creditsSprite, buttonSprite, serverSprite, client1Sprite, client2Sprite;
bool pressed = false;
float timerTransition, transitionDelay = 0.3f;
sf::Clock myClock;

sf::Font MyFont;
int currentState = 0, connectionType = -1, shipsLeft = 10;
char matrix[10][10];

void loadTextures()
{
	spaceshipTexture.loadFromFile("images\\spaceship.png");
	spaceTexture.loadFromFile("images\\space.png");
	explosionTexture.loadFromFile("images\\explosion.png");
	buttonTexture.loadFromFile("images\\button.png");
	bgTexture.loadFromFile("images\\bg.png");
}

void setSprites()
{
	bgSprite.setTexture(bgTexture);
	spaceSprite.setTexture(spaceTexture);
	spaceshipSprite.setTexture(spaceshipTexture);
	explosionSprite.setTexture(explosionTexture);
	buttonSprite.setTexture(buttonTexture);
	serverSprite.setTexture(buttonTexture);
	client1Sprite.setTexture(buttonTexture);
	client2Sprite.setTexture(buttonTexture);
}

int offset = 5, initialX = 150, initialY = 100;

void drawMatrix(vector<Battleship> *battleships)
{

	//char matrix[10][10];
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
			if (matrix[j][i] != '#')
			{
				switch (matrix[j][i])
				{
				case 'X':
				{
					currentSprite = spaceshipSprite;
				}
				break;
				case 2:
				{
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

void InitializeLocalMatrix()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			matrix[j][i] = '#';
		}
	}
}

void DrawLocalMatrix()
{
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
			if (matrix[j][i] != '#')
			{
				switch (matrix[j][i])
				{
				case 'X':
				{
					currentSprite = spaceshipSprite;
				}
				break;
				case 2:
				{
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

void keyboardFunc(sf::Event event)
{
	switch (event.key.code)
	{
	case sf::Keyboard::Escape:
		sfmlWindow.close();
		break;
	}
}

void PlaceSpaceShip(int x, int y)
{
	if (x > initialX && y > initialY)
	{
		int posX = ((x - initialX) / (40 + offset));
		posX = posX < 0 ? 0 : posX;
		posX = posX > 9 ? 9 : posX;

		int posY = ((y - initialY) / 35);
		posY = posY < 0 ? 0 : posY;
		posY = posY > 9 ? 9 : posY;

		std::cout << posX << std::endl;
		std::cout << posY << std::endl;
		if (matrix[posX][posY] == '#' && shipsLeft >= 1)
		{
			matrix[posX][posY] = 'X';
			shipsLeft -= 1;
		}
		else if (matrix[posX][posY] == 'X')
		{
			matrix[posX][posY] = '#';
			shipsLeft += 1;
		}
	}
}

void drawMatrixFromBattleships(vector<Battleship> *battleships)
{

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

void GenerateBattleships()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (matrix[j][i] == 'X')
			{
				Battleship newBS;
				Position pos;
				pos.x = j;
				pos.y = i;
				pos.alive = true;
				newBS.addPosition(pos);
				if (connectionType == 1)
					battleships1.push_back(newBS);
				else if (connectionType == 2)
					battleships2.push_back(newBS);
			}
		}
	}

	if (connectionType == 1)
	{
		player.initBattleships(battleships1);
		client1.sendBattleshipsToServer();
		cout << "Ready" << endl;
	}
	else if (connectionType == 2)
	{
		player.initBattleships(battleships2);
		client2.sendBattleshipsToServer();
		cout << "Ready" << endl;
	}
	else
	{
		server.waitForBattleships();
	}
}

void mouseFunc(sf::Event event)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		pressed = true;
		int mouseX = sf::Mouse::getPosition(sfmlWindow).x;
		int mouseY = sf::Mouse::getPosition(sfmlWindow).y;

		if (currentState == 0)
		{
			if (serverSprite.getGlobalBounds().contains(mouseX, mouseY))
			{
				connectionType = 0;
				InitializeLocalMatrix();
				currentState = 1;
			}
			else if (client1Sprite.getGlobalBounds().contains(mouseX, mouseY))
			{
				connectionType = 1;
				InitializeLocalMatrix();
				currentState = 1;
			}
			else if (client2Sprite.getGlobalBounds().contains(mouseX, mouseY))
			{
				connectionType = 2;
				InitializeLocalMatrix();
				currentState = 1;
			}
		}
		else if (currentState == 1)
		{
			if (buttonSprite.getGlobalBounds().contains(mouseX, mouseY))
			{
				GenerateBattleships();
				currentState = 2;
			}
			PlaceSpaceShip(mouseX, mouseY);
		}
	}
}

void PlaceShipsScreen()
{
	sfmlWindow.draw(bgSprite);
	sf::Text Title("Battleship", MyFont, 50);
	sf::Text Info("Ships: ", MyFont, 40);
	sf::Text Ready("Ready!", MyFont, 40);
	std::stringstream sLeft;
	sLeft << shipsLeft;
	sLeft << "/ 10";
	sf::Text Left(sLeft.str(), MyFont, 40);
	int x, y;
	y = 10;
	x = width / 2 - Title.getGlobalBounds().width / 2;

	y = (y + ySize);

	Title.setPosition(x, y);
	Title.setFillColor(sf::Color(255, 0, 0));
	y = height - (height / 3);
	x = width / 4;

	int ySize = 80, xSize = 200, offset = 10, xText;

	xText = x + (xSize / 4);
	Info.setPosition(xText, y + offset);

	xText += Info.getGlobalBounds().width + 10;
	Left.setPosition(xText, y + offset);

	buttonSprite.setPosition((width / 2) - 100, (height - (height / 4)));
	buttonSprite.setColor(sf::Color::Green);

	Ready.setPosition((width / 2) - 50, (height - (height / 4) + 10));
	if (shipsLeft == 0)
	{
		sfmlWindow.draw(buttonSprite);
		sfmlWindow.draw(Ready);
	}

	sfmlWindow.draw(Title);
	sfmlWindow.draw(Info);

	sfmlWindow.draw(Left);
	DrawLocalMatrix();
}

void gameLoop(int client)
{

	sfmlWindow.draw(bgSprite);

	sf::Text P1("Player 1", MyFont, 30);

	sf::Text P2("Player 2", MyFont, 30);

	Position p1;
	p1.x = 10;
	p1.y = 0;
	Position p2;
	p2.x = 10;
	p2.y = 420;

	if (connectionType == 1)
	{
		P1.setPosition(p2.x, p2.y);
		P2.setPosition(p1.x, p1.y);
	}
	else if (connectionType == 2)
	{
		P1.setPosition(p1.x, p1.y);
		P2.setPosition(p2.x, p2.y);
	}
	sfmlWindow.draw(P1);
	sfmlWindow.draw(P2);

	switch (connectionType)
	{
	case 0:
	{
		initialY = 30;
		drawMatrix(gameManager->getPlayer1()->getBattleships());
		initialY = 450;
		drawMatrix(gameManager->getPlayer2()->getBattleships());
	}
	break;
	case 1:
	{
		initialY = 450;
		drawMatrix(client1.getPlayer()->getBattleships());
	}
	break;
	case 2:
	{
		initialY = 450;
		drawMatrix(client2.getPlayer()->getBattleships());
	}
	break;
	}
}

void drawMenu()
{
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


/*	TODO Chamar isso quando mandar bomba para o servidor

Se for o cliente 1 que estiver mandando (gameManager.getPlayer1turn())
o client1 manda um Position pro servidor. Servidor retorna a position.
Se o alive for true, acertou, se for falso errou. Fazer o que precisar de acordo com isso


client1.sendMissileCoordinatesToServer(pos);
Position responsePosition = client1.waitForMissileResponse();

if (responsePosition.alive)
{
	cout << "Fucking hit on: " << pos.x << " x " << pos.y << endl;
}
else
{
	cout << "did not fucking hit on: " << pos.x << " x " << pos.y << endl;
}

enquanto o cliente 2 espera, chamar isso

Position responsePosition = client2.waitForMissileResponse();

if (responsePosition.alive)
{
cout << "Fucking hit on: " << pos.x << " x " << pos.y << endl;
}
else
{
cout << "did not fucking hit on: " << pos.x << " x " << pos.y << endl;
}

*/
void selectionLoopClient()
{
	Position pos;
	pos.x = 0;
	pos.y = 0;
	if (connectionType == 1)
	{
		if (gameManager->getPlayer1Turn())
		{
			//TODO tentei colocar isso aqui, mas ele chama isso assim que o jogador clica no Client 1. Tem que ser em outro lugar
			/*
			client1.sendMissileCoordinatesToServer(pos);
			client1.waitForMissileResponse();
			//Wait for missile check response
			Position responsePosition = client1.waitForMissileResponse();
			
			if (responsePosition.alive)
			{
				cout << "Fucking hit on: " << pos.x << " x " << pos.y << endl;
			}
			else
			{
				cout << "did not fucking hit on: " << pos.x << " x " << pos.y << endl;
			}*/
		}
	}
	else if (connectionType == 2)
	{
		/*client2.waitForMissileResponse();
		//Wait for missile check response
		Position responsePosition = client2.waitForMissileResponse();

		if (responsePosition.alive)
		{
			cout << "Fucking hit on: " << pos.x << " x " << pos.y << endl;
		}
		else
		{
			cout << "did not fucking hit on: " << pos.x << " x " << pos.y << endl;
		}*/
	}
	//gameManager->flitTurn();
}

//Chamar isso enquanto um dos clientes estiver selecionando a posição pra mandar a bomba
void selectionLoopServer()
{
	server.waitForMissile();
	gameManager->flitTurn();
}

//END SFML

//N�o usar essa fun��o. Usar interface para criar as battleships a setar elas.
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

void syncBattleships()
{
	switch (connectionType)
	{
	case 0:
	{ // server

		for (int i = 0; i < 2; i++)
		{
			server.addPositionToPlayerBattleships(1);
			server.addPositionToPlayerBattleships(2);
		}

		//drawMatrixFromBattleships(gameManager->getPlayer1()->getBattleships());
		//drawMatrixFromBattleships(gameManager->getPlayer2()->getBattleships());
	}
	break;
	case 1:
	{ //client 1
		client1.setPlayer(&player);

		for (int i = 0; i < battleships1.size(); i++)
		{
			client1.callSendBattleshipPositionToServer(battleships1.at(i).getPositions().at(0), -1);
		}
	}
	break;
	case 2:
	{ // client 2
		client2.setPlayer(&player);

		for (int i = 0; i < battleships2.size(); i++)
		{
			client2.callSendBattleshipPositionToServer(battleships2.at(i).getPositions().at(0), -1);
		}
	}
	break;
	}
}

void doConnectionConfiguration()
{
	switch (connectionType)
	{
	case 0:
	{ // server
		gameManager = server.getGameManager();
		server.initServer();
		//server.waitForBattleships();
	}
	break;
	case 1:
	{ //client 1
		gameManager = new GameManager();
		client1.setPlayer(&player);
		client1.initClient(2000);
		//player.initBattleships(battleships1);
		//client1.sendBattleshipsToServer();
	}
	break;
	case 2:
	{ // client 2
		gameManager = new GameManager();
		client2.setPlayer(&player);
		client2.initClient(2001);
		//player.initBattleships(battleships2);
		//client2.sendBattleshipsToServer();
	}
	break;
	}

	//syncBattleships();
}

int main()
{
	//string connectionType;

	//initPlayers();
	bool configured = false;

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
			case sf::Event::MouseButtonPressed:
				if (!pressed)
					mouseFunc(event);
				break;
			case sf::Event::MouseButtonReleased:
				if (pressed)
					pressed = false;
				break;
			default:
				break;
			}
			if (event.type == sf::Event::Closed)
				sfmlWindow.close();
		}
		//if (sf::Event::MouseButtonPressed)
		//	mouseFunc(event);

		switch (currentState)
		{
		case 0:
		{
			drawMenu();
		}
		break;
		case 1:
		{
			if (!configured)
			{
				configured = true;
				cout << "pre conf" << endl;
				doConnectionConfiguration();
				cout << "pos conf" << endl;
			}
			if (connectionType != 0)
			{
				PlaceShipsScreen();
				selectionLoopClient();
			}
			else
			{
				//sfmlWindow.clear(sf::Color::Black);
				server.waitForBattleships();
				selectionLoopServer();
			}
		}
		break;
		case 2:
		{
			sfmlWindow.clear(sf::Color::Black);

			//if (connectionType != 0) {
			gameLoop(connectionType);
			//	}
			//else {
			//	sfmlWindow.close();
			//}
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