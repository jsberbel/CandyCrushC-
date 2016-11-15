/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GameEngine.hh"
#include "GameScene.hh"
#include "System.hh"
#include "Logger.hh"
using namespace Logger;

GameEngine::GameEngine(std::string &&name, int &&screenWidth, int &&screenHeight) { Window::Instance(std::move(name), std::move(screenWidth), std::move(screenHeight)); }

void GameEngine::LoadMedia(void) {
	R.LoadFont<FontID::ARIAL>("fnt/arial.ttf", 30);
	R.LoadTexture<ObjectID::CELL_EMPTY>("gfx/empty.png");
	R.LoadTexture<ObjectID::CANDY_BLUE>("gfx/blue.png");
	R.LoadTexture<ObjectID::CANDY_RED>("gfx/red.png");
	R.LoadTexture<ObjectID::CANDY_ORANGE>("gfx/orange.png");
	R.LoadTexture<ObjectID::CANDY_PURPLE>("gfx/purple.png");
	R.LoadTexture<ObjectID::CANDY_GREEN>("gfx/green.png");
	R.LoadTexture<ObjectID::CANDY_YELLOW>("gfx/yellow.png");
	R.LoadTexture<ObjectID::BG_00>("gfx/bg.jpg");
}

void GameEngine::AddScenes(void) {
	SM.AddScene<GameScene>();
	m_curScene = SM.SetScene<GameScene>();
}

void GameEngine::Update(float deltaTime) {
	if (m_curScene) // If current screen exists
		switch (m_curScene->GetState()) { // Check for the state of the screen
			case SceneState::RUNNING:
			IM.Update();	// Update the input manager instance
			if (IM.HasQuit()) m_isRunning = false;
			m_curScene->Update(deltaTime); // Update the current screen if running
			break;
			case SceneState::EXIT: m_isRunning = false; break;
			case SceneState::SLEEP: default: break;
		}
}

void GameEngine::Draw(void) {
	if (m_curScene && m_curScene->CheckState<SceneState::RUNNING>()) { // If screen object exists and its state is running
		R.Clear(); // Clear screen buffer
		m_curScene->Draw(); // Call the draw method of the scene
		R.Render(); // Update screen buffer with all sprites that were pushed
	}
}

void GameEngine::Run(void) {
	LoadMedia(); // Load main game textures
	AddScenes(); // Generate game scenes and set first scene
	float m_deltatime{ .0f }; // Delta time in seconds
	auto lastTime = SDL_GetTicks(); // Last time sample in seconds
	float renderTime = 0.0f; // Time control for rendering
	m_isRunning = true;
	while (m_isRunning) {
		m_deltatime = float(SDL_GetTicks() - lastTime);
		lastTime = SDL_GetTicks();
		Update(m_deltatime);
		if (renderTime >= SCREEN_TICKS_PER_FRAME)
			Draw(), renderTime -= SCREEN_TICKS_PER_FRAME;
		//if (SDL_GetTicks() % 1000 < 1) std::cout << "Game Score: " << score << std::endl;
		renderTime += m_deltatime; // Updates the render timer
	}
}