#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "GameManager.h"
USING_NS_CC;


class Box : public Node
{
private:
	// Sprite
	Sprite* _box;


	// Gravity Variables
	float _gravity;
	bool _falling;
	float _lastVelocity;
	float _verticalVelocity;
	float _timeFalling;

	// Box Variables
	int _boxType;
public:
	Box(int);
	static Box* create(float gravity, int boxType);
	~Box();

	virtual bool init() override;
	void update(float delta);

	// Physics
	void SetGravity(float gravity);
	void SetFalling(bool falling);
	void SetVelocity(float y);
	float GetVelocity() { return _verticalVelocity; };
	void CheckPlatformCollisions(cocos2d::Sprite* collider);
	void CheckWallCollisions(cocos2d::Sprite* collider);
	void FlipPlayer();

	// Sprite
	Sprite* GetBoxSprite() { return _box; };
	void SetSprite(Sprite* newSprite) { _box = newSprite; this->addChild(_box); };

	void Land(cocos2d::Sprite* collider);
	void Fall(float delta);
	bool GetFalling() { return _falling; };
};

