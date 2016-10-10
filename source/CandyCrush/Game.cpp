#include "Game.h"
#include "Assert.h"

#define CELL_WIDTH 80
#define CELL_HEIGHT 80

Game::Game(int screenWidth, int screenHeight, int rows, int cols) : 
	grid(rows, cols, CELL_WIDTH, CELL_HEIGHT, screenWidth, screenHeight)
{
	//Initialize SDL & Set texture filtering to linear
	ASSERT(SDL_Init(SDL_INIT_VIDEO) < 0, "SDL could not initialize!"); 
	ASSERT(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"), "Warning: Linear texture filtering not enabled!");
	//Create window
	window = SDL_CreateWindow("Candy Crush", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	ASSERT(window == nullptr, "Window could not be created!");
	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	ASSERT(renderer == nullptr, "Renderer could not be created!");
	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	ASSERT(!(IMG_Init(imgFlags) & imgFlags), "SDL_image could not initialize!");
	//Init texture manager
	textureManager.Init(renderer);
}

Game::~Game() {
	//Destroy window	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = nullptr;
	renderer = nullptr;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Game::LoadTextures() {
	textureManager.LoadTexture(EMPTY_CELL, "../../res/gfx/empty.png");
	textureManager.LoadTexture(CANDY_BLUE, "../../res/gfx/blue.png");
	textureManager.LoadTexture(CANDY_RED, "../../res/gfx/red.png");
	textureManager.LoadTexture(CANDY_ORANGE, "../../res/gfx/orange.png");
	textureManager.LoadTexture(CANDY_PURPLE, "../../res/gfx/purple.png");
	textureManager.LoadTexture(CANDY_GREEN, "../../res/gfx/green.png");
	textureManager.LoadTexture(CANDY_YELLOW, "../../res/gfx/yellow.png");
	textureManager.LoadTexture(BG, "../../res/gfx/bg.jpg");
}

void Game::Update(float deltaTime) {
	// Poll SDL events
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
			case SDL_QUIT:
			isRunning = false;
			break;
			case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT) {
				mouseX = e.button.x;
				mouseY = e.button.y;
			} break;
			case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_LEFT) {
				auto difX = e.button.x - mouseX;
				auto difY = e.button.y - mouseY;
				if (abs(difX) > abs(difY)) grid.CheckSwap(difX < 0 ? LEFT : RIGHT, mouseX, mouseY);
				else grid.CheckSwap(difY < 0 ? UP : DOWN, mouseX, mouseY);
			} break;
		}
	}
	// Update grid
	grid.Update(deltaTime);
}

void Game::Draw() {
	SDL_RenderClear(renderer); //Clear screen
	textureManager.Render(BG, nullptr);
	grid.Draw(textureManager); //Render grid
	SDL_RenderPresent(renderer); //Update screen
}

void Game::Run() {
	LoadTextures();

	float deltaTime = 0.0f; //delta time in seconds
	auto lastTime = SDL_GetTicks(); //last time sample in seconds
	float renderTime = 0.0f; //time control for rendering

	isRunning = true;
	while (isRunning) {
		deltaTime = SDL_GetTicks() - lastTime;
		lastTime = SDL_GetTicks();
		Update(deltaTime);
		if (renderTime >= SCREEN_TICKS_PER_FRAME) {
			Draw();
			renderTime -= SCREEN_TICKS_PER_FRAME; //do not set to zero, remove the accumulated frame time to avoid skipping
		}
		renderTime += deltaTime; //updates the render timer
	}
}