#pragma once
#include "enemy.h"

class dragon : public enemy
{
public:
	dragon();
	~dragon();

	//에너미 클래스에 있는  함수 오버라이딩
	virtual void move();
	virtual void attack();
	virtual void enemyAI();
};

