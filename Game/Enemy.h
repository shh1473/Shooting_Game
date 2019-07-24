#pragma once

#include "EnemyBullet.h"

class Player;

class Enemy : public Sprite
{
public:
	Enemy();
	virtual ~Enemy();

	virtual void Update();
	virtual void Render();

	void Fight();

	void Fire();

public:
	bool isInvincible;
	unsigned life;
	float movingTime;
	float coolTime;
	float speed;
	float move;

	std::vector<EnemyBullet *> bullets;

	Player * player;

};