#pragma once

#include <stdbool.h>

#define TO_RADIANS(X) (X * 0.01745329252f)

#define MIN(A, B) (((A) < (B)) ? (A) : (B))
#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define CLAMP(X, MIN_VAL, MAX_VAL) (MAX((MIN(X, MAX_VAL)), (MIN_VAL)))

float wrapf(float x, float max, bool *has_wrapped);
float fcutoff(float x, float threshold);
int icutoff(int x, int threshold);
/* TODO: Maybe rename this something better to make it more understandable. */
int vcon(int x);
float speed_fps(int percent);
float lerpf(float a, float b, float t);
