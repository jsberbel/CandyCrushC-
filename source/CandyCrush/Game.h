#pragma once
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>

#include "Grid.h"
#include "TextureManager.h"

#define SCREEN_FPS 60.0f
#define SCREEN_TICKS_PER_FRAME (1.0f / SCREEN_FPS)

class Game {
	SDL_Window* window = nullptr; //The window we'll be rendering to
	SDL_Renderer* renderer = nullptr;
	TextureManager textureManager;
	Grid grid;
	bool isRunning;
	Sint32 mouseX = -1, mouseY = -1;

	void LoadTextures();
	void Update(float deltaTime);
	void Draw();
public:
	Game(int screenWidth, int screenHeight, int rows, int cols); // Starts up SDL and creates window
	~Game(); // Frees media and shuts down SDL
	void Run();
};
