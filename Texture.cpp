#include "DXUT.h"
#include "Texture.h"

Texture::Texture() :
	texture(nullptr)
{
	ZeroMemory(&info, sizeof(D3DXIMAGE_INFO));
}

Texture::~Texture()
{
	SAFE_RELEASE(texture);
}