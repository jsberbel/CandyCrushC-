#include "Game.h"
#include <exception>

// Screen dimension constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_ROWS 6
#define MAX_COLUMNS 7

int main(int argc, char* args[]) {
	try {
		Game game(SCREEN_WIDTH, SCREEN_HEIGHT, MAX_ROWS, MAX_COLUMNS);
		game.Run();
	} catch (std::exception e) {
		std::cerr << e.what() << std::endl;
		auto sdlError = SDL_GetError();
		if (strcmp(sdlError, "")) std::cerr << "SDL Error: " << sdlError << std::endl;
		auto imgError = IMG_GetError();
		if (strcmp(imgError, "")) std::cerr << "SDL_img Error: " << imgError << std::endl;
		std::cin.get();
	}
	return 0;
}