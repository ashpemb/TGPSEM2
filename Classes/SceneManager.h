#ifndef __SCENE1_SCENE_H__
#define __SCENE1_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
#include "Player.h"
#include "GameManager.h"
#include "AudioEngine.h"
#include "GameOverScene.h"
#include "Box.h"
#include "Switch.h"
#include "Platforms.h"

USING_NS_CC;

class SceneManager : public Scene
{
private:
	int _level;

	// Sprites that need to be passed to classes
	Sprite* _playerSprite;
	std::vector<cocos2d::Sprite*> _woodenSprites;
	std::vector<cocos2d::Sprite*> _metalSprites;
	std::vector<cocos2d::ui::CheckBox*> _gravSwitches;

	std::vector<cocos2d::Sprite*> _movingPlatformVertSprites;
	std::vector<cocos2d::Sprite*> _movingPlatformHorizSprites;

	int _score;
	Player* _player;
	cocos2d::ui::Text*	_timeLabel;

	Sprite*	_background1;
	Sprite*	_background2;
	Sprite*	_background3;
	Sprite*	_background4;
	Sprite* _blackTransparency;
	Sprite* _topHighlight;
	Sprite* _rightHighlight;
	Sprite* _bottomHighlight;
	Sprite* _leftHighlight;

	cocos2d::ui::Button* _startGame;

	AudioEngine* auEngine;
	std::vector<cocos2d::Sprite*> _platforms;
	std::vector<cocos2d::Sprite*> _walls;
	std::vector<cocos2d::ui::CheckBox*> _exit;
	std::vector<bool> _flipped;

	std::vector<Box*> _woodBoxes;
	std::vector<Box*> _metalBoxes;
	std::vector<Switch*> _switches;
	std::vector<Platforms*> _movingPlatformsVert;
	std::vector<Platforms*> _movingPlatformsHoriz;

	// GRAVITY
	// Gravity Orientation: 0 = Down; 1 = Left; 2 = Up; 3 = Right;
	int _gravityOrientation;
	float _flipGravityCooldown = 1.0f;	// One second cooldown
	void FlipGravity(int direction);

	// Touches
	bool _inTouch;
	cocos2d::Vec2 _initialTouchPos;
	bool isObjectTouched;

public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static SceneManager* createScene(int level);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	SceneManager(int level);
	virtual bool init();
	~SceneManager();

	void SetupTimer(Node* root);
	void SetupAudio(Node* root);
	void SetupButtons(Node* root);
	void SetupSprites(Node* root);
	void SetupBackground(Node* root);
	void SetupHighlights(Node* root);
	void SetupClasses();

	void CheckCollisions();

	void update(float delta);
	void ScheduleScore(float delta);
	void CheckNear();
	void CheckNearDoor();
	void IsPlayerInBounds();

	// implement the "static create()" method manually
	//CREATE_FUNC(Scene1);

	// Touch input
	void SetupTouches();
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	void SwitchPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void StartButtonPressed(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

};

#endif // __SCENE1_SCENE_H__
