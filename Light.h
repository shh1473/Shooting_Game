#pragma once

#include "Object.h"

class Light : public Object
{
public:
	Light();
	virtual ~Light();

public:
	D3DXVECTOR4 colorIntensity;

};

class AmbientLight : public Light
{
public:
	AmbientLight();
	virtual ~AmbientLight();

public:


};

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	virtual ~DirectionalLight();

	virtual void Update();

public:
	D3DXVECTOR4 direction;

};

class PointLight : public Light
{
public:
	PointLight();
	virtual ~PointLight();

public:
	float radius;

};

class SpotLight : public Light
{
public:
	SpotLight();
	virtual ~SpotLight();

	virtual void Update();

public:
	D3DXVECTOR4 direction;
	float innerAngle;
	float outerAngle;
	float radius;

};

struct AmbientLightData
{
	D3DXVECTOR4 colorIntensity;

};

struct DirectionalLightData
{
	D3DXVECTOR4 colorIntensity;
	D3DXVECTOR4 direction;

};

struct PointLightData
{
	D3DXVECTOR4 colorIntensity;
	D3DXVECTOR4 translationRadius;

};

struct SpotLightData
{
	D3DXVECTOR4 colorIntensity;
	D3DXVECTOR4 direction;
	D3DXVECTOR2 coneAngle;
	D3DXVECTOR4 translationRadius;

};