#pragma once
#include "gameNode.h"


class gameOver : public gameNode
{
private:
	int cnt = 0 ;
public:
	gameOver();
	~gameOver();
	
	HRESULT init();
	void release();
	void update();
	void render();
};