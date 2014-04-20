#include "SimpleDPad.h"

USING_NS_CC;

SimpleDPad::SimpleDPad(void)
{
	this->_delegate=NULL;
}


SimpleDPad::~SimpleDPad(void)
{
}

SimpleDPad* SimpleDPad::dPadWithFile(cocos2d::CCString* fileName,float radius)
{
	SimpleDPad* pRet=new SimpleDPad();

	if(pRet&&pRet->initWithFile(fileName,radius)){
		return pRet;
	}else{
		delete pRet;
		pRet=NULL;
		return NULL;
	}
}

bool SimpleDPad::initWithFile(cocos2d::CCString *filename,float radius)
{
	if(!CCSprite::initWithFile(filename->getCString())){
		return false;
	}

	this->_radius=radius;
	this->_direction=CCPointZero;
	this->_isHeld=false;
	this->scheduleUpdate();

	return true;
}

void SimpleDPad::onEnterTransitionDidFinish()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true); //ע�ᵥ�㴥��
}

void SimpleDPad::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this); //ע������
}

void SimpleDPad::update(float dt)
{
	if(this->_isHeld){
		this->_delegate->isHoldingDirector(this,this->_direction);
	}
}

bool SimpleDPad::ccTouchBegan(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent)
{
	float distanceSQ=ccpDistanceSQ(pTouch->getLocation(),this->getPosition()); //���������ƽ��

	if(distanceSQ<=this->_radius*this->_radius){
		this->updateDirectionForTouchLocation(pTouch->getLocation());
		this->_isHeld=true;
		return true;
	}

	return false;
}

void SimpleDPad::ccTouchMoved(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent)
{
	this->updateDirectionForTouchLocation(pTouch->getLocation());
}

void SimpleDPad::ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent)
{
	this->_direction=CCPointZero;
	this->_isHeld=false;
	this->_delegate->simpleDPadTouchEnded(this);
}

void SimpleDPad::updateDirectionForTouchLocation(cocos2d::CCPoint location)
{
	float radians=ccpToAngle(ccpSub(location,this->getPosition())); //�󻡶�ֵ
	float degrees=-1*CC_RADIANS_TO_DEGREES(radians); //���ݻ�����Ƕ�

	if(degrees>=-22.5&&degrees<=22.5){
		this->_direction=ccp(1,0);
	}
	else if(degrees>22.5&&degrees<67.5)
	{
		this->_direction=ccp(1,-1);
	}
	else if(degrees>=67.5&&degrees<=112.5){
		this->_direction=ccp(0,-1);
	}
	else if(degrees>112.5&&degrees<157.5){
		this->_direction=ccp(-1,-1);
	}
	else if(degrees>=157.5 || degrees<=-157.5){
		this->_direction=ccp(-1,0);
	}
	else if(degrees<-22.5&&degrees>-67.5){
		this->_direction=ccp(1,1);
	}
	else if(degrees<=-67.5&&degrees>=-112.5){
		this->_direction=ccp(0,1);
	}
	else if(degrees<-112.5&&degrees>-157.5){
		this->_direction=ccp(-1,1);
	}

	this->_delegate->didChangeDirectorTo(this,this->_direction);
}

