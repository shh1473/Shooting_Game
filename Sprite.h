#pragma once

#include "Object.h"
#include "Material.h"
#include "Shader.h"
#include "VertexDeclaration.h"
#include "Light.h"

#define SPRITE_TINT 0x0001
#define SPRITE_DIFFUSE_MAP 0x0002
#define SPRITE_SPECULAR_MAP 0x0004
#define SPRITE_NORMAL_MAP 0x0008
#define SPRITE_AMBIENT_LIGHT 0x0010
#define SPRITE_DIRECTIONAL_LIGHT 0x0020
#define SPRITE_POINT_LIGHT 0x0040
#define SPRITE_SPOT_LIGHT 0x0080
#define SPRITE_SPECULAR 0x0100

#define SPRITE_MAX_DIRECTIONAL_LIGHT 1
#define SPRITE_MAX_POINT_LIGHT 3
#define SPRITE_MAX_SPOT_LIGHT 3

class Sprite : public Object
{
public:
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texCoord;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

	struct Index
	{
		WORD _0;
		WORD _1;
		WORD _2;
	};

public:
	Sprite(Shader *shader, VertexDeclaration *vertexDeclaration, Material *material, unsigned renderFlags);
	virtual ~Sprite();

	virtual void Render();

	void ChangeFrameIndex(unsigned i, unsigned j);

public:
	unsigned renderFlags;
	unsigned widthFrameIndex;
	unsigned heightFrameIndex;
	D3DXVECTOR4 color;
	Shader *shader;
	VertexDeclaration *vertexDeclaration;
	Material *material;
	Vertex vertices[4];
	Index indices[2];
	IDirect3DVertexBuffer9 *vertexBuffer;
	IDirect3DIndexBuffer9 *indexBuffer;

	AmbientLight *ambientLight;
	unsigned directionalLightCount;
	DirectionalLight *directionalLights[SPRITE_MAX_DIRECTIONAL_LIGHT];
	unsigned pointLightCount;
	PointLight *pointLights[SPRITE_MAX_POINT_LIGHT];
	unsigned spotLightCount;
	SpotLight *spotLights[SPRITE_MAX_SPOT_LIGHT];

};