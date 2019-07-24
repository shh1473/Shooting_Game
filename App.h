#pragma once

#include <string>

#include "Camera.h"
#include "Timer.h"
#include "Sound.h"
#include "Shader.h"
#include "VertexDeclaration.h"
#include "Material.h"
#include "Surface.h"
#include "Font.h"

class App
{
public:
	App();

	void Initialize();
	void Shutdown();

	void Update();
	void Render();

	bool IsPressed(unsigned char key);
	bool IsPressing(unsigned char key);
	bool IsReleased(unsigned char key);
	bool IsReleasing(unsigned char key);

	void CreateTexture(wstring path, wstring name);
	void CreateSound(wstring path, wstring name);
	void CreateShader(
		wstring path,
		wstring name,
		D3DXMACRO *macros = nullptr);
	void CreateVertexDeclaration(
		wstring name,
		unsigned flag = NULL);
	void CreateMaterial(
		wstring name,
		unsigned widthCount = 1,
		unsigned heightCount = 1,
		wstring diffuseMapName = L"Default",
		wstring specularMapName = L"Default",
		wstring normalMapName = L"Default");
	void CreateSurface(
		wstring name,
		unsigned width = 0,
		unsigned height = 0);
	void CreateFont_(
		wstring name,
		wstring fontName,
		int height,
		unsigned fontWeight,
		BOOL isItalic);

public:
	// 입력
	bool nowKeys[256];
	bool oldKeys[256];
	POINT mousePos;

	// 씬
	Object *nowScene;
	Object *nextScene;

	// 카메라
	Camera *camera;

	// 시간
	bool isSlowMode;
	float deltaTime;
	list<Timer*> timers;

	// 자원
	map<wstring, Texture*> textures;
	map<wstring, Sound*> sounds;
	map<wstring, Shader*> shaders;
	map<wstring, VertexDeclaration*> vertexDeclarations;
	map<wstring, Material*> materials;
	map<wstring, Surface*> surfaces;
	map<wstring, Font*> fonts;

	// 스프라이트
	ID3DXSprite * sprite;

};

extern App app;