#pragma once
#include <typeindex>
#include <unordered_map>
#include "Scene.h"
#include "Assert.h"

class GameEngine;		// Forward declaration of the main game app interface

// Screen list class to store the whole game screens
class SceneManager {
public:
	explicit SceneManager(GameEngine *gameEngine) : m_gameEngine(gameEngine), m_curSceneIndex(typeid(nullptr)) {}
	~SceneManager() {
		for (auto &scene : m_scenes) scene.second->Destroy();	// Remove elements of the screen and destroy the screen
		m_curSceneIndex = typeid(nullptr);						// Set current screen index to -1
	}
	template<class S> void AddScene() {
		if (m_curSceneIndex == typeid(nullptr)) m_curSceneIndex = typeid(S);
		static_assert(std::is_base_of<Scene, S>::value, "Can't add scene that doesn't inherit from IScene");
		ASSERT(GetScene<S>() == nullptr);
		S *newScene = new S;
		newScene->gameEngine = m_gameEngine;		// Set the game app reference parent of the new screen
		newScene->Build();						// Initialize the new screen
		m_scenes[typeid(S)] = newScene;
	}
	template<class S> S *SetScene() {
		static_assert(std::is_base_of<Scene, S>::value, "Can't add scene that doesn't inherit from IScene");
		Scene *newScene = GetScene<S>();
		ASSERT(newScene != nullptr);
		auto curScene = GetCurScene();
		curScene->OnExit();
		curScene->SetState(SceneState::SLEEP);
		m_curSceneIndex = typeid(S);
		curScene = newScene;
		curScene->SetState(SceneState::RUNNING);
		curScene->OnEntry();
		return dynamic_cast<S*>(curScene);
	}
	template<class S> S *GetScene() {
		auto scene = m_scenes.find(typeid(S));
		return (scene != m_scenes.end()) ? dynamic_cast<S*>(scene->second) : nullptr;
	}
protected:
	std::unordered_map<std::type_index, Scene*> m_scenes;	// Array of screens
	std::type_index m_curSceneIndex;
	GameEngine *m_gameEngine;								// Reference pointer to current game app
	Scene *GetCurScene() {
		ASSERT(m_curSceneIndex != typeid(nullptr));		// Check if current screen exists
		auto scene = m_scenes.find(m_curSceneIndex);
		return (scene != m_scenes.end()) ? static_cast<Scene*>(scene->second) : nullptr;
	}
};