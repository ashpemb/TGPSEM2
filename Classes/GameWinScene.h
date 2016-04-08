#ifndef __GAMEWIN_SCENE_H__
#define __GAMEWIN_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include "ScoreManager.h"

class GameWinScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameWinScene);

	void AudioTesting();

	// Touch input
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	//Button event
	void NextLevelButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void RetryButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void MainMenuButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void MuteButtonPressed();


private:

	cocos2d::Sprite*		_background;
	cocos2d::Sprite*		_rating;
	cocos2d::Sprite*		_time;
	cocos2d::Sprite*		_star1;
	cocos2d::Sprite*		_star2;
	cocos2d::Sprite*		_star3;
	cocos2d::Sprite*		_starDis;
	cocos2d::Sprite*		_muteButton;
	cocos2d::ui::Button*    _nextLevelButton;
	cocos2d::ui::Button*    _retryButton;
	cocos2d::ui::Button*    _mainMenuButton;

	cocos2d::ui::Text*	_timeLabel;

	AudioEngine*			auEngine;
	bool muted;

};

#endif // __GAMEWIN_SCENE_H__
