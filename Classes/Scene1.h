#ifndef __SCENE1_SCENE_H__
#define __SCENE1_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
#include "Player.h"
#include "SimpleAudioEngine.h"
#include "GameManager.h"

class Scene1 : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void update(float delta);
	void ScheduleScore(float delta);

	// implement the "static create()" method manually
	CREATE_FUNC(Scene1);

private:
	Player* player;

	// Gravity
	const float gravity = -9.81f;

	cocos2d::ui::Text*	 TimeLabel;

};

#endif // __SCENE1_SCENE_H__
