#include "DXUT.h"
#include "Board.h"

Board::Board() :
	Sprite(
		app.shaders[L"BackgroundSprite"],
		app.vertexDeclarations[L"Normal"],
		app.materials[L"Board"],
		SPRITE_DIFFUSE_MAP |
		SPRITE_SPECULAR_MAP |
		SPRITE_AMBIENT_LIGHT |
		SPRITE_POINT_LIGHT |
		SPRITE_SPECULAR)
{
	pointLightCount = 1;
}

Board::~Board()
{

}