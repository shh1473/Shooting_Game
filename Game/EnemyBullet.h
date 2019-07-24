#pragma once

#include "Bullet.h"

class Player;

class EnemyBullet : public Bullet
{
public:
	EnemyBullet();
	virtual ~EnemyBullet();

	virtual void Update();

public:
	Player * player;

};