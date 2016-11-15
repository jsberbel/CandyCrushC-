/******************************************************************
 * Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
 * This can not be copied, modified and/or distributed without    *
 * express permission of the copyright owner.                     *
 ******************************************************************/

#pragma once
#include <SDL/SDL.h>
#include <string>
#include <cstdio>

#ifdef NDEBUG
	#define ASSERT(cnd) (cnd)
	#define ASSERT_MSG(cnd, ...) (cnd)
#else
	// Determine if a condition is not fulfilled, then show a message box with the error
	// @cnd the condition to check
	#define ASSERT(cnd) \
		([](decltype(cnd) &&c, const char* const f) { \
			if (!!c) return c; \
			char buffer[200]; \
			sprintf(buffer, "File: %s\nLine: %-5d\nFunction: %-30.30s", __FILE__, __LINE__, f); \
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ("ERROR: " + std::string(#cnd)).c_str(), buffer, nullptr); \
			exit(-1); \
		})(cnd, __FUNCTION__);
	// Determine if a condition is not fulfilled, then show a message box with the error and some programmer's additional text
	// @cnd the condition to check
	// @... additional description text to show on the message box
	#define ASSERT_MSG(cnd, ...) \
		([&](decltype(cnd) &&c, const char* const f) { \
			if (!!c) return c; \
			char buffer[500]; \
			sprintf(buffer, "Description: %s\n\nFile: %s\nLine: %-5d\nFunction: %-30.30s", &(__VA_ARGS__)[0], __FILE__, __LINE__, f); \
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ("ERROR: " + std::string(#cnd)).c_str(), buffer, nullptr); \
			exit(-1); \
		})(cnd, __FUNCTION__);
#endif