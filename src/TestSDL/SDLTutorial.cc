#include <string>			// Needed to process error messages
#include <SDL\SDL.h>		// Always needs to be included for an SDL app
#include <SDL\SDL_ttf.h>	// Used for loading fonts and rendering text
#include <SDL\SDL_image.h>	// Used for loading and drawing sprites
#include <SDL\SDL_mixer.h>	// Used for loading and playing audio files

int main(int, char*[]) {
	using namespace std::string_literals; // Enables to use an array of characters with an "s" at the end as an string literal
	try {
		// --- INIT ---
		SDL_Log("Executable built in %s", SDL_GetBasePath());
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "Unable to initialize SDL subsystems"s;
		const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
		if (!(IMG_Init(imgFlags) & imgFlags)) throw "Unable to initialize SDL_image"s;
		const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
		if (!(Mix_Init(mixFlags) & mixFlags)) throw "Unable to initialize SDL_mixer"s;
		if (TTF_Init() != 0) throw "Unable to initialize SDL_ttf"s;

		// --- WINDOW ---
		const int WIDTH{ 640 }, HEIGHT{ 480 };
		SDL_Window *window{ SDL_CreateWindow("Hello SDL",
											  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
											  WIDTH, HEIGHT,
											  SDL_WINDOW_SHOWN) };
		if (window == nullptr) throw "Unable to initialize the SDL_Window"s;

		// --- RENDERER ---
		SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
		if (renderer == nullptr) throw "Unable to initialize the SDL_Renderer"s;

		// --- SPRITES ---
		SDL_Texture *bgTexture		{ IMG_LoadTexture(renderer, "../../res/gfx/bg.jpg") };
		SDL_Texture *playerTexture	{ IMG_LoadTexture(renderer, "../../res/gfx/blue.png") };
		if (bgTexture == nullptr || playerTexture == nullptr) throw "Unable to create the SDL textures for the sprites"s;
		SDL_Rect bgRect			{ 0, 0, WIDTH, HEIGHT };
		SDL_Rect playerRect		{ (WIDTH >> 1) - 50, (HEIGHT >> 1) - 50, 100, 100 };
		SDL_Rect playerTarget	{ 0, 0, 100, 100 };
		//SDL_Rect rectSrc{ 0, 0, 0, 0 };
		//SDL_QueryTexture(texturePlayer, nullptr, nullptr, &rectSrc.w, &rectSrc.h);
		//rectSrc.w >>= 1, rectSrc.h >>= 1;

		// --- TEXT ---
		TTF_Font *font{ TTF_OpenFont("../../res/fnt/candy.ttf", 50) };
		if (font == nullptr) throw "Unable to initialize the TTF_Font"s;
		SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "DELICIOUS", SDL_Color{255, 0, 255, 255}) };
		if (tmpSurf == nullptr) TTF_CloseFont(font), throw "Unable to create the SDL text surface"s;
		SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(renderer, tmpSurf) };
		SDL_Rect textRect{ (WIDTH >> 1) - (tmpSurf->w >> 1), 
							static_cast<int>(HEIGHT*.8f) - (tmpSurf->h >> 1), 
							tmpSurf->w, tmpSurf->h };
		SDL_FreeSurface(tmpSurf);
		TTF_CloseFont(font);

		// --- AUDIO ---
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) throw "Unable to initialize SDL_mixer audio systems"s;
		Mix_Music *soundtrack{ Mix_LoadMUS("../../res/sfx/theme.wav") };
		if (!soundtrack) throw "Unable to load the Mix_Music soundtrack"s;
		Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
		Mix_PlayMusic(soundtrack, -1);
		
		// --- GAME LOOP ---
		SDL_Event evnt;
		for (bool isRunning{ true }; isRunning;) {
			// HANDLE EVENTS
			while (SDL_PollEvent(&evnt)) {
				switch (evnt.type) {
					case SDL_QUIT:			isRunning = false; break;
					case SDL_MOUSEMOTION:	playerTarget.x = evnt.motion.x - 50; playerTarget.y = evnt.motion.y - 50; break;
					default:;
				}
			}
			// UPDATE
			playerRect.x += (playerTarget.x - playerRect.x) / 10;
			playerRect.y += (playerTarget.y - playerRect.y) / 10;
			// DRAW
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
			SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
			SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
			SDL_RenderPresent(renderer);
		}

		// --- DESTROY ---
		Mix_CloseAudio();
		SDL_DestroyTexture(bgTexture);
		SDL_DestroyTexture(textTexture);
		SDL_DestroyTexture(playerTexture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	} catch (const std::string &msg) { // Catch exceptions and print them as an error msg box
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", ("Description: " + msg + "\nSDL Error:" + SDL_GetError()).c_str(), nullptr);
	}
	// --- EXIT ---
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}