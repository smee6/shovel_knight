#pragma once
#include "enemy.h"

class wizard : public enemy
{
public:
	wizard();
	~wizard();

	//에너미 클래스에 있는  함수 오버라이딩
	virtual void attack();
	virtual void die();
};

