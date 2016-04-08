#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "SceneManager.h"
#include "AudioEngine.h"
#include "MenuScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d;

class CreditsScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CreditsScene);

	void update(float delta);

	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	void ReturnButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);

	void ReturnToMenu();

	void RotatePlanet();

private:
	cocos2d::ui::Button* _returnButton;

	cocos2d::Sprite*				_logo;
	cocos2d::Sprite*				_planet;
	cocos2d::Sprite*				_ship;
	cocos2d::Sprite*				_creditsBG;
	std::vector<cocos2d::Sprite*>	_stars;

	float _rotatePlanetTimerDefault;
	float _rotatePlanetTimer;

	AudioEngine*			auEngine;
	bool muted;
};

