#pragma once
#include "gameNode.h"

//적을 찍어내는 부모클래스

enum ENEMYSTATE		//적의 상태
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
	RECT			_rc;

	int				_currentFrameX;		//이미지 프레임 번호로 사용할 변수
	int				_currentFrameY;

	int				_count;				//적 이미지 프레임 카운트 용
	char			_enemyName;			//init 함수 첫번 째 매개변수를 쓰기위해 만든 변수
	
public:
	enemy();
	~enemy();

	virtual HRESULT init();
	virtual HRESULT init(char imageName, POINT position, ENEMYDIRECTION enemyDirection);
	virtual void release();
	virtual void update();
	virtual void render();

	void draw();		//그려주는 함수
	void enemyFrame();	//적의 상태에 따라 프레임돌리는 함수
	void move();		//움직임 함수
	void attack();		//공격 함수
	void die();			//죽음 처리 함수
	void enemyAI();		//적의 상태를 정의하는 함수
	

	//에너미 렉트에 대한 접근자
	inline RECT getRect() { return _rc; }
};

