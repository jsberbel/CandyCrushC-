/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include <string>
#include <SDL/SDL.h>
#include "Assert.hh"

#define W Window::Instance()

// Window class to store the whole info related to the game screen interface
class Window {
	Window(const std::string &&name, int &&screenWidth, int &&screenHeight) : m_name(name), m_screenWidth(screenWidth), m_screenHeight(screenHeight) {
		//Initialize SDL & Set texture filtering to linear
		ASSERT(!SDL_Init(SDL_INIT_EVERYTHING));
		ASSERT(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"));
		//Create window
		m_SDLWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		ASSERT(m_SDLWindow != nullptr);
	}
	Window(const Window &rhs) = delete;
	Window &operator=(const Window &rhs) = delete;
public:
	inline static Window &Instance(std::string &&name = "", int &&screenWidth = 0, int &&screenHeight = 0) {
		static Window window(std::move(name), std::move(screenWidth), std::move(screenHeight));
		return window;
	}
	~Window() {
		SDL_DestroyWindow(m_SDLWindow), m_SDLWindow = nullptr; //Destroy window
		SDL_Quit(); //Quit SDL subsystems
	}
	inline SDL_Window* operator()(void) const		{ return m_SDLWindow; }
	inline int GetWidth(void) const					{ return m_screenWidth; };
	inline int GetHeight(void) const				{ return m_screenHeight; };
	inline const std::string &GetName(void) const	{ return m_name; };
private:
	SDL_Window* m_SDLWindow{ nullptr };
	const int m_screenWidth, m_screenHeight;
	const std::string m_name;
};