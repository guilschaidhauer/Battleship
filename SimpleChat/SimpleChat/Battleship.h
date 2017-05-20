#pragma 
#include <vector>

using namespace std;

struct position {
	char x, y;
	bool alive;
};

class Battleship
{
public:
	Battleship();
	~Battleship();

private:
	bool alive;
	vector<position> posistions;
};

