#pragma once
#include "enemy.h"

class wizard : public enemy
{
public:
	wizard();
	~wizard();

	void attack();
	void enemyAI();
};

