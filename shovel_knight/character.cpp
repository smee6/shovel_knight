#include "pch.h"
#include "character.h"
#include "mapCamera.h"
#include "object.h"

character::character()
{
}

character::~character()
{
}

HRESULT character::init() // 인잇
{
    IMAGEMANAGER->addFrameImage("캐릭터_아이들", "image/shovel_character_idle.bmp", 1224, 192, 12, 2, true, RGB(255, 0, 255));
    IMAGEMANAGER->addFrameImage("캐릭터_달리기", "image/shovel_character_run.bmp", 720, 210, 6, 2, true, RGB(255, 0, 255));
    IMAGEMANAGER->addFrameImage("캐릭터_점프1", "image/shovel_character_jump1.bmp", 93, 204, 1, 2, true, RGB(255, 0, 255));
    IMAGEMANAGER->addFrameImage("캐릭터_점프2", "image/shovel_character_jump2.bmp", 99, 204, 1, 2, true, RGB(255, 0, 255));
    IMAGEMANAGER->addFrameImage("캐릭터_공격", "image/shovel_character_attack.bmp", 900, 210, 4, 2, true, RGB(255, 0, 255));
    IMAGEMANAGER->addFrameImage("캐릭터_점프공격", "image/shovel_character_jumpattack.bmp", 900, 210, 4, 2, true, RGB(255, 0, 255));
    IMAGEMANAGER->addFrameImage("캐릭터_점프하단공격", "image/shovel_character_jumpbottomattack.bmp", 72, 216, 1, 2, true, RGB(255, 0, 255));
    IMAGEMANAGER->addFrameImage("캐릭터_피격", "image/shovel_character_hurt.bmp", 105, 234, 1, 2, true, RGB(255, 0, 255));
    IMAGEMANAGER->addFrameImage("캐릭터_죽음", "image/shovel_character_death.bmp", 333, 204, 3, 2, true, RGB(255, 0, 255));
    IMAGEMANAGER->addFrameImage("캐릭터_사다리", "image/shovel_character_hang.bmp", 150, 96, 2, 1, true, RGB(255, 0, 255));

    _characterImg = IMAGEMANAGER->findImage("캐릭터_아이들");
    _x = 400;
    _y = 580;
    _state = IDLE;
    _direction = 0;
    _isPixelCollision = true;
    _speed = _gravity = _jumpPower = 0;
    _currentHP = _maxHP = 8;
    _currentFrame = _count = 0;
    _damage = 1;
    _hangCount = _rcNum = 0;

    _imgRect = RectMakeCenter(_x, _y, _characterImg->getFrameWidth(), _characterImg->getFrameHeight());
    _collisionRect = RectMakeCenter(_x, _y, 55, 96);

    return S_OK;
}

void character::release()
{
}

void character::update() // 업데이트
{
    gravity();
    collision();
    controll();
    hang();
    attack();
    imgFrameSetting();
    
    // 렉트 갱신
    _imgRect = RectMakeCenter(_x, _y, _characterImg->getFrameWidth(), _characterImg->getFrameHeight());
    _collisionRect = RectMakeCenter(_x, _y, 55, 96);
}

void character::controll() // 캐릭터 컨트롤키 처리
{
    if (_state != ATTACK && _state != HURT) // 공격 상태, 피격 상태가 아닐 때
    {
        // 달리기
        if (KEYMANAGER->isStayKeyDown('A'))
        {
            if (_state != JUMP && _state != JUMPATTACK && _state != JUMPBOTTOMATTACK && _state != HANG)
            {
                _state = RUN;
                imgSetting();
            }

            _direction = 1;
            _speed = _mapCamera->getSpeed();
            _x -= _speed;
        }
        if (KEYMANAGER->isStayKeyDown('D'))
        {
            if (_state != JUMP && _state != JUMPATTACK && _state != JUMPBOTTOMATTACK && _state != HANG)
            {
                _state = RUN;
                imgSetting();
            }

            _direction = 0;
            _speed = _mapCamera->getSpeed();
            _x += _speed;
        }

        // 아이들
        if (KEYMANAGER->isOnceKeyUp('A'))
        {
            if (_state != JUMP && _state != JUMPATTACK && _state != JUMPBOTTOMATTACK && _state != HANG)
            {
                _state = IDLE;
                imgSetting();
            }
            
            _direction = 1;
            _speed = 0;
        }
        if (KEYMANAGER->isOnceKeyUp('D'))
        {
            if (_state != JUMP && _state != JUMPATTACK && _state != JUMPBOTTOMATTACK && _state != HANG)
            {
                _state = IDLE;
                imgSetting();
            }

            _direction = 0;
            _speed = 0;
        }

        // 공격
        if (KEYMANAGER->isOnceKeyDown('J'))
        {
            if (_state != JUMPATTACK)
            {
                if (_state == JUMP || _state == JUMPBOTTOMATTACK) // 점프 상태
                {
                    _state = JUMPATTACK;
                    imgSetting();                  
                }
                else if(_state == IDLE || _state == RUN)// 점프 상태가 아닐 때
                {
                    _state = ATTACK;
                    imgSetting();
                }
            }
        }

        // 점프
        if (KEYMANAGER->isOnceKeyDown('K') || KEYMANAGER->isOnceKeyDown(VK_SPACE))
        {
            if (_state != JUMP && _state != JUMPATTACK && _state != JUMPBOTTOMATTACK && _state != HANG)
            {
                _jumpPower = JUMPPOWER;
                _gravity = GRAVITY;
                _state = JUMP;
                imgSetting();
                _isPixelCollision = false;
            }
        }

        // 점프 하단 공격
        if (_state == JUMP) // 점프 상태
        {
            if (KEYMANAGER->isStayKeyDown('S'))
            {
                _state = JUMPBOTTOMATTACK;
                imgSetting();
            }
        }

        // 사다리 타기
        for (int i = 0; i < _object->getLadderMax(); i++)
        {
            RECT temp;
            RECT ladder = _object->getLadder(i).rc;
            if (IntersectRect(&temp, &_collisionRect, &ladder))
            {
                if (KEYMANAGER->isStayKeyDown('A') && _state == HANG)
                {
                    _gravity = GRAVITY;
                    _state = JUMP;
                    imgSetting();
                }
                if (KEYMANAGER->isStayKeyDown('D') && _state == HANG)
                {
                    _gravity = GRAVITY;
                    _state = JUMP;
                    imgSetting();
                }
                if (KEYMANAGER->isStayKeyDown('W')) 
                {
                    _state = HANG;
                    imgSetting();
                    _hangCount++;
                    _gravity = _jumpPower = 0;

                    _x = (temp.left + temp.right) / 2;
                    _y -= _mapCamera->getSpeed() / 1.5;
                }
                if (KEYMANAGER->isStayKeyDown('S'))
                {
                    _state = HANG;
                    imgSetting();
                    _hangCount++;
                    _gravity = _jumpPower = 0;

                    _x = (ladder.left + ladder.right) / 2; //(temp.left + temp.right) / 2;
                    _y += _mapCamera->getSpeed() / 1.5;
                }
            }

            // 캐릭터 하단에 사다리가 있을 경우 밑으로 내려갈 수 있도록 처리
            POINT check;
            check.x = _x;
            check.y = _collisionRect.bottom + 10;
            if (PtInRect(&ladder, check))
            {
                if (KEYMANAGER->isStayKeyDown('S') && !IntersectRect(&temp, &_collisionRect, &ladder))
                {
                    _state = HANG;
                    imgSetting();
                    _hangCount++;
                    _gravity = _jumpPower = 0;

                    _x = (ladder.left + ladder.right) / 2;
                    _y += _mapCamera->getSpeed() / 1.5;
                }
            }
        }
    }
}

void character::imgSetting() // 상태에 따라 이미지 처리
{
    // 이미지 변경
    switch (_state)
    {
    case IDLE:
        _characterImg = IMAGEMANAGER->findImage("캐릭터_아이들");
        break;
    case RUN:
        _characterImg = IMAGEMANAGER->findImage("캐릭터_달리기");
        break;
    case JUMP:
        _characterImg = IMAGEMANAGER->findImage("캐릭터_점프1");
        break;
    case ATTACK:
        _characterImg = IMAGEMANAGER->findImage("캐릭터_공격");
        break;
    case JUMPATTACK:
        _characterImg = IMAGEMANAGER->findImage("캐릭터_점프공격");
        break;
    case JUMPBOTTOMATTACK:
        _characterImg = IMAGEMANAGER->findImage("캐릭터_점프하단공격");
        break;
    case HURT:
        _characterImg = IMAGEMANAGER->findImage("캐릭터_피격");
        break;
    case DEATH:
        _characterImg = IMAGEMANAGER->findImage("캐릭터_죽음");
        break;
    case HANG:
        _characterImg = IMAGEMANAGER->findImage("캐릭터_사다리");
        break;
    }

    // 현재 이미지에 맞게 커런트 프레임 초기화
    if (_state != IDLE && _state != RUN)
    {
        if (_direction == 0) _currentFrame = 0;
        else if (_direction == 1) _currentFrame = _characterImg->getMaxFrameX();
    }
}

void character::imgFrameSetting() // 캐릭터 이미지 프레임 처리
{
    // 프레임Y 이미지 갱신
    _characterImg->setFrameY(_direction);

    // 카운트가 일정 수치마다 프레임X 갱신
    _count++;
    if (_count % 5 == 0)
    {
        // 프레임 초기화
        if (_state != HANG) // 사다리 상태가 아닐 때(사다리를 예외처리함)
        {
            if (_direction == 0)
            {
                _currentFrame++;
                if (_currentFrame > _characterImg->getMaxFrameX()) _currentFrame = 0;
            }
            else
            {
                _currentFrame--;
                if (_currentFrame < 0) _currentFrame = _characterImg->getMaxFrameX();
            }

            // X프레임 이미지 갱신
            _characterImg->setFrameX(_currentFrame);
        }
    }

    // 카운트 초기화
    if (_count >= 5) _count = 0;
}

void character::gravity() // 캐릭터 중력 처리
{
    // 캐릭터가 중력받아서 내려가기 시작할 때 이미지 갱신
    if (_jumpPower < 0 && _state != JUMPBOTTOMATTACK && _state != JUMPATTACK && _state != HURT && _state != HANG)
        _characterImg = IMAGEMANAGER->findImage("캐릭터_점프2");

    // 중력 값 적용
    if (_state == JUMP || _state == JUMPATTACK || _state == JUMPBOTTOMATTACK || _state == HURT) // 점프 상태, 피격 상태일 때
    {
        _y -= _jumpPower;
        _jumpPower -= _gravity;
    }

    // 피격 상태일 때 바라본 방향의 뒤로 밀림
    if (_state == HURT) 
    {
        if (_direction == 0) _x -= _mapCamera->getSpeed();
        else _x += _mapCamera->getSpeed();
    }
}

void character::hang() // 캐릭터 사다리 타기 처리
{
    // 사다리 타기 이미지 갱신(사다리 타기는 이동 안 할때는 붙잡고 있어야 해서 예외 처리)
    if (_state == HANG)
    {
        if (_hangCount % 10 == 0) _characterImg->setFrameX(0);
        else if (_hangCount % 19 == 0) _characterImg->setFrameX(1);

        if (_hangCount >= 19) _hangCount = 1;
    }

    // 어떤 사다리 충돌했는지
    for (int i = 0; i < _object->getLadderMax(); i++)
    {
        RECT temp;
        RECT ladder = _object->getLadder(i).rc;
        if (IntersectRect(&temp, &_collisionRect, &ladder))
        {
            _hangRcNum = i;
        }
    }

    // 사다리 벗어나면
    RECT temp;
    RECT ladder = _object->getLadder(_hangRcNum).rc;
    if (!IntersectRect(&temp, &_collisionRect, &ladder))
    {
        if (ladder.top >= _collisionRect.bottom)
        {
            _hangRcNum = 0;
            if (_state == HANG) _state = IDLE;
            imgSetting();
        }

        if (ladder.bottom <= _collisionRect.top)
        {
            _hangRcNum = 0;
            if (_state == HANG) _state = JUMP;
            imgSetting();
            _gravity = GRAVITY;
        }
    }  
}

void character::collision() // 캐릭터 충돌 처리
{
    // 바닥 픽셀 충돌 처리
    if (_state == JUMP || _state == JUMPATTACK || _state == JUMPBOTTOMATTACK || _state == HURT)
    {
        int proveY = _y + ((_collisionRect.bottom - _collisionRect.top) / 2) - _mapCamera->getCamY();
        for (int i = proveY - 10; i < proveY + 10; i++)
        {
            if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), _x - _mapCamera->getCamX(), i) == RGB(255, 0, 255))
            {
                _y = i - (_collisionRect.bottom - _collisionRect.top) / 2 - 5 + _mapCamera->getCamY();
                _gravity = _jumpPower = 0;
                _state = IDLE;
                imgSetting();
                _isPixelCollision = true;
                break;
            }
        }
    }

    // 벽면 픽셀 충돌 처리
    if (_direction == 0) // 오른쪽 보고 있을 때
    {
        int proveX = _x + ((_collisionRect.right - _collisionRect.left) / 2) - _mapCamera->getCamX();
        for (int i = proveX - 10; i < proveX + 10; i++)
        {
            if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), i, _y - _mapCamera->getCamY()) == RGB(255, 0, 255))
            {
                _x = i - (_collisionRect.right - _collisionRect.left) / 2 - _mapCamera->getSpeed() + _mapCamera->getCamX();
                break;
            }
        }
    }
    else // 왼쪽 보고 있을 때
    {
        int proveX = _x - ((_collisionRect.right - _collisionRect.left) / 2) - _mapCamera->getCamX();
        for (int i = proveX + 10; i >= proveX - 10; i--)
        {
            if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), i, _y - _mapCamera->getCamY()) == RGB(255, 0, 255))
            {
                _x = i + (_collisionRect.right - _collisionRect.left) / 2 + _mapCamera->getSpeed() + _mapCamera->getCamX();
                break;
            }
        }
    }

    // 장애물과 렉트 충돌 처리
    for (int i = 0; i < _object->getPlatformrMax(); i++)
    {
        RECT temp;
        RECT platform = _object->getPlatform(i).rc;
        if (IntersectRect(&temp, &_collisionRect, &platform))
        {
            float width = temp.right - temp.left;
            float height = temp.bottom - temp.top;

            if (width <= height) // 옆면 충돌
            {
                if (_collisionRect.left <= platform.left) // 왼쪽에서 충돌
                {
                    _x -= _mapCamera->getSpeed();
                }
                else // 오른쪽에서 충돌
                {
                    _x += _mapCamera->getSpeed();
                }
            }

            if (width > height) // 상하 충돌
            {
                _rcNum = i; // 현재 어떤 장애물에 충돌했는지 기록

                if (_collisionRect.bottom <= platform.bottom) // 밑에서 충돌
                {
                    _y -= height;
                    _gravity = _jumpPower = 0; // 중력, 점프파워 초기화
                    _state = IDLE;
                    imgSetting();
                }
                else // 위에서 충돌
                {
                    _y += height;
                    _jumpPower = 0;
                }
            }
        }
    }

    // 밑에 장애물이 없다면 중력받아 떨어지는 처리
    if (_isPixelCollision == false)
    {
        RECT platform = _object->getPlatform(_rcNum).rc;
        POINT check;
        if (_direction == 0) check.x = _collisionRect.left;
        else check.x = _collisionRect.right;
        //check.x = _x;
        check.y = _collisionRect.bottom;

        if (!PtInRect(&platform, check) && _state == RUN)
        {
            _gravity = GRAVITY;
            _state = JUMP;
            imgSetting();
        }
    }

    //// 피격 테스트용 렉트
    //RECT temp2;
    //if (IntersectRect(&temp2, &_collisionRect, &_attackRc))
    //{
    //    if (_state == JUMPBOTTOMATTACK) // 하단 공격일 때는 위로 다시 튀어오름
    //    {
    //        _jumpPower = JUMPPOWER;
    //        _gravity = GRAVITY;
    //    }
    //    else hitDamage(1); // 아닐 땐 데미지 받음
    //}
}

void character::attack() // 캐릭터 공격 처리
{
    // 공격 초기화
    if (_state == ATTACK)
    {
        if (_direction == 0)
        {
            if (_currentFrame == _characterImg->getMaxFrameX())
            {
                _state = IDLE;
                imgSetting();
            }
        }
        else
        {
            if (_currentFrame == 0)
            {
                _state = IDLE;
                imgSetting();
            }
        }
    }

    // 점프 공격 초기화
    if (_state == JUMPATTACK)
    {
        if (_direction == 0)
        {
            if (_currentFrame == _characterImg->getMaxFrameX())
            {
                _state = JUMP;
                imgSetting();
            }
        }
        else
        {
            if (_currentFrame == 0)
            {
                _state = JUMP;
                imgSetting();
            }
        }
    }
}

void character::hitDamage(float damage) // 캐릭터 피격 시 처리
{
    _currentHP -= damage;
    _state = HURT;
    imgSetting();

    // 피격 당하면 뒤로 밀리는 처리를 위해
    _jumpPower = JUMPPOWER / 1.3;
    _gravity = GRAVITY;

    //death();
}

void character::death() // 캐릭터 죽음 처리
{
    if(_currentHP <= 0)
    {
        _state = DEATH;
        imgSetting();
    }
}

void character::render() // 캐릭터 렌더
{
    // 캐릭터 이미지 렌더
    _characterImg->frameRender(getMemDC(), _imgRect.left, _imgRect.top);

    // 캐릭터 렉트 렌더
    if (KEYMANAGER->isToggleKey(VK_TAB))
    {
        Rectangle(getMemDC(), _imgRect);
        Rectangle(getMemDC(), _collisionRect);
    }

    //// 현재 프레임
    //char str[128];
    //sprintf_s(str, "_currentFrame : %d", _currentFrame);
    //TextOut(getMemDC(), 0, 100, str, strlen(str));
}


