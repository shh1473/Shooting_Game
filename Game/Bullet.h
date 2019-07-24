#pragma once

#include "../SunrinEngine.h"

class Bullet : public Sprite
{
public:
	Bullet(Material * material);
	virtual ~Bullet();

	virtual void Update();

public:
	float lifeTime;
	float speed;
	D3DXVECTOR2 move;

};