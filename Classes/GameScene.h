#pragma once
#include "F:\yt\game\cocos2d-x-2.2\cocos2dx\layers_scenes_transitions_nodes\CCScene.h"
#include "cocos2d.h"
#include "GameLayer.h"
#include "HudLayer.h"

class GameScene : public cocos2d::CCScene
{
public:
	GameScene(void);
	~GameScene(void);

	virtual bool init();

	CREATE_FUNC(GameScene);  //create����  ���޲� ����ֵGameScene���͵�static������

	GameLayer* _gameLayer;
	HudLayer* _hudLayer;

};

