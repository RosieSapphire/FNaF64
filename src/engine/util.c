#include <stdlib.h>
#include <math.h>

#include "engine/util.h"

#define DRAW_SCALE 0.3333f

float wrapf(float x, float max, bool *has_wrapped)
{
	bool wrapped = false;
	while (x >= max) {
		float dist = x - max;
		x = dist;
		wrapped = true;
	}

	if (has_wrapped)
		*has_wrapped = wrapped;

	return x;
}

float fcutoff(float x, float threshold)
{
	if (fabsf(x) < threshold)
		return 0.0f;

	return x;
}

int icutoff(int x, int threshold)
{
	if (abs(x) < threshold)
		return 0.0f;

	return x;
}

int vcon(int x)
{
	return (int)roundf((float)x * DRAW_SCALE);
}

float speed_fps(int percent)
{
	return ((float)percent * 0.01f) * 60.0f;
}

float lerpf(float a, float b, float t)
{
	return a + (b - a) * t;
}
