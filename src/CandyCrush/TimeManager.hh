#pragma once
#include <SDL/SDL.h>
#include <functional>

#define SCREEN_FPS 120.0f
#define SCREEN_TICKS_PER_FRAME (1000.0f / SCREEN_FPS)

#define TM TimeManager::Instance()

class TimeManager {
public:
	inline static TimeManager &Instance(void) {
		static TimeManager timeManager;
		return timeManager;
	}
	void FPSBegin() {
		m_deltatime = float(SDL_GetTicks() - lastTime);
		lastTime = SDL_GetTicks();
	}
	void FPSEnd(std::function<void()> drawFunction) {
		if (renderTime >= SCREEN_TICKS_PER_FRAME) drawFunction(), renderTime -= SCREEN_TICKS_PER_FRAME;
		renderTime += m_deltatime; // Updates the render timer
	}
	Uint32 GetCurTime() { return SDL_GetTicks(); };
	inline float GetDeltaTime() { return m_deltatime; };
private:
	TimeManager() = default;
	TimeManager(const TimeManager &rhs) = delete;
	TimeManager &operator=(const TimeManager &rhs) = delete;
private:
	float m_deltatime{ .0f }; // Delta time in seconds
	Uint32 lastTime{ SDL_GetTicks() }; // Last time sample in seconds
	float renderTime{ 0.0f }; // Time control for rendering
};