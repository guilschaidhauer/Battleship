#include "stdafx.h"
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 200), "Hello World");

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
	}

	return 0;
}