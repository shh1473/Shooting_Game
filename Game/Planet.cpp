#include "DXUT.h"
#include "Planet.h"
#include "GameCamera.h"

Planet::Planet() :
	Sprite(
		app.shaders[L"BackgroundSprite"],
		app.vertexDeclarations[L"TangentBinormal"],
		app.materials[L"Planet"],
		SPRITE_DIFFUSE_MAP |
		SPRITE_SPECULAR_MAP |
		SPRITE_AMBIENT_LIGHT |
		SPRITE_POINT_LIGHT |
		SPRITE_SPECULAR),
	reflection(new PointLight())
{
	AddChild(reflection);

	translation.z = -10.0f;
	translation.y = -600.0f;
	rotation.z = 30.0f;

	pointLightCount = 1;
	pointLights[0] = reflection;

	reflection->colorIntensity = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 2.0f);
	reflection->radius = 300.0f;
	reflection->translation.z = -100.0f;
}

Planet::~Planet()
{

}

void Planet::Update()
{
	Object::Update();

	coolTime -= app.deltaTime;

	if (coolTime < 0.0f)
	{
		Rise();
	}

	if (movingTime > 0.0f)
	{
		movingTime -= app.deltaTime;

		translation.x += move.x * speed * app.deltaTime;
		translation.y += move.y * speed * app.deltaTime;
		rotation.z += move.z * speed * app.deltaTime;

		camera->thresholdIntensityViewportInv.y = 1.0f + (maxTime - abs(movingTime - maxTime)) / maxTime;
		ambientLight->colorIntensity.w = 0.7f + camera->thresholdIntensityViewportInv.y * 0.2f;
	}
}

void Planet::Rise()
{
	D3DXVECTOR3 start;
	D3DXVECTOR3 end;

	start.x = -300.0f - Math::RandomFloat(200.0f, 400.0f);
	start.y = Math::RandomFloat(-450.0f, 450.0f);
	start.z = Math::RandomFloat(-360.0f, 360.0f);

	end.x = 300.0f + Math::RandomFloat(200.0f, 400.0f);
	end.y = Math::RandomFloat(-450.0f, 450.0f);
	end.z = Math::RandomFloat(-720.0f, 720.0f);

	translation.x = start.x;
	translation.y = start.y;
	rotation.z = start.z;
	move = end - start;

	movingTime = Math::RandomFloat(3.0f, 5.0f);
	speed = 1.0f / movingTime;

	maxTime = movingTime * 0.5f;
	coolTime = Math::RandomFloat(5.0f, 10.0f);
}