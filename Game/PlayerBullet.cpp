#include "DXUT.h"
#include "PlayerBullet.h"
#include "Player.h"
#include "EnemySpawner.h"

PlayerBullet::PlayerBullet() :
	Bullet(app.materials[L"PlayerBullet"]),
	kills(nullptr),
	enemySpawner(nullptr)
{

}

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Update()
{
	for (auto iter : enemySpawner->enemies)
	{
		if (!iter->isInvincible && rect.IsIntersect(iter->rect))
		{
			--iter->life;

			if (iter->life <= 0)
			{
				++iter->player->killCount;

				kills->text = L"óġ: " + std::to_wstring(iter->player->killCount);
			}

			isDestroyed = true;
		}
	}

	Bullet::Update();
}