#pragma once

#include "PlayerBullet.h"

class GameCamera;
class EnemySpawner;

class Player : public Sprite
{
public:
	Player();
	virtual ~Player();

	virtual void Update();
	virtual void Render();

	void Fire();
	void Shot();

	void SetAmbientLight(AmbientLight * light);
	void SetPointLight(PointLight * light);

public:
	unsigned life;
	unsigned killCount;
	float coolTime;

	GameCamera * camera;
	Sprite * thruster;
	std::vector<PlayerBullet *> bullets;

	Text * health;
	Text * kills;
	Text * dead;

	EnemySpawner * enemySpawner;

};