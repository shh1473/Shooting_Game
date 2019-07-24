#include "DXUT.h"
#include "Math.h"

Math::Math()
{

}

float Math::Angle(D3DXVECTOR2 v1, D3DXVECTOR2 v2)
{
	return atan2(v2.y - v1.y, v2.x - v1.x);
}

int Math::RandomInt(int min, int max)
{
	static default_random_engine engine(timeGetTime());
	uniform_int_distribution<int> d(min, max);

	return d(engine);
}

float Math::RandomFloat(float min, float max)
{
	static default_random_engine engine(timeGetTime());
	uniform_real_distribution<float> d(min, max);

	return d(engine);
}

float Math::Clamp(float min, float max, float value)
{
	value = (min > value) ? min : value;
	value = (max < value) ? max : value;

	return value;
}

Rect::Rect() :
	left(0.0f),
	top(0.0f),
	right(0.0f),
	bottom(0.0f)
{

}

Rect::Rect(float left, float top, float right, float bottom) :
	left(left),
	top(top),
	right(right),
	bottom(bottom)
{

}

bool Rect::IsIntersect(Rect rect)
{
	return
		(left < rect.right) &&
		(top > rect.bottom) &&
		(right > rect.left) &&
		(bottom < rect.top);
}

float Rect::GetWidth()
{
	return right - left;
}

float Rect::GetHeight()
{
	return bottom - top;
}

D3DXVECTOR2 Rect::GetCenter()
{
	return D3DXVECTOR2(right - left, bottom - top) * 0.5f;
}