#include "pch.h"
#include "geom.h"
#include <math.h>

void scale_vec(float s, vec3* vec) {
    vec->x *= s;
    vec->y *= s;
    vec->z *= s;
}

void normalize(vec3* vec) {
    float s = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
    scale_vec(1.f / (s != 0 ? s : .00001f), vec);
}

vec3 cross(vec3 a, vec3 b) {
    return vec3{ a.y * b.z - a.z * b.y, -a.x * b.z + b.x * a.z, a.x * b.y - b.x * a.y };
}

vec3 sum(vec3 a, vec3 b) {
    vec3 sum;
    sum.x = a.x + b.x;
    sum.y = a.y + b.y;
    sum.z = a.z + b.z;
}