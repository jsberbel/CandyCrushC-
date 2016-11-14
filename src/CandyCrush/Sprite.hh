#pragma once
#include <SDL/SDL.h>
#include "ObjectID.hh"

struct Transform {
	union {
		SDL_Rect rect;
		struct { int x, y, h, w; };
	};
};

struct Sprite {
	Transform transform;
	ObjectID id;
};