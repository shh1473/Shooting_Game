#include "DXUT.h"
#include "Camera.h"

Camera::Camera(Surface *renderTarget) :
	viewWidth(0.0f),
	viewHeight(0.0f),
	nearZ(0.1f),
	farZ(1000.0f),
	eye(0.0f, 0.0f, 0.0f),
	look(0.0f, 0.0f, 0.0f),
	up(0.0f, 1.0f, 0.0f),
	surface(nullptr)
{
	D3DXMatrixIdentity(&viewMatrix);
	D3DXMatrixIdentity(&projMatrix);

	surface = renderTarget;
	viewWidth = static_cast<float>(surface->desc.Width);
	viewHeight = static_cast<float>(surface->desc.Height);
}

void Camera::Update()
{
	Object::Update();

	eye = translation;
	look = translation;

	look.z += 100.0f;

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &look, &up);
	D3DXMatrixOrthoLH(&projMatrix, viewWidth, viewHeight, nearZ, farZ);
}

void Camera::Ready()
{
	DXUTGetD3D9Device()->SetRenderTarget(0, surface->surface);
	DXUTGetD3D9Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);
}