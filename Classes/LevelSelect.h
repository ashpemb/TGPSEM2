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
#define LEVELCOUNT 8
#define GALAXYCOUNT 14
class LevelSelect : public cocos2d::Layer
{
public:
	struct Level
	{
		int _ID;
		std::string _CustomLevelName;
		Sprite3D* _Sprite;
		Vec3 _Scale;
		float _Rotation;
		Vec3 _RotationAngles;
		bool _IsFocused;
		Vec3 _Destination;
		int _StarRating;
		std::string _BestTimeMinutes;
		std::string _BestTimeSeconds;
	};
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
	void onAcceleration(cocos2d::Acceleration* accel, cocos2d::Event* event);
	void update(float);
	void UpdateTimer(float dt);
	void GoToGameScene(cocos2d::Ref *sender);
	void LevelLeft(cocos2d::Ref *sender);
	void LevelRight(cocos2d::Ref *sender);
	void LevelMovement(); 
	void LevelRotation();
	void LevelScaling();
	TouchManager* touchMGR;
	float accelRotation;
	// implement the "static create()" method manually
	CREATE_FUNC(LevelSelect);
private:
	std::vector<Vec3> _LevelPositions;
	std::vector<Level> _AllLevels;
	Vec3 _FocusedDestination;
	float _LevelMovementSpeed;
	ScoreManager* _ScoreManager;
	int LevelSelected;
	Sprite* _InfoBox;
	Sprite* _Background;
	Size winSize;
	std::vector<ParticleGalaxy*> _GalaxyParticle;
	cocos2d::ui::Button*	_startGame;
	Label* labelTouchInfo;
	int screenSizeY;
	int screenSizeX;
};

#endif // __LevelSelect_SCENE_H__
