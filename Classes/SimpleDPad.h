#pragma once
#include "F:\yt\game\cocos2d-x-2.2\cocos2dx\sprite_nodes\CCSprite.h"
#include "cocos2d.h"

class SimpleDPad;
//���������
class SimpleDPadDelegate
{
public:
	//ִ�� ���߶���
	virtual void didChangeDirectorTo(SimpleDPad* simpleDPad,cocos2d::CCPoint direction)=0;  //���麯��
	virtual void isHoldingDirector(SimpleDPad* simpleDPad,cocos2d::CCPoint direction)=0;

	//����������������õĺ���
	virtual void simpleDPadTouchEnded(SimpleDPad* simpleDpad)=0;
};

class SimpleDPad : public cocos2d::CCSprite,public cocos2d::CCTargetedTouchDelegate
{
public:
	SimpleDPad(void);
	~SimpleDPad(void);

	static SimpleDPad* dPadWithFile(cocos2d::CCString* fileName,float radius);
	bool initWithFile(cocos2d::CCString *filename,float radius);

	void onEnterTransitionDidFinish();
	void onExit();

	void update(float dt);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);

	void updateDirectionForTouchLocation(cocos2d::CCPoint location);  //�����㷽��

	SimpleDPadDelegate* _delegate; //ί��

	bool _isHeld;

	float _radius; //�뾶
	cocos2d::CCPoint _direction;  //���� ����
};

