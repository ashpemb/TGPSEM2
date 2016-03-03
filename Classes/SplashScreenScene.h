#ifndef __SPLASHSCREEN_SCENE_H__
#define __SPLASHSCREEN_SCENE_H__

#include "cocos2d.h"

class SplashScreenScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(SplashScreenScene);

private:
	void TimeMainMenu(float time);

	cocos2d::Sprite*		_background;

};

#endif // __SPLASHSCREEN_SCENE_H__
