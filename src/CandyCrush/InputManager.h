//#pragma once
//#include <SDL.h>
//#include <queue>
//#include <unordered_map>
//
//enum class INPUT_VALUE { EMPTY, DOWN, HOLD, UP };
//enum MOUSE_BUTTON { MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT, MOUSE_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE, MOUSE_BUTTON_RIGHT = SDL_BUTTON_RIGHT };
//
//class InputManager {
//	InputManager() = default;
//	~InputManager() = default;
//public:
//	static InputManager &Instance() {
//		static InputManager inputManager;
//		return inputManager;
//	}
//	void EmptyInput() {
//		while (!m_inputValues.empty()) {
//			auto value = m_inputValues.front();
//			if (value != nullptr)
//				switch (*value) {
//					case INPUT_VALUE::DOWN: *value = INPUT_VALUE::HOLD; value = nullptr; break;
//					case INPUT_VALUE::UP:	*value = INPUT_VALUE::EMPTY; value = nullptr;
//				}
//			m_inputValues.pop();
//		}
//	}
//	void Update() {
//		EmptyInput();
//		SDL_Event evnt;
//		while (SDL_PollEvent(&evnt)) {
//			switch (evnt.type) {
//				case SDL_QUIT:				m_exit = true; break;
//				case SDL_MOUSEMOTION:		m_mouseX = evnt.motion.x, m_mouseY = evnt.motion.y; break;
//				case SDL_MOUSEBUTTONDOWN:	m_inputValues.push(&(m_inputMap[evnt.button.button] = INPUT_VALUE::DOWN)); break;
//				case SDL_MOUSEBUTTONUP:		m_inputValues.push(&(m_inputMap[evnt.button.button] = INPUT_VALUE::UP)); break;
//				case SDL_KEYDOWN:			m_inputValues.push(&(m_inputMap[evnt.key.keysym.sym] = (int(evnt.key.repeat)) ? INPUT_VALUE::HOLD : INPUT_VALUE::DOWN)); break;
//				case SDL_KEYUP:				m_inputValues.push(&(m_inputMap[evnt.key.keysym.sym] = INPUT_VALUE::UP)); break;
//				case SDL_MOUSEWHEEL:		m_mouseWheel = evnt.wheel.y;
//			}
//		}
//	}
//	inline bool HasQuit() const						{ return m_exit; };
//	inline Sint32 GetMouseX() const					{ return m_mouseX; };
//	inline Sint32 GetMouseY() const					{ return m_mouseY; };
//	inline Sint32 GetMouseWheel() const				{ return m_mouseWheel; };
//	template<MOUSE_BUTTON btn> bool IsMouseDown()	{ return CheckInput<btn, INPUT_VALUE::DOWN>(); }
//	template<MOUSE_BUTTON btn> bool IsMouseHold()	{ return CheckInput<btn, INPUT_VALUE::HOLD>(); }
//	template<MOUSE_BUTTON btn> bool IsMouseUp()		{ return CheckInput<btn, INPUT_VALUE::UP>(); }
//	template<char keyID> bool IsKeyDown()			{ return CheckInput<keyID, INPUT_VALUE::DOWN>(); }
//	template<char keyID> bool IsKeyHold()			{ return CheckInput<keyID, INPUT_VALUE::HOLD>(); }
//	template<char keyID> bool IsKeyUp()				{ return CheckInput<keyID, INPUT_VALUE::UP>(); }
//private:
//	template<char keyID, INPUT_VALUE value>
//	bool CheckInput() {
//		auto it = m_inputMap.find(keyID);
//		if (it != m_inputMap.end()) return it->second == value;
//		return false;
//	};
//	std::unordered_map<char, INPUT_VALUE> m_inputMap;
//	std::queue<INPUT_VALUE*> m_inputValues;
//	Sint32 m_mouseX = 0, m_mouseY = 0;
//	Sint32 m_mouseWheel = 0;
//	bool m_exit = false;
//};
//
//#undef EMPTY_INPUT

#pragma once
#include <SDL.h>
#include <queue>
#include <unordered_map>

struct INPUT_VALUE {
	enum State { UP = SDL_RELEASED, DOWN, EMPTY, HOLD, } state = EMPTY;
	State operator()(State newState = EMPTY) {
		switch (newState) {
			case DOWN:
			if (state == EMPTY)
				return state = DOWN;
			if (state != UP)
				return state = HOLD;
			case UP:
			return state = UP;
			case EMPTY:
			if (state == EMPTY)
				return state;
			if (state == UP)
				return state = EMPTY;
			return state = HOLD;
		}
	}
};
enum MOUSE_BUTTON { MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT, MOUSE_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE, MOUSE_BUTTON_RIGHT = SDL_BUTTON_RIGHT };

class InputManager {
	InputManager() = default;
	~InputManager() = default;
public:
	static InputManager &Instance() {
		static InputManager inputManager;
		return inputManager;
	}
	void Update() {
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
				case SDL_QUIT:				m_exit = true; break;
				case SDL_MOUSEMOTION:		m_mouseX = evnt.motion.x, m_mouseY = evnt.motion.y; break;
				case SDL_MOUSEWHEEL:		m_mouseWheel = evnt.wheel.y;

				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
											m_inputMap[evnt.button.button]((INPUT_VALUE::State)evnt.button.state); break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:				m_inputMap[evnt.key.keysym.sym]((INPUT_VALUE::State)evnt.key.state); break;
			}
		}
	}
	inline bool HasQuit() const { return m_exit; };
	inline Sint32 GetMouseX() const { return m_mouseX; };
	inline Sint32 GetMouseY() const { return m_mouseY; };
	inline Sint32 GetMouseWheel() const { return m_mouseWheel; };
	template<MOUSE_BUTTON btn> bool IsMouseDown() { return CheckInput<btn, INPUT_VALUE::DOWN>(); }
	template<MOUSE_BUTTON btn> bool IsMouseHold() { return CheckInput<btn, INPUT_VALUE::HOLD>(); }
	template<MOUSE_BUTTON btn> bool IsMouseUp() { return CheckInput<btn, INPUT_VALUE::UP>(); }
	template<char keyID> bool IsKeyDown() { return CheckInput<keyID, INPUT_VALUE::DOWN>(); }
	template<char keyID> bool IsKeyHold() { return CheckInput<keyID, INPUT_VALUE::HOLD>(); }
	template<char keyID> bool IsKeyUp() { return CheckInput<keyID, INPUT_VALUE::UP>(); }
private:
	template<char keyID, INPUT_VALUE::State value>
	bool CheckInput() {
		auto it = m_inputMap.find(keyID);
		if (it != m_inputMap.end()) return (it->second() == value);
		return false;
	};
	std::unordered_map<char, INPUT_VALUE> m_inputMap;
	Sint32 m_mouseX = 0, m_mouseY = 0;
	Sint32 m_mouseWheel = 0;
	bool m_exit = false;
};

#undef EMPTY_INPUT