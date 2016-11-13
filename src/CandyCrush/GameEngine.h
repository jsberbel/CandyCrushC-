#pragma once
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>

#include "Window.h"
#include "Renderer.h"
#include "InputManager.h"
#include "SceneManager.h"

#define SCREEN_FPS 60.0f
#define SCREEN_TICKS_PER_FRAME (1.0f / SCREEN_FPS)

class Scene;
class GameEngine {
public:
	Window window;
	Renderer renderer;
	InputManager inputManager;
	SceneManager sceneManager;
	explicit GameEngine(std::string &&name, int screenWidth, int screenHeight);
	~GameEngine() = default;
	void Run();
private:
	bool m_isRunning{ true };			// Whether game is running or not
	Scene *m_curScene{ nullptr };
	void LoadTextures();
	void Update(float deltaTime);
	void Draw();
};