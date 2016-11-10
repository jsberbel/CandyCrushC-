#pragma once
#include <SDL.h>
#include "ObjectID.h"

struct Transform {
	union {
		SDL_Rect rect;
		struct { int x, y, h, w; };
	};
};

struct Sprite {
	Transform transform;
	OBJECT_ID id;
};