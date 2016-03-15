#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
#include "GameManager.h"
#include "FloorButton.h"
#include "cocostudio/CCComExtensionData.h"
USING_NS_CC;

class Door : public cocos2d::Node
{
private:
	cocos2d::Sprite* _sprite;
	FloorButton* _linkedButton;
	float _timer;
	bool _open;
public:
	Door();
	static Door* create();
	~Door();

	virtual bool init() override;

	void update(float delta);

	// Sprite
	Sprite* GetSprite() { return _sprite; };
	void SetSprite(std::vector<FloorButton*> buttons, Sprite* newSprite);

	// Door
	bool Open(float delta);
	bool Close(float delta);
};

