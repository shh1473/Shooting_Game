#include "DXUT.h"
#include "EnemyBullet.h"
#include "Player.h"

EnemyBullet::EnemyBullet() :
	Bullet(app.materials[L"EnemyBullet"]),
	player(nullptr)
{

}

EnemyBullet::~EnemyBullet()
{

}

void EnemyBullet::Update()
{
	if (rect.IsIntersect(player->rect))
	{
		--player->life;

		if (player->isActivated)
		{
			player->Shot();
		}

		isDestroyed = true;
	}

	Bullet::Update();
}