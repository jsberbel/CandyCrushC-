/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include <string>

#define SCREEN_FPS 60.0f
#define SCREEN_TICKS_PER_FRAME (1.0f / SCREEN_FPS)

class Scene; // Forward declaration of Scene class (it doesn't need to be included if it's used after)
// Game Engine class that serves as the Controller
class GameEngine {
public:
	explicit GameEngine(std::string &&name, int &&screenWidth, int &&screenHeight);
	~GameEngine() = default;
	void Run(void);
private:
	bool m_isRunning{ true };		// Whether game is running or not
	Scene *m_curScene{ nullptr };	// Pointer to the current scene
	void LoadMedia(void);
	void AddScenes(void);
	void Update(float deltaTime);
	void Draw(void);
};