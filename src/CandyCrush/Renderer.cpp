#include "Renderer.h"
#include "Assert.h"

Renderer::Renderer(int screenWidth, int screenHeight) : m_screenWidth(screenWidth), m_screenHeight(screenHeight) {
	//Initialize SDL & Set texture filtering to linear
	ASSERT(SDL_Init(SDL_INIT_VIDEO) < 0);
	ASSERT(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"));
	//Create window
	m_SDLWindow = SDL_CreateWindow("Candy Crush", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	ASSERT(m_SDLWindow == nullptr);
	//Create renderer for window
	m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
	ASSERT(m_SDLRenderer == nullptr);
	//Initialize renderer color
	SDL_SetRenderDrawColor(m_SDLRenderer, 255, 255, 255, 255);
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	ASSERT(!(IMG_Init(imgFlags) & imgFlags));
}

Renderer::~Renderer() {
	for (auto &t : m_textureData) SDL_DestroyTexture(t.second), t.second = nullptr; 
	//Destroy window	
	SDL_DestroyRenderer(m_SDLRenderer), m_SDLWindow = nullptr;
	SDL_DestroyWindow(m_SDLWindow), m_SDLRenderer = nullptr;	
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Renderer::LoadTexture(OBJECT_ID id, const std::string &path) {
	SDL_Texture* newTexture = nullptr; //The final texture
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	ASSERT_MSG(loadedSurface == nullptr, "Unable to load image " + path);
	//Create texture from surface pixels
	m_textureData[id] = SDL_CreateTextureFromSurface(m_SDLRenderer, loadedSurface);
	ASSERT_MSG(m_textureData[id] == nullptr, "Unable to create texture from " + path);
	SDL_FreeSurface(loadedSurface); //Get rid of old loaded surface
}

void Renderer::PushSprite(const Sprite &sprite) {
	SDL_RenderCopy(m_SDLRenderer, m_textureData[sprite.id], nullptr, &sprite.transform.rect);
}