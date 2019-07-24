#pragma once

#include "../SunrinEngine.h"
#include "Enemy.h"

class EnemySpawner : public Object
{
public:
	EnemySpawner();
	virtual ~EnemySpawner();

	virtual void Update();

	void SpawnEnemy();

public:
	bool isSpawning;
	float coolTime;
	std::vector<Enemy *> enemies;

	AmbientLight * ambientLight;
	PointLight * pointLight;
	Player * player;

};