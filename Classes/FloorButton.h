#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/CCComExtensionData.h"

#include "Player.h"
#include "Box.h"

USING_NS_CC;

class FloorButton : public Node
{
private:
	Sprite* _buttonSprite;
	cocos2d::ui::CheckBox* _gravSwitch;

	bool _activated;
	float _timer;

	// Button Orientation: 0 = Down; 1 = Left; 2 = Up; 3 = Right;
	int _orientation;
public:
	FloorButton(int direction);
	~FloorButton();
	static FloorButton* create(int direction);

	virtual bool init() override;

	void SetOrientation(int orientation) { _orientation = orientation; };
	int GetOrientation() { return _orientation; };

	// Active
	void SetActive(bool activeState);
	bool GetActive() { return _activated; };

	// Collisions
	void CheckPlayerCollisions(Player* collider);
	void CheckBoxCollisions(Box* collider);

	//Sprite
	Sprite* GetSprite() { return _buttonSprite; };
	void SetSprite(Sprite* newSprite);

	// Timer
	float GetTimer() { return _timer; };
};

