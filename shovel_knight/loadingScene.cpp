#include "pch.h"
#include "loadingScene.h"
#include "progressBar.h"

loadingScene::loadingScene()
	: _background(nullptr), 
	_loadingBar(nullptr),
	_currentCount(0)

{
}


loadingScene::~loadingScene()
{
}

HRESULT loadingScene::init()
{
	_background = IMAGEMANAGER->addImage("배경", "타르코프로딩.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	_loadingBar = new progressBar;
	_loadingBar->init(0, WINSIZEY - 50, WINSIZEX, 50);
	_loadingBar->setGauge(0, 0);

	CreateThread(
		NULL,				//스레드 보안속성(자식윈도우 존재할때)
		NULL,				//스레드의 스택크기(0이면 메인쓰레드 동일)
		threadFunction,		//사용할 함수
		this,				//스레드 매개변수(this 로 뒀으니 본 클래스)
		NULL,				//스레드 특성
		NULL);				//스레드 ID

	return S_OK;
}

void loadingScene::release()
{
	SAFE_DELETE(_loadingBar);
}

void loadingScene::update()
{
	_loadingBar->update();
	_loadingBar->setGauge(_currentCount, LOADINGMAX);

	if (_currentCount == LOADINGMAX)
	{
		//타르코프게임씬
		SCENEMANAGER->changeScene("인게임씬");
	}
}

void loadingScene::render()
{
	_background->render(getMemDC());
	_loadingBar->render();
}

DWORD CALLBACK threadFunction(LPVOID lpParameter)
{
	loadingScene* loadingHelper = (loadingScene*)lpParameter;

	//여러분은 그냥 여기에 사용하실 이미지랑 사운드 추가만 하면됩니다

	while (loadingHelper->_currentCount != LOADINGMAX)
	{
		IMAGEMANAGER->addImage("시작", "타르코프게임.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

		//이렇게 안하면 눈에 보이지도 않아요
		Sleep(1);

		loadingHelper->_currentCount++;
	}

	return 0;
}
