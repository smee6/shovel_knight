#pragma once
#include "gameNode.h"
#include "character.h"

class uiManager : public gameNode
{

private:
	


public:
	uiManager();
	~uiManager();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

