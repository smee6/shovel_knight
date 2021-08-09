#pragma once
#include "gameNode.h"


#define WIDTH 80
#define HEIGHT 100

class character;
class object;

class mapCamera : public gameNode
{
private:

	// background image
	image* _background;

	float _xPower, _yPower;

	character* _character;
	object* _object;

	// Camera
	RECT _camera;
	int _camX, _camY;

	int _mapCountX;
	int _mapCountY;

	bool _isCamMove;
	bool _isCamMove2;

	int _LR;
	int _TB;

	bool _camFollowX;
	bool _camFollowY;

public:
	mapCamera();
	~mapCamera();

	virtual HRESULT init();			//초기화 함수
	virtual void release();			//메모리 해제 함슈
	virtual void update();			//연산하는 함수
	virtual void render(HDC hdc);	//그리기 함수

	void Collision();



	void SetCamera(RECT& camera, int left, int top, int width, int height);
	void CameraMove(RECT& camera);


	void MakeObject(RECT& rect, int left, int right, int top, int bottom);		// 그 외 오브젝트(카메라 이동)


	//void RendObject(RECT rect[], int max);
	//void RendObject(RECT& rect);

	void setCharacterAddressLink(character* character) { _character = character; }			// 캐릭터 클라스 링크 
	void setObjectMemoryAddressLink(object* object) { _object = object; }			// 오브젝트 클라스 링크 
};
