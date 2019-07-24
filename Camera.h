#pragma once

#include "Object.h"
#include "Surface.h"

class Camera : public Object
{
public:
	Camera(Surface *renderTarget);

	virtual void Update();

	virtual void Ready();

public:
	float viewWidth;
	float viewHeight;
	float nearZ;
	float farZ;
	D3DXVECTOR3 eye;
	D3DXVECTOR3 look;
	D3DXVECTOR3 up;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projMatrix;
	Surface *surface;

};