#pragma once
#include "F:\yt\game\cocos2d-x-2.2\cocos2dx\layers_scenes_transitions_nodes\CCLayer.h"
#include "cocos2d.h"
#include "SimpleDPad.h"

class HudLayer : public cocos2d::CCLayer
{
public:
	HudLayer(void);
	~HudLayer(void);

	virtual bool init();

	SimpleDPad* _dPad;

	CREATE_FUNC(HudLayer);

	void update(float dt);
};

