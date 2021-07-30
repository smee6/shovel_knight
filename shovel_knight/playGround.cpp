#include "pch.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

//초기화는 여기에다 해라!!!
HRESULT playGround::init()
{
	gameNode::init(true);

	IMAGEMANAGER->addImage("배경", "image/shovel_knight_map.bmp", 1280, 720, true, RGB(255, 0, 255));
	_backGround = IMAGEMANAGER->findImage("배경");

	SCENEMANAGER->addScene("로딩씬", new loadingScene);
	SCENEMANAGER->addScene("인게임씬", new inGameScene);
	
	SCENEMANAGER->changeScene("로딩씬");

	_character = new character;
	_character->init();


	return S_OK;
}

//메모리 해제는 여기다 해라!!!!
void playGround::release()
{
	gameNode::release();


}

//연산처리는 여기다가!
void playGround::update()
{
	gameNode::update();

	_character->update();

	SCENEMANAGER->update();
	
}

//여기다 그려줘라!!!
void playGround::render()
{
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//==============위에는 제발 건드리지 마라 ============
	
	SCENEMANAGER->render();

	_backGround->render(getMemDC(), 0, 0);

	_character->render();

	TIMEMANAGER->render(getMemDC());

	//=============== 밑에도 건들지마라 ================
	_backBuffer->render(getHDC(), 0, 0);

}

