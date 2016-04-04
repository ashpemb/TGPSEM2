#ifndef __SCENEMANAGER_SCENE_H__
#define __SCENEMANAGER_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"
#include "TouchManager.h"
#include "AudioEngine.h"

#include "GameWinScene.h"
#include "GameOverScene.h"

#include "Player.h"
#include "Platforms.h"
#include "Wall.h"
#include "Door.h"
#include "Switch.h"
#include "SwitchTimer.h"

USING_NS_CC;

class SceneManager : public cocos2d::Layer
{
private:
	int _level;
	int _score;

	// AUDIO
	AudioEngine* _auEngine;

	// TOUCHES
	TouchManager*	_touchMGR;
	bool			_inTouch;
	cocos2d::Vec2	_initialTouchPos;
	bool			_isObjectTouched;

	// UI
	cocos2d::ui::Text*		_timeLabel;
	cocos2d::ui::Button*	_startGame;

	// BACKGROUND
	cocos2d::Sprite*	_background1;
	cocos2d::Sprite*	_background2;
	cocos2d::Sprite*	_background3;
	cocos2d::Sprite*	_background4;
	cocos2d::Sprite* _blackTransparency;

	// LEVEL ELEMENTS
	std::vector<cocos2d::Sprite*>		_platforms;
	std::vector<cocos2d::Sprite*>		_walls;
	std::vector<cocos2d::Sprite*>		_solidDoorSprites;
	std::vector<cocos2d::ui::CheckBox*> _exit;
	std::vector<cocos2d::Sprite*>		_railStart;			// DEBUG NOTE: Rails are not yet complete
	std::vector<cocos2d::Sprite*>		_railEnd;			// DEBUG NOTE: Rails are not yet complete
	std::vector<cocos2d::Sprite*>		_woodCrateSpawn;	// DEBUG NOTE: Spawn locations are for debugging only
	std::vector<cocos2d::Sprite*>		_metalCrateSpawn;	// DEBUG NOTE: Spawn locations are for debugging only
	cocos2d::Sprite*					_playerSpawn;		// DEBUG NOTE: Spawn locations are for debugging only

	Player*						_player;
	std::vector<Box*>			_woodBoxes;
	std::vector<Box*>			_metalBoxes;
	std::vector<Platforms*>		_movingPlatformsVertical;
	std::vector<Platforms*>		_movingPlatformsHorizontal;
	std::vector<Wall*>			_movingWallsVertical;
	std::vector<Wall*>			_movingWallsHorizontal;
	std::vector<FloorButton*>	_buttons;
	std::vector<Door*>			_doors;
	std::vector<Door*>			_hatches;
	std::vector<Switch*>		_switches;
	std::vector<SwitchTimer*>	_timedSwitches;

	// GRAVITY
	int		_previousDirection;
	int		_gravityOrientation;			// Gravity Orientation: 0 = Down; 1 = Left; 2 = Up; 3 = Right;
	bool	_gravityHighlight;
	float	_flipGravityCooldown = 1.0f;	// One second cooldown	
	
	// GRAVITY HIGHLIGHTS
	cocos2d::Sprite*	_topHighlight;
	float				_topHighlightTime;
	bool				_topHighlightEnabled;

	cocos2d::Sprite*	_rightHighlight;
	float				_rightHighlightTime;
	bool				_rightHighlightEnabled;

	cocos2d::Sprite*	_bottomHighlight;
	float				_bottomHighlightTime;
	bool				_bottomHighlightEnabled;

	cocos2d::Sprite*	_leftHighlight;
	float				_leftHighlightTime;
	bool				_leftHighlightEnabled;

private:
	// SETUP CLASSES
	bool SetupTimer(Node* root);
	bool SetupAudio(Node* root);
	bool SetupButtons(Node* root);
	bool SetupBackground(Node* root);
	bool SetupHighlights(Node* root);

	bool SetupPlayer(Node* root);
	bool SetupPlatforms(Node* root);
	bool SetupWalls(Node* root);
	bool SetupBoxes(Node* root);
	bool SetupFloorButtons(Node* root);
	bool SetupDoors(Node* root);
	bool SetupSwitches(Node* root);
	bool SetupExits(Node* root);
	bool SetupRails(Node* root);
	bool SetupSpawns(Node* root); // TESTING

	// TOUCH CLASSES
	bool SetupTouches();

	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	void SwitchPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void DoorPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void StartButtonPressed(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void SwitchTimerPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);

	// LEVEL DATA
	bool SetLevel(int level) { if (_level = level) return true; };
	bool SetGravityOrientation(int orientation) { if (_gravityOrientation = orientation) return true; };

	void FlipGravity(int direction);
	void RevertGravity();

	void CheckCollisions();
	void CheckNear(float delta);
	void CheckNearTimer(float delta);
	void CheckNearDoor(float delta);
	void IsPlayerInBounds();
	void IsCrateInBounds();
public:
	static cocos2d::Scene* createScene(int level);
	CREATE_FUNC(SceneManager);
	virtual bool init();

	bool SetupNode();
	void update(float delta);
	
	void ScheduleScore(float delta);
};

#endif // __SCENEMANAGER_SCENE_H__