#ifndef __SPLASHSCREEN_SCENE_H__
#define __SPLASHSCREEN_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class SplashScreenScene : public cocos2d::LayerColor
{
private:
	void TimeMainMenu(float time);

	//cocos2d::Sprite*		_background;

	// Timers
	bool	_complete;
	float	_vikingTime;
	float	_davidTime;
	float	_ashTime;
	float	_alexTime;
	float	_joshTime;
	float	_jamesTime;
	float	_mattTime;
	float	_holdTime;

	// Sprites
	cocos2d::Sprite* _vikingLogo;
	cocos2d::Sprite* _david;
	cocos2d::Sprite* _ash;
	cocos2d::Sprite* _alex;
	cocos2d::Sprite* _josh;
	cocos2d::Sprite* _james;
	cocos2d::Sprite* _matt;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	void update(float delta);

	// implement the "static create()" method manually
	CREATE_FUNC(SplashScreenScene);

};

#endif // __SPLASHSCREEN_SCENE_H__
