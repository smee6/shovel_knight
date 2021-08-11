#include "pch.h"
#include "enemy.h"

enemy::enemy()
{
}

enemy::~enemy()
{
}

HRESULT enemy::init()
{
	return S_OK;
}

HRESULT enemy::init(const char* imageName, POINT position, ENEMYDIRECTION enemyDirection)
{
	_enemyState = E_IDLE;
	_enemyDirection = enemyDirection;
	_currentFrameX = _currentFrameY = 0;
	_imageCount  = 0;
	
	_enemyName = imageName;
	_str = " and idle";
	_strSum = _enemyName + _str;

	_imageName = IMAGEMANAGER->findImage(_strSum);

	_x = position.x;
	_y = position.y - _imageName->getFrameHeight();

	_randCount = RND->getFromIntTo(0, 100);
	_moveCount = 0;
	_attackCount = 0;
	_dieCount = 0;
	_hitCount = 0;

	_jumpPower = 5.0f;
	_gravity = 0.16f;

	return S_OK;
}

void enemy::release()
{
}


void enemy::update()
{
	enemyAI();
	enemyFrame();

	_rc = RectMake(_x, _y,
		_imageName->getFrameWidth(), _imageName->getFrameHeight());
	
	if (_enemyName == "steed")
	{
		_proveRC = RectMake(_x - 300, _y - 50,
			_imageName->getFrameWidth() + 600, _imageName->getFrameHeight() + 100);
	}
	if (_enemyName == "dragon")
	{
		_proveRC = RectMake(_x - 300, _y - 200,
			_imageName->getFrameWidth() + 600, _imageName->getFrameHeight() + 200);
	}
}

void enemy::render()
{
	draw();

	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		Rectangle(getMemDC(), _proveRC);
		Rectangle(getMemDC(), _rc);
	}
}

void enemy::draw()
{
		_imageName->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
}

void enemy::enemyFrame()
{
	_imageCount++;

	if (_imageCount % 10 == 0)
	{
		if (_enemyDirection == E_RIGHT)
		{
			if (_imageName->getMaxFrameX() > 2)
			{
				if (_currentFrameX >= _imageName->getMaxFrameX()) _currentFrameX = 0;

				//_imageName->setFrameX(_currentFrameX);
				_currentFrameX++;
				_currentFrameY = 0;
			}
			if (_imageName->getMaxFrameX() == 1)
			{
				//_imageName->setFrameX(_currentFrameX);
				_currentFrameY = 0;

				_currentFrameX == 0 ? _currentFrameX = 1 : _currentFrameX = 0;
			}
			if(_imageName->getMaxFrameX() == 0)
			{
				_currentFrameX = 0;
				_currentFrameY = 0;
			}
		}
		if (_enemyDirection == E_LEFT)
		{
			if (_imageName->getMaxFrameX() > 2 || _imageName->getMaxFrameX() == 0)
			{
				if (_currentFrameX <= 0) _currentFrameX = _imageName->getMaxFrameX();

				//_imageName->setFrameX(_currentFrameX);
				_currentFrameX--;
				_currentFrameY = 1;
				if (_enemyName == "dragon")
				{
					_currentFrameY = 0;
				}
				else
				{
					_currentFrameY = 1;
				}
			}
			if (_imageName->getMaxFrameX() == 1)
			{
				//_imageName->setFrameX(_currentFrameX);
				_currentFrameY = 1;
				
				_currentFrameX == 0 ? _currentFrameX = 1 : _currentFrameX = 0;
				
			}
			if (_imageName->getMaxFrameX() == 0)
			{
				_currentFrameX = 0;
				_currentFrameY = 1;
			}
		}

		_imageCount = 0;
	}

	switch (_enemyState)
	{
	case E_IDLE:
		_str = " and idle";
		_strSum = _enemyName + _str;
		_imageName = IMAGEMANAGER->findImage(_strSum);
		
		break;
	case E_MOVE:
		_str = " and move";
		_strSum = _enemyName + _str;
		_imageName = IMAGEMANAGER->findImage(_strSum);
		move();
		break;
	case E_ATTACK:
		_str = " and attack";
		_strSum = _enemyName + _str;
		_imageName = IMAGEMANAGER->findImage(_strSum);
		attack();
		break;
	case E_DIE:
		_str = " and die";
		_strSum = _enemyName + _str;
		_imageName = IMAGEMANAGER->findImage(_strSum);
		die();
		break;
	}
}

void enemy::move()
{
}

void enemy::attack()
{
}

void enemy::die()
{
	/*
	if (플레이어의 x가 적렉트x보다 크다면)
	{
		_enemyDirection = E_LEFT;
		_x -= 4;
		_y -= _jumpPower;
		_jumpPower -= _gravity;
	}
	if (플레이어의 x가 적렉트x보다 작다면)
	{
		_enemyDirection = E_RIGHT;
		_x += 4;
		_y -= _jumpPower;
		_jumpPower -= _gravity;
	}
	*/
}

void enemy::enemyAI()
{
	/*if (플레이어의 공격에 맞았을 시)

	 {
		_defenseCount = 0;
		_isDefense = true;
		
	 }
	 if(_isDefense == true)
	 {
		_defenseCount ++;
		if( _defenseCount == 50)
		{
			_isDefense = false;
		}
	 }
	*/
}
