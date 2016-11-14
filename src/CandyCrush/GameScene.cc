#include "GameScene.hh"
#include "System.hh"
#include "Logger.hh"
using namespace Logger;

GameScene::GameScene() : m_grid(MAX_ROWS, MAX_COLUMNS, CELL_WIDTH, CELL_HEIGHT) {
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, BACKGROUND };
}

GameScene::~GameScene(){
}

void GameScene::OnEntry() {
}

void GameScene::OnExit() {
}

void GameScene::Update(float deltatime) {
	static auto mouseX = 0, mouseY = 0;
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		Print("mxp: ", mouseX);
		mouseX = IM.GetMouseX(), mouseY = IM.GetMouseY();
	} else if (IM.IsMouseUp<MOUSE_BUTTON_LEFT>()) {
		Print("mx: ", mouseX);
		Print("mxn: ", IM.GetMouseX());
		auto difX = IM.GetMouseX() - mouseX, difY = IM.GetMouseY() - mouseY;
		m_grid.CheckMouseSwift((abs(difX) > abs(difY)) ? (difX < 0 ? LEFT : RIGHT) : (difY < 0 ? UP : DOWN), mouseX, mouseY);
	}
	m_grid.Update(deltatime, score); // Update grid
	if (IM.IsKeyHold<'x'>()) Print("x hold");
	if (IM.IsKeyDown<'w'>()) Print("w down");
	if (IM.IsKeyUp<'f'>()) Print("f up");
}

void GameScene::Draw() {
	R.PushSprite(m_background); // Render background
	m_grid.Draw(); // Render grid
}
