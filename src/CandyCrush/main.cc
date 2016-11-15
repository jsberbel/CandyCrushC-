/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GameEngine.hh"
#include <SDL/SDL.h>

// Game constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char* args[]) {
	//IOManager::TestXML("test.xml");	
	GameEngine("Candy Crush", SCREEN_WIDTH, SCREEN_HEIGHT).Run();
	return 0;
}