#pragma once

#include "Bullet.h"

class EnemySpawner;

class PlayerBullet : public Bullet
{
public:
	PlayerBullet();
	virtual ~PlayerBullet();

	virtual void Update();

public:
	Text * kills;

	EnemySpawner * enemySpawner;

};