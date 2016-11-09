#pragma once
#include "Renderer.h"

// Movement type when swapping candies
enum MOVE_TYPE { UP, LEFT, DOWN, RIGHT };

// Candy structure to store its position when animating and its texture
struct Candy {
	SDL_Rect rect;
	OBJECT_ID id = EMPTY_CANDY;
};

// Cell structure to store its position, its texture and its candy (wether is empty or not)
struct Cell {
	SDL_Rect rect;
	OBJECT_ID id = EMPTY_CELL;
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
	Cell **cellData;
	int rows, cols;
	GridState gameState;

	SwapInfo swapInfo;
	ShiftInfo shiftInfo;

	bool CheckNeighbours(int i, int j);
	size_t KillNeighbours(int i, int j);
	inline OBJECT_ID &CandyID(int i, int j) const { return cellData[i][j].candy.id; }
	inline SDL_Rect &CandyRect(int i, int j) const { return cellData[i][j].candy.rect; }
public:
	Grid(int rows, int cols, int cellWidth, int cellHeight, int screenWidth, int screenHeight);
	~Grid();
	void CheckMouseSwift(MOVE_TYPE move, Sint32 mouseX, Sint32 mouseY);
	void Update(Uint32 deltaTime, int &score);
	void Draw(Renderer &textureManager);
};