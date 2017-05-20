#pragma 
#include <vector>

using namespace std;

struct position {
	int x, y;
	bool alive;
};

class Battleship
{
public:
	Battleship();
	~Battleship();

	vector<position> getPositions() { return positions; };

private:
	bool alive;
	vector<position> positions;
};

