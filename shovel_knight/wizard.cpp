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
	if (_hitCount == 2) _enemyState = E_DIE;
}
