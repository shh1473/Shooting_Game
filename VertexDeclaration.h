#pragma once

#include "Common.h"

#define VERTEX_DECL_TEXCOORD 0x0001
#define VERTEX_DECL_NORMAL 0x0002
#define VERTEX_DECL_TANGENT_AND_BINORMAL 0x0004

class VertexDeclaration
{
public:
	VertexDeclaration();
	~VertexDeclaration();

public:
	IDirect3DVertexDeclaration9 *vertexDeclaration;

};