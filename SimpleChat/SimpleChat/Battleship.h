#pragma 
#include <vector>

using namespace std;

struct Position {
	int x, y;
	bool alive;
};

/*sf::Packet& operator <<(sf::Packet& packet, const position& m)
{
	return packet << m.alive << m.x << m.y;
}
sf::Packet& operator >> (sf::Packet& packet, position& m)
{
	return packet >> m.alive >> m.x >> m.y;
}*/

class Battleship
{
public:
	Battleship();
	~Battleship();

	vector<Position> getPositions() { return positions; };
	void addPosition(Position position) { positions.push_back(position); };

private:
	bool alive;
	vector<Position> positions;
};

