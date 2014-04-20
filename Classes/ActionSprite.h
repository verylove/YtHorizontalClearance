
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

	//5�ֶ����ķ���
	void idle();  //����
	void attack();  //����
	void hurtWithDamage(float damage);  //����
	virtual void dead(); //����
	void walkWithDirection(cocos2d::CCPoint direction);  //����

	void updateDesiredPosition(float dt);  //����desiredPosition

	BoundingBox createBoundingBoxWithOrigin(cocos2d::CCPoint origin,cocos2d::CCSize size);
	void transformBoxes();

	void setPosition(cocos2d::CCPoint position);
	//����
	cocos2d::CCAction* _idleAction;
	cocos2d::CCAction* _attackAction;
	cocos2d::CCAction* _walkAction;
	cocos2d::CCAction* _hurtAction;
	cocos2d::CCAction* _deadAction;

	ActionState _actionState;  //��ǰ������״̬

	float _walkSpeed;  //��������

	float _hitPoints;  //����ֵ

	float _damage;  //�����˺�ֵ

	cocos2d::CCPoint _velocity;  //������ƶ��ٶ�

	cocos2d::CCPoint _desiredPosition;  //Ҫ����λ��

	float _centerToSides;  //�������ĵ��ߵľ���
	float _centerToBottom;  //�������ĵ��ײ��ľ���


	BoundingBox _hitBox; //���������
	BoundingBox _attackBox; //�������

};
