#pragma once
#include "singletonBase.h"
#include <string>
#include <map>

class gameNode;

class sceneManager : public singletonBase<sceneManager>
{
private:
	typedef map<string, gameNode*>				mapSceneList;
	typedef map<string, gameNode*>::iterator	mapSceneIter;

private:
	static gameNode* _currentScene;

	mapSceneList _mSceneList;
	bool isUIcall;
	int _gold = 0;
	int _mana = 0;

public:
	sceneManager();
	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();

	gameNode* addScene(string sceneName, gameNode* scene);

	HRESULT changeScene(string sceneName);


	int hp = 0;
	void nowHP(int x);


	// SCENEMANAGER-> getGold() 같은 식으로 사용
	int getGold();
	void setGold(int x);

	int getMana();
	void setMana(int x);

};

