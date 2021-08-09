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

HRESULT enemy::init(char imageName, POINT position, ENEMYDIRECTION enemyDirection)
{
	_enemyState = E_MOVE;
	_enemyDirection = enemyDirection;
	_currentFrameX = _currentFrameY = 0;
	_count  = 0;
	//_imageName = IMAGEMANAGER->findImage(imageName);
	_rc = RectMakeCenter(position.x, position.y,
		_imageName->getFrameWidth(), _imageName->getFrameHeight());
	_enemyName = imageName;
	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
	enemyAI();
	enemyFrame();
}

void enemy::render()
{
	draw();
}

void enemy::draw()
{
	if (_enemyDirection == E_RIGHT)
	{
		_imageName->frameRender(getMemDC(), _rc.left, _rc.top
			, _currentFrameX, 0);
	}
	if (_enemyDirection == E_LEFT)
	{
		_imageName->frameRender(getMemDC(), _rc.left, _rc.top
			, _currentFrameX, 1);
	}
}

void enemy::enemyFrame()
{
	_count++;

	if (_count % 5 == 0)
	{
		if (_enemyDirection == E_RIGHT)
		{
			if (_currentFrameX >= _imageName->getMaxFrameX()) _currentFrameX = 0;

			_imageName->setFrameX(_currentFrameX);
			_currentFrameX++;
			_currentFrameY = 0;

		}
		if (_enemyDirection == E_LEFT)
		{
			if (_currentFrameX < 0) _currentFrameX = _imageName->getMaxFrameX();

			_imageName->setFrameX(_currentFrameX);
			_currentFrameX--;
			_currentFrameY = 1;
		}

		_count = 0;
	}
	
	switch (_enemyState)
	{
	case E_IDLE:
		

		break;
	case E_MOVE:
		/*_enemyName;
		char str1[128] = " and move";
		strcat_s(_enemyName, str1);
		_imageName = IMAGEMANAGER->findImage(_enemyName);
		move();
		*/
		break;
	case E_ATTACK:
		

		break;
	case E_DIE:
		

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
}

void enemy::enemyAI()
{
}
