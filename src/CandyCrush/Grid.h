#pragma once
#include "Renderer.h"

// Movement type when swapping candies
enum MOVE_TYPE { UP, LEFT, DOWN, RIGHT };

// Cell structure to store its position, its texture and its candy (wether is empty or not)
struct Cell : Sprite {
	Sprite candy;
};

// Info structure for swapping candies
struct SwapInfo {
	int fromX, fromY, toX, toY;
	Transform fromPos;
	Transform toPos;
	float percent = 0.0f;
	bool reSwap = false;
	void Set(int fx, int fy, int tx, int ty, const Transform &fp, const Transform &tp) {
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
	const int m_rows, m_cols;
	GridState gameState;

	SwapInfo swapInfo;
	ShiftInfo shiftInfo;

	bool CheckNeighbours(int i, int j);
	int KillNeighbours(int i, int j);
	inline OBJECT_ID &CandyID(int i, int j) const { return cellData[i][j].candy.id; }
	inline Transform &CandyTransform(int i, int j) const { return cellData[i][j].candy.transform; }
public:
	Grid(int rows, int cols, int cellWidth, int cellHeight, Window &window);
	~Grid();
	void CheckMouseSwift(MOVE_TYPE move, Sint32 mouseX, Sint32 mouseY);
	void Update(float deltaTime, int &score);
	void Draw(Renderer &textureManager);
};