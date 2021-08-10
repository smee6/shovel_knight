#pragma once
#include "gameNode.h"
#include "character.h"
#include "uiManager.h"
#include "object.h"
#include "mapCamera.h"
#include "enemyManager.h"

class inGameScene : public gameNode
{
	uiManager* _ui;
	character* _character;
	image* _backGround;
	mapCamera* _mapCamera;
	object* _object;
	enemyManager* _enemyManager;

public:
	inGameScene();
	~inGameScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

