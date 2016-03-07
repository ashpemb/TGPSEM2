#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

class Switch : public Node
{
private:
	cocos2d::ui::CheckBox* _gravSwitch;

	// Gravity Orientation: 0 = Down; 1 = Left; 2 = Up; 3 = Right;
	int _orientation;
public:
	Switch();
	~Switch();
	static Switch* create();

	virtual bool init() override;

	void SetOrientation(int orientation) { _orientation = orientation; };
	int GetOrientation() { return _orientation; };

	void CheckNear(cocos2d::Sprite* player);

	//Sprite
	cocos2d::ui::CheckBox* GetSprite() { return _gravSwitch; };
	void SetSprite(cocos2d::ui::CheckBox* newSprite);
};

