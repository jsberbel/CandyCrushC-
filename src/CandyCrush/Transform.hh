#pragma once
#include <SDL/SDL.h>

//#define Transform SDL_Rect

struct Transform {
	union {
		SDL_Rect rect;
		struct { Sint32 x, y, w, h; };
	};
	Transform(int x_, int y_) : x(x_), y(y_), w(0), h(0) {};
	Transform(int x_, int y_, int w_, int h_) : x(x_), y(y_), w(w_), h(h_) {};
	Transform() { x = y = w = h = 0; };
	inline const SDL_Rect &operator()() const { return rect; };
	friend bool operator==(Transform &lhs, Transform &rhs) {
		return (lhs.x == rhs.x &&
				lhs.y == rhs.y &&
				lhs.w == rhs.w &&
				lhs.h == rhs.h);
	};
};