#pragma once
#include <math.h>

float euclidean_distance(float x, float y) {
	return sqrtf((x * x) + (y * y));
}