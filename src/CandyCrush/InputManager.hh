/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include <queue>
#include <iostream>
#include <SDL/SDL.h>
#include <unordered_map>

//! Defines a shortcut to the InputManager Singleton instance.
#define IM InputManager::Instance()

//! Encapsulates everything related to mouse coordinates.
struct MouseCoords { 
	Sint32 x{ 0 }, y{ 0 };
	MouseCoords() = default;
	MouseCoords(Sint32 x_, Sint32 y_) : x(x_), y(y_) {};
	friend MouseCoords operator-(const MouseCoords &lhs, const MouseCoords &rhs) { return std::move(MouseCoords(lhs.x - rhs.x, lhs.y - rhs.y)); };
	friend std::ostream &operator<<(std::ostream &os, const MouseCoords &rhs) { return os << '(' << rhs.x << ", " << rhs.y << ')'; };
};

//! Identifies each button pressed with the mouse (left | middle | right).
enum MouseButton {
	MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_RIGHT
};

//! Identifies additional key buttons to be pressed (Esc, Shift, Ctrl, Alt, key arrows, ...).
enum KeyButton : int { 
	KEY_BUTTON_DOWN = SDLK_DOWN,
	KEY_BUTTON_UP = SDLK_UP,
	KEY_BUTTON_LEFT = SDLK_LEFT,
	KEY_BUTTON_RIGHT = SDLK_RIGHT,
	KEY_BUTTON_ESCAPE = SDLK_ESCAPE,
	KEY_BUTTON_BACKSPACE = SDLK_BACKSPACE,
	KEY_BUTTON_ENTER = SDLK_RETURN,
	KEY_BUTTON_TAB = SDLK_TAB,
	KEY_BUTTON_LCTRL = SDLK_LCTRL,
	KEY_BUTTON_RCTRL = SDLK_RCTRL,
	KEY_BUTTON_LALT = SDLK_LALT,
	KEY_BUTTON_RALT = SDLK_RALT,
	KEY_BUTTON_LSHIFT = SDLK_LSHIFT,
	KEY_BUTTON_RSHIFT = SDLK_RSHIFT,
};

//! Controls all input info and stores it to be used in other pats of the code as a Singleton.
class InputManager {
public:
	/**
	 * Creates a Singleton instance of the InputManager class.
	 * @return A reference to the static instance.
	 */
	inline static InputManager &Instance() {
		static InputManager inputManager;
		return inputManager;
	}
	/**
	 * Unpacks game input events and processes them.
	 */ 
	void Update(void) {
		ProcessStates(); // Controls the state of each event stored in the dictionary
		SDL_Event evnt; // Stores the different types of input events [https://wiki.libsdl.org/SDL_Event]
		while (SDL_PollEvent(&evnt)) { // While input events exist, unpack them and store them in the SDL_Event variable one by one
			switch (evnt.type) { // Check the event type (keyboard, mouse, window, joystick, ...)
				case SDL_QUIT:				m_exit = true; break; // The cross button of the window has been pressed
				case SDL_MOUSEMOTION:		m_mouseCoords.x = evnt.motion.x; m_mouseCoords.y = evnt.motion.y; break; // Store the mouse coords when mouse is moved over the screen
				case SDL_MOUSEWHEEL:		m_mouseWheel = evnt.wheel.y; // Store the movement direction of the wheel when it is used
				case SDL_MOUSEBUTTONDOWN:	m_inputValues.push(&(m_inputMap[evnt.button.button] = InputValue::DOWN)); break; // Push the event mouse down to the queue of processes
				case SDL_MOUSEBUTTONUP:		m_inputValues.push(&(m_inputMap[evnt.button.button] = InputValue::UP)); break; // Push the event mouse up to the queue of processes
				case SDL_KEYDOWN:			m_inputValues.push(&(m_inputMap[evnt.key.keysym.sym] = (evnt.key.repeat) ? InputValue::HOLD : InputValue::DOWN)); break; // Push the event key down to the queue of processes
				case SDL_KEYUP:				m_inputValues.push(&(m_inputMap[evnt.key.keysym.sym] = InputValue::UP)); break; // Push the event key up to the queue of processes
			}
		}
	}
	/**
	 * Checks if the cross button of the window has been pressed.
	 * @return A bool that determines the condition.
	 */
	inline bool HasQuit(void) const { 
		return m_exit; 
	};
	/**
	 * Get the mouse coordinates.
	 * @return A MouseCoords structure that stores an x and y value.
	 */
	inline const MouseCoords &GetMouseCoords(void) const {
		return m_mouseCoords;
	};
	/**
	 * Get the mouse wheel info.
	 * @return A signed integer that represents the negative or positive movement.
	 */
	inline Sint32 GetMouseWheel(void) const {
		return m_mouseWheel; 
	};
	/**
	 * Check if a mouse button is triggered down.
	 * @tparam btn Indicates the button to be checked (left | middle | right).
	 * @return A boolean value that indicates if the event was found.
	 */
	template<MouseButton btn> bool IsMouseDown(void) {
		return CheckInput<btn, InputValue::DOWN>();
	}
	/**
	 * Check if a mouse button is being held down.
	 * @tparam btn Indicates the button to be checked (left | middle | right).
	 * @return A boolean value that indicates if the event was found.
	 */
	template<MouseButton btn> bool IsMouseHold(void) {
		return CheckInput<btn, InputValue::HOLD>();
	}
	/**
	 * Check if a mouse button is triggered up.
	 * @tparam btn Indicates the button to be checked (left | middle | right).
	 * @return A boolean value that indicates if the event was found.
	 */
	template<MouseButton btn> bool IsMouseUp(void) {
		return CheckInput<btn, InputValue::UP>();
	}
	/**
	 * Check if a key button is triggered down.
	 * @tparam key Indicates the key to be checked (e.g., 'a', '0', KEY_BUTTON_DOWN, KEY_BUTTON_ESCAPE, ...).
	 * @return A boolean value that indicates if the event was found.
	 */
	template<Sint32 key> bool IsKeyDown(void) {
		return CheckInput<key, InputValue::DOWN>();
	}
	/**
	 * Check if a mouse button is being held down.
	 * @tparam key Indicates the key to be checked (e.g., 'a', '0', KEY_BUTTON_DOWN, KEY_BUTTON_ESCAPE, ...).
	 * @return A boolean value that indicates if the event was found.
	 */
	template<Sint32 key> bool IsKeyHold(void) {
		return CheckInput<key, InputValue::HOLD>();
	}
	/**
	 * Check if a key button is triggered up.
	 * @tparam key Indicates the key to be checked (e.g., 'a', '0', KEY_BUTTON_DOWN, KEY_BUTTON_ESCAPE, ...).
	 * @return A boolean value that indicates if the event was found.
	 */
	template<Sint32 key> bool IsKeyUp(void) {
		return CheckInput<key, InputValue::UP>();
	}
private:
	/**
	 * Defines each state of the input event stored (DOWN -> HOLD | UP -> EMPTY).
	 */
	enum class InputValue {
		UP = SDL_RELEASED,
		DOWN,
		HOLD,
		EMPTY
	};
	/**
	 * Default constructor.
	 */
	InputManager() = default;
	/**
	 * Deleted copy constructor.
	 */
	InputManager(const InputManager &rhs) = delete;
	/**
	 * Deleted copy assignment operator.
	 */
	InputManager &operator=(const InputManager &rhs) = delete;
	/**
	 * Processes the states of the events stored in the queue.
	 * Each event stored as DOWN shall turn to HOLD new iteration.
	 * Each event stored as UP shall turn to EMPTY new iteration rather than deleting it.
	 */
	void ProcessStates(void) {
		while (!m_inputValues.empty()) { // While queue stills filled
			auto value = m_inputValues.front(); // Get the front element of the queue
			switch (*value) { // Check if new value is DOWN or UP
				case InputValue::DOWN:	*value = InputValue::HOLD; value = nullptr; break; // Set value to HOLD and set the pointer to null
				case InputValue::UP:	*value = InputValue::EMPTY; value = nullptr; // Set value to EMPTY and set the pointer to null
			} 
			m_inputValues.pop(); // Deletes value element from the queue
		}
	}
	/**
	 * Processes the states of the events stored in the queue.
	 * @tparam id Represents the keyID or the btnID to check for.
	 * @tparam value Represents the value to be compared to as the state of the event.
	 * @return Brings back a boolean value as a result of the comparison.
	 */
	template<Sint32 id, InputValue value> bool CheckInput(void) {
		auto it = m_inputMap.find(id); // Check for the id inside the dictionary
		return (it != m_inputMap.end()) ? it->second == value : false; // Return the result of the search
	}
private:
	std::unordered_map<Sint32, InputValue> m_inputMap;		//!< Dictionary that stores elements with a key as a character and a value as an state.
	std::queue<InputValue*> m_inputValues;					//!< Queue that only stores the events added at the same time to be processed.
	MouseCoords m_mouseCoords;								//!< Mouse coordinates main instance where mouse input info is stored.
	Sint32 m_mouseWheel{ 0 };								//!< Mouse wheel info which is stored for vertical movement.
	bool m_exit = false;									//!< Trigger condition that sets to true when the user clicks the cross window button.
};