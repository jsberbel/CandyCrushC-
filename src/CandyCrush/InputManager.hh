/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include <SDL/SDL.h>
#include <queue>
#include <unordered_map>
#include <iostream>

#define IM InputManager::Instance()

// Struct that encapsulates the mouse coordinates
struct MouseCoords { 
	Sint32 x{ 0 }, y{ 0 };
	MouseCoords() = default;
	MouseCoords(Sint32 x_, Sint32 y_) : x(x_), y(y_) {};
	friend MouseCoords &&operator-(const MouseCoords &lhs, const MouseCoords &rhs) { return std::move(MouseCoords(lhs.x - rhs.x, lhs.y - rhs.y)); };
	friend std::ostream &operator<<(std::ostream &os, const MouseCoords &rhs) { return os << '(' << rhs.x << ", " << rhs.y << ')'; };
};

// Enum button that identifies each button pressed with the mouse
enum MouseButton { MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT, MOUSE_BUTTON_MIDDLE, MOUSE_BUTTON_RIGHT };

// InputManager class that controls all input info and stores it to be used in other pats of the code
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
	void Update(void) {
		EmptyInput();
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
				case SDL_QUIT:				m_exit = true; break;
				case SDL_MOUSEMOTION:		m_mouseCoords.x = evnt.motion.x; m_mouseCoords.y = evnt.motion.y; break;
				case SDL_MOUSEWHEEL:		m_mouseWheel = evnt.wheel.y;
				case SDL_MOUSEBUTTONDOWN:	m_inputValues.push(&(m_inputMap[evnt.button.button] = InputValue::DOWN)); break;
				case SDL_MOUSEBUTTONUP:		m_inputValues.push(&(m_inputMap[evnt.button.button] = InputValue::UP)); break;
				case SDL_KEYDOWN:			m_inputValues.push(&(m_inputMap[evnt.key.keysym.sym] = (evnt.key.repeat) ? InputValue::HOLD : InputValue::DOWN)); break;
				case SDL_KEYUP:				m_inputValues.push(&(m_inputMap[evnt.key.keysym.sym] = InputValue::UP)); break;
			}
		}
	}
	inline bool HasQuit(void) const { return m_exit; };
	inline const MouseCoords &GetMouseCoords(void) const { return m_mouseCoords; };
	inline Sint32 GetMouseWheel(void) const { return m_mouseWheel; };
	template<MouseButton btn> bool IsMouseDown(void) { return CheckInput<btn, InputValue::DOWN>(); }
	template<MouseButton btn> bool IsMouseHold(void) { return CheckInput<btn, InputValue::HOLD>(); }
	template<MouseButton btn> bool IsMouseUp(void) { return CheckInput<btn, InputValue::UP>(); }
	template<char keyID> bool IsKeyDown(void) { return CheckInput<keyID, InputValue::DOWN>(); }
	template<char keyID> bool IsKeyHold(void) { return CheckInput<keyID, InputValue::HOLD>(); }
	template<char keyID> bool IsKeyUp(void) { return CheckInput<keyID, InputValue::UP>(); }
private:
	std::unordered_map<char, InputValue> m_inputMap;
	std::queue<InputValue*> m_inputValues;
	MouseCoords m_mouseCoords;
	Sint32 m_mouseWheel{ 0 };
	bool m_exit = false;
	void EmptyInput(void) {
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
	bool CheckInput(void) {
		auto it = m_inputMap.find(keyID);
		if (it != m_inputMap.end()) return it->second == value;
		return false;
	}
};