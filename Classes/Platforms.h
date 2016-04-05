#ifndef __Platforms_SCENE_H__
#define __Platforms_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
#include "GameManager.h"
#include "Player.h"
#include "Box.h"
USING_NS_CC;

class Platforms : public cocos2d::Node
{
private:
	Player* _playerRef;
	std::vector<Box*> _woodenRef;
	std::vector<Box*> _metalRef;

	Sprite* _movingPlat;
	Sprite* _touchZone;
	float OScale;
	float NScale;
	float OTouchZoneYScale;
	float NTouchZoneYScale;

	int platformType = 0; // Gives platform types a number: 1 = Horizontal, 2 = vertical, 0 = no platform
	bool CollidingWithWall;
	bool CollidingWithPlatform;

public:
	Platforms(Player* ref, std::vector<Box*> wood, std::vector<Box*> metal);
	static Platforms* create(Player* ref, std::vector<Box*> wood, std::vector<Box*> metal);
	~Platforms();

	virtual bool init() override;

	void update(float delta);

	void SetSprite(Sprite* newSprite);
	void setZoneSprite();
	Sprite* getSprite() { return _movingPlat; };
	Sprite* getTouchZone() { return _touchZone; }

	void PlatformType(int type) { platformType = type; }

	void Selected();
	void UnSelected();

	void MovePlatformHorizontal(cocos2d::Vec2 T);
	void MovePlatformVertical(cocos2d::Vec2 T);

	void CheckPlatformCollisions(cocos2d::Sprite* collider);
	void CheckWallCollisions(cocos2d::Sprite* collider);

	cocos2d::Sprite* sprite;

	// Touch input
	virtual void onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

};

#endif // __Platforms_SCENE_H__