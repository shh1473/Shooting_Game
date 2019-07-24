#include "DXUT.h"
#include "VertexDeclaration.h"

VertexDeclaration::VertexDeclaration() :
	vertexDeclaration(nullptr)
{
	
}

VertexDeclaration::~VertexDeclaration()
{
	SAFE_RELEASE(vertexDeclaration);
}