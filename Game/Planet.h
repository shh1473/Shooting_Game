#pragma once

#include "../SunrinEngine.h"

class GameCamera;

class Planet : public Sprite
{
public:
	Planet();
	virtual ~Planet();

	virtual void Update();

	void Rise();

public:
	PointLight * reflection;
	GameCamera * camera;

	float maxTime;
	float movingTime;
	float coolTime;
	float speed;
	D3DXVECTOR3 move;

};