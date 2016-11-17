/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include <ctime>
#include "Grid.hh"
#include "Window.hh"
#include "IOManager.hh"
#include "TimeManager.hh"

Grid::Grid(std::string &&filename, int cellWidth, int cellHeight) {
	auto lvlData(IOManager::LoadLevel(std::move(filename), m_rows, m_cols));
	cellData = new Cell*[m_rows];
	for (int i = 0; i < m_rows; ++i) cellData[i] = new Cell[m_cols];
	srand(unsigned(time(nullptr)));
	for (int i = 0; i < m_rows; ++i) {
		for (int j = 0; j < m_cols; ++j) {
			cellData[i][j].transform = { j * (cellWidth + 2) + ((W.GetWidth() - cellWidth*m_cols) >> 1),
										i * (cellHeight + 2) + ((W.GetHeight()- cellHeight*m_rows) >> 1),
										cellWidth, cellHeight };
			cellData[i][j].objectID = ObjectID::CELL_EMPTY;
			CandyTransform(i, j) = cellData[i][j].transform;
			CandyID(i, j) = lvlData[i][j];
		}
	}
	gridState = GridState::LINE_CHECKING;
}

Grid::~Grid() {
	for (int i = 0; i < m_rows; ++i) delete[] cellData[i];
	delete[] cellData;
}

void Grid::CheckMouseSwift(const MouseCoords &mouseBegin, const MouseCoords &mouseEnd) {
	if (gridState == GridState::WAITING) {
		auto mouseDif = mouseEnd - mouseBegin;
		for (int i = 0; i < m_rows; ++i) {
			for (int j = 0; j < m_cols; ++j) {
				if (cellData[i][j].transform.x < mouseBegin.x && 
					cellData[i][j].transform.y < mouseBegin.y &&
					cellData[i][j].transform.x + cellData[i][j].transform.w > mouseBegin.x && 
					cellData[i][j].transform.y + cellData[i][j].transform.h > mouseBegin.y) {
					switch ((abs(mouseDif.x) > abs(mouseDif.y)) ? (mouseDif.x < 0 ? MoveType::LEFT : MoveType::RIGHT) : (mouseDif.y < 0 ? MoveType::UP : MoveType::DOWN)) {
						case MoveType::LEFT:
						if (j - 1 > -1)	if (CandyID(i, j - 1) != ObjectID::CANDY_EMPTY)
							gridState = GridState::SWAPPING_CANDIES, swapInfo.Set(i, j, i, j - 1, CandyTransform(i, j), CandyTransform(i, j-1)); break;
						case MoveType::UP:
						if (i - 1 > -1)	if (CandyID(i-1, j) != ObjectID::CANDY_EMPTY)
							gridState = GridState::SWAPPING_CANDIES, swapInfo.Set(i, j, i - 1, j, CandyTransform(i, j), CandyTransform(i-1, j)); break;
						case MoveType::RIGHT:
						if (j + 1 < m_cols) if (CandyID(i, j + 1) != ObjectID::CANDY_EMPTY)
							gridState = GridState::SWAPPING_CANDIES, swapInfo.Set(i, j, i, j + 1, CandyTransform(i, j), CandyTransform(i, j+1)); break;
						case MoveType::DOWN:
						if (i + 1 < m_rows) if (CandyID(i+1, j) != ObjectID::CANDY_EMPTY)
							gridState = GridState::SWAPPING_CANDIES, swapInfo.Set(i, j, i + 1, j, CandyTransform(i, j), CandyTransform(i+1, j)); break;
					}  break;
				}
			}
		}
	}
}

bool Grid::CheckNeighbours(int i, int j) {
	auto id = CandyID(i, j);
	if (j - 1 > -1 && j + 1 < m_cols)	if (id == CandyID(i, j - 1) && id == CandyID(i, j + 1)) return true; // candy is in the middle horizontally
	if (i - 1 > -1 && i + 1 < m_rows)	if (id == CandyID(i - 1, j) && id == CandyID(i + 1, j)) return true; // candy is in the middle vertically
	if (j - 2 > -1)						if (id == CandyID(i, j - 1) && id == CandyID(i, j - 2)) return true; // candy is in the left
	if (i - 2 > -1)						if (id == CandyID(i - 1, j) && id == CandyID(i - 2, j)) return true; // candy is above
	if (j + 2 < m_cols)					if (id == CandyID(i, j + 1) && id == CandyID(i, j + 2)) return true; // candy is in the right
	if (i + 2 < m_rows)					if (id == CandyID(i + 1, j) && id == CandyID(i + 2, j)) return true; // candy is downwards
	return false;
}

int Grid::KillNeighbours(int i, int j) {
	auto id = CandyID(i, j);
	std::vector<Sprite*> candies; // vector of candies to be supressed
	std::vector<Sprite*> temp; // temp vector to check each line of candies
	// check vertically
	if (i + 1 < m_rows) for (int n = i+1; n < m_rows; ++n) if (id == CandyID(n, j)) temp.push_back(&cellData[n][j].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	if (i - 1 > -1) for (int n = i-1; n >= 0; --n) if (id == CandyID(n, j)) temp.push_back(&cellData[n][j].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	// check horizontally
	if (j + 1 < m_cols) for (int n = j+1; n < m_cols; ++n) if (id == CandyID(i, n)) temp.push_back(&cellData[i][n].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	if (j - 1 > -1) for (int n = j-1; n >= 0; --n) if (id == CandyID(i, n)) temp.push_back(&cellData[i][n].candy); else break;
	if (temp.size() > 1) candies.insert(candies.end(), temp.begin(), temp.end()); temp.clear();
	// check if main vector is filled, and then if so, kill candy neighbours
	if (!candies.empty()) {
		candies.push_back(&cellData[i][j].candy);
		for (auto c : candies) c->objectID = ObjectID::CANDY_EMPTY;
	}
	return int(candies.size());
}

inline int Lerp(float v0, float v1, float t) {
	return int(fma(t, v1, fma(-t, v0, v0)));
}

void Grid::Update(int &score) {
	switch (gridState) {
		case GridState::SWAPPING_CANDIES: {
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
				if (reSwap) { reSwap = false; gridState = GridState::WAITING; return; }
				if (!CheckNeighbours(in, jn) && !CheckNeighbours(i0, j0)) {
					std::swap(swapInfo.fromPos, swapInfo.toPos);
					std::swap(swapInfo.fromX, swapInfo.toX);
					std::swap(swapInfo.fromY, swapInfo.toY);
					reSwap = true; return;
				} else gridState = GridState::LINE_CHECKING;
			} else percent += TM.GetDeltaTime()*0.01f;
		} break;
		case GridState::LINE_CHECKING: { // check each line
			for (int i = m_rows - 1; i >= 0; --i)
				for (int j = m_cols - 1; j >= 0; --j)
					if (CandyID(i, j) != ObjectID::CANDY_EMPTY) { score += KillNeighbours(i, j); }
			gridState = GridState::SHIFTING_CANDIES;
			return;
		} break;
		case GridState::SHIFTING_CANDIES: {
			auto y0 = shiftInfo.fromPos, yf = shiftInfo.toPos, i = shiftInfo.i, j = shiftInfo.j;
			auto &percent = shiftInfo.percent;
			static bool endShifting = true;
			if (endShifting) {
				for (int i = m_rows - 2; i >= 0; --i) {
					for (int j = 0; j < m_cols; ++j) {
						if (CandyID(i, j) != ObjectID::CANDY_EMPTY && CandyID(i + 1, j) == ObjectID::CANDY_EMPTY) {
							shiftInfo.i = i, shiftInfo.j = j;
							shiftInfo.fromPos = CandyTransform(i, j).y;
							shiftInfo.toPos = CandyTransform(i + 1, j).y;
							percent = 0.0f;
							endShifting = false;
							return;
						}
					}
				} gridState = GridState::ADDING_CANDIES; return;
			} else {
				CandyTransform(i, j).y = Lerp(float(y0), float(yf), percent); // Lerp candies movement down
				// Check if swapping finished
				if (percent > 1.0f) {
					CandyTransform(i, j).y = yf;
					CandyTransform(i + 1, j).y = y0;
					std::swap(cellData[i][j].candy, cellData[i + 1][j].candy);
					if (i + 1 < m_rows)
						if (CandyID(i + 1, j) == ObjectID::CANDY_EMPTY) {
							shiftInfo.i = i, shiftInfo.j = j;
							shiftInfo.fromPos = CandyTransform(i, j).y;
							shiftInfo.toPos = CandyTransform(i + 1, j).y;
							return;
						}
					endShifting = true; return;
				} else percent += TM.GetDeltaTime()*0.01f;
			}
		} break;
		case GridState::ADDING_CANDIES: {
			static bool endAdding;
			endAdding = true;
			for (int i = 0; i < m_cols; ++i)
				if (CandyID(0, i) == ObjectID::CANDY_EMPTY) CandyID(0, i) = ObjectID(rand() % int(ObjectID::CANDY_MAX)), endAdding = false;
			gridState = (endAdding) ? GridState::WAITING : GridState::LINE_CHECKING;
		} break;
	}
}

void Grid::Draw() {
	for (int i = 0; i < m_rows; ++i) for (int j = 0; j < m_cols; ++j) cellData[i][j].Draw();
	for (int i = 0; i < m_rows; ++i) for (int j = 0; j < m_cols; ++j) if (CandyID(i, j) != ObjectID::CANDY_EMPTY) cellData[i][j].candy.Draw();
}