#include "DXUT.h"
#include "App.h"
#include "EffectCamera.h"

EffectCamera::EffectCamera(Shader * shader, VertexDeclaration * vertexDeclaration, Surface * renderTarget, unsigned renderFlags) :
	Camera(renderTarget),
	renderFlags(renderFlags),
	shader(shader),
	vertexDeclaration(vertexDeclaration),
	thresholdIntensityViewportInv(0.5f, 1.0f, 0.0f, 0.0f),
	colorContrast(1.0f, 1.0f, 1.0f, 0.0f),
	vertexBuffer(nullptr),
	indexBuffer(nullptr)
{
	D3DXMatrixIdentity(&filter);

	for (unsigned i = 0; i < CAMERA_MAX_SOURCEMAP; ++i) { sourceMaps[i] = nullptr; }

	ZeroMemory(vertices, sizeof(Vertex) * 4);
	ZeroMemory(indices, sizeof(Index) * 2);

	DXUTGetD3D9Device()->CreateVertexBuffer(
		sizeof(Vertex) * 4,
		0,
		0,
		D3DPOOL_MANAGED,
		&vertexBuffer,
		nullptr);

	DXUTGetD3D9Device()->CreateIndexBuffer(
		sizeof(Index) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&indexBuffer,
		nullptr);

	vertices[0].position = { -1.0f, 1.0f, 0.0f };
	vertices[0].texCoord = { 0.0f, 0.0f };
	vertices[1].position = { 1.0f, 1.0f, 0.0f };
	vertices[1].texCoord = { 1.0f, 0.0f };
	vertices[2].position = { 1.0f, -1.0f, 0.0f };
	vertices[2].texCoord = { 1.0f, 1.0f };
	vertices[3].position = { -1.0f, -1.0f, 0.0f };
	vertices[3].texCoord = { 0.0f, 1.0f };

	indices[0] = { 0, 1, 2 };
	indices[1] = { 0, 2, 3 };

	void * tmpVerticesData;
	vertexBuffer->Lock(0, sizeof(Vertex) * 4, &tmpVerticesData, 0);
	CopyMemory(tmpVerticesData, vertices, sizeof(Vertex) * 4);
	vertexBuffer->Unlock();

	void * tmpIndicesData;
	indexBuffer->Lock(0, sizeof(Index) * 2, &tmpIndicesData, 0);
	CopyMemory(tmpIndicesData, indices, sizeof(Index) * 2);
	indexBuffer->Unlock();
}

EffectCamera::~EffectCamera()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void EffectCamera::Ready()
{
	DXUTGetD3D9Device()->SetRenderTarget(0, sourceMaps[0]->surface);
	DXUTGetD3D9Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);
}

void EffectCamera::Effect()
{
	shader->shader->SetTexture(shader->shader->GetParameterByName(NULL, "SourceMap0"), sourceMaps[0]->texture);

	shader->shader->SetVector(shader->shader->GetParameterByName(NULL, "ThresholdIntensityViewportInv"), &thresholdIntensityViewportInv);

	if (renderFlags & CAMERA_COLOR_CONTRAST)
	{
		shader->shader->SetVector(shader->shader->GetParameterByName(NULL, "ColorContrast"), &colorContrast);
	}

	if (renderFlags & CAMERA_FILTER)
	{
		shader->shader->SetMatrix(shader->shader->GetParameterByName(NULL, "Filter"), &filter);
	}

	if (renderFlags & CAMERA_BLUR || renderFlags & CAMERA_BLOOM)
	{
		shader->shader->SetTexture(shader->shader->GetParameterByName(NULL, "SourceMap1"), sourceMaps[1]->texture);
		shader->shader->SetTexture(shader->shader->GetParameterByName(NULL, "SourceMap2"), sourceMaps[2]->texture);
		shader->shader->SetTexture(shader->shader->GetParameterByName(NULL, "SourceMap3"), sourceMaps[3]->texture);
	}

	shader->shader->Begin(nullptr, NULL);

	DXUTGetD3D9Device()->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	DXUTGetD3D9Device()->SetIndices(indexBuffer);
	DXUTGetD3D9Device()->SetVertexDeclaration(vertexDeclaration->vertexDeclaration);

	if (renderFlags & CAMERA_BLOOM)
	{
		shader->shader->BeginPass(0);

		DXUTGetD3D9Device()->SetRenderTarget(0, sourceMaps[1]->surface);
		DXUTGetD3D9Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);

		DXUTGetD3D9Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		shader->shader->EndPass();
	}

	if (renderFlags & CAMERA_BLUR || renderFlags & CAMERA_BLOOM)
	{
		if (renderFlags & CAMERA_BLUR && !(renderFlags & CAMERA_BLOOM))
		{
			DXUTGetD3D9Device()->StretchRect(sourceMaps[0]->surface, NULL, sourceMaps[1]->surface, NULL, D3DTEXF_NONE);
		}

		shader->shader->BeginPass(1);

		DXUTGetD3D9Device()->SetRenderTarget(0, sourceMaps[2]->surface);
		DXUTGetD3D9Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);

		DXUTGetD3D9Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		shader->shader->EndPass();

		shader->shader->BeginPass(2);

		DXUTGetD3D9Device()->SetRenderTarget(0, sourceMaps[3]->surface);
		DXUTGetD3D9Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);

		DXUTGetD3D9Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		shader->shader->EndPass();
	}

	shader->shader->BeginPass(3);
	
	DXUTGetD3D9Device()->SetRenderTarget(0, surface->surface);
	DXUTGetD3D9Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);

	DXUTGetD3D9Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	shader->shader->EndPass();

	shader->shader->End();

	DXUTGetD3D9Device()->StretchRect(surface->surface, NULL, app.surfaces[L"BackBuffer"]->surface, NULL, D3DTEXF_NONE);
}