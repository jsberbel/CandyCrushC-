#pragma once

#define SCENE_INDEX_EMPTY 0

enum class SceneState {
	NONE,		// Default empty state
	RUNNING,	// State of updating the current screen
	EXIT,		// State of leaving the current game
	SLEEP
};

class GameEngine;
class Scene {
	friend class SceneManager;
public:
	explicit Scene() = default;
	virtual ~Scene() = default;
	// Called when the screen is added into an screen list
	virtual void Build() = 0;
	// Called at the end of the game app
	virtual void Destroy() = 0;
	// Called when entering into a new screen
	virtual void OnEntry() = 0;
	// Called when leaving from a screen
	virtual void OnExit() = 0;
	// Main update screen function called in game loop
	virtual void Update(float deltatime) = 0;
	// Main draw screen function called in game loop
	virtual void Draw() = 0;
	void SetState(SceneState state) { currentState = state; };
	SceneState GetState() const { return currentState; };
	bool CheckState(SceneState state) const { return currentState == state; };
protected:
	SceneState currentState{ SceneState::NONE };	// Current state of the screen, NONE by default
	GameEngine* gameEngine{ nullptr };				// Reference pointer to current game app
};