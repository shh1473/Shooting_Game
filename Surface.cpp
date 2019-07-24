#include "DXUT.h"
#include "Surface.h"

Surface::Surface() :
	texture(nullptr),
	surface(nullptr)
{
	ZeroMemory(&desc, sizeof(D3DSURFACE_DESC));
}

Surface::~Surface()
{
	SAFE_RELEASE(surface);
	SAFE_RELEASE(texture);
}