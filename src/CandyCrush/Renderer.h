#pragma once
#include <SDL_image.h>
#include <unordered_map>
#include "Assert.h"
#include "Sprite.h"
#include "Window.h"

class Renderer {
public:
	Renderer(Window &window) {
		//Create renderer for window
		m_SDLRenderer = SDL_CreateRenderer(window(), -1, SDL_RENDERER_ACCELERATED);
		ASSERT(m_SDLRenderer != nullptr);
		//Initialize renderer color
		SDL_SetRenderDrawColor(m_SDLRenderer, 255, 255, 255, 255);
		//Initialize PNG loading
		constexpr int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
		ASSERT((IMG_Init(imgFlags) & imgFlags));
	}
	~Renderer() {
		for (auto &t : m_textureData) SDL_DestroyTexture(t.second), t.second = nullptr;	
		SDL_DestroyRenderer(m_SDLRenderer), m_SDLRenderer = nullptr;
		IMG_Quit();
	}
	void LoadTexture(OBJECT_ID id, const std::string &path) {
		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		ASSERT_MSG(loadedSurface != nullptr, "Unable to load image " + path);
		//Create texture from surface pixels
		m_textureData[id] = SDL_CreateTextureFromSurface(m_SDLRenderer, loadedSurface);
		ASSERT_MSG(m_textureData[id] != nullptr, "Unable to create texture from " + path);
		SDL_FreeSurface(loadedSurface); //Get rid of old loaded surface
	}
	void PushSprite(const Sprite &sprite) {
		SDL_RenderCopy(m_SDLRenderer, m_textureData[sprite.id], nullptr, &sprite.transform.rect);
	}
	inline void Clear() const { SDL_RenderClear(m_SDLRenderer); };
	inline void Render() const { SDL_RenderPresent(m_SDLRenderer); };
private:
	SDL_Renderer* m_SDLRenderer{ nullptr };
	std::unordered_map<OBJECT_ID, SDL_Texture*> m_textureData;
};