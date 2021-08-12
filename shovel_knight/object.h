#pragma once
#include "gameNode.h"


#define PLATFORMMAX 5
#define SANDBLOCKMAX 128
#define LADDERMAX 15
#define NPCMAX 3
#define BUBBLEMAX 20

class character;
class mapCamera;

struct tagPlatform //움직이는 발판
{
	image* objectImage;								//	오브젝트의 이미지
	RECT rc;										//	오브젝트의 렉트
	float x, y;										//	오브젝트의 중점 X, Y 값
	float speed;									//	오브젝트 이동속도 값
	float min, max;									//	오브젝트 이동가능한 최소, 최대값

	int type;
	int index;

	bool isDirection;								//	오브젝트의 방향전환을 위한 불값					
};

struct tagSandBlock //흙더미
{
	image* objectImage;								//	오브젝트의 이미지
	RECT rc;										//	오브젝트의 렉트
	float x, y;										//	오브젝트의 중점 X, Y 값

	int type;
	int index;
	int currentFrame;								// 프레임 이미지 인덱스
};

struct tagLadder //사다리
{
	RECT rc;										//	오브젝트의 렉트
	float x, y;										//	오브젝트의 중점 X, Y 값
	float height;									//  오브젝트의 세로 길이

	int type;
	int index;
};

struct tagNPC // npc
{
	image* npcImage;								//  npc의 이미지
	RECT rc;										//	npc의 렉트
	float x, y;										//	npc의 중점 X, Y 값
	float width, height;							//  npc의 가로, 세로 길이

	int type;
	int index;
};

struct tagBubble // 물방울 
{
	image* objectImage;								//  오브젝트의 이미지
	RECT rc;										//	오브젝트의 렉트
	float x, y;										//	오브젝트의 중점 X, Y 값
	float startX, startY;							//  오브젝트의 시작 X, Y 값
	float width, height;							//  오브젝트의 가로, 세로 길이
	float min, max;									//	오브젝트의 X의 최소, 최대값
	float topMax;									//	오브젝트의 Y의 최대값
	float speed;									//	오브젝트 이동속도 값

	int type;
	int index;
	bool isAlive;									//  방울이 터졌는지에 대한 여부
	bool isDirection;								//	오브젝트의 방향전환을 위한 불값
};


class object : public gameNode
{
private:
	//전역변수로 사용할만한 것들

	int _count;		// 프레임 카운트

	tagPlatform _platform[PLATFORMMAX];

	tagSandBlock _sandBlock[SANDBLOCKMAX];

	tagLadder _ladder[LADDERMAX];

	tagNPC _npc[NPCMAX];

	tagBubble _bubble[BUBBLEMAX];

	character* _character;

	mapCamera* _mapCamera;



	//1번타입 - 캐릭터와 충돌했을때 진로를 막는것들 - 발판, 흙더미
	//2번타입 - 캐릭터와 충돌했을때 사라지는것들 - 체력회복 오브젝트, 보석류
	//3번타입 - 캐릭터와 충돌했을때 데미지를 받는것들 - 물방울, 파이어볼

	//1번타입의 인덱스 값 설정 - 1.좌우 발판, 2.상하 발판, 10.큰 흙더미, 11.작은 흙더미, 21. 사다리

	//2번타입의 인덱스 값 설정 - 1.사과, 2.치킨, 10.큰다이아, 11.동전, 12. 사파이어

	//3번타입의 인덱스 값 설정 - 1.물방울, 2.파이어볼

public:
	object();
	~object();

	void collision();
	void gravity();
	void imgSetting();															// 상태에 따라 이미지 처리
	void imgFrameSetting();														// 캐릭터 이미지 프레임 처리
	void objectMove();															// 오브젝트들의 움직임 처리

	void platformSetting();														// 오브젝트들의 위치값 설정을 위한 함수
	void sandBlockSetting();
	void potionSetting();
	void jewelSetting();
	void bubbleSetting();
	void fireBallSetting();
	void ladderSetting();
	void npcSetting();

	int getPlatformrMax() { return PLATFORMMAX; }
	int getLadderMax() { return LADDERMAX; }
	int getNPCMAX() { return NPCMAX; }
	int getSandBlockMAX() { return SANDBLOCKMAX; }
	int getBubbleMAx() { return BUBBLEMAX; }

	tagPlatform getPlatform(int arrNum) { return _platform[arrNum]; }
	tagLadder getLadder(int arrNum) {	return _ladder[arrNum];}
	tagNPC getNPC(int arrNum) { return _npc[arrNum]; }
	tagSandBlock getSandBlock(int arrNum) { return _sandBlock[arrNum]; }
	tagBubble getBUbble(int arrNum) { return _bubble[arrNum]; }


	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();


	void setMapCameraMemoryAddressLink(mapCamera* mapCamera) { _mapCamera = mapCamera; }			// 맵매니저 클라스 링크 
	
	void setcharacterMemoryAddressLink(character* character) { _character = character; }			// 캐릭터 클라스 링크 

};
