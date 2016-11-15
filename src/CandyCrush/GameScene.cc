/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GameScene.hh"
#include "System.hh"
#include "Logger.hh"
using namespace Logger;

#define CELL_WIDTH 80
#define CELL_HEIGHT 80

GameScene::GameScene(void) : m_grid("lvl/testLvl.dat", CELL_WIDTH, CELL_HEIGHT) {
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BG_00 };
	m_scoreText = R.CreateTextShaded<FontID::ARIAL>({ 0, 0 }, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
}

GameScene::~GameScene(void){
}

void GameScene::OnEntry(void) {
}

void GameScene::OnExit(void) {
}

void GameScene::Update(float deltatime) {
	static MouseCoords mouseCoords(0,0);
	if (IM.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		Println("===============");
		Println("mxp: ", mouseCoords);
		mouseCoords = IM.GetMouseCoords();
	} else if (IM.IsMouseUp<MOUSE_BUTTON_LEFT>()) {
		Println("mxn: ", IM.GetMouseCoords());
		m_grid.CheckMouseSwift(mouseCoords, IM.GetMouseCoords());
	}
	m_grid.Update(deltatime, m_score); // Update grid
	// Test InputManager key methods
	if (IM.IsKeyHold<'x'>()) Println("x hold");
	if (IM.IsKeyDown<'w'>()) Println("w down");
	if (IM.IsKeyUp<'f'>()) Println("f up");
}

void GameScene::Draw(void) {
	R.Push(m_background); // Render background
	m_grid.Draw(); // Render grid
	R.Push(m_scoreText, "Score: " + std::to_string(m_score));
}
