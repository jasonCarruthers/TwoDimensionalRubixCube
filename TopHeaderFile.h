#pragma once
using namespace std;


//Constants
const int SCREEN_WIDTH = 80; //in units of "cell" (FONTSIZE10)
const int SCREEN_HEIGHT = 12;
const int FONTSIZE10_WIDTH = 8; //in units of pixel
const int FONTSIZE10_HEIGHT = 12;
const int TILE_WIDTH = 10; //in units of "cell" (FONTSIZE10)
const int TILE_HEIGHT = 7;
const int BOARD_WIDTH = 3; //in units of tile
const int BOARD_HEIGHT = 3;
const double MOUSE_PRESS_OFFSET_DURATION = 0.5;

//Enumerators

//Classes
class Location
{
public:
	int mX;
	int mY;
public:
	Location()
	{
	}

	Location(int x, int y)
	{
		mX = x;
		mY = y;
	}
};

class Tile : public Location
{
public:
	WORD mColor;
	int mIdNum;
	bool mIsMousedOver;
	bool mIsPressed;
public:
	Tile()
	{
	}

	Tile(Location loc, WORD color, int id_num)
	{
		mX = loc.mX;
		mY = loc.mY;

		mColor = color;

		mIdNum = id_num;

		mIsMousedOver = false;
		mIsPressed = false;
	}
};

class Mouse
{
public:
	LPPOINT mLoc;
	double mTimeOfLastMousePress;
public:
	Mouse()
	{
	}

	Mouse(double time_of_last_mouse_press)
	{
		mLoc = new POINT();
		mTimeOfLastMousePress = time_of_last_mouse_press;
	}
};