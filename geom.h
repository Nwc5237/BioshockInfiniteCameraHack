#pragma once
#include "pch.h"

struct vec3 {
	float x;
	float y;
	float z;
};

void scale_vec(float s, vec3* vec);
void normalize(vec3* vec);
vec3 cross(vec3 a, vec3 b);
vec3 sum(vec3 a, vec3 b);