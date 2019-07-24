#pragma once

#include "Common.h"
#include "Math.h"

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void Update();
	virtual void Render();

	virtual void UpdateMatrix();

	void AddChild(Object *child);

	D3DXVECTOR3 GetWorldTranslation();
	D3DXVECTOR3 GetWorldRotation();
	D3DXVECTOR3 GetWorldScaling();

public:
	bool isDestroyed;
	bool isActivated;
	float width;
	float height;
	Rect rect;

	D3DXVECTOR3 translation;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scaling;
	D3DXMATRIX worldMatrix;

	Object *parent;
	vector<Object*> children;

};