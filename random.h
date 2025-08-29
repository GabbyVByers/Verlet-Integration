#pragma once

#include "vec2f.h"
#include <cmath>

inline float randomFloat(float min, float max)
{
    return ((rand() / (float)RAND_MAX) * (max - min)) + min;
}

inline Vec2f randomVec2f(float min, float max)
{
    return
    {
        randomFloat(min, max),
        randomFloat(min, max)
    };
}

inline Color3f randomColor3f()
{
    return
    {
        randomFloat(0.25f, 1.0f),
        randomFloat(0.25f, 1.0f),
        randomFloat(0.25f, 1.0f)
    };
}

