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

void wizard::die()
{
}

void wizard::enemyAI()
{
	if (_hitCount == 2)
	{
		if (_isSmoke == true) _enemyState = E_SMOKE;
		if (_isSmoke == false) _enemyState = E_DIE;
	}
}
