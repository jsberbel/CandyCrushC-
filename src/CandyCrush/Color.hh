#pragma once
#include <SDL/SDL.h>

//#define Color SDL_Color

struct Color {
	union {
		SDL_Color col;
		struct { Uint8 r, g, b, a; };
	};
	Color(int r_, int g_, int b_, int a_) : r(r_), g(g_), b(b_), a(a_) {};
	Color() { r = g = b = a = 0; };
	inline const SDL_Color &operator()() const { return col; };
	friend bool operator==(Color &lhs, Color &rhs) {
		return (lhs.r == rhs.r &&
				lhs.g == rhs.g &&
				lhs.b == rhs.b &&
				lhs.a == rhs.a);
	};
};
