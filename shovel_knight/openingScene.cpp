#include "pch.h"
#include "openingScene.h"

openingScene::openingScene()
{
}

openingScene::~openingScene()
{
}

HRESULT openingScene::init()
{
	IMAGEMANAGER->addImage("title1", "image/UI/open_1.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("title2", "image/UI/open_2.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("title3", "image/UI/open_3.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("title4", "image/UI/open_4.bmp", 1280, 720, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("option1", "image/UI/op_1.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("option2", "image/UI/op_2.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("option3", "image/UI/op_3.bmp", 1280, 720, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("sound1", "image/UI/sound_1.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("sound2", "image/UI/sound_2.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("sound3", "image/UI/sound_3.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("sound4", "image/UI/sound_4.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("sound5", "image/UI/sound_5.bmp", 1280, 720, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("bgm1", "image/UI/sound_1.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bgm2", "image/UI/sound_2.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bgm3", "image/UI/sound_3.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bgm4", "image/UI/sound_4.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bgm5", "image/UI/sound_5.bmp", 1280, 720, true, RGB(255, 0, 255));

	return S_OK;
}

void openingScene::release()
{
}

void openingScene::update()
{
	if (!isOption) {
		if (KEYMANAGER->isOnceKeyDown('S') && titleNum < 4) {
			titleNum += 1;
		}
		if (KEYMANAGER->isOnceKeyDown('W') && titleNum > 1) {
			titleNum -= 1;
		}
	}
	if (isOption) {
		if (KEYMANAGER->isOnceKeyDown('S') && optionNum < 3) {
			optionNum += 1;
		}
		if (KEYMANAGER->isOnceKeyDown('W') && optionNum > 1) {
			optionNum -= 1;
		}
	}

	if (!isOption) {
		switch (titleNum)
		{
		case 1:
			if (KEYMANAGER->isOnceKeyDown('J')) {

				SCENEMANAGER->changeScene("�ΰ��Ӿ�");

			}
		case 2:
			if (KEYMANAGER->isOnceKeyDown('J')) {


			}
		case 3:
			if (KEYMANAGER->isOnceKeyDown('J')) {
				isOption = true;

			}
		case 4:
			if (KEYMANAGER->isOnceKeyDown('J')) {
				PostQuitMessage(0);
			}
		}
	}
	
	if (isOption) {
		switch (optionNum)
		{
		case 1:
			if (KEYMANAGER->isOnceKeyDown('J')) {
				isOption = false;
			}
		case 2:
			if (KEYMANAGER->isOnceKeyDown('A') && sound > 1) {
				sound--;

			}
			if (KEYMANAGER->isOnceKeyDown('D') && sound < 5) {
				sound++;

			}


		case 3:
			if (KEYMANAGER->isOnceKeyDown('A') && bgm > 1) {
				bgm--;

			}
			if (KEYMANAGER->isOnceKeyDown('D') && bgm < 5) {
				bgm++;

			}
		}
	}



}

void openingScene::render()
{

	if (titleNum == 1) IMAGEMANAGER->findImage("title1")->render(getMemDC());
	if (titleNum == 2) IMAGEMANAGER->findImage("title2")->render(getMemDC());
	if (titleNum == 3) IMAGEMANAGER->findImage("title3")->render(getMemDC());
	if (titleNum == 4) IMAGEMANAGER->findImage("title4")->render(getMemDC());

	if (isOption) {
		if (optionNum == 1) IMAGEMANAGER->findImage("option1")->render(getMemDC());
		if (optionNum == 2) IMAGEMANAGER->findImage("option2")->render(getMemDC());
		if (optionNum == 3) IMAGEMANAGER->findImage("option3")->render(getMemDC());

		if (sound == 1) IMAGEMANAGER->findImage("sound1")->render(getMemDC());
		if (sound == 2) IMAGEMANAGER->findImage("sound2")->render(getMemDC());
		if (sound == 3) IMAGEMANAGER->findImage("sound3")->render(getMemDC());
		if (sound == 4) IMAGEMANAGER->findImage("sound4")->render(getMemDC());
		if (sound == 5) IMAGEMANAGER->findImage("sound5")->render(getMemDC());

		if (bgm == 1) IMAGEMANAGER->findImage("bgm1")->render(getMemDC(), 0, 47);
		if (bgm == 2) IMAGEMANAGER->findImage("bgm2")->render(getMemDC(), 0, 47);
		if (bgm == 3) IMAGEMANAGER->findImage("bgm3")->render(getMemDC(), 0, 47);
		if (bgm == 4) IMAGEMANAGER->findImage("bgm4")->render(getMemDC(), 0, 47);
		if (bgm == 5) IMAGEMANAGER->findImage("bgm5")->render(getMemDC(), 0, 47);
	}



}
