#pragma once
#include <SDL/SDL.h>
#include <queue>
#include <unordered_map>

#define IM InputManager::Instance()

enum MouseButton { MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT, MOUSE_BUTTON_MIDDLE, MOUSE_BUTTON_RIGHT };

class InputManager {
	InputManager() = default;
	InputManager(const InputManager &rhs) = delete;
	InputManager &operator=(const InputManager &rhs) = delete;
	enum class InputValue { UP = SDL_RELEASED, DOWN, HOLD, EMPTY };
public:
	~InputManager() = default;
	inline static InputManager &Instance() {
		static InputManager inputManager;
		return inputManager;
	}
	void Update() {
		EmptyInput();
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
				case SDL_QUIT:				m_exit = true; break;
				case SDL_MOUSEMOTION:		m_mouseX = evnt.motion.x, m_mouseY = evnt.motion.y; break;
				case SDL_MOUSEWHEEL:		m_mouseWheel = evnt.wheel.y;
				case SDL_MOUSEBUTTONDOWN:	m_inputValues.push(&(m_inputMap[evnt.button.button] = InputValue::DOWN)); break;
				case SDL_MOUSEBUTTONUP:		m_inputValues.push(&(m_inputMap[evnt.button.button] = InputValue::UP)); break;
				case SDL_KEYDOWN:			m_inputValues.push(&(m_inputMap[evnt.key.keysym.sym] = (evnt.key.repeat) ? InputValue::HOLD : InputValue::DOWN)); break;
				case SDL_KEYUP:				m_inputValues.push(&(m_inputMap[evnt.key.keysym.sym] = InputValue::UP)); break;
			}
		}
	}
	inline bool HasQuit() const { return m_exit; };
	inline Sint32 GetMouseX() const { return m_mouseX; };
	inline Sint32 GetMouseY() const { return m_mouseY; };
	inline Sint32 GetMouseWheel() const { return m_mouseWheel; };
	template<MouseButton btn> bool IsMouseDown() { return CheckInput<btn, InputValue::DOWN>(); }
	template<MouseButton btn> bool IsMouseHold() { return CheckInput<btn, InputValue::HOLD>(); }
	template<MouseButton btn> bool IsMouseUp() { return CheckInput<btn, InputValue::UP>(); }
	template<char keyID> bool IsKeyDown() { return CheckInput<keyID, InputValue::DOWN>(); }
	template<char keyID> bool IsKeyHold() { return CheckInput<keyID, InputValue::HOLD>(); }
	template<char keyID> bool IsKeyUp() { return CheckInput<keyID, InputValue::UP>(); }
private:
	std::unordered_map<char, InputValue> m_inputMap;
	std::queue<InputValue*> m_inputValues;
	Sint32 m_mouseX = 0, m_mouseY = 0;
	Sint32 m_mouseWheel = 0;
	bool m_exit = false;
	void EmptyInput() {
		while (!m_inputValues.empty()) {
			auto value = m_inputValues.front();
			if (value != nullptr)
				switch (*value) {
					case InputValue::DOWN: *value = InputValue::HOLD; value = nullptr; break;
					case InputValue::UP:	*value = InputValue::EMPTY; value = nullptr;
				}
			m_inputValues.pop();
		}
	}
	template<char keyID, InputValue value>
	bool CheckInput() {
		auto it = m_inputMap.find(keyID);
		if (it != m_inputMap.end()) return it->second == value;
		return false;
	}
};