#include "ActionSprite.h"

USING_NS_CC;

ActionSprite::ActionSprite(void)
{
	this->_actionState=kActionStateNone;
}


ActionSprite::~ActionSprite(void)
{
}

void ActionSprite::idle()
{
	if(this->_actionState!=kActionStateIdle){
		this->stopAllActions();
		this->runAction(this->_idleAction);

		this->_actionState=kActionStateIdle;
		this->_velocity=CCPointZero;
	}
}

void ActionSprite::attack()
{
	if(this->_actionState==kActionStateIdle || this->_actionState==kActionStateAttack || this->_actionState==kActionStateWalk){
		this->stopAllActions();
		this->_actionState=kActionStateAttack;
		this->runAction(this->_attackAction);

	}
}

void ActionSprite::walkWithDirection(cocos2d::CCPoint direction)
{
	if(this->_actionState==kActionStateIdle){
		this->stopAllActions();
		this->runAction(this->_walkAction);
		this->_actionState=kActionStateWalk;
	}

	if(this->_actionState==kActionStateWalk){
		this->_velocity=ccp(direction.x*this->_walkSpeed,direction.y*this->_walkSpeed);

		if(this->_velocity.x>=0){
			this->setScaleX(1.0);
		}else{
			this->setScaleX(-1.0);
		}
	}
}

void ActionSprite::updateDesiredPosition(float dt)
{
	if(this->_actionState==kActionStateWalk){
		this->_desiredPosition=ccpAdd(this->getPosition(),ccpMult(this->_velocity,dt));
	}
}

BoundingBox ActionSprite::createBoundingBoxWithOrigin(cocos2d::CCPoint origin,cocos2d::CCSize size)
{
	BoundingBox boundingBox;

	boundingBox.original.origin=origin;
	boundingBox.original.size=size;

	boundingBox.actual.origin=ccpAdd(this->getPosition(),ccp(boundingBox.original.origin.x,boundingBox.original.origin.y));
	boundingBox.actual.size=size;

	return boundingBox;
}

void ActionSprite::transformBoxes()
{
	this->_hitBox.actual.origin=ccpAdd(this->getPosition(),ccp(_hitBox.original.origin.x,_hitBox.original.origin.y));

	_attackBox.actual.origin=ccpAdd(this->getPosition(),ccp(_attackBox.original.origin.x+(this->getScaleX()==-1?(-_attackBox.original.size.width-_hitBox.original.size.width):0),_attackBox.original.origin.y));

}

void ActionSprite::setPosition(CCPoint position)
{
	CCSprite::setPosition(position);
	this->transformBoxes();
}

void ActionSprite::hurtWithDamage(float damage)
{
	if(this->_actionState!=kActionDead){
		this->stopAllActions();
		_actionState=kActionStateHurt;
		this->runAction(_hurtAction);

		_hitPoints-=damage;

		if(_hitPoints<=0){
			this->dead();
		}
	}
}

void ActionSprite::dead()
{
	this->stopAllActions();
	this->runAction(_deadAction);
	_hitPoints=0;
	_actionState=kActionDead;
}