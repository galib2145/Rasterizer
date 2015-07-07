#include "geometry.h"

#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS

float edgeFunction(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2) {
    return (v2.x - v0.x) * (v1.y - v0.y) - (v2.y - v0.y) * (v1.x - v0.x);
}

float min3(const float &a, const float &b, const float &c)
{ return std::min(a, std::min(b, c)); }

float max3(const float &a, const float &b, const float &c)
{ return std::max(a, std::max(b, c)); }

#endif
