#include "DXUT.h"
#include "Light.h"
#include "Math.h"

Light::Light() :
	colorIntensity(1.0f, 1.0f, 1.0f, 1.0f)
{

}

Light::~Light()
{

}

AmbientLight::AmbientLight()
{

}

AmbientLight::~AmbientLight()
{

}

DirectionalLight::DirectionalLight() :
	direction(0.0f, 0.0f, 1.0f, 1.0f)
{

}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::Update()
{
	static D3DXVECTOR3 tmpDirection;
	static D3DXMATRIX rotationMatrix;

	tmpDirection.x = 0.0f;
	tmpDirection.y = 0.0f;
	tmpDirection.z = 1.0f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(rotation.y), D3DXToRadian(rotation.x), D3DXToRadian(rotation.z));

	D3DXVec3TransformNormal(&tmpDirection, &tmpDirection, &rotationMatrix);
	D3DXVec3Normalize(&tmpDirection, &tmpDirection);

	direction.x = tmpDirection.x;
	direction.y = tmpDirection.y;
	direction.z = tmpDirection.z;
}

PointLight::PointLight() :
	radius(100.0f)
{

}

PointLight::~PointLight()
{

}

SpotLight::SpotLight() :
	direction(0.0f, 0.0f, 1.0f, 1.0f),
	innerAngle(0.0f),
	outerAngle(0.0f),
	radius(100.0f)
{

}

SpotLight::~SpotLight()
{

}

void SpotLight::Update()
{
	static D3DXVECTOR3 tmpDirection;
	static D3DXMATRIX rotationMatrix;

	tmpDirection.x = 0.0f;
	tmpDirection.y = 0.0f;
	tmpDirection.z = 1.0f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(rotation.y), D3DXToRadian(rotation.x), D3DXToRadian(rotation.z));

	D3DXVec3TransformNormal(&tmpDirection, &tmpDirection, &rotationMatrix);
	D3DXVec3Normalize(&tmpDirection, &tmpDirection);

	direction.x = tmpDirection.x;
	direction.y = tmpDirection.y;
	direction.z = tmpDirection.z;
}