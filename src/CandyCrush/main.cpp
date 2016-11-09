#include "Game.h"
#include <exception>

// Screen and grid dimensions constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_ROWS 6		// --> provisional
#define MAX_COLUMNS 7	// --> provisional

/*
GAME
	- renderer: el encargado de pintar las imágenes
	- grid: almacena toda la información lógica del juego
	- isRunning: bool para indicar si el juego está en activo o se ha terminado

RENDERER
	- SDLWindow: la ventana con la que podemos interactuar
	- SDLRenderer: el encargado de pintar las imágenes por pantalla
	- textureData: el que almacena la información de las texturas cargadas de fichero

GRID
	- cellData: matriz de celdas que contienen los candies
	- rows & cols: ints para las filas y las columnas de la matriz
	- gridState: game state de la máquina de estados del funcionamiento de la matriz
	(...)
OBJECTID
	Contiene el enum y la información necesaria sobre los IDs de cada game object
*/

int main(int argc, char* args[]) {
	try {
		Game game(SCREEN_WIDTH, SCREEN_HEIGHT, MAX_ROWS, MAX_COLUMNS);
		game.Run();
	} catch (std::exception e) {
		std::cerr << e.what() << std::endl;
		auto sdlError = SDL_GetError();
		if (strcmp(sdlError, "")) std::cerr << "SDL Error: " << sdlError << std::endl;
		auto imgError = IMG_GetError();
		if (strcmp(imgError, "") && strcmp(imgError, sdlError)) std::cerr << "SDL_img Error: " << imgError << std::endl;
		std::cin.get();
	}
	return 0;
}