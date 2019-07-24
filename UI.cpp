#include "DXUT.h"
#include "UI.h"

UI::UI() :
	rotationCenter(0.0f, 0.0f),
	scalingCenter(0.0f, 0.0f)
{

}

UI::~UI()
{

}

void UI::UpdateMatrix()
{
	static D3DXVECTOR2 translation2D;
	static D3DXVECTOR2 scaling2D;

	translation2D.x = translation.x;
	translation2D.y = translation.y;

	scaling2D.x = scaling.x;
	scaling2D.y = scaling.y;

	D3DXMatrixTransformation2D(&worldMatrix, &scalingCenter, 0.0f, &scaling2D, &rotationCenter, rotation.z, &translation2D);
}