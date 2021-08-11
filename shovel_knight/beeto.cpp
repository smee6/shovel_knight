#include "pch.h"
#include "beeto.h"

beeto::beeto()
{
}

beeto::~beeto()
{
}

void beeto::move()		
{
	_moveCount++;
	
	if (_randCount < _moveCount && _moveCount <= 100 + _randCount)
	{
		_enemyDirection = E_LEFT;
		_x -= 1;
	}
	if (_moveCount > 100 + _randCount && _moveCount <= 200 + _randCount)
	{
		_enemyDirection = E_RIGHT;
		_x += 1;
	}
	if (_moveCount > 200 + _randCount)
	{
		_moveCount = _randCount;
	}
}

void beeto::die()
{
}

void beeto::enemyAI()
{
	if (_enemyState == E_IDLE) _enemyState = E_MOVE;
	if (_hitCount == 1) _enemyState = E_DIE;
}
