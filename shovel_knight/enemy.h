#pragma once
#include "gameNode.h"

//적을 찍어내는 부모클래스

enum ENEMYSTATE		//적의 상태 구분
{
	//E == enemy
	E_IDLE,
	E_MOVE,
	E_ATTACK,
	E_DIE
};

enum ENEMYDIRECTION	//적 좌우 구분
{
	E_LEFT,
	E_RIGHT
};

class enemy : public gameNode
{
protected:
	ENEMYSTATE		_enemyState;		
	ENEMYDIRECTION	_enemyDirection;

	image*			_imageName;			//적이 사용할 이미지 이름
	RECT			_rc;				//적 렉트
	RECT			_proveRC;			//적이 플레이어를 감지하는 렉트

	//이미지 프레임 번호로 사용할 변수
	int				_currentFrameX;
	int				_currentFrameY;

	//적 렉트 left,top
	float _x; 
	float _y;

	int				_imageCount;				//적 이미지 프레임 카운트 용
	
	//이미지 이름을 조합하기 위한 문자열 변수
	string			_enemyName;				
	string			_str;
	string			_strSum;

	//적 패턴을 카운트별로 나누기 위한 변수
	int _randCount;
	int _moveCount;
	int _attackCount;
	int _dieCount;
	int _hitCount;	//플레이어에게 맞은 횟수 카운트

	bool _isDefense;	//플레이어에게 맞은 상태일 때 피격되지 않게 하는 변수
	int _defenseCount;	//카운트가 지나면 다시 _isHit를 false로 바꿀 변수

	//적 죽었을 때 모션에 필요한 변수
	float _jumpPower;
	float _gravity;

public:
	enemy();
	~enemy();

	virtual HRESULT init();
	virtual HRESULT init(const char* imageName, POINT position, ENEMYDIRECTION enemyDirection);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void draw();		//그려주는 함수
	virtual void enemyFrame();	//적의 상태에 따라 프레임돌리는 함수
	virtual void move();		//움직임 함수
	virtual void attack();		//공격 함수
	virtual void die();			//죽음 처리 함수

	virtual void enemyAI();		//적의 상태를 정의하는 함수

	//에너미 렉트에 대한 접근자
	inline RECT getRect() { return _rc; }
	inline RECT getproveRect() { return _proveRC; }

	bool getDefense() { return _isDefense; }	// true일 때 플레이어와 충돌 되지 않게 하는 접근자
	//int getHit() { return _hitCount; }
	
	//플레이어와 충돌 시 카운트 증가 시킬 설정자
	void setHit(int i) { _hitCount += i; }
};

