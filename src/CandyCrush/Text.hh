/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "Transform.hh"
#include "Renderer.hh"
#include "Color.hh"
#include "GUID.hh"
#include <utility>

// Text class to draw UI text into the screen with an specified font
class Text {
	friend class Renderer;
public:
	Transform transform;
	FontID fontID;
	Text() = default;
	virtual ~Text() { if (texture) SDL_DestroyTexture(texture), texture = nullptr; };
	Text(Transform &&t, FontID &&i, Color &&c) : transform(std::move(t)), fontID(std::move(i)), color(std::move(c)) {};
	Text(Text &&other) : transform(std::move(other.transform)), fontID(std::move(other.fontID)), color(std::move(other.color)), texture(std::move(other.texture)) {};
	Text& Text::operator=(Text&& other) {
		if (this != &other)
			SDL_DestroyTexture(texture),
			transform = other.transform, fontID = other.fontID, color = other.color, texture = other.texture,
			other.transform = { 0,0,0,0 }, other.color = { 0,0,0,0 }, other.texture = nullptr;
		return *this;
	}
#define LOAD_TEXT_TEXTURE(surface, msg) \
		ASSERT_MSG(surface != nullptr, "Unable to load image " + msg); \
		texture = R.CreateTexture(surface); \
		ASSERT_MSG(texture != nullptr, "Unable to create texture from text: " + msg); \
		transform.w = surface->w; \
		transform.h = surface->h; \
		SDL_FreeSurface(surface);
	template<FontID id>
	static Text CreateTextBlended(Transform &&transform, Color &&color, const std::string &&msg = " ") {
		Text newText(std::move(transform), std::move(id), std::move(color));
		auto surface = TTF_RenderText_Blended(R.m_fontData[id], msg.c_str(), color());
		LOAD_TEXT_TEXTURE(surface, msg);
		return std::move(newText);
	}
	template<FontID id>
	static Text CreateTextSolid(Transform &&transform, Color &&color, const std::string &&msg = " ") {
		Text newText(std::move(transform), std::move(id), std::move(color));
		auto surface = TTF_RenderText_Solid(R.m_fontData[id], msg.c_str(), color());
		LOAD_TEXT_TEXTURE(surface, msg);
		return std::move(newText);
	}
	void ChangeTextBlended(const std::string &msg, Color &&newColor = {}) {
		auto surface = TTF_RenderText_Blended(R.GetFont(fontID), msg.c_str(), (newColor) ? newColor() : color());
		SDL_DestroyTexture(texture);
		LOAD_TEXT_TEXTURE(surface, msg);
	}
	void ChangeTextSolid(const std::string &msg, Color &&newColor = {}) {
		auto surface = TTF_RenderText_Solid(R.GetFont(fontID), msg.c_str(), (newColor) ? newColor() : color());
		SDL_DestroyTexture(texture);
		LOAD_TEXT_TEXTURE(surface, msg);
	}
	void Draw() { R.Push(texture, transform); }
	virtual void DrawUpdate(const std::string &msg) {
		ChangeTextBlended(msg, std::move(color));
		Draw();
	}
protected:
	Color color;
	SDL_Texture *texture{ nullptr };
	inline SDL_Texture *operator()(void) const { return texture; };
};

// Text derived class that adds the peculiarity to have a background
class TextBG : public Text {
	friend class Renderer;
public:
	TextBG() = default;
	TextBG(Transform &&t, FontID &&i, Color &&fg, Color &&bg) : Text(std::move(t), std::move(i), std::move(fg)), backgroundColor(bg) {};
	TextBG(TextBG &&other) : Text(std::move(other)), backgroundColor(std::move(other.backgroundColor)) {};
	TextBG& TextBG::operator=(TextBG&& other) {
		if (this != &other)
			SDL_DestroyTexture(texture),
			transform = other.transform, fontID = other.fontID, color = other.color, texture = other.texture, backgroundColor = other.backgroundColor,
			other.transform = { 0,0,0,0 }, other.color = { 0,0,0,0 }, other.texture = nullptr;
		return *this;
	}
	template<FontID id>
	static TextBG CreateTextShaded(Transform &&transform, Color &&foregroundColor, Color &&backgroundColor, const std::string &&msg = " ") {
		TextBG newText(std::move(transform), std::move(id), std::move(foregroundColor), std::move(backgroundColor));
		auto surface = TTF_RenderText_Shaded(R.m_fontData[id], msg.c_str(), foregroundColor(), backgroundColor());
		LOAD_TEXT_TEXTURE(surface, msg);
		return std::move(newText);
	}
	void ChangeTextShaded(const std::string &msg, Color &&foregroundColor = {}, Color &&backgroundColor = {}) {
		auto surface = TTF_RenderText_Shaded(R.GetFont(fontID), msg.c_str(),
			(foregroundColor) ? foregroundColor() : color(),
											 (backgroundColor) ? backgroundColor() : backgroundColor());
		SDL_DestroyTexture(texture);
		LOAD_TEXT_TEXTURE(surface, msg);
	}
	void DrawUpdate(const std::string &msg) override {
		ChangeTextShaded(msg, std::move(color), std::move(backgroundColor));
		Draw();
	}
private:
	Color backgroundColor;
};

#undef LOAD_TEXT_TEXTURE