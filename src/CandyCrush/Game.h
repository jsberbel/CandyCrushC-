#pragma once
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>

#include "Grid.h"
#include "Window.h"
#include "Renderer.h"
#include "InputManager.h"

#define SCREEN_FPS 60.0f
#define SCREEN_TICKS_PER_FRAME (1.0f / SCREEN_FPS)

class Game {
public:
	Game(Window &window, Renderer &renderer, Grid &grid);
	~Game() = default;
	void Run();
private:
	Grid &m_grid;
	Sprite m_background;

	Window &m_window;
	Renderer &m_renderer;
	InputManager &m_inputManager;

	bool m_isRunning;
	int score = 0;

	void LoadTextures();
	void Update(Uint32 deltaTime);
	void Draw();
};
