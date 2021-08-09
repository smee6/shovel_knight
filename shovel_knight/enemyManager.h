#pragma once
#include "gameNode.h"
#include "beeto.h"
#include "wizard.h"
#include "steed.h"
#include <vector>
#include "bullets.h"

class enemyManager : public gameNode
{
private:
	typedef vector<enemy*>				vEnemy;
	typedef vector<enemy*>::iterator	viEnemy;

private:
	vEnemy		_vEnemy;
	viEnemy		_viEnemy;

	
	bullet* _bullet;

	
public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void enemySetting();	//적을 만드는 함수
	POINT enemyPosition(int i);	//적 위치 함수

	void enemyBulletFire();

	void removeEnemy(int arrNum);

	void collision();

	void enemyImageStorage();		//적 이미지 저장 함수

	vector<enemy*> getVMinion() { return _vEnemy; }
	vector<enemy*>::iterator getVIMinion() { return _viEnemy; }
};

