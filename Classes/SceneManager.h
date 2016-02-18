#ifndef __SCENE1_SCENE_H__
#define __SCENE1_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
#include "Player.h"
#include "GameManager.h"
#include "AudioEngine.h"
#include "Box.h"

USING_NS_CC;

class SceneManager : public Scene
{
private:
	int _level;

	// Sprites that need to be passed to classes
	Sprite* _playerSprite;
	std::vector<cocos2d::Sprite*> _woodenSprites;
	std::vector<cocos2d::Sprite*> _metalSprites;

	int _score;
	Player* _player;
	cocos2d::ui::Text*	_timeLabel;
	Sprite*	_background1;
	Sprite*	_background2;
	Sprite*	_background3;
	Sprite*	_background4;
	Sprite* _blackTransparency;
	cocos2d::ui::Button* _startGame;

	AudioEngine* auEngine;
	std::vector<cocos2d::Sprite*> _platforms;
	std::vector<cocos2d::Sprite*> _walls;
	std::vector<cocos2d::ui::CheckBox*> _gravSwitches;
	std::vector<bool> _flipped;

	std::vector<Box*> _woodBoxes;
	std::vector<Box*> _metalBoxes;

	// Gravity
	float _gravity = -3.81f;
	float _flipGravityCooldown = 1.0f;	// One second cooldown
	void FlipGravity();

	// Touches
	bool _inTouch;
	cocos2d::Vec2 _initialTouchPos;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static SceneManager* createScene(int level);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	SceneManager(int level);
	virtual bool init();
	~SceneManager();

	void SetupCocosElements();

	void CheckCollisions();

	void update(float delta);
	void ScheduleScore(float delta);
	void CheckNear();
	void IsPlayerInBounds();

	// implement the "static create()" method manually
	//CREATE_FUNC(Scene1);

	// Touch input
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	void SwitchPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void StartButtonPressed(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
};

#endif // __SCENE1_SCENE_H__
