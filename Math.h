#pragma once

#include "Common.h"

class Math
{
public:
	Math();

	static float Angle(D3DXVECTOR2 v1, D3DXVECTOR2 v2);
	static int RandomInt(int min, int max);
	static float RandomFloat(float min, float max);
	static float Clamp(float min, float max, float value);

};

class Rect
{
public:
	Rect();
	Rect(float left, float top, float right, float bottom);

	bool IsIntersect(Rect rect);

	float GetWidth();
	float GetHeight();
	D3DXVECTOR2 GetCenter();

public:
	float left;
	float top;
	float right;
	float bottom;

};