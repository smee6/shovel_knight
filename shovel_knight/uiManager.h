#pragma once
#include "gameNode.h"
#include "character.h"

class uiManager : public gameNode
{

private:
	bool goatShopOpen;
	int	 goatCnt = 1;

	bool fireShopOpen;
	int	 fireCnt = 1;
	

public:
	uiManager();
	~uiManager();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void goatshop();

	void goatshopOn();
	void goatshopOff();

	void fireshop();

	void fireshopOn();
	void fireshopOff();
};

