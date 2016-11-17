/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "ID.hh"
#include "Renderer.hh"
#include "Transform.hh"

// Sprite class that serves as part of the Model
struct Sprite {
	Transform transform;
	ObjectID objectID;
	void Draw() { R.Push(objectID, transform); };
};