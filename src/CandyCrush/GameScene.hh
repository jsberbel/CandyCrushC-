/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "Scene.hh"
#include "Grid.hh"
#include "Text.hh"

class GameScene : public Scene {
public:
	explicit GameScene();
	~GameScene() override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(float deltatime) override;
	void Draw(void) override;
private:
	Grid m_grid;
	Sprite m_background;
	TextBG m_scoreText;
	int m_score{ 0 };
};