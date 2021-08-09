#pragma once
#include "enemy.h"

class dragon : public enemy
{
public:
	dragon();
	~dragon();

	void move();
	void attack();
	void enemyAI();
};

