#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "stdio.h"
USING_NS_CC;

#include "Player.h"

class Scene1 : public cocos2d::Layer
{
private:
	Player* player;
public:
	Scene1();
	~Scene1();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	virtual void update(float delta);
};

