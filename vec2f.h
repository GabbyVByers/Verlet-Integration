#pragma once

struct Color3f
{
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
};

struct Vertex
{
	float x, y;
	float r, g, b;
	float radius;
};

struct Vec2f
{
	float x = 0.0f;
	float y = 0.0f;

	Vec2f operator + (const Vec2f& vec) const
	{
		return { x + vec.x, y + vec.y };
	}

	Vec2f operator - (const Vec2f& vec) const
	{
		return { x - vec.x, y - vec.y };
	}

	Vec2f operator * (const float value) const
	{
		return { x * value, y * value };
	}

	Vec2f operator / (const float value) const
	{
		return { x / value, y / value };
	}
};

inline float lengthSquared(const Vec2f& vec)
{
	return (vec.x * vec.x) + (vec.y * vec.y);
}

inline float length(const Vec2f& vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

inline void normalize(Vec2f& vec)
{
	float len = length(vec);
	vec.x = vec.x / len;
	vec.y = vec.y / len;
}

inline float dot(const Vec2f& a, const Vec2f& b)
{
	return (a.x * b.x) + (a.y * b.y);
}