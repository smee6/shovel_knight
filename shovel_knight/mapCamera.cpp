#include "pch.h"
#include "mapCamera.h"
#include "character.h"
#include "object.h"
#include "enemyManager.h"

mapCamera::mapCamera()
{
}

mapCamera::~mapCamera()
{
}


//초기화는 여기에다 해라!!!
HRESULT mapCamera::init()
{
	gameNode::init();

	// 배경
	_background = IMAGEMANAGER->addImage("배경", "image/map_black.bmp", 23000, 4500, true, RGB(255, 0, 255));
	_background_magenta = IMAGEMANAGER->addImage("배경마젠타", "image/map_magenta.bmp", 23000, 4500, false, RGB(255, 0, 255));

	// 카메라
	//_camera.left = _character->getCharacterX() - (WIDTH / 2);
	//_camera.right = _character->getCharacterX() + (WIDTH / 2);
	// 카메라 중간s

	_camera.top = 200;
	_camera.bottom = WINSIZEY - 200;

	_camX = 0;
	_camY = -2200;

	_mapCountX = 1;
	_mapCountY = 0;

	_camFollowX = true;
	_camFollowY = true;

	return S_OK;
}

//메모리 해제는 여기다 해라!!!!
void mapCamera::release()
{
	gameNode::release();


}


//연산처리는 여기다가!
void mapCamera::update()
{
	gameNode::update();

	_camera.left = _character->getCharacterX() - (WIDTH / 2);
	_camera.right = _character->getCharacterX() + (WIDTH / 2);

	CameraMove(_camera);

	

}

//여기다 그려줘라!!!
void mapCamera::render()
{
	_background->render(getMemDC(), _camX, _camY);

	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		_background_magenta->render(getMemDC(), _camX, _camY);
		Rectangle(getMemDC(), _camera);

	}

	char str[128];

	sprintf_s(str, "cameraleft : %d", _camera.left);
	TextOut(getMemDC(), 10, 70, str, strlen(str));

	sprintf_s(str, "camX : %d", _camX);
	TextOut(getMemDC(), 10, 90, str, strlen(str));
	sprintf_s(str, "camY : %d", _camY);
	TextOut(getMemDC(), 100, 90, str, strlen(str));

	sprintf_s(str, "_TB : %d", _TB);
	TextOut(getMemDC(), 100, 300, str, strlen(str));
}

void mapCamera::SetCamera(RECT& camera, int left, int top, int width, int height)
{
	camera.left = left;
	camera.right = _camera.left + width;


	camera.top = top;
	camera.bottom = _camera.top + height;
}


void mapCamera::CameraMove(RECT& camera)
{
	//CAMERAMANAGER->SetCamera(_camera, 200, 200, 400, 400);
	//CAMERAMANAGER->MoveCamera(_player.rc, _background, _player.speed, _camX, _camY);

	// 카메라 체크 상자 설정
	SetCamera(camera, (WINSIZEX / 2) - (WIDTH / 2), 200, WIDTH, 400);

	// camera 상자 안에서만 플레이어가 직접 이동(혹은 카메라 이동이 불가능할 경우)
	if (_background->getHeight() <= WINSIZEY - _camY || _camY == 0 || _character->getCharacterRect().bottom < camera.bottom || _character->getCharacterRect().top > camera.top)
	{
		//_character->setCharacterY(_character->getCharacterY() - _character->getJumpPower());
		//_character->setJumpPower(_character->getJumpPower() - _character->getGravity());
	}

	// 배경화면의 왼쪽 끝이 윈도우 창의 왼쪽이랑 같아지면 이동 정지
	// 배경화면의 오른쪽 끝이 윈도우 창의 오른쪽이랑 같아지면 이동 정지
	
	//x값 이동
	if (_camX >= 0 || _camX >= -5050)
	{
	}
	else
	{
		_character->setSpeed(SPEED);
		_camFollowX = false;
	}

	//y값 이동
	/*if (_camY <= 0 || _camY < -2139)
	{
	}
	else
	{
		_character->setSpeed(SPEED);
		_camFollowY = false;
	}*/



	//if (_camY >= -1495) // 11일 학원가서 까먹지말고 이부분 수정할것
	//{
	//	_character->setSpeed(SPEED);
	//	_camY = -1480;
	//	_camFollowY = false;
	//}


	// 1. 사각형을 이용하는 방법
	// 일정 범위의 사각형 rc와 캐릭터의 rc의 같은 요소(left, right, top, bottom)의 값이 같아지면
	// 해당 방향으로 카메라 이동

	//if (_player.x - _camX == -3840) _camFollow = false;

	// 카메라 왼쪽으로 이동 (배경은 오른쪽으로)
	if (_character->getState() != ATTACK && _character->getState() != SKILL)
	{
		if (_camFollowX)
		{
			if (_character->getCharacterX() <= camera.left && KEYMANAGER->isStayKeyDown('A'))
			{
				if (_camX < 0)
				{
					_camX += SPEED;
					_character->setSpeed(0);
					_character->setCharacterX(_camera.left);
				}
			}
			// 카메라 오른쪽으로 이동 (배경은 왼쪽으로) _player.x >= camera.right - 40 && 
			else if (_character->getCharacterX() >= camera.right && KEYMANAGER->isStayKeyDown('D'))
			{
				if (_camX + _background->getWidth() >= WINSIZEX)
				{
					_camX -= SPEED;
					_character->setSpeed(0);
					_character->setCharacterX(_camera.right);
				}

			}
			else
			{
				_character->setSpeed(SPEED);
			}
		}
	}



	if (_character->getCharacterX() - _camX >= WINSIZEX * _mapCountX - 160)
	{
		_mapCountX++;
		_isCamMove = true;
		_LR = 1;

	}
	else if (_character->getCharacterX() - _camX < WINSIZEX * (_mapCountX - 1) - 160)
	{
		_mapCountX--;
		_isCamMove = true;
		_LR = 0;
	}
	else if (_camX == -(WINSIZEX * (_mapCountX - 1)) + 160) //맵이 넘어가다가 일정좌표값에서 캠무브가 꺼지는부분
	{
		_isCamMove = false;

	}


	if (!_camFollowX)
	{
		if (_LR && _isCamMove)
		{
			_camX -= SPEED;
			_character->setSpeed(0);
			_character->setCharacterX(_character->getCharacterX() - SPEED);
			//_player.x -= SPEED;
		}
		else if (!_LR && _isCamMove)
		{
			_camX += SPEED;
			_character->setSpeed(0);
			_character->setCharacterX(_character->getCharacterX() + SPEED);
			//_player.x += SPEED;
		}
		else
		{
			_character->setSpeed(SPEED);
		}
	}

	//카메라 위로 이동
	// camfollowy는 init에서 true 상태임
	// 필수!!! 캐릭터 살짝 움직이기 - 안하면 맵 무지성으로 길을잃음

	// 0보다 작을 때 바텀이
	if (_character->getCharacterRect().bottom < 0)
	{

		_camFollowY = false;
		temp = _camY;	// 한번만 받아온다 (temp에 저장)
		_TB = 0;
	}
	// 탑이 WINSIZEY보다 클때
	if (_character->getCharacterRect().top > WINSIZEY)
	{
		_camFollowY = false;
		temp = _camY;	// 한번만 받아온다 (temp에 저장)
		_TB = 1;
	}

	if (!_camFollowY)
	{
		if (_TB == 0)
		{
			// 움직이는 모션
			_camY += SPEED;
			_character->setJumpPower(0);
			_character->setCharacterY(_character->getCharacterY() + SPEED);

			// _camY가 temp + WINSIZEY 보다 커지면 멈춰라
			if (_camY > temp + WINSIZEY) _camFollowY = true;
		}
		else
		{
			// 움직이는 모션
			_camY -= SPEED;
			_character->setJumpPower(0);
			_character->setCharacterY(_character->getCharacterY() - SPEED);

			// _camY가 temp - WINSIZEY 보다 작아지면 멈춰라
			if (_camY < temp -  WINSIZEY) _camFollowY = true;
		}
		
		
	}
	

	/*if (!_camFollowY && _TB == 0) //false 상태
	{
		_camY += SPEED;
		_character->setJumpPower(0);
		_character->setCharacterY(_character->getCharacterY() + SPEED);

		if (_camY > -1500 + ((_mapCountY - 1) * WINSIZEY)) _camFollowY = true;
	}
	else
	{
		if (_character->getCharacterY() >= camera.bottom)
		{
			if (2850 - _mapCountY * WINSIZEY >= WINSIZEY - _camY)
			{
				_camY += _character->getJumpPower();
				_character->setCharacterY(camera.bottom);
			}
		}
	}
	if (_character->getCharacterY() < 0) //초기값이 0이되면 안될거같은데~ 총 6칸
	{
		//위로가는거니까 마이너스로 되어야되는거 아닌가?
		_mapCountY++;
		_camFollowY = false;
		_TB = 0;
	}
	//아래로 내려간다
	if (!_camFollowY && _TB == 1) //false 상태
	{

		_camY -= SPEED;//
		_character->setJumpPower(0);
		_character->setCharacterY(_character->getCharacterY() - SPEED);//

		if (_camY < -2850 - ((_mapCountY - 1) * WINSIZEY)) _camFollowY = true;
	}
	else
	{
		if (_character->getCharacterY() >= camera.top)
		{
			if (2850 - _mapCountY * WINSIZEY >= WINSIZEY - _camY)
			{
				_camY += _character->getJumpPower();
				_character->setCharacterY(camera.top);
			}
		}
	}
	if (_character->getCharacterY() > WINSIZEY) //초기값이 0이되면 안될거같은데~ 총 6칸
	{
		//위로가는거니까 마이너스로 되어야되는거 아닌가?
		_mapCountY--;
		_camFollowY = false;
		_TB = 1;
		
	}*/


	// 2. 좌표값을 이용하는 방법
	// 캐릭터의 중심 x,y좌표값을 일정 좌표값과 비교해서
	// 카메라 이동
}

void mapCamera::MakeObject(RECT& rect, int left, int top, int width, int height)
{
	rect.left = _camX + left;
	rect.right = rect.left + width;
	rect.top = _camY + top;
	rect.bottom = rect.top + height;
}