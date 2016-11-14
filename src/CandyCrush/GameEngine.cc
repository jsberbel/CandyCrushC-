#include "GameEngine.hh"
#include "GameScene.hh"
#include "System.hh"
#include "Logger.hh"
using namespace Logger;

GameEngine::GameEngine(std::string &&name, int screenWidth, int screenHeight) {
	Window::Instance(name, screenWidth, screenHeight);
	SM.AddScene<GameScene>();
	m_curScene = SM.SetScene<GameScene>();
}

void GameEngine::LoadTextures(void) {
	R.LoadTexture<EMPTY_CELL>("gfx/empty.png");
	R.LoadTexture<CANDY_BLUE>("gfx/blue.png");
	R.LoadTexture<CANDY_RED>("gfx/red.png");
	R.LoadTexture<CANDY_ORANGE>("gfx/orange.png");
	R.LoadTexture<CANDY_PURPLE>("gfx/purple.png");
	R.LoadTexture<CANDY_GREEN>("gfx/green.png");
	R.LoadTexture<CANDY_YELLOW>("gfx/yellow.png");
	R.LoadTexture<BACKGROUND>("gfx/bg.jpg");
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
		R.Clear(); // Clear screen
		m_curScene->Draw(); // Then call the draw method of the screen
		R.Render(); // Update screen
	}
}

void GameEngine::Run(void) {
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