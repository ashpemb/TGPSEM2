#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

#include "stdio.h"
#include <iomanip>

#include "Scene1.h"
#include "AudioEngine.h"
#include "LevelSelect.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d;

class MenuScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MenuScene);

	void AudioTesting();

	// Touch input
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	//Button event
	void StartButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void SettingsButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void ExitButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void MuteButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);


	void StartGame();

private:
	cocos2d::ui::Button*    _startButton;
	cocos2d::ui::Button*    _settingsButton;
	cocos2d::ui::Button*    _exitButton;
	cocos2d::Sprite*		_muteButton;

	cocos2d::Sprite*		_background;

	Scene1 * gameScene;

	AudioEngine*			auEngine;
	bool muted;
};


#endif // __MENU_SCENE_H__
