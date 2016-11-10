#pragma once
#include <SDL.h>
#include <string>
#include <iostream>

#ifdef NDEBUG
#define ASSERT(cnd) cnd
#else
#define ASSERT(cnd) \
if (cnd) { \
	([](const char* const f) { \
		char buffer[200]; \
		sprintf(buffer, "File: %s\nLine: %-5d\nFunction: %-30.30s", __FILE__, __LINE__, f); \
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ("ERROR: " + std::string(#cnd)).c_str(), buffer, nullptr); \
		exit(EXIT_FAILURE); \
	})(__FUNCTION__); \
}
#define ASSERT_MSG(cnd, ...) \
if (cnd) { \
	([&](const char* const f) { \
		char buffer[500]; \
		sprintf(buffer, "Description: %s\n\nFile: %s\nLine: %-5d\nFunction: %-30.30s", &(__VA_ARGS__)[0], __FILE__, __LINE__, f); \
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ("ERROR: " + std::string(#cnd)).c_str(), buffer, nullptr); \
		exit(EXIT_FAILURE); \
	})(__FUNCTION__); \
}
#endif