#include "Grid.h"
#include <iostream>
#include <ctime>

Grid::Grid(int rows, int cols, int cellWidth, int cellHeight, int screenWidth, int screenHeight) {
	this->rows = rows;
	this->cols = cols;
	cellData = new Cell*[rows];
	for (int i = 0; i < rows; ++i) cellData[i] = new Cell[cols];
	srand(unsigned(time(nullptr)));
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			cellData[i][j].transform = { j * (cellWidth + 2) + ((screenWidth - cellWidth*cols) >> 1),
									i * (cellHeight + 2) + ((screenHeight - cellHeight*rows) >> 1), 
									cellWidth, cellHeight };
			cellData[i][j].id = EMPTY_CELL;
			CandyTransform(i, j) = cellData[i][j].transform;
			CandyID(i, j) = OBJECT_ID(rand() % MAX_DEFAULT_CANDIES);
		}
	}
	gameState = LINE_CHECKING;
}

Grid::~Grid() {
	for (int i = 0; i < rows; ++i) delete[] cellData[i];
	delete[] cellData;
}

void Grid::CheckMouseSwift(MOVE_TYPE move, Sint32 mouseX, Sint32 mouseY) {
	if (gameState == WAITING) {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				if (cellData[i][j].transform.x < mouseX && cellData[i][j].transform.y < mouseY &&
					cellData[i][j].transform.x + cellData[i][j].transform.w > mouseX && cellData[i][j].transform.y + cellData[i][j].transform.h > mouseY) {
					switch (move) {
						case LEFT:	if (j - 1 > -1) if (CandyID(i, j - 1) != EMPTY_CANDY)
							gameState = SWAPPING_CANDIES, swapInfo.Set(i, j, i, j - 1, CandyTransform(i, j), CandyTransform(i, j-1)); break;
						case UP:	if (i - 1 > -1) if (CandyID(i-1, j) != EMPTY_CANDY)
							gameState = SWAPPING_CANDIES, swapInfo.Set(i, j, i - 1, j, CandyTransform(i, j), CandyTransform(i-1, j)); break;
						case RIGHT: if (j + 1 < cols) if (CandyID(i, j + 1) != EMPTY_CANDY)
							gameState = SWAPPING_CANDIES, swapInfo.Set(i, j, i, j + 1, CandyTransform(i, j), CandyTransform(i, j+1)); break;
						case DOWN:	if (i + 1 < rows) if (CandyID(i+1, j) != EMPTY_CANDY)
							gameState = SWAPPING_CANDIES, swapInfo.Set(i, j, i + 1, j, CandyTransform(i, j), CandyTransform(i+1, j)); break;
					}  break;
				}
			}
		}
	}
}

bool Grid::CheckNeighbours(int i, int j) {
	auto id = CandyID(i, j);
	if (j - 1 > -1 && j + 1 < cols) if (id == CandyID(i, j - 1) && id == CandyID(i, j + 1)) return true; // candy is in the middle horizontally
	if (i - 1 > -1 && i + 1 < rows) if (id == CandyID(i - 1, j) && id == CandyID(i + 1, j)) return true; // candy is in the middle vertically
	if (j - 2 > -1)					if (id == CandyID(i, j - 1) && id == CandyID(i, j - 2)) return true; // candy is in the left
	if (i - 2 > -1)					if (id == CandyID(i - 1, j) && id == CandyID(i - 2, j)) return true; // candy is above
	if (j + 2 < cols)				if (id == CandyID(i, j + 1) && id == CandyID(i, j + 2)) return true; // candy is in the right
	if (i + 2 < rows)				if (id == CandyID(i + 1, j) && id == CandyID(i + 2, j)) return true; // candy is downwards
	return false;
}

int Grid::KillNeighbours(int i, int j) {
	auto id = CandyID(i, j);
	std::vector<Sprite*> candies; // vector of candies to be supressed
	std::vector<Sprite*> temp; // temp vector to check each line of candies
	// check vertically
	if (i + 1 < rows) for (int n = i+1; n < rows; ++n) if (id == CandyID(n, j)) temp.push_back(&cellData[n][j].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	if (i - 1 > -1) for (int n = i-1; n >= 0; --n) if (id == CandyID(n, j)) temp.push_back(&cellData[n][j].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	// check horizontally
	if (j + 1 < cols) for (int n = j+1; n < cols; ++n) if (id == CandyID(i, n)) temp.push_back(&cellData[i][n].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	if (j - 1 > -1) for (int n = j-1; n >= 0; --n) if (id == CandyID(i, n)) temp.push_back(&cellData[i][n].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	// check if main vector is filled, and then if so, kill candy neighbours
	if (!candies.empty()) {
		candies.push_back(&cellData[i][j].candy);
		for (auto c : candies) c->id = EMPTY_CANDY;
	}
	return int(candies.size());
}

inline int Lerp(float v0, float v1, float t) {
	return int(fma(t, v1, fma(-t, v0, v0)));
}

void Grid::Update(Uint32 deltaTime, int &score) {
	switch (gameState) {
		case SWAPPING_CANDIES: {
			auto fromPos = swapInfo.fromPos, toPos = swapInfo.toPos;
			auto i0 = swapInfo.fromX, j0 = swapInfo.fromY, in = swapInfo.toX, jn = swapInfo.toY;
			auto &percent = swapInfo.percent;
			auto &reSwap = swapInfo.reSwap;
			// Lerp candies movement
			CandyTransform(i0, j0).x = Lerp(float(fromPos.x), float(toPos.x), percent);
			CandyTransform(i0, j0).y = Lerp(float(fromPos.y), float(toPos.y), percent);
			CandyTransform(in, jn).x = Lerp(float(toPos.x), float(fromPos.x), percent);
			CandyTransform(in, jn).y = Lerp(float(toPos.y), float(fromPos.y), percent);
			// Check if swapping finished
			if (percent > 1.0f) {
				CandyTransform(i0, j0) = toPos;
				CandyTransform(in, jn) = fromPos;
				std::swap(cellData[i0][j0].candy, cellData[in][jn].candy);
				percent = 0.0f;
				if (reSwap) { reSwap = false; gameState = WAITING; return; }
				if (!CheckNeighbours(in, jn)) {
					std::swap(swapInfo.fromPos, swapInfo.toPos);
					std::swap(swapInfo.fromX, swapInfo.toX);
					std::swap(swapInfo.fromY, swapInfo.toY);
					reSwap = true;
				} else gameState = LINE_CHECKING;
			} else percent += deltaTime*0.01f;
		} break;
		case LINE_CHECKING: { // check each line
			for (int i = rows - 1; i >= 0; --i)
				for (int j = cols - 1; j >= 0; --j)
					if (CandyID(i, j) != EMPTY_CANDY) { score += KillNeighbours(i, j); }
			gameState = SHIFTING_CANDIES; 
			return;
		} break;
		case SHIFTING_CANDIES: {
			auto y0 = shiftInfo.fromPos, yf = shiftInfo.toPos, i = shiftInfo.i, j = shiftInfo.j;
			auto &percent = shiftInfo.percent;
			static bool endShifting = true;
			if (endShifting) {
				for (int i = rows - 2; i >= 0; --i) {
					for (int j = 0; j < cols; ++j) {
						if (CandyID(i, j) != EMPTY_CANDY && CandyID(i + 1, j) == EMPTY_CANDY) {
							shiftInfo.i = i, shiftInfo.j = j;
							shiftInfo.fromPos = CandyTransform(i, j).y;
							shiftInfo.toPos = CandyTransform(i + 1, j).y;
							percent = 0.0f;
							endShifting = false;
							return;
						}
					}
				} gameState = ADDING_CANDIES; return;
			} else {
				CandyTransform(i, j).y = Lerp(float(y0), float(yf), percent); // Lerp candies movement down
				// Check if swapping finished
				if (percent > 1.0f) {
					CandyTransform(i, j).y = yf;
					CandyTransform(i + 1, j).y = y0;
					std::swap(cellData[i][j].candy, cellData[i + 1][j].candy);
					if (i + 1 < rows)
						if (CandyID(i + 1, j) == EMPTY_CANDY) {
							shiftInfo.i = i, shiftInfo.j = j;
							shiftInfo.fromPos = CandyTransform(i, j).y;
							shiftInfo.toPos = CandyTransform(i + 1, j).y;
							return;
						}
					endShifting = true; return;
				} else percent += deltaTime*0.01f;
			}
		} break;
		case ADDING_CANDIES: {
			static bool endAdding;
			endAdding = true;
			for (int i = 0; i < cols; ++i)
				if (CandyID(0, i) == EMPTY_CANDY) CandyID(0, i) = OBJECT_ID(rand() % MAX_DEFAULT_CANDIES + 1), endAdding = false;
			gameState = (endAdding) ? WAITING : LINE_CHECKING;
		} break;
	}
}

void Grid::Draw(Renderer &textureManager) {
	for (int i = 0; i < rows; ++i) for (int j = 0; j < cols; ++j) textureManager.PushSprite(cellData[i][j]);
	for (int i = 0; i < rows; ++i) for (int j = 0; j < cols; ++j) if (CandyID(i,j) != EMPTY_CANDY) textureManager.PushSprite(cellData[i][j].candy);
}