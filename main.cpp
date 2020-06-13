#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct CartisianCoordinates
{
	int x;
	int y;
	void setCartisianCoordinates(int xO, int yO)
	{
		x = xO;
		y = yO;
	}
	void printCartisianCoordinates()
	{
		cout << x << " " << y;
	}
};

class RBot
{
	CartisianCoordinates gridCoordinates;
	char orientation;
public:
	// The initial position must be gotten from the first three inputs of every line of each bot,
	// The upper right coordinates are gotten from the first line of the file (input)
	void setRBot(int xO, int yO, char orientationO)
	{
		orientation = orientationO;
		gridCoordinates.setCartisianCoordinates(xO, yO);
	}
	// returns true if the passed paremeters match with the current coordinates
	bool matchCoordinates(int xO, int yO)
	{
		if (xO == gridCoordinates.x && yO == gridCoordinates.y)
			return true;
		return
			false;
	}
	CartisianCoordinates getCoordinates()
	{
		return gridCoordinates;
	}
	// Keep the same orientation but move on the grid
	void moveForward()
	{
		switch (orientation)
		{
		case 'N':
			gridCoordinates.y++;
			break;
		case 'S':
			gridCoordinates.y--;
			break;
		case 'E':
			gridCoordinates.x++;
			break;
		case 'W':
			gridCoordinates.x--;
			break;
		default:
			cout << "The orientation could only be N,S,E or W" << endl;
		}
	}
	// Keep the same orientation but move on the grid
	void moveBackward()
	{
		switch (orientation)
		{
		case 'N':
			gridCoordinates.y--;
			break;
		case 'S':
			gridCoordinates.y++;
			break;
		case 'E':
			gridCoordinates.x--;
			break;
		case 'W':
			gridCoordinates.x++;
			break;
		default:
			cout << "The orientation could only be N,S,E or W" << endl;
		}
	}
	// stay on the same position of the grid but just change orientation
	void moveLeft()
	{
		switch (orientation)
		{
		case 'N':
			orientation = 'W';
			break;
		case 'S':
			orientation = 'E';
			break;
		case 'E':
			orientation = 'N';
			break;
		case 'W':
			orientation = 'S';
			break;
		default:
			cout << "The orientation could only be N,S,E or W" << endl;
		}
	}
	// stay on the same position of the grid but just change orientation
	void moveRight()
	{
		switch (orientation)
		{
		case 'N':
			orientation = 'E';
			break;
		case 'S':
			orientation = 'W';
			break;
		case 'E':
			orientation = 'S';
			break;
		case 'W':
			orientation = 'N';
			break;
		default:
			cout << "The orientation could only be N,S,E or W" << endl;
		}
	}
	// returns true if the bot has gone out of bounds
	// x*y will be less than zero if any of x or y is negative. Both can't be negative at the same time and this won't overflow the int size since the max number could be 50* 50 = 2500
	bool CheckIfOutOfBound(int xUpperRight, int yUpperRight)
	{
		if (gridCoordinates.x > xUpperRight || gridCoordinates.y > yUpperRight || (gridCoordinates.x* gridCoordinates.y < 0) )
			return true;
		return false;
	}
	void printRBot()
	{
		gridCoordinates.printCartisianCoordinates();
		cout << " " << orientation;
	}
};

class RBotHandler
{
	// Upper x cordinate
	int xUpperRight;
	// Upper y cordinate
	int yUpperRight;
	RBot theBotObject;
	// The SCENT needs only the coordinates, orientation doesn't matter.
	vector<CartisianCoordinates> scentListOfLostBot;
public:
	RBotHandler(int x, int y)
	{
		xUpperRight = x;
		yUpperRight = y;
	}
	void setTheBotObject(int xO, int yO, char orientationO)
	{
		theBotObject.setRBot(xO, yO, orientationO);
	}
	// returns true if the current coordinates match with any of the ones in 'scentListOfLostBot'.
	bool CheckIfScentFromLostList()
	{
		for (vector<CartisianCoordinates>::iterator it = scentListOfLostBot.begin(); it != scentListOfLostBot.end(); ++it)
		{
			if (theBotObject.matchCoordinates(it->x, it->y))
				return true;
		}
		return false;
	}
	//
	bool forwardCommand()
	{
		theBotObject.moveForward();
		// CheckIfOutOfBound returns true if bot is out of bound
		if (theBotObject.CheckIfOutOfBound(xUpperRight, yUpperRight))
		{
			// move back because either instruction we processed has to be ignored or we've to print the previous position with the word 'LOST'
			theBotObject.moveBackward();
			// if SCENT not found, means it is the first bot who has fallen off-grid
			if (!CheckIfScentFromLostList())
			{
				// save the SCENT of the bot in the list
				scentListOfLostBot.push_back(theBotObject.getCoordinates());
				return false; // stop parsing the input string, print that bot is lost, move to next line (other bot)
			}
			// now it is the point from where another bot also moved out of bounds. So, just ignore this instruction
			return true;
		}
		return true;
	}
	void leftCommand()
	{
		theBotObject.moveLeft();
	}
	void rightCommand()
	{
		theBotObject.moveRight();
	}
	void printPosition()
	{
		theBotObject.printRBot();
	}
};

int main() 
{
	ifstream in;
	int tempX, tempY = 0;
	char tempOrientation = '0';
	in.open("inputBot.txt");
	in >> tempX;
	in >> tempY;
	// store the UpperX and UpperY coordinates to be used for the whole file
	RBotHandler rBotHandlerObject(tempX, tempY);
	while (in.good())
	{
		// get the initial X and Y coordinates and Orientation  of the bot being processed
		in >> tempX; in >> tempY; in >> tempOrientation;
		// store the initial X and Y coordinates and Orientation  of the bot being processed
		rBotHandlerObject.setTheBotObject(tempX, tempY, tempOrientation);
		string strInstructionCommands;
		// get the instructions/commands which define movement of the Robot
		in >> strInstructionCommands;
		bool flagOutOfBound = 0;
		for (int i = 0; i < strInstructionCommands.length() && !flagOutOfBound; i++)
		{
			switch (strInstructionCommands[i])
			{
			case 'F':
				// process forward command and if ZERO is returned then we must break this loop
				// false is returned if bot goes out of bounds
				if (!rBotHandlerObject.forwardCommand())
				{
					flagOutOfBound = 1;
				}
				break;
			case 'L':
				rBotHandlerObject.leftCommand();
				break;
			case 'R':
				rBotHandlerObject.rightCommand();
				break;
			default:
				cout << "The Instruction String can contain only F, L or R" << endl;
				break;
			}
		}
		rBotHandlerObject.printPosition();
		if (flagOutOfBound)
			cout << " LOST" << endl;
		else
			cout << endl;
	}
	return 0;
}