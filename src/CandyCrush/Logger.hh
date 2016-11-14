#pragma once
#include <string>
#include <iostream>

namespace Logger {

	template <typename T>
	constexpr void Print(T elm) {
	#ifdef _DEBUG
		std::cout << elm << std::endl;
	#endif
	}

	template <typename T, typename ...Args>
	constexpr void Print(T cur, Args... nxt) {
	#ifdef _DEBUG
		std::cout << cur;
		Print(nxt...);
	#endif
	}

};