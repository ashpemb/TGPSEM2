#pragma once
#include "cocos2d.h"
#include "TouchManager.h"
#include "ScalingObject.h"


using namespace cocos2d;

class InfoBox
{
public:
	ScalingObject* infoBoxBackground;
	Label* label;
	InfoBox();
	~InfoBox();
	void SetPosition(Vec2 infoboxPos);
	void SetInfoBoxBackground(std::string imageLoc);
	void SetText(std::string InfoboxText);
};