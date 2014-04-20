#include "GameScene.h"

USING_NS_CC;

GameScene::GameScene(void)
{
	this->_gameLayer=NULL;
	this->_hudLayer=NULL;
}


GameScene::~GameScene(void)
{
}

bool GameScene::init()
{
	if(!CCScene::init()){
		return false;
	}

	_gameLayer=GameLayer::create();

	this->addChild(_gameLayer);

	_hudLayer=HudLayer::create();
	this->addChild(_hudLayer);

	return true;
}
