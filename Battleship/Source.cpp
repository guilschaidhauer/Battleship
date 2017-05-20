#include <iostream>
#include <ctime>
#include <sstream>
using namespace std;

class board {
private:
	char grid[10][10];
public:
	void initializeGrid() {
		for (int row = 0; row<10; row++) {
			for (int column = 0; column<10; column++) {
				grid[row][column] = '~';
			}
		}
	}
	void displayGrid(bool hideShips = false) {
		int y = 10;
		for (int i = 0; i<10; i++) {
			if (y != 10) { cout << " "; }
			cout << y;                                    //display Y-axis numbers
			for (int c = 0; c<10; c++) {
				cout << " ";
				if (grid[i][c] == 'X') {
					if (hideShips == true) {
						cout << "~";
					}
					else {
						cout << grid[i][c];
					}
				}
				else { cout << grid[i][c]; }
			}
			y--;
			cout << endl;
		}
		cout << "   A B C D E F G H I J" << endl;           //display X-axis letters
	}
	char getSpace(int x, int y) {
		return grid[y][x];
	}
	void addSpace(char space, int x, int y) {
		grid[y][x] = space;
	}
};

class ship {
private:
	int length;
	board* boardOP;
public:
	ship(int L, board* BOP) {
		length = L;
		boardOP = BOP;
	}
	bool buildShip(int *X, int *Y, int dX, int dY, board *boardOP) {
		int shipStore[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };              //stores the coordinates of the checked (empty) spaces
		for (int i = 0; i<length; i++) {
			shipStore[i * 2] = *X;
			shipStore[i * 2 + 1] = *Y;
			if (*Y + dY<10 && *Y + dY>-1                                      //check adjacent space is within the grid boundaries
				&& *X + dX<10 && *X + dX>-1
				&& boardOP->getSpace(*X + dX, *Y + dY) == '~') {                   //check adjacent space is empty ('~')
				*Y = *Y + dY;
				*X = *X + dX;
			}
			else { return false; }
		}
		for (int c = 0; c / 2<length; c += 2) {
			boardOP->addSpace('X', shipStore[c], shipStore[c + 1]);
		}
		return true;
	}
	int getLength() { return length; }
};

class player {
protected:
	board* thisBoardOP;
	board* otherBoardOP;
	int X;
	int Y;
	int hits;
public:
	virtual void go() {}
	player() {
		hits = 0;
	}
	bool won() {
		if (hits == 17) {
			return true;
		}
		else { return false; }
	}
	bool hit() {
		if (otherBoardOP->getSpace(X - 1, 10 - Y) == 'X') {
			cout << "hit!" << endl;
			hits++;
			otherBoardOP->addSpace('#', X - 1, 10 - Y);
			return true;
		}
		else {
			cout << "miss!" << endl;
			otherBoardOP->addSpace('O', X - 1, 10 - Y);
			return false;
		}
	}
	void arrangeShips() {
		ship aircraftCarrier(5, thisBoardOP);
		positionShip(&aircraftCarrier);

		ship battleship(4, thisBoardOP);
		positionShip(&battleship);

		ship cruiser(3, thisBoardOP);
		positionShip(&cruiser);

		ship destroyer(3, thisBoardOP);
		positionShip(&destroyer);

		ship patrolBoat(2, thisBoardOP);
		positionShip(&patrolBoat);
	}
	void positionShip(ship* type) {
		while (1) {
			int dY = 0;
			int dX = 0;
			dX = (rand() % 2) - (rand() % 2);		//0, -1, 1, 0
			if (dX == 0) {
				dY = (rand() % 2);
				if (dY == 0) { (dY)--; }			//1, -1
			}
			else { dY = 0; }
			int Y = (rand() % 10);
			int X = (rand() % 10);
			if (thisBoardOP->getSpace(X, Y) == '~') {                                       //Ship can only be placed in the sea ('~')
				if (type->buildShip(&X, &Y, dX, dY, thisBoardOP) == false) {
					dX *= -1;
					dY *= -1;
					if (type->buildShip(&X, &Y, dX, dY, thisBoardOP) == true) { return; }        //try again in the other direction
				}
				else { return; }
			}
		}
	}
};

class human : public player {
public:
	human(board* tBOP, board* oBOP) {
		thisBoardOP = tBOP;
		otherBoardOP = oBOP;
	}
	void go() {
		setXY();
		hit();
		cout << X << endl;
		cout << Y << endl;
	}
	void setXY() {
		string input;
		while (1) {
			do {
				cout << endl << "Enter x coordinate:" << endl;
				char letterX = '\0';
				cin >> input;
				if (input.length() == 1) { letterX = input[0]; }       //convert string input to single char
				X = convertLetter(letterX);                       //convert char to integer. Will return 0 if not between A-J
				if (X == 0) { cout << endl << "Please enter a valid letter between A-J" << endl; }
			} while (X == 0);
			do {
				cout << endl << "Enter y coordinate:" << endl;
				cin >> input;
				stringstream(input) >> Y;                         //convert string input to integer
				if (Y>10 || Y<1) { cout << "Please enter a valid number between 1-10" << endl; }
			} while (Y>10 || Y<1);
			if (otherBoardOP->getSpace(X - 1, 10 - Y) == '#'
				|| otherBoardOP->getSpace(X - 1, 10 - Y) == 'O') {
				cout << endl << "You cannot shoot a space that has already been hit!" << endl;
			}
			else { break; }
		}
	}
	int convertLetter(char letter) {
		if (int(letter)>64 && int(letter)<75) {
			return int(letter) - 64;                          //65 = upper case A
		}
		else if (int(letter)>96 && int(letter)<107) {
			return int(letter) - 96;                          //97 = lower case a
		}
		return 0;
	}
};

class computer : public player {
private:
	int originX;	    	//coordinates of first hit
	int originY;	    	//"
	int vX;                 //vector from origin
	int vY;                 //"
	bool foundEnd1;         //found upper or right end (+)
	bool foundEnd2;         //found lower or left end (-)
	bool foundDirection;	//found direction of ship
	bool oppositeCheck;     //space opposite vector is no good (off the board or empty)
	int distance;           //the distance from (originX+vX,originY+vY) to the other side of the ship found so far
	int count;              //number of directions checked
public:
	computer(board* tBOP, board* oBOP) {
		thisBoardOP = tBOP;
		otherBoardOP = oBOP;
		originX = 0;
		originY = 0;
		vX = 0;
		vY = 0;
		foundEnd1 = false;
		foundEnd2 = false;
		foundDirection = false;
		oppositeCheck = false;
		distance = 2;
		count = 0;
	}
	void go() {
		setXY();
		if (hit() == true) {
			if (originX == 0) {     					//first hit on a ship
				originX = X;
				originY = Y;
			}
			else if (foundDirection == false) {    	//found direction of ship (second hit)
				foundDirection = true;
				if (oppositeCheck == true) {
					if (vX<0 || vY<0) {
						foundEnd1 = true;
					}
					else { foundEnd2 = true; }
				}
			}
		}
		else if (foundDirection == true) {                         //missed a particular ship
			if (X - originX>0 || Y - originY>0) {
				foundEnd1 = true;		//upper or right end found (+)
			}
			else { foundEnd2 = true; }                              //lower or left end found (-)
		}
	}
	void setXY() {
		if (originX != 0) {                         //ship has been hit
			if (foundEnd1 == false
				|| foundEnd2 == false) {
				if (foundDirection == false) {		//direction of ship has not been found yet
					findDirection();
				}
				else {                          //direction of ship has been found
					findRest();
				}
			}
			if (foundEnd1 == true
				&& foundEnd2 == true) {		//entire ship has been destroyed (at least thats what it looks like)
				originX = 0;              //reset everything to do with finding the rest of a ship
				originY = 0;
				vX = 0;
				vY = 0;
				foundEnd1 = false;
				foundEnd2 = false;
				foundDirection = false;
				oppositeCheck = false;
				distance = 2;
				count = 0;
			}
		}
		if (originX == 0) {                //ship has not been hit yet (or has been completely dealt with)
			do {
				X = (rand() % 10) + 1;
				Y = (rand() % 10) + 1;
			} while (otherBoardOP->getSpace(X - 1, 10 - Y) == 'O'         //9-(Y-1)
				|| otherBoardOP->getSpace(X - 1, 10 - Y) == '#');
		}
	}
	void findDirection() {                   //sets X & Y when the direction of the ship is not known
		if (count == 0) {
			vX = (rand() % 2) - (rand() % 2);       // 0, -1, 1, 0
			if (vX == 0) {
				vY = (rand() % 2);
				if (vY == 0) { (vY)--; }          // 1, -1
			}
			else { vY = 0; }
		}
		while (count<4) {                                                     //loop if this direction is blocked
			if (count == 2) {
				oppositeCheck = false; int i = vX; vX = vY; vY = i;
			}
			else if (count != 0) { oppositeCheck = true; vX *= -1; vY *= -1; }         //when count=1 and count=3 the other side is no good
			count++;
			if (originX + vX<11 && originX + vX>0
				&& originY + vY<11 && originY + vY>0
				&& otherBoardOP->getSpace((originX + vX) - 1, 10 - (originY + vY)) != '#'
				&&otherBoardOP->getSpace((originX + vX) - 1, 10 - (originY + vY)) != 'O') {
				X = originX + vX;
				Y = originY + vY;
				return;
			}
		}
		foundEnd1 = true;             //rare case when no more ship can be found
		foundEnd2 = true;
	}
	void findRest() {                //sets X & Y when the direction of the ship is known
		int adjustment = 0;
		if (foundEnd1 == true) {                                //upper or right direction blocked
			if (vX<0 || vY<0) {
				adjustment = 1;
			}
			else { adjustment = -distance; }
		}
		else if (foundEnd2 == true) {                          //lower or left direction blocked
			if (vX<0 || vY<0) {
				adjustment = -distance;
			}
			else { adjustment = 1; }
		}
		else {
			adjustment = (rand() % 2);
			if (adjustment == 0) { adjustment = -distance; }        //adjustment = 1, -2, -3, -4, -5
		}
		if (vX<0 || vY<0) { adjustment *= -1; }                    //adjustment = -1, 2, 3, 4, 5
		int i = 0;
		while (i<2 && (foundEnd1 == false || foundEnd2 == false)) {                               //loop if its blocked on one side
			if (vX == 0) {                                     //vertical ship
				if (originY + vY + adjustment>0
					&& originY + vY + adjustment<11
					&& otherBoardOP->getSpace(originX - 1, 10 - (originY + vY + adjustment)) != '#'
					&&otherBoardOP->getSpace(originX - 1, 10 - (originY + vY + adjustment)) != 'O') {
					Y = originY + vY + adjustment;                                            //set Y
					if (adjustment == 1 || adjustment == -1) { vY += adjustment; }
					distance++;
					return;
				}
			}
			else {                                          //horizontal ship
				if (originX + vX + adjustment>0
					&& originX + vX + adjustment<11
					&& otherBoardOP->getSpace((originX + vX + adjustment) - 1, 10 - originY) != '#'
					&&otherBoardOP->getSpace((originX + vX + adjustment) - 1, 10 - originY) != 'O') {
					X = originX + vX + adjustment;                                            //set X
					if (adjustment == 1 || adjustment == -1) { vX += adjustment; }
					distance++;
					return;
				}
			}
			if (adjustment == 1) {
				adjustment = -distance; foundEnd1 = true;            //switch adjustment to other side
			}
			else if (adjustment == -1) {
				adjustment = distance; foundEnd2 = true;      //an end has been found
			}
			else if (adjustment == distance) {
				adjustment = -1; foundEnd1 = true;
			}
			else { adjustment = 1; foundEnd2 = true; }
			i++;
		}

	}
};

int main() {
	srand((unsigned int)(time(0)));

	board boardO1;
	boardO1.initializeGrid();

	board boardO2;
	boardO2.initializeGrid();

	human humanO(&boardO2, &boardO1);
	humanO.arrangeShips();

	computer computerO(&boardO1, &boardO2);
	computerO.arrangeShips();

	player* playerOP = &humanO;

	while (1) {
		boardO1.displayGrid(true);
		cout << endl;
		boardO2.displayGrid();
		playerOP->go();
		if (playerOP->won() == true) {
			cout << endl;
			if (playerOP == &humanO) {
				cout << "You win!" << endl;
			}
			else {
				cout << "Computer wins!" << endl;
			}
			cout << endl;
			boardO1.displayGrid(true);
			cout << endl;
			boardO2.displayGrid();
			break;
		}
		if (playerOP == &humanO) {
			playerOP = &computerO;
		}
		else { playerOP = &humanO; }
	}

	return 0;
}