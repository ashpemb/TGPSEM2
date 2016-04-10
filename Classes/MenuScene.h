#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "SceneManager.h"
#include "AudioEngine.h"
#include "LevelSelect.h"
#include "CreditsScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d;

class MenuScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MenuScene);

	void update(float delta);

	void AudioTesting();

	// Touch input
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	//Button event
	void StartButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void CreditsButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void ExitButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void MuteButtonPressed();


	void StartGame();
	void Credits();

	// Background
	void RotatePlanet();
private:
	cocos2d::ui::Button*    _startButton;
	cocos2d::ui::Button*    _creditsButton;
	cocos2d::ui::Button*    _exitButton;
	cocos2d::Sprite*		_muteButton;

	cocos2d::Sprite*				_logo;
	cocos2d::Sprite*				_planet;
	cocos2d::Sprite*				_ship;
	std::vector<cocos2d::Sprite*>	_stars;

	float _rotatePlanetTimerDefault;
	float _rotatePlanetTimer;

	AudioEngine*			auEngine;
	bool muted;
};


#endif // __MENU_SCENE_H__
