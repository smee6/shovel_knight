#include "pch.h"
#include "inGameScene.h"


inGameScene::inGameScene()
{
}


inGameScene::~inGameScene()
{
}

HRESULT inGameScene::init()
{

	_backGround = IMAGEMANAGER->findImage("배경");

	_character = new character;
	_ui = new uiManager;

	_character->init();
	_ui->init();

	return S_OK;
}

void inGameScene::release()
{
}

void inGameScene::update()
{
	_character->update();
	
	SCENEMANAGER->nowHP(_character->getCurrentHp());
	//캐릭터의 현재 에이치피를 씬매니져로 전송

}

void inGameScene::render()
{
	IMAGEMANAGER->findImage("배경")->render(getMemDC());

	_backGround->render(getMemDC(), 0, 0);

	_character->render();

	_ui->render();
	
}
