#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"

class GameOverScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);

	void AudioTesting();

	// Touch input
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	//Button event
	void RetryButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void MainMenuButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void MuteButtonPressed();


private:

	cocos2d::Sprite*		_background;
	cocos2d::Sprite*		_muteButton;
	cocos2d::ui::Button*    _retryButton;
	cocos2d::ui::Button*    _mainMenuButton;

	std::vector<cocos2d::Sprite*>	_stars;

	AudioEngine*			auEngine;
	bool muted;

};

#endif // __GAMEOVER_SCENE_H__
