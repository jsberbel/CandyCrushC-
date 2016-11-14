#pragma once
#include "Scene.hh"
#include "Grid.hh"

#define MAX_ROWS 6
#define MAX_COLUMNS 7
#define CELL_WIDTH 80
#define CELL_HEIGHT 80

class GameScene : public Scene {
public:
	explicit GameScene();
	~GameScene() override;
	void OnEntry() override;
	void OnExit() override;
	void Update(float deltatime) override;
	void Draw() override;
private:
	Grid m_grid;
	Sprite m_background;
	int score{ 0 };
};