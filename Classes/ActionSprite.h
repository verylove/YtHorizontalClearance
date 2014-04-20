
#pragma once
#include "F:\yt\game\cocos2d-x-2.2\cocos2dx\sprite_nodes\CCSprite.h"
#include "cocos2d.h"

typedef enum _ActionState{
	kActionStateNone=0,
	kActionStateIdle,
	kActionStateAttack,
	kActionStateWalk,
	kActionStateHurt,
	kActionDead
}ActionState;

typedef struct{
	cocos2d::CCRect actual;
	cocos2d::CCRect original;
}BoundingBox;

class ActionSprite : public cocos2d::CCSprite
{
public:
	ActionSprite(void);
	~ActionSprite(void);

	//5种动作的方法
	void idle();  //空闲
	void attack();  //攻击
	void hurtWithDamage(float damage);  //受伤
	virtual void dead(); //死亡
	void walkWithDirection(cocos2d::CCPoint direction);  //行走

	void updateDesiredPosition(float dt);  //计算desiredPosition

	BoundingBox createBoundingBoxWithOrigin(cocos2d::CCPoint origin,cocos2d::CCSize size);
	void transformBoxes();

	void setPosition(cocos2d::CCPoint position);
	//动作
	cocos2d::CCAction* _idleAction;
	cocos2d::CCAction* _attackAction;
	cocos2d::CCAction* _walkAction;
	cocos2d::CCAction* _hurtAction;
	cocos2d::CCAction* _deadAction;

	ActionState _actionState;  //当前动作的状态

	float _walkSpeed;  //行走速率

	float _hitPoints;  //生命值

	float _damage;  //攻击伤害值

	cocos2d::CCPoint _velocity;  //这个是移动速度

	cocos2d::CCPoint _desiredPosition;  //要到的位置

	float _centerToSides;  //精灵中心到边的距离
	float _centerToBottom;  //精灵中心到底部的距离


	BoundingBox _hitBox; //精灵的身体
	BoundingBox _attackBox; //精灵的手

};
