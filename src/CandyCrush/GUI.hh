/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "ID.hh"
#include "Color.hh"
#include "Renderer.hh"
#include "Transform.hh"

// Static GUI class for rendering UI Objects purposes
class GUI {
public:
	template<FontID fontID>
	static void DrawTextBlended(std::string &&msg, Transform &&transform, Color &&color) {
		R.Push(TTF_RenderText_Blended(R.GetFont<fontID>(), msg.c_str(), color()), transform);
	}
	template<FontID fontID>
	static void DrawTextSolid(std::string &&msg, Transform &&transform, Color &&color) {
		R.Push(TTF_RenderText_Solid(R.GetFont<fontID>(), msg.c_str(), color()), transform);
	}
	template<FontID fontID>
	static void DrawTextShaded(std::string &&msg, Transform &&transform, Color &&foregroundColor, Color &&backgroundColor) {
		R.Push(TTF_RenderText_Shaded(R.GetFont<fontID>(), msg.c_str(), foregroundColor(), backgroundColor()), transform);
	}
};