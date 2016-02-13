#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "GameManager.h"
USING_NS_CC;

class Player : public Node
{
private:
	// Sprite
	Sprite* _playerSprite;

	// Gravity variables
	float _gravity;

	bool _falling;
	float _lastVelocity;
	float _verticalVelocity;
	float _timeFalling;

	// Move to
	float _targetX;
	float _speed;

	void MoveToTarget(float deltaTime);
public:
	Player();
	static Player* create(float gravity);
	~Player();

	virtual bool init() override;
	
	void update(float delta);

	// Physics
	void SetGravity(float gravity);
	void SetFalling(bool falling);
	void FlipPlayer();

	void SetVelocity(float y);
	float GetVelocity() { return _verticalVelocity; };

	void CheckPlatformCollisions(cocos2d::Sprite* collider);
	void CheckWallCollisions(cocos2d::Sprite* collider);

	//Sprite
	Sprite* GetSprite() { return _playerSprite; };
	void SetSprite(Sprite* newSprite);

	void Land(cocos2d::Sprite* collider);
	void Fall(float delta);
	bool GetFalling() { return _falling; };

	// Movement
	void SetTarget(float target);
};

