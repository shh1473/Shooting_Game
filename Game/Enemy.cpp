#include "DXUT.h"
#include "Enemy.h"
#include "Player.h"

Enemy::Enemy() :
	Sprite(
		app.shaders[L"ShipSprite"],
		app.vertexDeclarations[L"TangentBinormal"],
		app.materials[L"Enemy"],
		SPRITE_TINT |
		SPRITE_DIFFUSE_MAP |
		SPRITE_SPECULAR_MAP |
		SPRITE_NORMAL_MAP |
		SPRITE_AMBIENT_LIGHT |
		SPRITE_POINT_LIGHT |
		SPRITE_SPECULAR),
	isInvincible(true),
	life(3),
	movingTime(0.0f),
	coolTime(0.0f),
	speed(0.0f),
	move(0.0f),
	bullets(),
	player(nullptr)
{
	pointLightCount = 1;

	width = 64.0f;
	height = 64.0f;

	translation.y = 600.0f;
	translation.z = -20.0f;
	rotation.z = 180.0f;
}

Enemy::~Enemy()
{
	for (auto iter : bullets)
	{
		delete iter;
	}
}

void Enemy::Update()
{
	coolTime -= app.deltaTime;

	if (movingTime > 0.0f)
	{
		movingTime -= app.deltaTime;

		translation.y += move * speed * app.deltaTime;
	}
	else
	{
		isInvincible = false;

		if (coolTime < 0.0f)
		{

			coolTime = Math::RandomFloat(1.5f, 2.5f);

			Fire();
		}
	}

	if (life <= 0)
	{
		isDestroyed = true;
	}
	else if (life == 1)
	{
		color.y = 0.7f;
		color.z = 0.7f;
	}

	for (auto iter = bullets.begin(); iter != bullets.end();)
	{
		if ((*iter)->isDestroyed)
		{
			delete * iter;

			iter = bullets.erase(iter);
		}
		else
		{
			(*iter)->Update();

			++iter;
		}
	}

	Sprite::Update();
}

void Enemy::Render()
{
	Sprite::Render();

	for (auto iter : bullets)
	{
		iter->Render();
	}
}

void Enemy::Fight()
{
	float end;

	translation.x = Math::RandomFloat(-250.0f, 250.0f);

	end = Math::RandomFloat(100.0f, 300.0f);

	move = end - translation.y;

	movingTime = Math::RandomFloat(1.0f, 3.0f);
	speed = 1.0f / movingTime;
}

void Enemy::Fire()
{
	EnemyBullet * bullet = new EnemyBullet();
	bullets.push_back(bullet);

	bullet->rotation.z = D3DXToDegree(atan2(player->translation.y - translation.y, player->translation.x - translation.x)) - 90.0f;

	D3DXVECTOR2 move(player->translation.x - translation.x, player->translation.y - translation.y);
	D3DXVec2Normalize(&move, &move);

	bullet->ambientLight = ambientLight;
	bullet->pointLights[0] = pointLights[0];

	bullet->lifeTime = 3.0f;
	bullet->speed = 500.0f;
	bullet->move = move;
	bullet->player = player;

	bullet->translation = translation;
	bullet->translation.z = translation.z + 1.0f;
}