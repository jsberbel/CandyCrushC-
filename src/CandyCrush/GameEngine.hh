#pragma once
#include <string>

#define SCREEN_FPS 60.0f
#define SCREEN_TICKS_PER_FRAME (1.0f / SCREEN_FPS)

class Scene;
class GameEngine {
public:
	explicit GameEngine(std::string &&name, int screenWidth, int screenHeight);
	~GameEngine() = default;
	void Run(void);
private:
	bool m_isRunning{ true };		// Whether game is running or not
	Scene *m_curScene{ nullptr };	// Pointer to the current scene
	void LoadTextures(void);
	void Update(float deltaTime);
	void Draw(void);
};