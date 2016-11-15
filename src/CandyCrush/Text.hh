#pragma once
#include "Transform.hh"
#include "Color.hh"
#include "GUID.hh"

class Text {
	friend class Renderer;
public:
	Transform transform;
	FontID id;
	Text() = default;
	virtual ~Text() { SDL_DestroyTexture(texture), texture = nullptr; };
	Text(Transform t, FontID i) : transform(t), id(i) {};
	inline SDL_Texture *operator()() const { return texture; };
	inline void SetTexture(SDL_Texture *t) { texture = t; };
private:
	SDL_Texture *texture;
};

class TextBlended : public Text {
	friend class Renderer;
public:
	TextBlended() = default;
	TextBlended(Transform t, FontID i, Color c) : Text(t, i), color (c) {};
private:
	Color color;
};

class TextShaded : public Text {
	friend class Renderer;
public:
	TextShaded() = default;
	TextShaded(Transform t, FontID i, Color fg, Color bg) : Text(t, i), foregroundColor(fg), backgroundColor(bg) {};
private:
	Color foregroundColor, backgroundColor;
};