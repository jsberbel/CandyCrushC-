#pragma once
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <unordered_map>
#include "Resource.hh"
#include "Window.hh"
#include "Assert.hh"
#include "Logger.hh"
#include "Sprite.hh"
#include "Text.hh"

#define R Renderer::Instance()

enum FontStyle { 
	FONT_STYLE_BOLD = TTF_STYLE_BOLD, 
	FONT_STYLE_ITALIC = TTF_STYLE_ITALIC,
	FONT_STYLE_UNDERLINE = TTF_STYLE_UNDERLINE,
	FONT_STYLE_NORMAL = TTF_STYLE_NORMAL
};

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
	template<FontID id>
	void LoadFont(std::string &&filename, int size) {
		//Check if font ID already has a font associated
		auto f = m_fontData.find(id);
		if (f != m_fontData.end()) Print("FontID ", int(id), " already used with another true type font, overriding it with ", filename);
		// Load font from specified path
		ASSERT((m_fontData[id] = TTF_OpenFont(RESOURCE_FILE(filename), size)) != nullptr);
	}
	template<FontID id, FontStyle style>
	inline void SetFontStyle() {
		TTF_SetFontStyle(m_fontData[id], style);
	}
#define LOAD_SURFACE(text, surface, msg) \
	text.SetTexture(SDL_CreateTextureFromSurface(R.m_SDLRenderer, surface)); \
	ASSERT_MSG(text() != nullptr, "Unable to create texture from text: " + msg); \
	text.transform.w = surface->w; \
	text.transform.h = surface->h; \
	SDL_FreeSurface(surface);
	template<FontID id>
	TextBlended &&CreateTextBlended(Transform &&transform, std::string &&msg, Color &&color) {
		TextBlended newText(transform, id, color);
		auto surface = TTF_RenderText_Blended(R.m_fontData[id], msg.c_str(), color());
		ASSERT_MSG(surface != nullptr, "Unable to load image " + msg);
		LOAD_SURFACE(newText, surface, msg);
		return std::move(newText);
	}
	template<FontID id>
	TextShaded &&CreateTextShaded(Transform &&transform, const std::string &msg, Color &&foregroundColor, Color &&backgroundColor) {
		TextShaded newText(transform, id, foregroundColor, backgroundColor);
		auto surface = TTF_RenderText_Shaded(R.m_fontData[id], msg.c_str(), foregroundColor(), backgroundColor());
		ASSERT_MSG(surface != nullptr, "Unable to load image " + msg);
		LOAD_SURFACE(newText, surface, msg);
		return std::move(newText);
	}
	void ChangeTextBlended(TextBlended &text, const std::string &msg, Color &&color = Color()) {
		auto surface = TTF_RenderText_Blended(R.m_fontData[text.id], msg.c_str(), (color == Color()) ? text.color() : color());
		ASSERT_MSG(surface != nullptr, "Unable to load image " + msg);
		SDL_DestroyTexture(text());
		LOAD_SURFACE(text, surface, msg);
	}
	void ChangeTextShaded(TextShaded &text, const std::string &msg, Color &&foregroundColor = Color(), Color &&backgroundColor = Color()) {
		auto surface = TTF_RenderText_Shaded(R.m_fontData[text.id], msg.c_str(),
											(foregroundColor == Color()) ? text.foregroundColor() : foregroundColor(),
											(backgroundColor == Color()) ? text.backgroundColor() : backgroundColor());
		ASSERT_MSG(surface != nullptr, "Unable to load image " + msg);
		SDL_DestroyTexture(text());
		LOAD_SURFACE(text, surface, msg);
	}
#undef LOAD_SURFACE
	template<ObjectID id>
	void LoadTexture(std::string &&filename) {
		//Check if object ID already has a texture associated
		auto t = m_textureData.find(id);
		if (t != m_textureData.end()) Print("ObjectID ", int(id), " already used with another texture, overriding it with ", filename);
		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(RESOURCE_FILE(filename));
		ASSERT_MSG(loadedSurface != nullptr, "Unable to load image " + filename);
		//Create texture from surface pixels
		m_textureData[id] = SDL_CreateTextureFromSurface(m_SDLRenderer, loadedSurface);
		ASSERT_MSG(m_textureData[id] != nullptr, "Unable to create texture from " + filename);
		SDL_FreeSurface(loadedSurface); //Get rid of loaded surface
	}
	inline void Push(const Sprite &sprite) {
		SDL_RenderCopy(m_SDLRenderer, m_textureData[sprite.id], nullptr, &sprite.transform());
	}
	inline void Push(Text &text) {
		SDL_RenderCopy(m_SDLRenderer, text(), nullptr, &text.transform());
	}
	inline void Push(TextBlended &text, const std::string &msg) {
		//(dynamic_cast<TextBlended*>(text) != nullptr) ? ChangeTextBlended(dynamic_cast<TextBlended*>(text), msg) : ChangeTextShaded(dynamic_cast<TextShaded*>(text), msg);
		ChangeTextBlended(text, msg);
		Push(text);
	}
	inline void Push(TextShaded &text, const std::string &msg) {
		ChangeTextShaded(text, msg);
		Push(text);
	}
	inline void Clear(void) const { SDL_RenderClear(m_SDLRenderer); };
	inline void Render(void) const { SDL_RenderPresent(m_SDLRenderer); };
private:
	std::unordered_map<ObjectID, SDL_Texture*> m_textureData;
	std::unordered_map<FontID, TTF_Font*> m_fontData;
	SDL_Renderer* m_SDLRenderer{ nullptr };
};