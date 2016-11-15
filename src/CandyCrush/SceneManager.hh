/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include <typeindex>
#include <unordered_map>
#include "Scene.hh"
#include "Assert.hh"

#define SM SceneManager::Instance()

// SceneManager class to store and control the whole game scenes
class SceneManager {
	SceneManager() = default;
	SceneManager(const SceneManager &rhs) = delete;
	SceneManager &operator=(const SceneManager &rhs) = delete;
public:
	inline static SceneManager &Instance() {
		static SceneManager sceneManager;
		return sceneManager;
	}
	~SceneManager() {
		for (auto &scene : m_scenes) delete scene.second;	// Remove elements of the screen and destroy the screen
		m_curSceneIndex = typeid(nullptr);					// Set current screen index to nullptr
	}
	template<class S> void AddScene(void) {
		if (m_curSceneIndex == typeid(nullptr)) m_curSceneIndex = typeid(S);
		static_assert(std::is_base_of<Scene, S>::value, "Can't add scene that doesn't inherit from IScene");
		ASSERT(GetScene<S>() == nullptr);
		S *newScene = new S;
		m_scenes[typeid(S)] = newScene;
	}
	template<class S> S *SetScene(void) {
		static_assert(std::is_base_of<Scene, S>::value, "Can't add scene that doesn't inherit from IScene");
		Scene *newScene = GetScene<S>();
		ASSERT(newScene != nullptr);
		auto curScene = GetCurScene();
		curScene->OnExit();
		curScene->SetState<SceneState::SLEEP>();
		m_curSceneIndex = typeid(S);
		curScene = newScene;
		curScene->SetState<SceneState::RUNNING>();
		curScene->OnEntry();
		return dynamic_cast<S*>(curScene);
	}
	template<class S> S *GetScene(void) {
		auto scene = m_scenes.find(typeid(S));
		return (scene != m_scenes.end()) ? dynamic_cast<S*>(scene->second) : nullptr;
	}
protected:
	std::unordered_map<std::type_index, Scene*> m_scenes;	// Array of screens
	std::type_index m_curSceneIndex{ typeid(nullptr) };
	Scene *GetCurScene(void) {
		ASSERT(m_curSceneIndex != typeid(nullptr));		// Check if current screen exists
		auto scene = m_scenes.find(m_curSceneIndex);
		return (scene != m_scenes.end()) ? static_cast<Scene*>(scene->second) : nullptr;
	}
};