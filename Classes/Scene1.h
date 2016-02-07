#ifndef __SCENE1_SCENE_H__
#define __SCENE1_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
#include "Player.h"
#include "GameManager.h"
#include "AudioEngine.h"

class Scene1 : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	~Scene1();

	void CheckCollisions();

	void update(float delta);
	void ScheduleScore(float delta);
	void CheckNear();
	void IsPlayerInBounds();

	// implement the "static create()" method manually
	CREATE_FUNC(Scene1);

	// Touch input
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	void SwitchPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
private:
	int _score;
	Player* _player;
	cocos2d::ui::Text*	_timeLabel;
	Sprite*	_background1;
	Sprite*	_background2;
	Sprite*	_background3;
	Sprite*	_background4;
	Sprite* _blackTransparency;
	Sprite* _startPos;

	AudioEngine* auEngine;
	std::vector<cocos2d::Sprite*> _platforms;
	std::vector<cocos2d::Sprite*> _walls;
	std::vector<cocos2d::ui::CheckBox*> _gravSwitches;
	std::vector<bool> _flipped;

	// Gravity
	float _gravity = -3.81f;
	float _flipGravityCooldown = 1.0f;	// One second cooldown
	void FlipGravity();

	// Touches
	bool _inTouch;
	cocos2d::Vec2 _initialTouchPos;
};

#endif // __SCENE1_SCENE_H__
