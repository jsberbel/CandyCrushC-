/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once

// Enum state fot each Scene in the Game
enum class SceneState {
	NONE,		// Default empty state
	RUNNING,	// State of updating the current screen
	EXIT,		// State of leaving the current game
	SLEEP		// State of leaving scene stand by
};

// Scene class that serves as part of the Model
class Scene {
	friend class SceneManager;
public:
	explicit Scene() = default;
	virtual ~Scene() = default;
	// Called when entering into a new scene
	virtual void OnEntry(void) = 0;
	// Called when leaving from a scene
	virtual void OnExit(void) = 0;
	// Main update scene function called in game loop
	virtual void Update(void) = 0;
	// Main draw scene function called in game loop
	virtual void Draw(void) = 0;
	// State methods
	template <SceneState state> inline void SetState(void) { currentState = state; };
	inline SceneState GetState(void) const { return currentState; };
	template <SceneState state> inline bool CheckState(void) const { return currentState == state; };
protected:
	SceneState currentState{ SceneState::NONE }; // Current state of the scene, NONE by default
};