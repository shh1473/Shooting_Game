#pragma once

#include "Texture.h"

class Material
{
public:
	Material(unsigned widthCount, unsigned heightCount, wstring diffuseMapName, wstring specularMapName, wstring normalMapName);
	~Material();

public:
	unsigned widthFrameCount;
	unsigned heightFrameCount;
	float width;
	float height;
	float widthRatioPerFrame;
	float heightRatioPerFrame;
	D3DXVECTOR4 tint;
	float specularIntensity;
	float specularPower;
	Texture *diffuseMap;
	Texture *specularMap;
	Texture *normalMap;

};