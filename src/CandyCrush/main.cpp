#include "Game.h"
#include <exception>

// Screen and grid dimensions constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_ROWS 6
#define MAX_COLUMNS 7
#define CELL_WIDTH 80
#define CELL_HEIGHT 80

/*
GAME
	- renderer: el encargado de pintar las im�genes
	- grid: almacena toda la informaci�n l�gica del juego
	- isRunning: bool para indicar si el juego est� en activo o se ha terminado

RENDERER
	- SDLWindow: la ventana con la que podemos interactuar
	- SDLRenderer: el encargado de pintar las im�genes por pantalla
	- textureData: el que almacena la informaci�n de las texturas cargadas de fichero

GRID
	- cellData: matriz de celdas que contienen los candies
	- rows & cols: ints para las filas y las columnas de la matriz
	- gridState: game state de la m�quina de estados del funcionamiento de la matriz
	(...)

OBJECTID
	Contiene el enum y la informaci�n necesaria sobre los IDs de cada game object
*/

int main(int argc, char* args[]) {
	Window window("Candy Crush", SCREEN_WIDTH, SCREEN_HEIGHT);
	Renderer renderer(window);
	Grid grid(MAX_ROWS, MAX_COLUMNS, CELL_WIDTH, CELL_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
	Game game(window, renderer, grid);
	game.Run();
	return 0;
}