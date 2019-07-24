#include "DXUT.h"
#include "Sprite.h"
#include "App.h"

Sprite::Sprite(Shader *shader, VertexDeclaration *vertexDeclaration, Material *material, unsigned renderFlags) :
	renderFlags(renderFlags),
	widthFrameIndex(0),
	heightFrameIndex(0),
	color(1.0f, 1.0f, 1.0f, 1.0f),
	shader(shader),
	vertexDeclaration(vertexDeclaration),
	material(material),
	vertexBuffer(nullptr),
	indexBuffer(nullptr),
	ambientLight(nullptr),
	directionalLightCount(0),
	pointLightCount(0),
	spotLightCount(0)
{
	for (unsigned i = 0; i < SPRITE_MAX_DIRECTIONAL_LIGHT; ++i) { directionalLights[i] = nullptr; }
	for (unsigned i = 0; i < SPRITE_MAX_POINT_LIGHT; ++i) { pointLights[i] = nullptr; }
	for (unsigned i = 0; i < SPRITE_MAX_SPOT_LIGHT; ++i) { spotLights[i] = nullptr; }

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

	vertices[0].position = { material->width * -0.5f, material->height * 0.5f, 0.0f };
	vertices[0].texCoord = { material->widthRatioPerFrame * widthFrameIndex, material->heightRatioPerFrame * heightFrameIndex };
	vertices[1].position = { material->width * 0.5f, material->height * 0.5f, 0.0f };
	vertices[1].texCoord = { material->widthRatioPerFrame * (widthFrameIndex + 1), material->heightRatioPerFrame * heightFrameIndex };
	vertices[2].position = { material->width * 0.5f, material->height * -0.5f, 0.0f };
	vertices[2].texCoord = { material->widthRatioPerFrame * (widthFrameIndex + 1), material->heightRatioPerFrame * (heightFrameIndex + 1) };
	vertices[3].position = { material->width * -0.5f, material->height * -0.5f, 0.0f };
	vertices[3].texCoord = { material->widthRatioPerFrame * widthFrameIndex, material->heightRatioPerFrame * (heightFrameIndex + 1) };

	for (unsigned i = 0; i < 4; ++i)
	{
		vertices[i].normal = { 0.0f, 0.0f, -1.0f };
		vertices[i].tangent = { 1.0f, 0.0f, 0.0f };
		vertices[i].binormal = { 0.0f, 1.0f, 0.0f };
	}

	indices[0] = { 0, 1, 2 };
	indices[1] = { 0, 2, 3 };

	void *tmpVerticesData;
	vertexBuffer->Lock(0, sizeof(Vertex) * 4, &tmpVerticesData, 0);
	CopyMemory(tmpVerticesData, vertices, sizeof(Vertex) * 4);
	vertexBuffer->Unlock();

	void *tmpIndicesData;
	indexBuffer->Lock(0, sizeof(Index) * 2, &tmpIndicesData, 0);
	CopyMemory(tmpIndicesData, indices, sizeof(Index) * 2);
	indexBuffer->Unlock();
}

Sprite::~Sprite()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Sprite::Render()
{
	Object::Render();

	static D3DXMATRIX WVPMatrix;

	D3DXMatrixMultiply(&WVPMatrix, &worldMatrix, &app.camera->viewMatrix);
	D3DXMatrixMultiply(&WVPMatrix, &WVPMatrix, &app.camera->projMatrix);

	shader->shader->SetMatrix(shader->shader->GetParameterByName(NULL, "WVPMatrix"), &WVPMatrix);
	shader->shader->SetVector(shader->shader->GetParameterByName(NULL, "Color"), &color);

	if (renderFlags & SPRITE_TINT)
	{
		shader->shader->SetVector(shader->shader->GetParameterByName(NULL, "Tint"), &material->tint);
	}

	if (renderFlags & SPRITE_DIFFUSE_MAP)
	{
		shader->shader->SetTexture(shader->shader->GetParameterByName(NULL, "DiffuseMap"), material->diffuseMap->texture);
	}

	if (renderFlags & SPRITE_SPECULAR_MAP)
	{
		shader->shader->SetTexture(shader->shader->GetParameterByName(NULL, "SpecularMap"), material->specularMap->texture);
	}

	if (renderFlags & SPRITE_NORMAL_MAP)
	{
		shader->shader->SetTexture(shader->shader->GetParameterByName(NULL, "NormalMap"), material->normalMap->texture);
	}

	if (renderFlags & SPRITE_AMBIENT_LIGHT ||
		renderFlags & SPRITE_DIRECTIONAL_LIGHT ||
		renderFlags & SPRITE_POINT_LIGHT ||
		renderFlags & SPRITE_SPOT_LIGHT)
	{
		shader->shader->SetMatrix(shader->shader->GetParameterByName(NULL, "WorldMatrix"), &worldMatrix);
	}

	if (renderFlags & SPRITE_AMBIENT_LIGHT)
	{
		static AmbientLightData ambientLightData;

		if (ambientLight)
		{
			ambientLightData.colorIntensity = ambientLight->colorIntensity;
		}
		else
		{
			ZeroMemory(&ambientLightData, sizeof(AmbientLightData));
		}

		shader->shader->SetValue(shader->shader->GetParameterByName(NULL, "AmbLight"), &ambientLightData, sizeof(AmbientLightData));
	}

	if (renderFlags & SPRITE_DIRECTIONAL_LIGHT)
	{
		static DirectionalLightData directionalLightData[SPRITE_MAX_DIRECTIONAL_LIGHT];

		for (unsigned i = 0; i < directionalLightCount; ++i)
		{
			if (directionalLights[i])
			{
				directionalLightData[i].colorIntensity = directionalLights[i]->colorIntensity;
				directionalLightData[i].direction = directionalLights[i]->direction;
			}
			else
			{
				ZeroMemory(&directionalLightData[i], sizeof(DirectionalLightData));
				directionalLightData[i].direction.z = 1.0f;
			}
		}

		shader->shader->SetValue(shader->shader->GetParameterByName(NULL, "DirLights"), directionalLightData, sizeof(DirectionalLightData) * directionalLightCount);
	}

	if (renderFlags & SPRITE_POINT_LIGHT)
	{
		static PointLightData pointLightData[SPRITE_MAX_POINT_LIGHT];

		for (unsigned i = 0; i < pointLightCount; ++i)
		{
			if (pointLights[i])
			{
				static D3DXVECTOR3 worldTranslation;

				worldTranslation = pointLights[i]->GetWorldTranslation();

				pointLightData[i].colorIntensity = pointLights[i]->colorIntensity;
				pointLightData[i].translationRadius.x = worldTranslation.x;
				pointLightData[i].translationRadius.y = worldTranslation.y;
				pointLightData[i].translationRadius.z = worldTranslation.z;
				pointLightData[i].translationRadius.w = pointLights[i]->radius;
			}
			else
			{
				ZeroMemory(&pointLightData[i], sizeof(PointLightData));
				pointLightData[i].translationRadius.w = 1.0f;
			}
		}

		shader->shader->SetValue(shader->shader->GetParameterByName(NULL, "PntLights"), pointLightData, sizeof(PointLightData) * pointLightCount);
	}

	if (renderFlags & SPRITE_SPOT_LIGHT)
	{
		static SpotLightData spotLightData[SPRITE_MAX_SPOT_LIGHT];

		for (unsigned i = 0; i < spotLightCount; ++i)
		{
			if (spotLights[i])
			{
				static D3DXVECTOR3 worldTranslation;

				worldTranslation = spotLights[i]->GetWorldTranslation();

				spotLightData[i].colorIntensity = spotLights[i]->colorIntensity;
				spotLightData[i].direction = spotLights[i]->direction;
				spotLightData[i].coneAngle.x = cos(D3DXToRadian(spotLights[i]->innerAngle));
				spotLightData[i].coneAngle.y = cos(D3DXToRadian(spotLights[i]->outerAngle));
				spotLightData[i].translationRadius.x = worldTranslation.x;
				spotLightData[i].translationRadius.y = worldTranslation.y;
				spotLightData[i].translationRadius.z = worldTranslation.z;
				spotLightData[i].translationRadius.w = spotLights[i]->radius;
			}
			else
			{
				ZeroMemory(&spotLightData[i], sizeof(SpotLightData));
				spotLightData[i].direction.z = 1.0f;
				spotLightData[i].coneAngle.x = 1.0f;
				spotLightData[i].translationRadius.w = 1.0f;
			}
		}

		shader->shader->SetValue(shader->shader->GetParameterByName(NULL, "SptLights"), spotLightData, sizeof(SpotLightData) * spotLightCount);
	}

	if (renderFlags & SPRITE_SPECULAR)
	{
		static D3DXVECTOR4 cameraTranslataion(0.0f, 0.0f, 0.0f, 1.0f);

		cameraTranslataion.x = app.camera->translation.x;
		cameraTranslataion.y = app.camera->translation.y;
		cameraTranslataion.z = app.camera->translation.z;

		shader->shader->SetVector(shader->shader->GetParameterByName(NULL, "CameraTranslation"), &cameraTranslataion);
		shader->shader->SetFloat(shader->shader->GetParameterByName(NULL, "SpecularIntensity"), material->specularIntensity);
		shader->shader->SetFloat(shader->shader->GetParameterByName(NULL, "SpecularPower"), material->specularPower);
	}

	shader->shader->Begin(nullptr, NULL);
	shader->shader->BeginPass(0);

	DXUTGetD3D9Device()->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	DXUTGetD3D9Device()->SetIndices(indexBuffer);
	DXUTGetD3D9Device()->SetVertexDeclaration(vertexDeclaration->vertexDeclaration);

	DXUTGetD3D9Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	shader->shader->EndPass();
	shader->shader->End();
}

void Sprite::ChangeFrameIndex(unsigned i, unsigned j)
{
	widthFrameIndex = j;
	heightFrameIndex = i;

	vertices[0].texCoord = { material->widthRatioPerFrame * widthFrameIndex, material->heightRatioPerFrame * heightFrameIndex };
	vertices[1].texCoord = { material->widthRatioPerFrame * (widthFrameIndex + 1), material->heightRatioPerFrame * heightFrameIndex };
	vertices[2].texCoord = { material->widthRatioPerFrame * (widthFrameIndex + 1), material->heightRatioPerFrame * (heightFrameIndex + 1) };
	vertices[3].texCoord = { material->widthRatioPerFrame * widthFrameIndex, material->heightRatioPerFrame * (heightFrameIndex + 1) };

	void *tmpVerticesData;
	vertexBuffer->Lock(0, sizeof(Vertex) * 4, &tmpVerticesData, 0);
	CopyMemory(tmpVerticesData, vertices, sizeof(Vertex) * 4);
	vertexBuffer->Unlock();
}