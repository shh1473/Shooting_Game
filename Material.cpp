#include "DXUT.h"
#include "Material.h"
#include "App.h"

Material::Material(
	unsigned widthCount,
	unsigned heightCount,
	wstring diffuseMapName,
	wstring specularMapName,
	wstring normalMapName) :
	widthFrameCount(widthCount),
	heightFrameCount(heightCount),
	width(0.0f),
	height(0.0f),
	widthRatioPerFrame(0.0f),
	heightRatioPerFrame(0.0f),
	tint(1.0f, 1.0f, 1.0f, 1.0f),
	specularIntensity(1.0f),
	specularPower(20.0f),
	diffuseMap(app.textures[diffuseMapName]),
	specularMap(app.textures[specularMapName]),
	normalMap(app.textures[normalMapName])
{
	D3DSURFACE_DESC surfaceDesc;
	diffuseMap->texture->GetLevelDesc(0, &surfaceDesc);

	width = static_cast<float>(diffuseMap->info.Width) / widthFrameCount;
	height = static_cast<float>(diffuseMap->info.Height) / heightFrameCount;

	widthRatioPerFrame = width / surfaceDesc.Width;
	heightRatioPerFrame = height / surfaceDesc.Height;
}

Material::~Material()
{

}