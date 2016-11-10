#pragma once
#include <SDL_image.h>
#include <unordered_map>
#include "Sprite.h"

class Renderer {
	SDL_Window* m_SDLWindow{ nullptr };
	SDL_Renderer* m_SDLRenderer{ nullptr };
	const int m_screenWidth, m_screenHeight;
	std::unordered_map<OBJECT_ID, SDL_Texture*> m_textureData;
public:
	Renderer(int screenWidth, int screenHeight);
	~Renderer();
	void LoadTexture(OBJECT_ID id, const std::string &path);
	void PushSprite(const Sprite &sprite);
	inline void Clear() const { SDL_RenderClear(m_SDLRenderer); };
	inline void Render() const { SDL_RenderPresent(m_SDLRenderer); };
	inline int GetScreenWidth() const { return m_screenWidth; };
	inline int GetScreenHeight() const { return m_screenHeight; };
};