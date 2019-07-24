#pragma once

#include "UI.h"
#include "Texture.h"

class Image : public UI
{
public:
	Image(Texture *texture);
	~Image();

	virtual void Render();

public:
	bool isEnableRect;
	Texture *texture;
	RECT rect;
	unsigned alpha;
	unsigned colorR;
	unsigned colorG;
	unsigned colorB;

};