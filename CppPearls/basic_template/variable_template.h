#pragma once

#include <iostream>
#include <numeric>
#include <numbers>


template<typename T>
inline constexpr T Integer = 42;

inline void variable_template_test() {
	std::cout << __PRETTY_FUNCTION__ << "\n";


	std::cout << Integer<int> << " " << Integer<long> << "\n";


	std::cout << "max=" << std::numeric_limits<int>::max() << "\n";
	std::cout << "min=" << std::numeric_limits<int>::min() << "\n";

	std::cout << "pi=" << std::numbers::pi << "\n";

}
