#include "GameScene.h"
#include "GameEngine.h"
#include "Logger.h"
using namespace lgr;

void GameScene::Build() {
	m_grid = new Grid(MAX_ROWS, MAX_COLUMNS, CELL_WIDTH, CELL_HEIGHT, gameEngine->window);
	m_background = { { 0, 0, gameEngine->window.GetScreenWidth(), gameEngine->window.GetScreenHeight() }, BACKGROUND };
}

void GameScene::Destroy() {
	delete m_grid;
}

void GameScene::OnEntry() {
}

void GameScene::OnExit() {
}

void GameScene::Update(float deltatime) {
	static auto mouseX = 0, mouseY = 0;
	if (gameEngine->inputManager.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		Print("mxp: ", mouseX);
		mouseX = gameEngine->inputManager.GetMouseX(), mouseY = gameEngine->inputManager.GetMouseY();
	} else if (gameEngine->inputManager.IsMouseUp<MOUSE_BUTTON_LEFT>()) {
		Print("mx: ", mouseX);
		Print("mxn: ", gameEngine->inputManager.GetMouseX());
		auto difX = gameEngine->inputManager.GetMouseX() - mouseX, difY = gameEngine->inputManager.GetMouseY() - mouseY;
		m_grid->CheckMouseSwift((abs(difX) > abs(difY)) ? (difX < 0 ? LEFT : RIGHT) : (difY < 0 ? UP : DOWN), mouseX, mouseY);
	}
	m_grid->Update(deltatime, score); // Update grid
}

void GameScene::Draw() {
	gameEngine->renderer.PushSprite(m_background); // Render background
	m_grid->Draw(gameEngine->renderer); // Render grid
}
