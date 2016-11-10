#pragma once
#include <string>
#include <iostream>

namespace lgr {

	template <typename T>
	constexpr void Print(T elm) {
		std::cout << elm << std::endl;
	}

	template <typename T, typename ...Args>
	constexpr void Print(T cur, Args... nxt) {
		std::cout << cur;
		Print(nxt...);
	}

};