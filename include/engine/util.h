#ifndef ENGINE_UTIL_H_
#define ENGINE_UTIL_H_

#include <stdbool.h>

float clampf(float x, float min, float max);
float wrapf(float x, float max, bool *has_wrapped);
float fcutoff(float x, float threshold);
int icutoff(int x, int threshold);
int vcon(int x);
float speed_fps(int percent);
float lerpf(float a, float b, float t);

#endif /* ENGINE_UTIL_H_ */
