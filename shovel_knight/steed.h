#pragma once
#include "enemy.h"

class steed : public enemy
{
public:
	steed();
	~steed();

	//에너미 클래스에 있는  함수 오버라이딩
	virtual void move();
	virtual void die();
	virtual void enemyAI();
};

