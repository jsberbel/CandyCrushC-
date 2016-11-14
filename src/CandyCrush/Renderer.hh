#pragma once
#include <SDL/SDL_image.h>
#include <unordered_map>
#include "Assert.hh"
#include "Sprite.hh"
#include "Window.hh"
#include "Resource.hh"

#define R Renderer::Instance()

class Renderer {
	Renderer() {
		//Create renderer for window
		m_SDLRenderer = SDL_CreateRenderer(Window::Instance()(), -1, SDL_RENDERER_ACCELERATED);
		ASSERT(m_SDLRenderer != nullptr);
		//Initialize renderer color
		SDL_SetRenderDrawColor(m_SDLRenderer, 255, 255, 255, 255);
		//Initialize PNG loading
		constexpr int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
		ASSERT((IMG_Init(imgFlags) & imgFlags));
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
		SDL_DestroyRenderer(m_SDLRenderer), m_SDLRenderer = nullptr;
		IMG_Quit();
	}
	template<ObjectID id>
	void LoadTexture(std::string &&filename) {
		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(RESOURCE_FILE(filename));
		ASSERT_MSG(loadedSurface != nullptr, "Unable to load image " + filename);
		//Create texture from surface pixels
		m_textureData[id] = SDL_CreateTextureFromSurface(m_SDLRenderer, loadedSurface);
		ASSERT_MSG(m_textureData[id] != nullptr, "Unable to create texture from " + filename);
		SDL_FreeSurface(loadedSurface); //Get rid of old loaded surface
	}
	void PushSprite(const Sprite &sprite) {
		SDL_RenderCopy(m_SDLRenderer, m_textureData[sprite.id], nullptr, &sprite.transform.rect);
	}
	inline void Clear() const { SDL_RenderClear(m_SDLRenderer); };
	inline void Render() const { SDL_RenderPresent(m_SDLRenderer); };
private:
	SDL_Renderer* m_SDLRenderer{ nullptr };
	std::unordered_map<ObjectID, SDL_Texture*> m_textureData;
};