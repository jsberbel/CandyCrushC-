#pragma once
#include "TextureManager.h"

// Movement type when swapping candies
enum MOVE_TYPE { UP, LEFT, DOWN, RIGHT };

// Candy structure to store its position when animating and its texture
struct Candy {
	SDL_Rect rect;
	TEXTURE_ID id = EMPTY_CANDY;
};

// Cell structure to store its position, its texture and its candy (wether is empty or not)
struct Cell {
	SDL_Rect rect;
	TEXTURE_ID id = EMPTY_CELL;
	Candy candy;
};

// Info structure for swapping candies
struct SwapInfo {
	int fromX, fromY, toX, toY;
	SDL_Rect fromPos;
	SDL_Rect toPos;
	float percent = 0.0f;
	bool reSwap = false;
	void Set(int fx, int fy, int tx, int ty, const SDL_Rect &fp, const SDL_Rect &tp) { 
		fromX = fx, fromY = fy, toX = tx, toY = ty, fromPos = fp, toPos = tp;
	};
};

// Shift structure for shifting candies
struct ShiftInfo {
	int i, j, fromPos, toPos;
	float percent = 0.0f;
};

// Grid state to follow an state machine when updating the game
enum GridState { WAITING, SWAPPING_CANDIES, LINE_CHECKING, ADDING_CANDIES, SHIFTING_CANDIES };

class Grid {
	Cell **data;
	int rows, cols;
	GridState gameState = LINE_CHECKING;
	SwapInfo swapInfo;
	ShiftInfo shiftInfo;
	bool endAdding, endShifting = true;

	bool CheckNeighbours(int i, int j);
	void KillNeighbours(int i, int j);
	inline TEXTURE_ID &CandyID(int i, int j) { return data[i][j].candy.id; }
	inline SDL_Rect &CandyRect(int i, int j) { return data[i][j].candy.rect; }
public:
	Grid(int rows, int cols, int cellWidth, int cellHeight, int screenWidth, int screenHeight);
	~Grid();

	inline int GetRows() { return rows; }
	inline int GetCols() { return cols; }

	void CheckSwap(MOVE_TYPE move, Sint32 mouseX, Sint32 mouseY);
	void Update(Uint32 deltaTime);
	void Draw(TextureManager &textureManager);
};