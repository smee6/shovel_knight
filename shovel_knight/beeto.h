#pragma once
#include "enemy.h"

class beeto : public enemy
{
public:
	beeto();
	~beeto();

	void move();		//에너미 클래스에 있는 움직임 함수 오버라이딩
	void enemyAI();
};

