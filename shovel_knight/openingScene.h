#pragma once
#include "gameNode.h"



class openingScene : public gameNode
{
private:
	int titleNum = 1;
	bool isOption = false;
	int optionNum = 1;
	int sound = 3;
	int bgm = 3;

public:
	openingScene();
	~openingScene();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};