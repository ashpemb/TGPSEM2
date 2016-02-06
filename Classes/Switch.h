#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

class Switch : public Node
{
private:
	cocos2d::ui::CheckBox* _gravSwitch;
public:
	Switch(cocos2d::ui::CheckBox* checkBox);
	~Switch();
	static Switch* create(cocos2d::ui::CheckBox* checkBox);

	virtual bool init() override;

	void CheckNear(cocos2d::Sprite* player);
};

