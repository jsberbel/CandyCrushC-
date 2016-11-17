/******************************************************************
 * Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
 * This can not be copied, modified and/or distributed without    *
 * express permission of the copyright owner.                     *
 ******************************************************************/

#pragma once
#include <string>
#include <cstdio>
#include <SDL/SDL.h>

 /**
  * @def ASSERT(cnd)
  * @brief Determine if @cnd is not fulfilled, then show a message box with the error.
  */
  /**
  * @def ASSERT_MSG(cnd)
  * @brief Determine if @cnd is not fulfilled, then show a message box with the error and @... as additional text.
  */
#ifdef NDEBUG
	#define ASSERT(cnd) (cnd)
	#define ASSERT_MSG(cnd, ...) (cnd)
#else
	#define ASSERT(cnd) \
		([](decltype(cnd) &&c, const char* const f) { \
			if (!!c) return c; \
			char buffer[200]; \
			sprintf(buffer, "File: %s\nLine: %-5d\nFunction: %-30.30s", __FILE__, __LINE__, f); \
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ("ERROR: " + std::string(#cnd)).c_str(), buffer, nullptr); \
			exit(-1); \
		})(cnd, __FUNCTION__);
	#define ASSERT_MSG(cnd, ...) \
		([&](decltype(cnd) &&c, const char* const f) { \
			if (!!c) return c; \
			char buffer[500]; \
			sprintf(buffer, "Description: %s\n\nFile: %s\nLine: %-5d\nFunction: %-30.30s", &(__VA_ARGS__)[0], __FILE__, __LINE__, f); \
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ("ERROR: " + std::string(#cnd)).c_str(), buffer, nullptr); \
			exit(-1); \
		})(cnd, __FUNCTION__);
#endif