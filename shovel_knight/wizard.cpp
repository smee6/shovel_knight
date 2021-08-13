#include "pch.h"
#include "wizard.h"

wizard::wizard()
{
}

wizard::~wizard()
{
}

void wizard::attack()
{
}



void wizard::enemyAI()
{
	enemy::enemyAI();
	
	if (_hitCount == 1)
	{
		if (_isSmoke == true) _enemyState = E_SMOKE;
		if (_isSmoke == false) _enemyState = E_DIE;
	}
}
