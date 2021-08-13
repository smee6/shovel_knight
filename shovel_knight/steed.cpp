#include "pch.h"
#include "steed.h"

steed::steed()
{
}

steed::~steed()
{
}

void steed::move()
{
	if (_enemyDirection == E_LEFT)		//플레이어가 왼쪽에 있을테니 왼쪽으로 돌진
	{
		_x -= 6;
	}
	if (_enemyDirection == E_RIGHT)		//플레이어가 오른쪽에 있을테니 오른쪽으로 돌진
	{
		_x += 6;
	}
}

void steed::die()
{
}

void steed::enemyAI()
{
	
	RECT temp;
	if (IntersectRect(&temp, &_characterRC, &_proveRC))	_enemyState = E_MOVE;
	if (_hitCount == 1)
	{
		if (_isSmoke == true) _enemyState = E_SMOKE;
		if (_isSmoke == false) _enemyState = E_DIE;
	}
}
