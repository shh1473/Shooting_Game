#pragma once

#include "Common.h"

class Texture
{
public:
	Texture();
	~Texture();

public:
	IDirect3DTexture9 *texture;
	D3DXIMAGE_INFO info;

};