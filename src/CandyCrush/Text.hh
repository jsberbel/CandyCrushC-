/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "Transform.hh"
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
	virtual ~Text() { SDL_DestroyTexture(texture), texture = nullptr; };
	Text(Transform &&t, FontID &&i, Color &&c) : transform(t), fontID(i), color(c) {};
private:
	Color color;
	SDL_Texture *texture;
	inline SDL_Texture *operator()(void) const { return texture; };
	inline void SetTexture(SDL_Texture *t) { texture = t; };
};

// Text derived class that adds the peculiarity to have a background
class TextBG : public Text {
	friend class Renderer;
public:
	TextBG() = default;
	TextBG(Transform &&t, FontID &&i, Color &&fg, Color &&bg) : Text(std::move(t), std::move(i), std::move(fg)), backgroundColor(bg) {};
private:
	Color backgroundColor;
};