
#pragma once
#include "F:\yt\game\cocos2d-x-2.2\cocos2dx\layers_scenes_transitions_nodes\CCLayer.h"
#include "cocos2d.h"
#include "Robot.h"
#include "Hero.h"
#include "SimpleDPad.h"

class GameLayer :
	public cocos2d::CCLayer,public SimpleDPadDelegate
{
public:
	GameLayer(void);
	~GameLayer(void);
	virtual bool init();
	

	void initTiledmap();

	CREATE_FUNC(GameLayer); 

	cocos2d::CCSpriteBatchNode* _actors;

	Hero *_hero;
	cocos2d::CCTMXTiledMap* _tilemp;

	void initHero();

	virtual void registerWithTouchDispatcher(void);

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	//执行 行走动作
	virtual void didChangeDirectorTo(SimpleDPad* simpleDPad,cocos2d::CCPoint direction);
	virtual void isHoldingDirector(SimpleDPad* simpleDPad,cocos2d::CCPoint direction);

	//触摸方向键结束调用的函数
	virtual void simpleDPadTouchEnded(SimpleDPad* simpleDpad);

	void update(float dt);

	void initRobots();

	void updateRobots(float dt);//实现机器人如何抉择及做出相应动作的方法

	void endGame();
	void restartGame(CCObject* pSender);

	cocos2d::CCArray* _robots;
};
