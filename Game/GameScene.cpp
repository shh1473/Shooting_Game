#include "DXUT.h"
#include "GameScene.h"

GameScene::GameScene() :
	camera(new GameCamera()),
	ambientLight(new GameAmbientLight()),
	health(new Text(app.fonts[L"Gulim"], L"체력: 0")),
	kills(new Text(app.fonts[L"Gulim"], L"처치: 0")),
	dead(new Text(app.fonts[L"BigGulim"], L"게임 오버")),
	board(new Board()),
	player(new Player()),
	planet(new Planet()),
	enemySpawner(new EnemySpawner())
{
	AddChild(camera);
	AddChild(ambientLight);
	AddChild(health);
	AddChild(kills);
	AddChild(dead);
	AddChild(board);
	AddChild(planet);
	AddChild(enemySpawner);
	AddChild(player);

	camera->thresholdIntensityViewportInv.x = 0.0f;

	ambientLight->colorIntensity = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	health->translation.x = 0.0f;
	kills->translation.x = 300.0f;
	dead->translation.x = 85.0f;
	dead->translation.y = -500.0f;
	dead->colorG = 0;
	dead->colorB = 0;

	board->ambientLight = ambientLight;
	board->pointLights[0] = planet->reflection;
	board->translation.z = 400.0f;

	player->SetAmbientLight(ambientLight);
	player->SetPointLight(planet->reflection);
	player->camera = camera;
	player->health = health;
	player->kills = kills;
	player->dead = dead;
	player->enemySpawner = enemySpawner;
	player->Shot();

	planet->ambientLight = ambientLight;
	planet->camera = camera;

	enemySpawner->ambientLight = ambientLight;
	enemySpawner->pointLight = planet->reflection;
	enemySpawner->player = player;
}

GameScene::~GameScene()
{
}

void GameScene::Update()
{
	Object::Update();
}

void GameScene::Render()
{
	app.camera = camera;
	app.camera->Ready();

	Object::Render();

	camera->Effect();
}