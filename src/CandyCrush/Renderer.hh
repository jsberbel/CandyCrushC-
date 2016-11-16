/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <unordered_map>
#include "Resource.hh"
#include "Window.hh"
#include "Assert.hh"
#include "Logger.hh"
#include "GUID.hh"

#define R Renderer::Instance()

// Enum to store text font style
enum FontStyle { 
	FONT_STYLE_BOLD = TTF_STYLE_BOLD, 
	FONT_STYLE_ITALIC = TTF_STYLE_ITALIC,
	FONT_STYLE_UNDERLINE = TTF_STYLE_UNDERLINE,
	FONT_STYLE_NORMAL = TTF_STYLE_NORMAL
};

// Biblical Renderer class to manage the View of the Game
class Renderer {
	Renderer() {
		//Create renderer for window
		m_SDLRenderer = SDL_CreateRenderer(W(), -1, SDL_RENDERER_ACCELERATED);
		ASSERT(m_SDLRenderer != nullptr);
		//Initialize renderer color
		SDL_SetRenderDrawColor(m_SDLRenderer, 255, 255, 255, 255);
		//Initialize PNG loading
		constexpr int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
		ASSERT((IMG_Init(imgFlags) & imgFlags));
		ASSERT(TTF_Init() != -1);
	}
	Renderer(const Renderer &rhs) = default;
	Renderer &operator=(const Renderer &rhs) = default;
public:
	inline static Renderer &Instance(void) {
		static Renderer renderer;
		return renderer;
	}
	~Renderer() {
		for (auto &t : m_textureData) SDL_DestroyTexture(t.second), t.second = nullptr;
		for (auto &f : m_fontData) TTF_CloseFont(f.second), f.second = nullptr;
		SDL_DestroyRenderer(m_SDLRenderer), m_SDLRenderer = nullptr;
		IMG_Quit();
		TTF_Quit();
	}
	template<ObjectID id>
	void LoadTexture(std::string &&filename) {
		//Load image at specified path
		auto loadedSurface = IMG_Load(RESOURCE_FILE(filename));
		ASSERT_MSG(loadedSurface != nullptr, "Unable to load image " + filename);
		//Create texture from surface pixels
		ASSERT_MSG(m_textureData.emplace(id, SDL_CreateTextureFromSurface(m_SDLRenderer, loadedSurface)).second, "Unable to create texture from " + filename);
		SDL_FreeSurface(loadedSurface); //Get rid of loaded surface
	}
	template<FontID id>
	void LoadFont(std::string &&filename, int size) {
		ASSERT_MSG(m_fontData.emplace(id, TTF_OpenFont(RESOURCE_FILE(filename), size)).second, "Unable to create font from " + filename);
	}
	template<FontID id, FontStyle style>
	void SetFontStyle() { TTF_SetFontStyle(m_fontData[id], style); }
	void Push(SDL_Texture *texture, Transform transform) {
		SDL_RenderCopy(m_SDLRenderer, texture, nullptr, &transform());
	}
	void Push(ObjectID objectID, Transform transform) {
		SDL_RenderCopy(m_SDLRenderer, m_textureData[objectID], nullptr, &transform());
	}
	void Clear(void) const { SDL_RenderClear(m_SDLRenderer); };
	void Render(void) const { SDL_RenderPresent(m_SDLRenderer); };
	inline TTF_Font* GetFont(FontID fontID) { return m_fontData[fontID]; };
	inline SDL_Texture* CreateTexture(SDL_Surface *surface) { return SDL_CreateTextureFromSurface(m_SDLRenderer, surface); };
private:
	std::unordered_map<ObjectID, SDL_Texture*> m_textureData;
	std::unordered_map<FontID, TTF_Font*> m_fontData;
	SDL_Renderer* m_SDLRenderer{ nullptr };
};