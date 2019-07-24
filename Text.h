#pragma once

#include "UI.h"
#include "Font.h"

class Text : public UI
{
public:
	Text(Font *font, wstring text);
	virtual ~Text();

	virtual void Render();

public:
	Font *font;
	wstring text;
	DWORD format;
	unsigned alpha;
	unsigned colorR;
	unsigned colorG;
	unsigned colorB;

};