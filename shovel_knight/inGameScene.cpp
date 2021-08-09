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
	_mapCamera = new mapCamera;
	_object = new object;

	_character->init();
	_ui->init();
	_mapCamera->init();
	_object->init();

	_character->setMapCameraMemoryAddressLink(_mapCamera);
	_character->setObjectMemoryAddressLink(_object);

	_mapCamera->setCharacterAddressLink(_character);
	_mapCamera->setObjectMemoryAddressLink(_object);

	_object->setcharacterMemoryAddressLink(_character);
	_object->setMapCameraMemoryAddressLink(_mapCamera);

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
