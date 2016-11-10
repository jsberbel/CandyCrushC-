#pragma once
#include <SDL.h>
#include <string>
#include "Assert.h"

class Window {
public:
	Window(std::string &&name, int screenWidth, int screenHeight) : m_name(name), m_screenWidth(screenWidth), m_screenHeight(screenHeight) {
		//Initialize SDL & Set texture filtering to linear
		ASSERT(!SDL_Init(SDL_INIT_EVERYTHING));
		ASSERT(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"));
		//Create window
		m_SDLWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		ASSERT(m_SDLWindow != nullptr);
	}
	~Window() {
		SDL_DestroyWindow(m_SDLWindow), m_SDLWindow = nullptr; //Destroy window
		SDL_Quit(); //Quit SDL subsystems
	}
	inline SDL_Window* operator()() const	{ return m_SDLWindow; }
	inline int GetScreenWidth() const		{ return m_screenWidth; };
	inline int GetScreenHeight() const		{ return m_screenHeight; };
private:
	SDL_Window* m_SDLWindow{ nullptr };
	const int m_screenWidth, m_screenHeight;
	const std::string m_name;
};