#pragma once
#include "gameNode.h"

#define SPEED 5.0f
#define WIDTH 55
#define HEIGHT 96

class character;
class object;

class mapCamera : public gameNode
{
private:
	int temp =0;
	// background image
	image* _background;
	image* _background_magenta;

	float _xPower, _yPower;

	character* _character;
	object* _object;

	// Camera
	RECT _camera;
	int _camX, _camY;				// 맵 전체의 left, top 값

	int _mapCountX;					// 맵을 1280으로 나눴을 때의 x값
	int _mapCountY;					// 맵을 720으로 나눴을 때의 y값

	bool _isCamMove;
	bool _isCamMove2;

	int _LR;
	int _TB = 0;

	bool _camFollowX;
	bool _camFollowY;

public:
	mapCamera();
	~mapCamera();

	virtual HRESULT init();			//초기화 함수
	virtual void release();			//메모리 해제 함슈
	virtual void update();			//연산하는 함수
	virtual void render();			//그리기 함수

	image* getBackGround() { return _background; }
	image* getBackGroundMagenta() { return _background_magenta; }

	int getCamX() { return _camX; }					//camX - 배경이 시작되는 left 좌표
	int getCamY() { return _camY; }					//camY - 배경이 시작되는 top 좌표

	bool getIsCamMove() { return _isCamMove; }

	float getSpeed() { return SPEED; }

	void SetCamera(RECT& camera, int left, int top, int width, int height);
	void CameraMove(RECT& camera);


	void MakeObject(RECT& rect, int left, int right, int top, int bottom);		// 그 외 오브젝트(카메라 이동)


	//void RendObject(RECT rect[], int max);
	//void RendObject(RECT& rect);

	void setCharacterAddressLink(character* character) { _character = character; }			// 캐릭터 클라스 링크 
	void setObjectMemoryAddressLink(object* object) { _object = object; }			// 오브젝트 클라스 링크 
};
