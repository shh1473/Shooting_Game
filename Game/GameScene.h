#pragma once

#include "../SunrinEngine.h"
#include "GameCamera.h"
#include "GameAmbientLight.h"
#include "Board.h"
#include "Player.h"
#include "Planet.h"
#include "EnemySpawner.h"

class GameScene : public Object
{
public:
	GameScene();
	virtual ~GameScene();

	virtual void Update();
	virtual void Render();

public:
	GameCamera * camera;

	GameAmbientLight * ambientLight;

	Text * health;
	Text * kills;
	Text * dead;

	Board * board;
	Player * player;
	Planet * planet;
	EnemySpawner * enemySpawner;

};