#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include "Assert.h"

#define MAX_CANDIES 6
enum TEXTURE_ID {
	CANDY_BLUE, CANDY_GREEN, CANDY_YELLOW, CANDY_RED, CANDY_PURPLE, CANDY_ORANGE, EMPTY_CANDY, EMPTY_CELL, BG
};

class TextureManager {
	std::unordered_map<TEXTURE_ID, SDL_Texture*> data;
	SDL_Renderer* renderer = nullptr; 
public:
	TextureManager() = default;
	~TextureManager() { for (auto &t : data) SDL_DestroyTexture(t.second), t.second = nullptr; }
	inline void Init(SDL_Renderer* &renderer) { this->renderer = renderer; }
	void LoadTexture(TEXTURE_ID id, const std::string &path) {
		SDL_Texture* newTexture = nullptr; //The final texture
		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		ASSERT(loadedSurface == nullptr, ("Unable to load image " + path).c_str());
		//Create texture from surface pixels
		data[id] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		ASSERT(data[id] == nullptr, ("Unable to create texture from " + path).c_str());
		SDL_FreeSurface(loadedSurface); //Get rid of old loaded surface
	}
	void Render(TEXTURE_ID id, SDL_Rect *rect) {
		SDL_RenderCopy(renderer, data[id], nullptr, rect);
	}
};