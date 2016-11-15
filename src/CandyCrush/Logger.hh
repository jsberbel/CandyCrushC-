/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include <string>
#include <iostream>

namespace Logger {

	// Function that prints an end of line
	static void Endln() {
	#ifdef _DEBUG
		std::cout << std::endl;
	#endif
	}

	// Function that prints an element and an end of line
	template <typename T>
	constexpr void Println(T elm) {
	#ifdef _DEBUG
		std::cout << elm << std::endl;
	#endif
	}

	// Function that prints some elements and an end of line
	template <typename T, typename ...Args>
	constexpr void Println(T cur, Args... nxt) {
	#ifdef _DEBUG
		std::cout << cur;
		Println(nxt...);
	#endif
	}

	// Function that prints an element
	template <typename T>
	constexpr void Print(T elm) {
	#ifdef _DEBUG
		std::cout << elm;
	#endif
	}

	// Function that prints some elements
	template <typename T, typename ...Args>
	constexpr void Print(T cur, Args... nxt) {
	#ifdef _DEBUG
		std::cout << cur;
		Print(nxt...);
	#endif
	}

};