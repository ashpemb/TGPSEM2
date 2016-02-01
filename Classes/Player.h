#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

class Player : public Node
{
public:
	Player();
	static Player* create();
	~Player();

	virtual bool init() override;
	
	void Update(float delta);
};

