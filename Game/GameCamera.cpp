#include "DXUT.h"
#include "GameCamera.h"

GameCamera::GameCamera() :
	EffectCamera(
		app.shaders[L"BloomCamera"],
		app.vertexDeclarations[L"TexCoord"],
		app.surfaces[L"Bloom"],
		CAMERA_COLOR_CONTRAST |
		CAMERA_FILTER |
		CAMERA_BLOOM)
{
	sourceMaps[0] = app.surfaces[L"Scene"];
	sourceMaps[1] = app.surfaces[L"Extract"];
	sourceMaps[2] = app.surfaces[L"HorizontalBlur"];
	sourceMaps[3] = app.surfaces[L"VerticalBlur"];

	thresholdIntensityViewportInv.z = 1.0f / static_cast<float>(sourceMaps[0]->desc.Width);
	thresholdIntensityViewportInv.w = 1.0f / static_cast<float>(sourceMaps[0]->desc.Height);

	translation.z = -500.0f;
}

GameCamera::~GameCamera()
{

}