#pragma once

enum class SceneState {
	NONE,		// Default empty state
	RUNNING,	// State of updating the current screen
	EXIT,		// State of leaving the current game
	SLEEP		// State of leaving scene stand by
};

class Scene {
	friend class SceneManager;
public:
	explicit Scene() = default;
	virtual ~Scene() = default;
	// Called when entering into a new screen
	virtual void OnEntry() = 0;
	// Called when leaving from a screen
	virtual void OnExit() = 0;
	// Main update screen function called in game loop
	virtual void Update(float deltatime) = 0;
	// Main draw screen function called in game loop
	virtual void Draw() = 0;
	// State methods
	template <SceneState state> inline void SetState() { currentState = state; };
	inline SceneState GetState() const { return currentState; };
	template <SceneState state> inline bool CheckState() const { return currentState == state; };
protected:
	SceneState currentState{ SceneState::NONE };	// Current state of the screen, NONE by default
};