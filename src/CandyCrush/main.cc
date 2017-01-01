/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GameEngine.hh"
#include "IOManager.hh"

// Game constants
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

int main(int argc, char* args[]) {
	IOManager::TestXML("cfg/test2.txt");
	GameEngine::Run<SCREEN_WIDTH, SCREEN_HEIGHT>("ENTI Crush");
	return 0;
}