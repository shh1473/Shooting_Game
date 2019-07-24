#pragma once

#include "Camera.h"
#include "Shader.h"
#include "VertexDeclaration.h"

#define CAMERA_COLOR_CONTRAST 0x0001
#define CAMERA_FILTER 0x0002
#define CAMERA_BLUR 0x0004
#define CAMERA_BLOOM 0x0008

#define CAMERA_MAX_SOURCEMAP 4

class EffectCamera : public Camera
{
public:
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texCoord;
	};

	struct Index
	{
		WORD _0;
		WORD _1;
		WORD _2;
	};

public:
	EffectCamera(Shader *shader, VertexDeclaration *vertexDeclaration, Surface * renderTarget, unsigned renderFlags);
	virtual ~EffectCamera();

	virtual void Ready();

	void Effect();

public:
	unsigned renderFlags;
	Shader *shader;
	VertexDeclaration *vertexDeclaration;
	Surface *sourceMaps[CAMERA_MAX_SOURCEMAP];
	D3DXVECTOR4 thresholdIntensityViewportInv;
	D3DXVECTOR4 colorContrast;
	D3DXMATRIX filter;
	Vertex vertices[4];
	Index indices[2];
	IDirect3DVertexBuffer9 *vertexBuffer;
	IDirect3DIndexBuffer9 *indexBuffer;

};