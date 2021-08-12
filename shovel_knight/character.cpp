#include "pch.h"
#include "character.h"
#include "mapCamera.h"
#include "object.h"
#include "uiManager.h"
#include "enemyManager.h"

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
    IMAGEMANAGER->addFrameImage("캐릭터_스킬", "image/shovel_character_skill.bmp", 252, 192, 2, 2, true, RGB(255, 0, 255));

    _characterImg = IMAGEMANAGER->findImage("캐릭터_아이들");
    _x = 200;
    _y = 380;
    _state = IDLE;
    _direction = 0;
    _isPixelCollision = true;
    _isRectCollision = false;
    _speed = _gravity = _jumpPower = 0;
    _currentHP = _maxHP = 8;
    _currentFrame = _count = 0;
    _damage = 1;
    _hangFrameCount = _rcNum = 0;

    _imgRect = RectMakeCenter(_x, _y, _characterImg->getFrameWidth(), _characterImg->getFrameHeight());
    _collisionRect = RectMakeCenter(_x, _y, 56, 96);

    return S_OK;
}

void character::release()
{
}

void character::update() // 업데이트
{
    gravity();
    controll();
    pixelCollision();
    rectCollision();
    attackRect();
    imgFrameSetting();

    // 렉트 갱신
    _imgRect = RectMakeCenter(_x, _y, _characterImg->getFrameWidth(), _characterImg->getFrameHeight());
    _collisionRect = RectMakeCenter(_x, _y, 56, 96);
}

void character::gravity() // 캐릭터 중력 처리
{
    // 캐릭터가 중력받아서 내려가기 시작할 때 이미지 갱신
    if (_jumpPower < 0 && _state == JUMP)
        _characterImg = IMAGEMANAGER->findImage("캐릭터_점프2");

    // 중력 값 적용
    if (_state == JUMP || _state == JUMPATTACK || _state == JUMPBOTTOMATTACK || _state == HURT || _state == SKILL) // 점프 상태, 피격 상태일 때
    {
        if (_jumpPower < -20.0f) _jumpPower = -20.0f;
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

void character::controll() // 캐릭터 컨트롤키 처리
{
    if (_state != ATTACK && _state != SKILL && _state != HURT && _state != DEATH) // 공격 상태, 피격 상태, 죽음 상태가 아닐 때
    {
        // 달리기
        if (KEYMANAGER->isStayKeyDown('A'))
        {
            run(1);
        }
        if (KEYMANAGER->isStayKeyDown('D'))
        {
            run(0);
        }

        // 아이들
        if (KEYMANAGER->isOnceKeyUp('A'))
        {
            idle(1);
        }
        if (KEYMANAGER->isOnceKeyUp('D'))
        {
            idle(0);
        }

        // 상점 이용
        for (int i = 0; i < _object->getNPCMAX(); i++)
        {
            RECT temp;
            RECT npc = _object->getNPC(i).rc; // 상점 npc 렉트 받아옴
            if (IntersectRect(&temp, &_collisionRect, &npc))
            {
                int npcIndex = i + 1;
                if (KEYMANAGER->isOnceKeyDown('J'))
                {
                    shop(npcIndex); // 충돌된 npc의 상점 이용
                }
            }
        }

        // 상점 이용 안 할 때 공격
        for (int i = 0; i < _object->getNPCMAX(); i++)
        {
            RECT temp;
            RECT npc = _object->getNPC(i).rc; // 상점 npc 렉트 받아옴
            if (KEYMANAGER->isOnceKeyDown('J') && !IntersectRect(&temp, &_collisionRect, &npc)) // 아무 npc랑 충돌되지 않았을 땐 공격
            {
                attack();
            }
        }

        // 점프
        if (KEYMANAGER->isOnceKeyDown('K') || KEYMANAGER->isOnceKeyDown(VK_SPACE))
        {
            if (_state != JUMP && _state != JUMPATTACK && _state != JUMPBOTTOMATTACK && _state != HANG)
            {
                jump();
            }
        }

        // 점프 하단 공격
        if (_state == JUMP) // 점프 상태
        {
            if (KEYMANAGER->isStayKeyDown('S'))
            {
                jumpBottomAttack();
            }
        }

        // 스킬 사용
        if (KEYMANAGER->isOnceKeyDown('U'))
        {
            skill();
        }

        // 사다리 타기
        for (int i = 0; i < _object->getLadderMax(); i++)
        {
            RECT temp;
            RECT ladder = _object->getLadder(i).rc;

            if (IntersectRect(&temp, &_collisionRect, &ladder)) // 사다리랑 충돌된 상태에서~
            {
                if (KEYMANAGER->isStayKeyDown('A') && _state == HANG) // 사다리를 붙잡은 상태에서 좌로 벗어나려고 할 때
                {
                    hangOut();
                }

                if (KEYMANAGER->isStayKeyDown('D') && _state == HANG) // 사다리를 붙잡은 상태에서 우로 벗어나려고 할 때
                {
                    hangOut();
                }
                if (KEYMANAGER->isStayKeyDown('W')) // 사다리 붙잡기 && 상단 이동 
                {
                    _x = (ladder.left + ladder.right) / 2;
                    _y -= _mapCamera->getSpeed() / 1.5;
                    hang();
                }
                if (KEYMANAGER->isStayKeyDown('S')) // 사다리 붙잡기 && 하단 이동 
                {
                    _x = (ladder.left + ladder.right) / 2;
                    _y += _mapCamera->getSpeed() / 1.5;
                    hang();
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
                    _x = (ladder.left + ladder.right) / 2;
                    _y += _mapCamera->getSpeed() / 1.5;
                    hang();
                }
            }
        }
    }
}

void character::pixelCollision() // 캐릭터 픽셀 충돌 처리
{
    // 걸어다닐 때 바닥에 픽셀 충돌 안 되면 바닥으로 떨어진다
    if (_state == IDLE || _state == RUN)
    {
        int proveYBottom = _collisionRect.bottom - _mapCamera->getCamY(); // 캐릭터 바닥을 검사하기 위한 변수
        int collisionCount = 0;                                           // 캐릭터 충돌 렉트의 좌우측이 모두 충돌되지 않는지 체크

        for (int i = proveYBottom; i < proveYBottom + 1; i++)
        {
            if (_isRectCollision == true) break; // 렉트 충돌 중일 때는 픽셀 충돌 필요 없으니 브레이크

            // 캐릭터 충돌 렉트의 오른쪽 값, proveYBottom 좌표 값에 마젠타가 없으면 체크
            if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), _collisionRect.left - _mapCamera->getCamX(), i) != RGB(255, 0, 255))
            {
                collisionCount++;
            }
            // 캐릭터 충돌 렉트의 오른쪽 값, proveYBottom 좌표 값에 마젠타가 없으면 체크
            if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), _collisionRect.right - _mapCamera->getCamX(), i) != RGB(255, 0, 255))
            {
                collisionCount++;
            }
            if (collisionCount >= 2) // 충돌 렉트의 좌우측이 모두 체크되면 바닥으로 떨어지는 처리
            {
                _gravity = GRAVITY;
                _state = JUMP;
                imgSetting();
                _isPixelCollision = false;
                break;
            }
        }
    }

    // 공중에 있는 상태에서 처리
    if (_state == JUMP || _state == JUMPATTACK || _state == JUMPBOTTOMATTACK || _state == HURT)
    {
        // 바닥 픽셀 충돌 처리
        int proveYBottom = _y + ((_collisionRect.bottom - _collisionRect.top) / 2) - _mapCamera->getCamY();

        for (int i = proveYBottom; i < proveYBottom + 10; i++)
        {
            if (_jumpPower > 0) continue;
            // 캐릭터 충돌 렉트의 오른쪽 값, proveYBottom 좌표 값에 마젠타가 있으면 충돌 처리
            if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), _collisionRect.left - _mapCamera->getCamX(), i) == RGB(255, 0, 255))
            {
                _y = i - (_collisionRect.bottom - _collisionRect.top) / 2 + _mapCamera->getCamY();
                _gravity = _jumpPower = 0;
                _state = IDLE;
                imgSetting();
                _isPixelCollision = true;
                break;
            }
            // 캐릭터 충돌 렉트의 오른쪽 값, proveYBottom 좌표 값에 마젠타가 있으면 충돌 처리
            if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), _collisionRect.right - _mapCamera->getCamX(), i) == RGB(255, 0, 255))
            {
                _y = i - (_collisionRect.bottom - _collisionRect.top) / 2 + _mapCamera->getCamY();
                _gravity = _jumpPower = 0;
                _state = IDLE;
                imgSetting();
                _isPixelCollision = true;
                break;
            }
        }

        // 상단 픽셀 충돌 처리
        int proveYTop = _y - ((_collisionRect.bottom - _collisionRect.top) / 2) - _mapCamera->getCamY();

        for (int i = proveYTop; i > proveYTop - 1; i--)
        {
            // 캐릭터 충돌 렉트의 왼쪽 값, proveYTop 좌표 값에 마젠타가 있으면 충돌 처리
            if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), _collisionRect.left - _mapCamera->getCamX(), i) == RGB(255, 0, 255))
            {
                _y = i + (_collisionRect.bottom - _collisionRect.top) / 2 + _mapCamera->getCamY() + 3;
                if (_jumpPower > 0) _jumpPower = 0;
                _gravity = GRAVITY;
                _state = JUMP;
                imgSetting();
                _isPixelCollision = false;
                break;
            }
            // 캐릭터 충돌 렉트의 오른쪽 값, proveYTop 좌표 값에 마젠타가 있으면 충돌 처리
            if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), _collisionRect.right - _mapCamera->getCamX(), i) == RGB(255, 0, 255))
            {
                _y = i + (_collisionRect.bottom - _collisionRect.top) / 2 + _mapCamera->getCamY() + 3;
                if (_jumpPower > 0) _jumpPower = 0;
                _gravity = GRAVITY;
                _state = JUMP;
                imgSetting();
                _isPixelCollision = false;
                break;
            }
        }
    }

    // 벽면 픽셀 충돌 처리
    if (_state != HANG && _state != DEATH)
    {
        if (_direction == 0) // 오른쪽 보고 있을 때
        {
            // 캐릭터 우측을 검사하기 위한 변수
            int proveXRight = _x + ((_collisionRect.right - _collisionRect.left) / 2) - _mapCamera->getCamX();

            for (int i = proveXRight - 1; i < proveXRight + 1; i++)
            {
                // proveXRight 좌표 값, 캐릭터 충돌 렉트의 상단 값에 마젠타가 있으면 충돌 처리 
                if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), i, _y - _mapCamera->getCamY()) == RGB(255, 0, 255))
                {
                    _x = i - (_collisionRect.right - _collisionRect.left) / 2 - _mapCamera->getSpeed() + _mapCamera->getCamX();
                    break;
                }
                // proveXRight 좌표 값, 캐릭터 충돌 렉트의 하단 값에 마젠타가 있으면 충돌 처리 
                if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), i, _collisionRect.bottom - 10 - _mapCamera->getCamY()) == RGB(255, 0, 255))
                {
                    _x = i - (_collisionRect.right - _collisionRect.left) / 2 - _mapCamera->getSpeed() + _mapCamera->getCamX();
                    break;
                }
            }
        }
        else // 왼쪽 보고 있을 때
        {
            // 캐릭터 좌측을 검사하기 위한 변수
            int proveXLeft = _x - ((_collisionRect.right - _collisionRect.left) / 2) - _mapCamera->getCamX();

            for (int i = proveXLeft + 1; i >= proveXLeft - 1; i--)
            {
                // proveXLeft 좌표 값, 캐릭터 충돌 렉트의 상단 값에 마젠타가 있으면 충돌 처리 
                if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), i, _y - _mapCamera->getCamY()) == RGB(255, 0, 255))
                {
                    _x = i + (_collisionRect.right - _collisionRect.left) / 2 + _mapCamera->getSpeed() + _mapCamera->getCamX();
                    break;
                }
                // proveXLeft 좌표 값, 캐릭터 충돌 렉트의 하단 값에 마젠타가 있으면 충돌 처리 
                if (GetPixel(_mapCamera->getBackGroundMagenta()->getMemDC(), i, _collisionRect.bottom - 10 - _mapCamera->getCamY()) == RGB(255, 0, 255))
                {
                    _x = i + (_collisionRect.right - _collisionRect.left) / 2 + _mapCamera->getSpeed() + _mapCamera->getCamX();
                    break;
                }
            }
        }
    }
}

void character::rectCollision() // 캐릭터 렉트 충돌 처리
{
    // 장애물과 렉트 충돌 처리
    for (int i = 0; i < _object->getPlatformrMax(); i++)
    {
        RECT temp;
        RECT platform = _object->getPlatform(i).rc;
        if (IntersectRect(&temp, &_collisionRect, &platform))
        {
            float width = temp.right - temp.left;
            float height = temp.bottom - temp.top;

            if (width > height) // 상하 충돌
            {
                _rcNum = i; // 현재 어떤 장애물에 충돌했는지 기록

                if (_collisionRect.bottom <= platform.bottom) // 밑에서 충돌
                {
                    _y -= height;
                    _gravity = _jumpPower = 0; // 중력, 점프파워 초기화
                    _state = IDLE;
                    imgSetting();
                    _isRectCollision = true;
                }
                else // 위에서 충돌
                {
                    _y += height;
                    _jumpPower = 0;
                }
            }

            if (width <= height) // 옆면 충돌
            {
                if (_collisionRect.left <= platform.left) // 왼쪽에서 충돌
                {
                    _x -= width;
                }
                else // 오른쪽에서 충돌
                {
                    _x += width;
                }
            }
        }
    }

    // 밑에 장애물이 없다면 중력받아 떨어지는 처리
    if (_isPixelCollision == false)
    {
        RECT platform = _object->getPlatform(_rcNum).rc;
        POINT check;
        check.x = _x;
        check.y = _collisionRect.bottom + 1;

        if (!PtInRect(&platform, check) && _state == RUN)
        {
            _gravity = GRAVITY;
            _state = JUMP;
            imgSetting();
            _isRectCollision = false;
        }
    }

    // 어떤 사다리 충돌했는지 체크
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
        if (ladder.top >= _collisionRect.bottom) // 사다리의 꼭대기 도착
        {
            _hangRcNum = 0;
            if (_state == HANG)
            {
                _state = IDLE;
                imgSetting();
            }
        }

        if (ladder.bottom <= _collisionRect.top) // 사다리의 밑부분 도착
        {
            _hangRcNum = 0;
            if (_state == HANG)
            {
                _state = JUMP;
                imgSetting();
                _gravity = GRAVITY;
            }
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

void character::attackRect() // 캐릭터 공격 렉트 처리
{
    // 공격, 점프 공격 시 렉트 생성
    if (_state == ATTACK || _state == JUMPATTACK)
    {
        if (_direction == 0) _attackCollisionRect = RectMakeCenter(_collisionRect.right + 13, _y, 140, 96); // 변경
        if (_direction == 1) _attackCollisionRect = RectMakeCenter(_collisionRect.left - 13, _y, 140, 96); // 변경
    }

    // 점프 하단 공격 렉트 생성
    if (_state == JUMPBOTTOMATTACK)
    {
        _attackCollisionRect = RectMakeCenter(_x, _y, 56, 96);
    }

    // 공격, 점프 공격, 점프 하단 공격 상태가 아니면 공격 렉트 초기화
    if (_state != ATTACK && _state != JUMPATTACK && _state != JUMPBOTTOMATTACK)
    {
        _attackCollisionRect = RectMakeCenter(0, 0, 0, 0);
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

            // 스킬 프레임 카운트 증가
            if (_state == SKILL) _skillFrameCount++;
        }
    }

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

    // 일정 스킬 애니메이션을 진행했으면 스킬 모션 초기화
    if (_skillFrameCount >= 6)
    {
        _skillFrameCount = 0;
        _state = IDLE;
        imgSetting();
    }

    // 사다리 타기 이미지 갱신(사다리 타기는 이동 안 할때는 붙잡고 있어야 해서 예외 처리)
    if (_state == HANG)
    {
        if (_hangFrameCount % 10 == 0) _characterImg->setFrameX(0);
        else if (_hangFrameCount % 19 == 0) _characterImg->setFrameX(1);

        if (_hangFrameCount >= 19) _hangFrameCount = 1;
    }

    // 카운트 초기화
    if (_count >= 5) _count = 0;
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
    case SKILL:
        _characterImg = IMAGEMANAGER->findImage("캐릭터_스킬");
        break;
    }

    // 현재 이미지에 맞게 커런트 프레임 초기화
    if (_state != IDLE && _state != RUN)
    {
        if (_direction == 0) _currentFrame = 0;
        else if (_direction == 1) _currentFrame = _characterImg->getMaxFrameX();
    }
}

void character::idle(bool direction)
{
    if (_state != JUMP && _state != JUMPATTACK && _state != JUMPBOTTOMATTACK && _state != HANG)
    {
        _state = IDLE;
        imgSetting();
    }

    _direction = direction;
    _speed = 0;
}

void character::run(bool direction)
{
    if (_state != JUMP && _state != JUMPATTACK && _state != JUMPBOTTOMATTACK && _state != HANG)
    {
        _state = RUN;
        imgSetting();
    }

    _direction = direction;
    _speed = _mapCamera->getSpeed();

    if(direction == 1) _x -= _speed;
    else if (direction == 0) _x += _speed;
}

void character::jump()
{
    _jumpPower = JUMPPOWER;
    _gravity = GRAVITY;
    _state = JUMP;
    imgSetting();
    _isPixelCollision = false;
    _isRectCollision = false;
}

void character::attack() // 캐릭터 공격 처리
{
    if (_state != JUMPATTACK)
    {
        if (_state == JUMP || _state == JUMPBOTTOMATTACK) // 점프 상태
        {
            _state = JUMPATTACK;
            imgSetting();
        }
        else if (_state == IDLE || _state == RUN)// 점프 상태가 아닐 때
        {
            _state = ATTACK;
            imgSetting();
        }
    }
}

void character::jumpBottomAttack()
{
    _state = JUMPBOTTOMATTACK;
    imgSetting();
}

void character::hang() // 캐릭터 사다리 타기 처리
{
    _state = HANG;
    imgSetting();
    _hangFrameCount++;
    _gravity = _jumpPower = 0;
}

void character::hangOut() // 캐릭터 사다리 벗어나는 처리
{
    _gravity = GRAVITY;
    _state = JUMP;
    imgSetting();
}

void character::skill()
{
    if (SCENEMANAGER->checkFire() == true) // 불지팡이 보유 중일 때
    {
        if (SCENEMANAGER->getMana() >= 5)
        {
            SCENEMANAGER->setMana(SCENEMANAGER->getMana() - 5);
            _state = SKILL;
            imgSetting();

            // fireball() 발사!!!
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

void character::shop(int arrNum) // 캐릭터 상점 이용 처리
{
    switch (arrNum)
    {
    case 1: // 대장장이 아저쒸
        _ui->talklingManOn();
        break;
    case 2: // 미녀 언니
        _ui->fireshopOn();
        break;
    case 3: // 앙마 염소
        _ui->goatshopOn();
        break;
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
        Rectangle(getMemDC(), _attackCollisionRect);
    }

    //// 현재 프레임
    char str[128];
    sprintf_s(str, "_currentFrame : %d", _currentFrame);
    TextOut(getMemDC(), 0, 220, str, strlen(str));

    sprintf_s(str, "_state : %d", _state);
    TextOut(getMemDC(), 0, 240, str, strlen(str));

    sprintf_s(str, "_direction : %d", _direction);
    TextOut(getMemDC(), 0, 160, str, strlen(str));

    sprintf_s(str, "_charcterX : %d", (int)_x);
    TextOut(getMemDC(), 0, 180, str, strlen(str));

    sprintf_s(str, "_charcterY : %d", (int)_y);
    TextOut(getMemDC(), 0, 200, str, strlen(str));
}


