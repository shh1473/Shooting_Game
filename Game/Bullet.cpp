#include "DXUT.h"
#include "Bullet.h"

Bullet::Bullet(Material * material) :
	Sprite(
		app.shaders[L"BackgroundSprite"],
		app.vertexDeclarations[L"Normal"],
		material,
		SPRITE_DIFFUSE_MAP |
		SPRITE_SPECULAR_MAP |
		SPRITE_AMBIENT_LIGHT |
		SPRITE_POINT_LIGHT |
		SPRITE_SPECULAR),
	lifeTime(0.0f),
	speed(0.0f),
	move(0.0f, 0.0f)
{
	width = 16.0f;
	height = 40.0f;
}

Bullet::~Bullet()
{

}

void Bullet::Update()
{
	Object::Update();

	translation.x += move.x * speed * app.deltaTime;
	translation.y += move.y * speed * app.deltaTime;

	lifeTime -= app.deltaTime;

	if (lifeTime < 0.0f)
	{
		isDestroyed = true;
	}
}