#pragma once
#include "gameNode.h"
#include "character.h"
#include "uiManager.h"


class inGameScene : public gameNode
{
	uiManager* _ui;
	character* _character;
	image* _backGround;

public:
	inGameScene();
	~inGameScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

