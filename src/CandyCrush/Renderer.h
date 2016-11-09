#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include "ObjectID.h"

class Renderer {
	SDL_Window* m_SDLWindow = nullptr;
	SDL_Renderer* m_SDLRenderer = nullptr; 
	std::unordered_map<OBJECT_ID, SDL_Texture*> m_textureData;
public:
	Renderer(int screenWidth, int screenHeight);
	~Renderer();
	void LoadTexture(OBJECT_ID id, const std::string &path);
	void PushImage(OBJECT_ID id, SDL_Rect *rect);
	inline void Clear() const { SDL_RenderClear(m_SDLRenderer); };
	inline void Render() const { SDL_RenderPresent(m_SDLRenderer); };
};