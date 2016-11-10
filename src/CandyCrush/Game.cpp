#include "Game.h"
#include "Logger.h"
using namespace lgr;

Game::Game(Window &window, Renderer &renderer, Grid &grid) :
	m_window(window),
	m_renderer(renderer),
	m_grid(grid),
	m_inputManager(InputManager::Instance())
{
	m_background = { { 0, 0, m_window.GetScreenWidth(), m_window.GetScreenHeight() }, BACKGROUND };
}

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
		Print("mxp: ", mouseX);
		mouseX = m_inputManager.GetMouseX(), mouseY = m_inputManager.GetMouseY();
	}
	else if (m_inputManager.IsMouseUp<MOUSE_BUTTON_LEFT>()) {
		Print("mx: ", mouseX);
		Print("mxn: ", m_inputManager.GetMouseX());
		auto difX = m_inputManager.GetMouseX() - mouseX, difY = m_inputManager.GetMouseY() - mouseY;
		m_grid.CheckMouseSwift((abs(difX) > abs(difY)) ? (difX < 0 ? LEFT : RIGHT) : (difY < 0 ? UP : DOWN), mouseX, mouseY);
	}
	if (m_inputManager.IsKeyHold<'x'>()) Print("x hold");
	if (m_inputManager.IsKeyDown<'w'>()) Print("w down");
	if (m_inputManager.IsKeyUp<'f'>()) Print("f up");
	if (m_inputManager.HasQuit()) m_isRunning = false;
	m_grid.Update(deltaTime, score); // Update grid
}

void Game::Draw() {
	m_renderer.Clear(); // Clear screen
	m_renderer.PushSprite(m_background); // Render background
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