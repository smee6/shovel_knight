#pragma once
#include "enemy.h"

class steed : public enemy
{
public:
	steed();
	~steed();

	void move();
	void enemyAI();
};

