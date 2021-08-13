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

	//시간에 따른 beeto 움직임 변화
	if (_randCount < _moveCount && _moveCount <= 100 + _randCount)	//좌
	{
		_enemyDirection = E_LEFT;
		_x -= 1;
	}
	if (_moveCount > 100 + _randCount && _moveCount <= 200 + _randCount)	//우
	{
		_enemyDirection = E_RIGHT;
		_x += 1;
	}
	if (_moveCount > 200 + _randCount)
	{
		_moveCount = _randCount;
	}
}


void beeto::enemyAI()
{
	enemy::enemyAI();
	
	if (_enemyState == E_IDLE) _enemyState = E_MOVE;	//기본상태가 움직임 상태
	
	if (_hitCount == 1)
	{
		if (_isSmoke == true) _enemyState = E_SMOKE;
		if (_isSmoke == false) _enemyState = E_DIE;
	}
	
}
