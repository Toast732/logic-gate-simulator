#include "raylib-cpp.hpp"

#pragma once

inline float randf(float min, float max, int decimals) {
	int _decimals = decimals + 1;
	return (float)GetRandomValue((int)(min * _decimals), (int)(max * _decimals)) / _decimals;
}

inline float randf(float min, float max) {
	return randf(min, max, 0);
}