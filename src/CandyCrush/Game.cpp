#include "Game.h"
#include "Assert.h"

#define CELL_WIDTH 80
#define CELL_HEIGHT 80

Game::Game(int screenWidth, int screenHeight, int rows, int cols) : 
	m_grid(rows, cols, CELL_WIDTH, CELL_HEIGHT, screenWidth, screenHeight),
	m_renderer(screenWidth, screenHeight),
	m_inputManager(InputManager::Instance())
{}

void Game::LoadTextures() {
	m_renderer.LoadTexture(EMPTY_CELL, "../../res/gfx/empty.png");
	m_renderer.LoadTexture(CANDY_BLUE, "../../res/gfx/blue.png");
	m_renderer.LoadTexture(CANDY_RED, "../../res/gfx/red.png");
	m_renderer.LoadTexture(CANDY_ORANGE, "../../res/gfx/orange.png");
	m_renderer.LoadTexture(CANDY_PURPLE, "../../res/gfx/purple.png");
	m_renderer.LoadTexture(CANDY_GREEN, "../../res/gfx/green.png");
	m_renderer.LoadTexture(CANDY_YELLOW, "../../res/gfx/yellow.png");
	m_renderer.LoadTexture(BACKGROUND, "../../res/gfx/bg.jpg");
}

void Game::Update(Uint32 deltaTime) {
	m_inputManager.Update();
	static auto mouseX = 0, mouseY = 0;
	if (m_inputManager.IsMouseDown<MOUSE_BUTTON_LEFT>()) {
		std::cout << "mxp: " << mouseX << std::endl;
		mouseX = m_inputManager.GetMouseX(), mouseY = m_inputManager.GetMouseY();
	}
	else if (m_inputManager.IsMouseUp<MOUSE_BUTTON_LEFT>()) {
		std::cout << "mx: " << mouseX << std::endl;
		std::cout << "mxn: " << m_inputManager.GetMouseX() << std::endl;
		auto difX = m_inputManager.GetMouseX() - mouseX, difY = m_inputManager.GetMouseY() - mouseY;
		m_grid.CheckMouseSwift((abs(difX) > abs(difY)) ? (difX < 0 ? LEFT : RIGHT) : (difY < 0 ? UP : DOWN), mouseX, mouseY);
	}
	if (m_inputManager.IsKeyHold<'x'>()) std::cout << "x hold" << std::endl;
	if (m_inputManager.IsKeyDown<'w'>()) std::cout << "w down" << std::endl;
	if (m_inputManager.IsKeyUp<'f'>()) std::cout << "f up" << std::endl;
	if (m_inputManager.HasQuit()) m_isRunning = false;
	m_grid.Update(deltaTime, score); // Update grid
}

void Game::Draw() {
	m_renderer.Clear(); // Clear screen
	m_renderer.PushImage(BACKGROUND, nullptr); // Render background
	m_grid.Draw(m_renderer); // Render grid
	m_renderer.Render(); // Update screen
}

void Game::Run() {
	LoadTextures(); // Load main game textures
	Uint32 deltaTime = 0; // Delta time in seconds
	auto lastTime = SDL_GetTicks(); // Last time sample in seconds
	float renderTime = 0.0f; // Time control for rendering
	m_isRunning = true;
	while (m_isRunning) {
		deltaTime = SDL_GetTicks() - lastTime;
		lastTime = SDL_GetTicks();
		Update(deltaTime);
		if (renderTime >= SCREEN_TICKS_PER_FRAME)
			Draw(), renderTime -= SCREEN_TICKS_PER_FRAME;
		//if (SDL_GetTicks() % 1000 < 1) std::cout << "Game Score: " << score << std::endl;
		renderTime += deltaTime; // Updates the render timer
	}
}