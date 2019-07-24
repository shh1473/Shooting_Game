#include "DXUT.h"
#include "Image.h"
#include "App.h"

Image::Image(Texture *texture) :
	isEnableRect(false),
	texture(texture),
	rect({ 0, 0, 0, 0 }),
	alpha(255),
	colorR(255),
	colorG(255),
	colorB(255)
{

}

Image::~Image()
{

}

void Image::Render()
{
	Object::Render();

	app.sprite->SetTransform(&worldMatrix);

	app.sprite->Begin(D3DXSPRITE_ALPHABLEND);

	app.sprite->Draw(texture->texture, ((isEnableRect) ? &rect : nullptr), nullptr, nullptr, D3DCOLOR_ARGB(alpha, colorR, colorG, colorB));

	app.sprite->End();
}