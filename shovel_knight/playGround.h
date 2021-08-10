#pragma once
#include "gameNode.h"
#include "inGameScene.h"
#include "loadingScene.h"
#include "character.h"
#include "object.h"
#include "mapCamera.h"
#include "openingScene.h"
#include "enemyManager.h"

class playGround : public gameNode
{
private:
	
	character* _character;
	image* _backGround;

	object* _object;

	mapCamera* _mapCamera;

	enemyManager* _enemyManager;

public:
	playGround();
	~playGround();

	virtual HRESULT init();			//초기화 함수
	virtual void release();			//메모리 해제 함슈
	virtual void update();			//연산하는 함수
	virtual void render();			//그리기 함수

	
};

