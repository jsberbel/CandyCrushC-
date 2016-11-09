#include "Renderer.h"
#include "Assert.h"

Renderer::Renderer(int screenWidth, int screenHeight) {
	//Initialize SDL & Set texture filtering to linear
	ASSERT(SDL_Init(SDL_INIT_VIDEO) < 0, "SDL could not initialize!");
	ASSERT(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"), "Warning: Linear texture filtering not enabled!");
	//Create window
	m_SDLWindow = SDL_CreateWindow("Candy Crush", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	ASSERT(m_SDLWindow == nullptr, "Window could not be created!");
	//Create renderer for window
	m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
	ASSERT(m_SDLRenderer == nullptr, "Renderer could not be created!");
	//Initialize renderer color
	SDL_SetRenderDrawColor(m_SDLRenderer, 255, 255, 255, 255);
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	ASSERT(!(IMG_Init(imgFlags) & imgFlags), "SDL_image could not initialize!");
}

Renderer::~Renderer() {
	for (auto &t : m_textureData) SDL_DestroyTexture(t.second), t.second = nullptr; 
	//Destroy window	
	SDL_DestroyRenderer(m_SDLRenderer);
	SDL_DestroyWindow(m_SDLWindow);
	m_SDLWindow = nullptr;
	m_SDLRenderer = nullptr;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Renderer::LoadTexture(OBJECT_ID id, const std::string &path) {
	SDL_Texture* newTexture = nullptr; //The final texture
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	ASSERT(loadedSurface == nullptr, ("Unable to load image " + path).c_str());
	//Create texture from surface pixels
	m_textureData[id] = SDL_CreateTextureFromSurface(m_SDLRenderer, loadedSurface);
	ASSERT(m_textureData[id] == nullptr, ("Unable to create texture from " + path).c_str());
	SDL_FreeSurface(loadedSurface); //Get rid of old loaded surface
}

void Renderer::PushImage(OBJECT_ID id, SDL_Rect *rect) {
	SDL_RenderCopy(m_SDLRenderer, m_textureData[id], nullptr, rect);
}