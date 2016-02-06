#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

class Player : public Node
{
private:
	// Sprite
	Sprite* _husky2;

	// Gravity variables
	float _gravity;

	bool _falling;
	float _lastVelocity;
	float _verticalVelocity;
	float _timeFalling;

	// Move to
	float _targetX;

	void moveToTarget();
public:
	Player();
	static Player* create(float gravity);
	~Player();

	virtual bool init() override;
	
	void update(float delta);

	// Physics
	void setGravity(float gravity);

	void setVelocity(float y);
	float getVelocity() { return _verticalVelocity; };

	void checkCollisions(cocos2d::Sprite* collider);

	//Sprite
	Sprite* getSprite() { return _husky2; };

	void land(cocos2d::Sprite* collider);
	void fall(float delta);
	bool getFalling() { return _falling; };
};

