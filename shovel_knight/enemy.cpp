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
	//_imageName = IMAGEMANAGER->findImage("beeto and move");
	_x = position.x;
	_y = position.y;
	_enemyName = imageName;
	_cast = _enemyName;

	_randCount = RND->getFromIntTo(0, 100);
	_moveCount = 0;
	_attackCount = 0;
	_diecount = 0;

	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
	enemyAI();
	enemyFrame();
	_rc = RectMakeCenter(_x, _y,
		_imageName->getFrameWidth(), _imageName->getFrameHeight());
}

void enemy::render()
{
	draw();
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

				_imageName->setFrameX(_currentFrameX);
				_currentFrameX++;
				_currentFrameY = 0;
			}
			if (_imageName->getMaxFrameX() <= 2)
			{
				//_currentFrameX = 1 ? 0 : 1;
			}
		}
		if (_enemyDirection == E_LEFT)
		{
			if (_imageName->getMaxFrameX() > 2)
			{
				if (_currentFrameX <= 0) _currentFrameX = _imageName->getMaxFrameX();

				_imageName->setFrameX(_currentFrameX);
				_currentFrameX--;
				if (_enemyName == "dragon")
				{
					_currentFrameY = 0;
				}
				else
				{
					_currentFrameY = 1;
				}
			}
			if (_imageName->getMaxFrameX() <= 2)
			{

			}
		}

		_imageCount = 0;
	}

	switch (_enemyState)
	{
	case E_IDLE:
		_str = " and idle";
		_strsum = _cast + _str;
		_imageName = IMAGEMANAGER->findImage(_strsum);
		
		break;
	case E_MOVE:
		_str = " and move";
		_strsum = _cast + _str;
		_imageName = IMAGEMANAGER->findImage(_strsum);
		move();
		break;
	case E_ATTACK:
		_str = " and attack";
		_strsum = _cast + _str;
		_imageName = IMAGEMANAGER->findImage(_strsum);
		attack();
		break;
	case E_DIE:
		_str = " and die";
		_strsum = _cast + _str;
		_imageName = IMAGEMANAGER->findImage(_strsum);
		die();
		break;
	}
}

void enemy::move()
{
	_x -= 1000;
}

void enemy::attack()
{
}

void enemy::die()
{
}

void enemy::enemyAI()
{
}
