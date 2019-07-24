#include "DXUT.h"
#include "Text.h"
#include "App.h"

Text::Text(Font *font, wstring text) :
	font(font),
	text(text),
	format(DT_LEFT | DT_TOP | DT_NOCLIP),
	alpha(255),
	colorR(255),
	colorG(255),
	colorB(255)
{

}

Text::~Text()
{

}

void Text::Render()
{
	Object::Render();

	app.sprite->SetTransform(&worldMatrix);

	app.sprite->Begin(D3DXSPRITE_ALPHABLEND);

	font->font->DrawText(app.sprite, text.c_str(), -1, NULL, format, D3DCOLOR_ARGB(alpha, colorR, colorG, colorB));

	app.sprite->End();
}