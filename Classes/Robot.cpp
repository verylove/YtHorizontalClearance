#include "Robot.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Robot::Robot(void)
{
	this->_nextDecisionTime=0;
}


Robot::~Robot(void)
{
}

bool Robot::init()
{
	if(!ActionSprite::init()){
		return false;
	}

	if(!ActionSprite::initWithSpriteFrameName("robot_idle_00.png")){
		return false;
	}

	int i;

	//空闲动作
	CCArray* idleFrames=CCArray::createWithCapacity(5);
	for(i=0;i<5;i++){
		CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("robot_idle_%02d.png",i)->getCString());
		idleFrames->addObject(frame);
	}
	CCAnimation* idleAnimation=CCAnimation::createWithSpriteFrames(idleFrames,0.1f);
	this->_idleAction=CCRepeatForever::create(CCAnimate::create(idleAnimation));
	this->_idleAction->retain();

	//攻击动作
	CCArray* attackFrames=CCArray::createWithCapacity(5);
	for(i=0;i<5;i++){
		CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("robot_attack_%02d.png",i)->getCString());
		attackFrames->addObject(frame);
	}
	CCAnimation* attackAnimation=CCAnimation::createWithSpriteFrames(attackFrames,0.05f);
	this->_attackAction=CCSequence::create(CCAnimate::create(attackAnimation),CCCallFunc::create(this,callfunc_selector(Robot::idle)),NULL);
	this->_attackAction->retain();

	//行走动作
	CCArray* walkFrames=CCArray::createWithCapacity(6);
	for(i=0;i<6;i++){
		CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("robot_walk_%02d.png",i)->getCString());
		walkFrames->addObject(frame);
	}
	CCAnimation* walkAnimation=CCAnimation::createWithSpriteFrames(walkFrames,0.1f);
	this->_walkAction=CCRepeatForever::create(CCAnimate::create(walkAnimation));
	this->_walkAction->retain();

	//受伤动作
	CCArray* hurtFrames=CCArray::createWithCapacity(3);
	for(i=0;i<3;i++){
		CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("robot_hurt_%02d.png",i)->getCString());
		hurtFrames->addObject(frame);
	}
	CCAnimation* hurtAnimation=CCAnimation::createWithSpriteFrames(hurtFrames,0.1f);
	this->_hurtAction=CCSequence::create(CCAnimate::create(hurtAnimation),CCCallFunc::create(this,callfunc_selector(Robot::idle)),NULL);
	this->_hurtAction->retain();

	//死亡动作
	CCArray* deadFrames=CCArray::createWithCapacity(5);
	for(i=0;i<5;i++){
		CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("robot_knockout_%02d.png",i)->getCString());
		deadFrames->addObject(frame);
	}
	CCAnimation* deadAnimation=CCAnimation::createWithSpriteFrames(deadFrames,0.1f);
	this->_deadAction=CCSequence::create(CCAnimate::create(deadAnimation),CCBlink::create(2.0,10.0),NULL);  //CCBlink 闪烁动作
	this->_deadAction->retain();

	this->_walkSpeed=80;
	this->_centerToBottom=39;
	this->_centerToSides=29;
	this->_hitPoints=100;
	this->_damage=10;


	this->_hitBox=this->createBoundingBoxWithOrigin(ccp(-this->_centerToSides,-this->_centerToBottom),CCSizeMake(this->_centerToSides*2,this->_centerToBottom*2));
	this->_attackBox=this->createBoundingBoxWithOrigin(ccp(this->_centerToSides,-5),CCSizeMake(25,20));

	return true;
}

void Robot::dead()
{
	ActionSprite::dead();

	SimpleAudioEngine::sharedEngine()->playEffect("pd_botdeath.wav");

}