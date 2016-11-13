#include "GameEngine.h"
#include "GameScene.h"
#include "Logger.h"
using namespace lgr;

GameEngine::GameEngine(std::string &&name, int screenWidth, int screenHeight) : 
	window(name, screenWidth, screenHeight), 
	renderer(window), 
	inputManager(InputManager::Instance()),
	sceneManager(this) 
{
	sceneManager.AddScene<GameScene>();
	m_curScene = sceneManager.SetScene<GameScene>();
}

void GameEngine::LoadTextures() {
	renderer.LoadTexture(EMPTY_CELL, "../../res/gfx/empty.png");
	renderer.LoadTexture(CANDY_BLUE, "../../res/gfx/blue.png");
	renderer.LoadTexture(CANDY_RED, "../../res/gfx/red.png");
	renderer.LoadTexture(CANDY_ORANGE, "../../res/gfx/orange.png");
	renderer.LoadTexture(CANDY_PURPLE, "../../res/gfx/purple.png");
	renderer.LoadTexture(CANDY_GREEN, "../../res/gfx/green.png");
	renderer.LoadTexture(CANDY_YELLOW, "../../res/gfx/yellow.png");
	renderer.LoadTexture(BACKGROUND, "../../res/gfx/bg.jpg");
}

void GameEngine::Update(float deltaTime) {
	if (m_curScene) { // If current screen exists
		switch (m_curScene->GetState()) { // Check for the state of the screen
			case SceneState::RUNNING:
			inputManager.Update();	// Update the input manager instance
			m_curScene->Update(deltaTime); // Update the current screen if running
			break;
			case SceneState::EXIT: m_isRunning = false; break;
			case SceneState::SLEEP: default: break;
		}
	}
	if (inputManager.IsKeyHold<'x'>()) Print("x hold");
	if (inputManager.IsKeyDown<'w'>()) Print("w down");
	if (inputManager.IsKeyUp<'f'>()) Print("f up");
	if (inputManager.HasQuit()) m_isRunning = false;
}

void GameEngine::Draw() {
	if (m_curScene && m_curScene->CheckState(SceneState::RUNNING)) { // If screen object exists and its state is running
		renderer.Clear(); // Clear screen
		m_curScene->Draw(); // Then call the draw method of the screen
		renderer.Render(); // Update screen
	}
}

void GameEngine::Run() {
	LoadTextures(); // Load main game textures
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