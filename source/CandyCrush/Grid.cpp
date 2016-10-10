#include "Grid.h"
#include <iostream>
#include <ctime>

Grid::Grid(int rows, int cols, int cellWidth, int cellHeight, int screenWidth, int screenHeight) {
	this->rows = rows;
	this->cols = cols;
	data = new Cell*[rows];
	for (int i = 0; i < rows; ++i) data[i] = new Cell[cols];
	// Init grid cells
	srand(unsigned(time(nullptr)));
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			data[i][j].rect.x = j * (cellWidth + 2) + ((screenWidth - cellWidth*cols) >> 1);
			data[i][j].rect.y = i * (cellHeight + 2) + ((screenHeight - cellHeight*rows) >> 1);
			data[i][j].rect.w = cellWidth;
			data[i][j].rect.h = cellHeight;
			data[i][j].id = EMPTY_CELL;
			CandyRect(i, j) = data[i][j].rect;
			CandyID(i, j) = TEXTURE_ID(rand() % MAX_CANDIES);
		}
	}
}

Grid::~Grid() {
	for (int i = 0; i < rows; ++i) delete[] data[i];
	delete[] data;
}

void Grid::CheckSwap(MOVE_TYPE move, Sint32 mouseX, Sint32 mouseY) {
	if (gameState == WAITING) {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				if (data[i][j].rect.x < mouseX && data[i][j].rect.y < mouseY &&
					data[i][j].rect.x + data[i][j].rect.w > mouseX && data[i][j].rect.y + data[i][j].rect.h > mouseY) {
					switch (move) {
						case LEFT:	if (j - 1 > -1) if (CandyID(i, j - 1) != EMPTY_CANDY)
							gameState = SWAPPING_CANDIES, swapInfo.Set(i, j, i, j - 1, CandyRect(i, j), CandyRect(i, j-1)); break;
						case UP:	if (i - 1 > -1) if (CandyID(i-1, j) != EMPTY_CANDY)
							gameState = SWAPPING_CANDIES, swapInfo.Set(i, j, i - 1, j, CandyRect(i, j), CandyRect(i-1, j)); break;
						case RIGHT: if (j + 1 < cols) if (CandyID(i, j + 1) != EMPTY_CANDY)
							gameState = SWAPPING_CANDIES, swapInfo.Set(i, j, i, j + 1, CandyRect(i, j), CandyRect(i, j+1)); break;
						case DOWN:	if (i + 1 < rows) if (CandyID(i+1, j) != EMPTY_CANDY)
							gameState = SWAPPING_CANDIES, swapInfo.Set(i, j, i + 1, j, CandyRect(i, j), CandyRect(i+1, j)); break;
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

void Grid::KillNeighbours(int i, int j) {
	auto id = CandyID(i, j);
	std::vector<Candy*> candies; // vector of candies to be supressed
	std::vector<Candy*> temp; // temp vector to check each line of candies
	// check vertically
	if (i + 1 < rows) for (int n = i+1; n < rows; ++n) if (id == CandyID(n, j)) temp.push_back(&data[n][j].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	if (i - 1 > -1) for (int n = i-1; n >= 0; --n) if (id == CandyID(n, j)) temp.push_back(&data[n][j].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	// check horizontally
	if (j + 1 < cols) for (int n = j+1; n < cols; ++n) if (id == CandyID(i, n)) temp.push_back(&data[i][n].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	if (j - 1 > -1) for (int n = j-1; n >= 0; --n) if (id == CandyID(i, n)) temp.push_back(&data[i][n].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	// check if main vector is filled, and then if so, kill candy neighbours
	if (!candies.empty()) {
		candies.push_back(&data[i][j].candy);
		for (auto c : candies) c->id = EMPTY_CANDY;
	}
}

inline int Lerp(float v0, float v1, float t) {
	return int(fma(t, v1, fma(-t, v0, v0)));
}

void Grid::Update(Uint32 deltaTime) {
	switch (gameState) {
		case SWAPPING_CANDIES: {
			auto fromPos = swapInfo.fromPos, toPos = swapInfo.toPos;
			auto i0 = swapInfo.fromX, j0 = swapInfo.fromY, in = swapInfo.toX, jn = swapInfo.toY;
			auto &percent = swapInfo.percent;
			auto &reSwap = swapInfo.reSwap;
			// Lerp candies movement
			CandyRect(i0, j0).x = Lerp(float(fromPos.x), float(toPos.x), percent);
			CandyRect(i0, j0).y = Lerp(float(fromPos.y), float(toPos.y), percent);
			CandyRect(in, jn).x = Lerp(float(toPos.x), float(fromPos.x), percent);
			CandyRect(in, jn).y = Lerp(float(toPos.y), float(fromPos.y), percent);
			// Check if swapping finished
			if (percent > 1.0f) {
				CandyRect(i0, j0) = toPos;
				CandyRect(in, jn) = fromPos;
				std::swap(data[i0][j0].candy, data[in][jn].candy);
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
					if (CandyID(i, j) != EMPTY_CANDY) { KillNeighbours(i, j); }
			gameState = SHIFTING_CANDIES; 
			return;
		} break;
		case SHIFTING_CANDIES: {
			auto y0 = shiftInfo.fromPos, yf = shiftInfo.toPos, i = shiftInfo.i, j = shiftInfo.j;
			auto &percent = shiftInfo.percent;
			if (endShifting) {
				for (int i = rows - 2; i >= 0; --i) {
					for (int j = 0; j < cols; ++j) {
						if (CandyID(i, j) != EMPTY_CANDY && CandyID(i + 1, j) == EMPTY_CANDY) {
							shiftInfo.i = i, shiftInfo.j = j;
							shiftInfo.fromPos = CandyRect(i, j).y;
							shiftInfo.toPos = CandyRect(i + 1, j).y;
							percent = 0.0f;
							endShifting = false;
							return;
						}
					}
				} gameState = ADDING_CANDIES; return;
			} else {
				CandyRect(i, j).y = Lerp(float(y0), float(yf), percent); // Lerp candies movement down
				// Check if swapping finished
				if (percent > 1.0f) {
					CandyRect(i, j).y = yf;
					CandyRect(i + 1, j).y = y0;
					std::swap(data[i][j].candy, data[i + 1][j].candy);
					if (i + 1 < rows)
						if (CandyID(i + 1, j) == EMPTY_CANDY) {
							shiftInfo.i = i, shiftInfo.j = j;
							shiftInfo.fromPos = CandyRect(i, j).y;
							shiftInfo.toPos = CandyRect(i + 1, j).y;
							return;
						}
					endShifting = true; return;
				} else percent += deltaTime*0.01f;
			}
		} break;
		case ADDING_CANDIES: {
			endAdding = true;
			for (int i = 0; i < cols; ++i)
				if (CandyID(0, i) == EMPTY_CANDY) CandyID(0, i) = TEXTURE_ID(rand() % MAX_CANDIES + 1), endAdding = false;
			gameState = (endAdding) ? WAITING : LINE_CHECKING;
		} break;
	}
}

void Grid::Draw(TextureManager &textureManager) {
	for (int i = 0; i < rows; ++i) for (int j = 0; j < cols; ++j) textureManager.Render(data[i][j].id, &data[i][j].rect);
	for (int i = 0; i < rows; ++i) for (int j = 0; j < cols; ++j) if (CandyID(i,j) != EMPTY_CANDY) textureManager.Render(CandyID(i, j), &CandyRect(i, j));
}