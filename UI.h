#pragma once

#include "Object.h"

class UI : public Object
{
public:
	UI();
	virtual ~UI();

	virtual void UpdateMatrix();

public:
	D3DXVECTOR2 rotationCenter;
	D3DXVECTOR2 scalingCenter;

};