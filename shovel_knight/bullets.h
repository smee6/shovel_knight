#pragma once
#include "gameNode.h"
#include <vector>

struct tagBullet
{
	image* bulletImage;		//총알의 이미지
	RECT rc;				//총알의 렉트
	float x, y;				//총알의 중점
	float angle;			//총알의 각도
	float radius;			//총알의 반지름
	float speed;			//총알의 스피드
	float fireX, fireY;		//총알의 발사위치
	bool isFire;			//총알의 발사유무
	int count;
};

//쏠때 생성하는 방식
class hellFire : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
	int _bulletMax;

public:
	hellFire() {};
	~hellFire() {};


	virtual HRESULT init(int bulletMax, float range);
	virtual void release();
	virtual void update();
	virtual void render();

	void fire(float x, float y, float angle);
	void move();

	void removeMissile(int arrNum);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet() { return _viBullet; }

};