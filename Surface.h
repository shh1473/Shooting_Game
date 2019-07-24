#pragma once

#include "Common.h"

class Surface
{
public:
	Surface();
	~Surface();

public:
	IDirect3DTexture9 *texture;
	IDirect3DSurface9 *surface;
	D3DSURFACE_DESC desc;

};