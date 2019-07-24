#include "DXUT.h"
#include "EnemySpawner.h"

EnemySpawner::EnemySpawner() :
	isSpawning(true),
	coolTime(0.0f),
	enemies()
{

}

EnemySpawner::~EnemySpawner()
{
	
}

void EnemySpawner::Update()
{
	for (auto iter = enemies.begin(); iter != enemies.end();)
	{
		if ((*iter)->isDestroyed)
		{
			iter = enemies.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	if (isSpawning)
	{
		coolTime -= app.deltaTime;

		if (coolTime < 0.0f)
		{
			SpawnEnemy();
		}
	}

	Object::Update();
}

void EnemySpawner::SpawnEnemy()
{
	Enemy * enemy = new Enemy();
	AddChild(enemy);
	enemies.push_back(enemy);

	enemy->ambientLight = ambientLight;
	enemy->pointLights[0] = pointLight;
	enemy->player = player;

	enemy->Fight();

	coolTime = Math::RandomFloat(1.0f, 5.0f);
}