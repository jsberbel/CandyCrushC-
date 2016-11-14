#include "GameEngine.hh"
#include <SDL/SDL.h>
#include "IOManager.hh"

// Game constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char* args[]) {
	//IOManager::TestXML("test.xml");
	GameEngine("Candy Crush", SCREEN_WIDTH, SCREEN_HEIGHT).Run();
	return 0;
}