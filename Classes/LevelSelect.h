#ifndef __LEVELSELECT_SCENE_H__
#define __LEVELSELECT_SCENE_H__

#include "cocos2d.h"
#include "TouchManager.h"
class LevelSelect : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
	void onAcceleration(cocos2d::Acceleration* accel, cocos2d::Event* event);
	void update(float);
	void UpdateTimer(float dt);
	void GoToGameScene(cocos2d::Ref *sender);
	void LevelLeft(cocos2d::Ref *sender);
	void LevelRight(cocos2d::Ref *sender);
	void LevelMovement(); 
	void LevelRotation();
	void LevelScaling();
	TouchManager* touchMGR;
	float accelRotation;
	// implement the "static create()" method manually
	CREATE_FUNC(LevelSelect);
private:
};

#endif // __LevelSelect_SCENE_H__
