#include "pch.h"
#include "object.h"
#include "character.h"
#include "mapCamera.h"

object::object()
{
}

object::~object()
{
}


HRESULT object::init()
{
	//필요한것
	//1순위 움직이는 발판(좌우, 상하), 흙(큰, 작), 보석류(광석), 먹을것(사과, 치킨) - 체력회복
	//2순위 물방울(용이 쏘는것, 바닥에서 올라오는 것), 파이어볼(유저, 몹),
	//물방울은 - 최초에 밑에서 상승했다가 일정범위에 도달하면 좌우로 조금씩 움직이는?






	platformSetting();
	sandBlockSetting();
	potionSetting();
	jewelSetting();
	bubbleSetting();
	fireBallSetting();

	return S_OK;
}

void object::release()
{
}

void object::update()
{

	gravity();
	collision();
	imgFrameSetting();
	objectMove();
}

void object::collision()
{

}

void object::gravity()
{
}

void object::imgSetting()
{
}

void object::imgFrameSetting()
{
}

void object::objectMove()
{

	//발판의 불값을 처리해주는 for문
	for (int i = 0; i < PLATFORMMAX; i++)
	{
		if (_platform[i].index == 1)
		{
			if (_platform[i].min > _platform[i].x)
			{
				_platform[i].isDirection = true;
			}
			else if (_platform[i].max < _platform[i].x)
			{
				_platform[i].isDirection = false;
			}
		}

		if (_platform[i].index == 2)
		{
			if (_platform[i].min > _platform[i].y)
			{
				_platform[i].isDirection = true;
			}
			else if (_platform[i].max < _platform[i].y)
			{
				_platform[i].isDirection = false;
			}
		}


	}


	for (int i = 0; i < PLATFORMMAX; i++)
	{
		//좌우발판의 움직임을 처리해주는 if문
		if (_platform[i].index == 1)
		{
			if (_platform[i].isDirection == true)
			{
				_platform[i].x += _platform[i].speed;
			}
			else
			{
				_platform[i].x -= _platform[i].speed;
			}
		}

		//상하발판의 움직임을 처리해주는 if문
		if (_platform[i].index == 2)
		{
			if (_platform[i].isDirection == true)
			{
				_platform[i].y += _platform[i].speed;
			}
			else
			{
				_platform[i].y -= _platform[i].speed;
			}
		}
	}



}

void object::platformSetting()
{
	//발판의 x, y값 설정 - 좌표값

	//좌우
	_platform[0].x = 100;
	_platform[0].y = 100;
	_platform[0].min = 50;     //수치 수정은 추후에 체크하면서
	_platform[0].max = 150;

	_platform[1].x = 200;
	_platform[1].y = 200;
	_platform[1].min = 50;
	_platform[1].max = 150;

	_platform[2].x = 300;
	_platform[2].y = 300;
	_platform[2].min = 50;
	_platform[2].max = 150;

	//상하
	_platform[3].x = 100;
	_platform[3].y = 100;

	_platform[4].x = 100;
	_platform[4].y = 100;

	//움직이는 발판 좌우 3개 - 0, 1, 2
	for (int i = 0; i < 3; i++)
	{
		_platform[i].isDirection = false;
		_platform[i].type = 1;
		_platform[i].index = 1;
		_platform[i].speed = SPEED;
		_platform[i].rc = RectMakeCenter(_platform[i].x, _platform[i].y, 100, 50);
		//_platform[i].objectImage = IMAGEMANAGER->addImage("목숨", "image/life.bmp", 50, 50, true, RGB(255, 0, 255));
	}

	//움직이는 발판 상하 2개 - 3, 4
	for (int i = 3; i < 5; i++)
	{
		_platform[i].isDirection = false;
		_platform[i].type = 1;
		_platform[i].index = 2;
		_platform[i].speed = SPEED;
		_platform[i].rc = RectMakeCenter(_platform[i].x, _platform[i].y, 100, 50);
		//_platform[i].objectImage = IMAGEMANAGER->addImage("목숨", "image/life.bmp", 50, 50, true, RGB(255, 0, 255));
	}

}

void object::sandBlockSetting()
{
	//발판처럼 느낌 비슷하게 가면 될거같다~

}

void object::potionSetting()
{
}

void object::jewelSetting()
{
}

void object::bubbleSetting()
{
}

void object::fireBallSetting()
{
}

void object::render()
{
}
