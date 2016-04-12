#ifndef __LEVELSELECT_SCENE_H__
#define __LEVELSELECT_SCENE_H__

#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include "SimpleAudioEngine.h"  
#include "cocos2d.h"
#include "SceneManager.h"
#include "TouchManager.h"
#include "ScalingObject.h"

#define Pi 3.14159265
#define LEVELCOUNT 4
#define GALAXYCOUNT 14

struct Level
{
	int			_ID;
	std::string _CustomLevelName;
	Sprite3D*	_Sprite;
	Vec3		_Scale;
	float		_Rotation;
	Vec3		_RotationAngles;
	bool		_IsFocused;
	Vec3		_Destination;
	int			_StarRating;
	std::string _BestTimeMinutes;
	std::string _BestTimeSeconds;
};

class LevelSelect : public cocos2d::Layer
{
private:
	// Level Data
	Sprite*				_InfoBox;
	std::vector<Vec3>	_LevelPositions;
	std::vector<Level>	_AllLevels;

	Vec3				_FocusedDestination;
	float				_LevelMovementSpeed;
	int					LevelSelected;

	int timer;

	// Buttons
	Label*					labelTouchInfo;
	cocos2d::ui::Button*	_backButton;

	// Background and screen
	cocos2d::Sprite*				_planet;
	cocos2d::Sprite*				_ship;
	std::vector<cocos2d::Sprite*>	_stars;
	int								screenSizeY;
	int								screenSizeX;

	float _rotatePlanetTimerDefault;
	float _rotatePlanetTimer;

	float _rotateShipTimerDefault;
	float _rotateShipTimer;
	bool _rotateShipLeft;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void update(float);
	void UpdateTimer(float dt);

	// Touch Methods
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
	void onAcceleration(cocos2d::Acceleration* accel, cocos2d::Event* event);

	void BackButtonPressed(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	// World Cube Methods
	void LevelLeft(cocos2d::Ref *sender);
	void LevelRight(cocos2d::Ref *sender);
	void LevelMovement(); 
	void LevelRotation();
	void LevelScaling();
	
	void GoToGameScene(cocos2d::Ref *sender);

	TouchManager* touchMGR;
	float accelRotation;

	// implement the "static create()" method manually
	CREATE_FUNC(LevelSelect);
};

#endif // __LevelSelect_SCENE_H__
