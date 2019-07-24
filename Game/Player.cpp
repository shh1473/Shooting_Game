#include "DXUT.h"
#include "Player.h"
#include "GameCamera.h"
#include "EnemySpawner.h"

Player::Player() :
	Sprite(
		app.shaders[L"ShipSprite"],
		app.vertexDeclarations[L"TangentBinormal"],
		app.materials[L"Player"],
		SPRITE_TINT |
		SPRITE_DIFFUSE_MAP |
		SPRITE_SPECULAR_MAP |
		SPRITE_NORMAL_MAP |
		SPRITE_AMBIENT_LIGHT |
		SPRITE_POINT_LIGHT |
		SPRITE_SPECULAR),
	life(5),
	killCount(0),
	coolTime(0.0f),
	thruster(new Sprite(
		app.shaders[L"BackgroundSprite"],
		app.vertexDeclarations[L"Normal"],
		app.materials[L"Fire"],
		SPRITE_DIFFUSE_MAP |
		SPRITE_SPECULAR_MAP |
		SPRITE_AMBIENT_LIGHT |
		SPRITE_POINT_LIGHT |
		SPRITE_SPECULAR))
{
	AddChild(thruster);

	width = 64.0f;
	height = 64.0f;

	pointLightCount = 1;
	translation.y = -350.0f;
	translation.z = -30.0f;

	thruster->pointLightCount = 1;
	thruster->translation.y = -30.0f;
	thruster->translation.z = 1.0f;
}

Player::~Player()
{
	for (auto iter : bullets)
	{
		delete iter;
	}
}

void Player::Update()
{
	if (coolTime > 0.0f)
	{
		coolTime -= app.deltaTime;
	}

	if (app.IsPressing('D'))
	{
		translation.x += 200.0f * app.deltaTime;
	}
	if (app.IsPressing('A'))
	{
		translation.x -= 200.0f * app.deltaTime;
	}
	if (app.IsPressing('W'))
	{
		translation.y += 200.0f * app.deltaTime;
	}
	if (app.IsPressing('S'))
	{
		translation.y -= 200.0f * app.deltaTime;
	}
	if (app.IsPressed(VK_SPACE) && coolTime <= 0.0f)
	{
		coolTime = 0.3f;

		Fire();
	}

	if (translation.x < -268.0f) { translation.x = -268.0f; }
	if (translation.x > 268.0f) { translation.x = 268.0f; }
	if (translation.y < -418.0f) { translation.y = -418.0f; }
	if (translation.y > 418.0f) { translation.y = 418.0f; }

	if (life <= 0)
	{
		isActivated = false;

		app.isSlowMode = true;

		camera->filter = D3DXMATRIX(
			0.393f, 0.769f, 0.189f, 0.0f,
			0.349f, 0.686f, 0.168f, 0.0f,
			0.272f, 0.534f, 0.131f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		dead->translation.y = 400.0f;

		enemySpawner->isSpawning = false;
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

void Player::Render()
{
	Sprite::Render();

	for (auto iter : bullets)
	{
		iter->Render();
	}
}

void Player::Fire()
{
	PlayerBullet * bullet = new PlayerBullet();
	bullets.push_back(bullet);

	D3DXVECTOR2 move(0.0f, 1.0f);
	D3DXVec2Normalize(&move, &move);

	bullet->ambientLight = ambientLight;
	bullet->pointLights[0] = pointLights[0];

	bullet->lifeTime = 3.0f;
	bullet->speed = 500.0f;
	bullet->move = move;
	bullet->kills = kills;
	bullet->enemySpawner = enemySpawner;

	bullet->translation = translation;
	bullet->translation.z = translation.z + 1.0f;
}

void Player::Shot()
{
	health->text = L"Ã¼·Â: " + std::to_wstring(life);
}

void Player::SetAmbientLight(AmbientLight * light)
{
	ambientLight = light;
	thruster->ambientLight = light;
}

void Player::SetPointLight(PointLight * light)
{
	pointLights[0] = light;
	thruster->pointLights[0] = light;
}