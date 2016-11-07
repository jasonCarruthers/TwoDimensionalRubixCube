#pragma once
using namespace std;


//Classes
class Game
{
public:
	Tile mTileArray[BOARD_WIDTH][BOARD_HEIGHT];
	Tile mEndGameArray[BOARD_WIDTH][BOARD_HEIGHT];
	HWND mHWND;
	HDC mHDC;
	double mElapsedTime;
	double mCurrentTime;
	WORD mDefaultScreenColor;
	Mouse mMouse;
public:
	Game()
	{
		mCurrentTime = static_cast<double>(clock() / 1000);
		mMouse = Mouse(mCurrentTime);
		mElapsedTime = 0.0;
	}
};
extern Game *mGame;