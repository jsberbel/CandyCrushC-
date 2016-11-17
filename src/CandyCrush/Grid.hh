/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "Sprite.hh"
#include "InputManager.hh"

class Grid {
public:
	Grid(std::string &&fileName, int cellWidth, int cellHeight);
	~Grid();
	void CheckMouseSwift(const MouseCoords &mouseBegin, const MouseCoords &mouseEnd);
	void Update(int &score);
	void Draw();
private:
	bool CheckNeighbours(int i, int j);
	int KillNeighbours(int i, int j);
	inline ObjectID &CandyID(int i, int j) const { return cellData[i][j].candy.objectID; }
	inline Transform &CandyTransform(int i, int j) const { return cellData[i][j].candy.transform; }
private:
	// Movement type when swapping candies
	enum class MoveType { UP, LEFT, DOWN, RIGHT };
	// Grid state to follow an state machine when updating the game
	enum class GridState { WAITING, SWAPPING_CANDIES, LINE_CHECKING, ADDING_CANDIES, SHIFTING_CANDIES };
	struct Cell : public Sprite { Sprite candy; } **cellData; // Cell structure to store its position, its texture and its candy (wether is empty or not)
	int m_rows, m_cols;
	GridState gridState;
	struct {
		int fromX, fromY, toX, toY;
		Transform fromPos;
		Transform toPos;
		float percent = 0.0f;
		bool reSwap = false;
		void Set(int fx, int fy, int tx, int ty, const Transform &fp, const Transform &tp) {
			fromX = fx, fromY = fy, toX = tx, toY = ty, fromPos = fp, toPos = tp;
		};
	} swapInfo; //Info structure for swapping candies
	struct {
		int i, j, fromPos, toPos;
		float percent = 0.0f;
	} shiftInfo; //Shift structure for shifting candies
};