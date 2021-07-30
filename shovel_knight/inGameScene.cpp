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
	return S_OK;
}

void inGameScene::release()
{
}

void inGameScene::update()
{
}

void inGameScene::render()
{

	IMAGEMANAGER->findImage("½ÃÀÛ")->render(getMemDC());
}
