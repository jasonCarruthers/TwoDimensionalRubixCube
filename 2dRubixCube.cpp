#include <iostream>
//#include <list>
#include <ctime>
#include <windows.h>
//#include <mmsystem.h>

#include "TopHeaderFile.h"
#include "BottomHeaderFile.h"
using namespace std;


//Function prototypes
void InitGame();
void InitBoard();

bool IsBoardSorted();
bool DoesTileContainMouse(Tile tile);

void DrawBoard();
void DrawTileIdNumbers();
void UpdateGame();
void UpdateUser();



//Global variables
Game *mGame;

//Functions
void InitGame()
{
	SetConsoleTitle(L"TileGame");
	HWND hWnd = FindWindow(NULL, L"TileGame");
	HDC hDC = GetDC(hWnd);

	mGame = new Game();

	//needed for drawing pixels
	mGame->mHWND = hWnd;
	mGame->mHDC = hDC;

	//get starting screen color attributes
	PCONSOLE_SCREEN_BUFFER_INFO screen_source = static_cast<PCONSOLE_SCREEN_BUFFER_INFO>(malloc(sizeof(PCONSOLE_SCREEN_BUFFER_INFO)));
	GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), screen_source );
	mGame->mDefaultScreenColor = screen_source->wAttributes;

	//hide cursor
	CONSOLE_CURSOR_INFO consoleCursorInfo;
	consoleCursorInfo.dwSize = 10;
	consoleCursorInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleCursorInfo);

	InitBoard();
}

void InitBoard()
{
	WORD color1 = BACKGROUND_RED;
	WORD color2 = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	WORD current_tile_color = color1;
	//Location offset(SCREEN_WIDTH/2 - BOARD_WIDTH/2, SCREEN_HEIGHT/2 - BOARD_HEIGHT/2); //in units of "cell"
	Location offset(0,0);
	int id_num = 1;

	for(int tile_y = 0; tile_y < BOARD_HEIGHT; tile_y++)
	{
		for(int tile_x = 0; tile_x < BOARD_WIDTH; tile_x++)
		{
			if( tile_x == BOARD_WIDTH - 1 && tile_y == BOARD_HEIGHT - 1 )
				id_num = -1;

			//create a checkerboard of color
			if( !(tile_x == 0 && tile_y != 0) )
				current_tile_color = (current_tile_color == color1) ? color2 : color1;

			//init game-board tiles
			mGame->mTileArray[tile_x][tile_y] = Tile(Location(offset.mX + tile_x * TILE_WIDTH,
				offset.mY + tile_y * TILE_HEIGHT), current_tile_color, id_num);
			//init end-game order of tiles
			mGame->mEndGameArray[tile_x][tile_y] = Tile(Location(offset.mX + tile_x * TILE_WIDTH,
				offset.mY + tile_y * TILE_HEIGHT), current_tile_color, id_num);

			id_num++;
		}
	}
}

bool IsBoardSorted()
{
	for(int tile_x = 0; tile_x < BOARD_WIDTH; tile_x++)
	{
		for(int tile_y = 0; tile_y < BOARD_HEIGHT; tile_y++)
		{
			//compare tiles one at a time; check id_nums
			if(mGame->mTileArray[tile_x][tile_y].mIdNum != mGame->mEndGameArray[tile_x][tile_y].mIdNum)
				return false;
		}
	}

	return true;
}

bool DoesTileContainMouse(Tile tile)
{
	if( mGame->mMouse.mLoc->x > tile.mX * FONTSIZE10_WIDTH && mGame->mMouse.mLoc->x < (tile.mX + TILE_WIDTH) * FONTSIZE10_WIDTH &&
		mGame->mMouse.mLoc->y > tile.mY * FONTSIZE10_HEIGHT && mGame->mMouse.mLoc->y < (tile.mY + TILE_HEIGHT) * FONTSIZE10_HEIGHT )
		return true;
	return false;
}

void DrawBoard()
{
	COORD cursor_loc;

	for(int tile_x = 0; tile_x < BOARD_WIDTH; tile_x++)
	{
		for(int tile_y = 0; tile_y < BOARD_HEIGHT; tile_y++)
		{
			//draws one tile at a time
			if(mGame->mTileArray[tile_x][tile_y].mIdNum >= 0)
			{
				if( DoesTileContainMouse(mGame->mTileArray[tile_x][tile_y]) )
					SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE),  BACKGROUND_BLUE | BACKGROUND_INTENSITY );
				else
					SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE),  mGame->mTileArray[tile_x][tile_y].mColor );

				for(int cell_x = 0; cell_x < TILE_WIDTH; cell_x++)
				{
					cursor_loc.X = static_cast<SHORT>(mGame->mTileArray[tile_x][tile_y].mX + cell_x);
					for(int cell_y = 0; cell_y < TILE_HEIGHT; cell_y++)
					{
						cursor_loc.Y = static_cast<SHORT>(mGame->mTileArray[tile_x][tile_y].mY + cell_y);

						//dont draw the spaces where the id_num is drawn within the tile, otherwise there's flicker
						if( !( (cursor_loc.X == static_cast<SHORT>(mGame->mTileArray[tile_x][tile_y].mX + TILE_WIDTH/2)
							&& cursor_loc.Y == static_cast<SHORT>(mGame->mTileArray[tile_x][tile_y].mY + TILE_HEIGHT/2)) ||
							(cursor_loc.X == static_cast<SHORT>(mGame->mTileArray[tile_x][tile_y].mX + TILE_WIDTH/2 + 1)
							&& cursor_loc.Y == static_cast<SHORT>(mGame->mTileArray[tile_x][tile_y].mY + TILE_HEIGHT/2) &&
							mGame->mTileArray[tile_x][tile_y].mIdNum >= 10) ) )
						{
							SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
							cout << " ";
						}
					}
				}
			}
		}
	}

	DrawTileIdNumbers();
}

void DrawTileIdNumbers()
{
	COORD cursor_loc;

	for(int tile_x = 0; tile_x < BOARD_WIDTH; tile_x++)
	{
		for(int tile_y = 0; tile_y < BOARD_HEIGHT; tile_y++)
		{
			if(mGame->mTileArray[tile_x][tile_y].mIdNum >= 0)
			{
				cursor_loc.X = static_cast<SHORT>(mGame->mTileArray[tile_x][tile_y].mX + TILE_WIDTH/2);
				cursor_loc.Y = static_cast<SHORT>(mGame->mTileArray[tile_x][tile_y].mY + TILE_HEIGHT/2);

				SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
				SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
				cout << mGame->mTileArray[tile_x][tile_y].mIdNum;
				SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), mGame->mDefaultScreenColor );
			}
		}
	}
}

void UpdateGame()
{
	//udpate elapsed time, used for mouse-press offset (so that the user doesn't accidentally double-click)
	mGame->mElapsedTime = static_cast<double>(clock() / 1000.0) - mGame->mCurrentTime;
	mGame->mCurrentTime = static_cast<double>(clock() / 1000.0);

	GetCursorPos(mGame->mMouse.mLoc);
	//To account for window border...
	mGame->mMouse.mLoc->x -= 5;
	mGame->mMouse.mLoc->y -= 29;
}

void UpdateUser()
{
	//check for mouse press
	for(int tile_x = 0; tile_x < BOARD_WIDTH; tile_x++)
	{
		for(int tile_y = 0; tile_y < BOARD_HEIGHT; tile_y++)
		{
			if( DoesTileContainMouse(mGame->mTileArray[tile_x][tile_y] ) && (GetAsyncKeyState( VK_LBUTTON ) &&
				mGame->mMouse.mTimeOfLastMousePress + MOUSE_PRESS_OFFSET_DURATION <= mGame->mCurrentTime) )
			{
				mGame->mMouse.mTimeOfLastMousePress = mGame->mCurrentTime;
				if( mGame->mTileArray[tile_x][tile_y].mIdNum >= 0 )
				{
					Tile temp_tile = mGame->mTileArray[tile_x][tile_y];

					//swap tile information
					if(tile_x > 0 && mGame->mTileArray[tile_x - 1][tile_y].mIdNum < 0)
					{
						mGame->mTileArray[tile_x][tile_y] = Tile(Location(mGame->mTileArray[tile_x][tile_y].mX, 
							mGame->mTileArray[tile_x][tile_y].mY), mGame->mTileArray[tile_x - 1][tile_y].mColor, 
							mGame->mTileArray[tile_x - 1][tile_y].mIdNum);

						mGame->mTileArray[tile_x - 1][tile_y] = Tile(Location(mGame->mTileArray[tile_x - 1][tile_y].mX, 
							mGame->mTileArray[tile_x - 1][tile_y].mY), temp_tile.mColor, temp_tile.mIdNum);
					}

					else if(tile_x < BOARD_WIDTH - 1 && mGame->mTileArray[tile_x + 1][tile_y].mIdNum < 0)
					{
						mGame->mTileArray[tile_x][tile_y] = Tile(Location(mGame->mTileArray[tile_x][tile_y].mX, 
							mGame->mTileArray[tile_x][tile_y].mY), mGame->mTileArray[tile_x + 1][tile_y].mColor, 
							mGame->mTileArray[tile_x + 1][tile_y].mIdNum);

						mGame->mTileArray[tile_x + 1][tile_y] = Tile(Location(mGame->mTileArray[tile_x + 1][tile_y].mX, 
							mGame->mTileArray[tile_x + 1][tile_y].mY), temp_tile.mColor, temp_tile.mIdNum);
					}

					else if(tile_y > 0 && mGame->mTileArray[tile_x][tile_y - 1].mIdNum < 0)
					{
						mGame->mTileArray[tile_x][tile_y] = Tile(Location(mGame->mTileArray[tile_x][tile_y].mX, 
							mGame->mTileArray[tile_x][tile_y].mY), mGame->mTileArray[tile_x][tile_y - 1].mColor, 
							mGame->mTileArray[tile_x][tile_y - 1].mIdNum);

						mGame->mTileArray[tile_x][tile_y - 1] = Tile(Location(mGame->mTileArray[tile_x][tile_y - 1].mX, 
							mGame->mTileArray[tile_x][tile_y - 1].mY), temp_tile.mColor, temp_tile.mIdNum);
					}

					else if(tile_y < BOARD_HEIGHT - 1 && mGame->mTileArray[tile_x][tile_y + 1].mIdNum < 0) 
					{
						mGame->mTileArray[tile_x][tile_y] = Tile(Location(mGame->mTileArray[tile_x][tile_y].mX, 
							mGame->mTileArray[tile_x][tile_y].mY), mGame->mTileArray[tile_x][tile_y + 1].mColor, 
							mGame->mTileArray[tile_x][tile_y + 1].mIdNum);

						mGame->mTileArray[tile_x][tile_y + 1] = Tile(Location(mGame->mTileArray[tile_x][tile_y + 1].mX, 
							mGame->mTileArray[tile_x][tile_y + 1].mY), temp_tile.mColor, temp_tile.mIdNum);
					}				
					
					system("cls");
					//DrawBoard();
					//system("pause");
					return;
				}
			}
		}
	}
}

int main()
{
	COORD cursor_loc;

	InitGame();

	while( !GetAsyncKeyState(VK_ESCAPE) /*&& !IsBoardSorted()*/ )
	{
		cursor_loc.X = 40;
		cursor_loc.Y = 0;
		SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
		cout << "(" << mGame->mMouse.mLoc->x << ", " << mGame->mMouse.mLoc->y << ")";

		UpdateGame();
		UpdateUser();

		DrawBoard();
	}

	//displays victory message
	cursor_loc.X = 0;
	cursor_loc.Y = 1;
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
	cout << "CONGRADULATIONS!!!\n";

	system("pause");
	return 0;
}