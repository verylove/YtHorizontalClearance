#pragma once
#include "actionsprite.h"

class Robot : public ActionSprite
{
public:
	Robot(void);
	~Robot(void);

	CREATE_FUNC(Robot);

	virtual bool init();
	virtual void dead();

	float _nextDecisionTime;  //�����˵ľ���ʱ��
};

