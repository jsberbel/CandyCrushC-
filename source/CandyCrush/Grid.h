#pragma once
#include "TextureManager.h"

enum MOVE_TYPE { UP, LEFT, DOWN, RIGHT };

struct Candy {
	SDL_Rect rect;
	TEXTURE_ID id = EMPTY_CANDY;
};

struct Cell {
	SDL_Rect rect;
	TEXTURE_ID id = EMPTY_CELL;
	Candy candy;
};

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

struct ShiftInfo {
	int i, j, fromPos, toPos;
	float percent = 0.0f;
};

enum GridState { WAITING, SWAPPING, LINE_CHECKING, ADDING, SHIFTING };

class Grid {
	Cell **data;
	int rows, cols;
	GridState gameState = WAITING;
	SwapInfo swapInfo;
	ShiftInfo shiftInfo;
	bool endCheckLine, endAdding, endShifting = true;

	int CheckNeighbours(int i, int j);
	void KillNeighbours(int i, int j);
	inline TEXTURE_ID &CandyID(int i, int j) { return data[i][j].candy.id; }
	inline SDL_Rect &CandyRect(int i, int j) { return data[i][j].candy.rect; }
public:
	Grid(int rows, int cols, int cellWidth, int cellHeight, int screenWidth, int screenHeight);
	~Grid();

	inline int GetRows() { return rows; }
	inline int GetCols() { return cols; }

	void CheckSwap(MOVE_TYPE move, Sint32 mouseX, Sint32 mouseY);
	void Update(float deltaTime);
	void Draw(TextureManager &textureManager);
};