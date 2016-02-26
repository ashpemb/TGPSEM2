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

	// Gravity variables
	float	_timeFalling;
	float	_gravity;

	bool	_orientationHorizontal;
	bool	_fallingHorizontal;
	float	_horizontalVelocityLast;
	float	_horizontalVelocity;

	bool	_orientationVertical;
	bool	_fallingVertical;
	float	_verticalVelocityLast;
	float	_verticalVelocity;

	// Box Variables
	int _boxType;
public:
	Box(int);
	static Box* create(int boxType);
	~Box();

	virtual bool init() override;
	void update(float delta);

	// Physics
	void SetGravity(float gravity);

	void SetOrientationVertical(bool orientation);
	void SetOrientationHorizontal(bool orientation);

	bool GetOrientationVertical() { return _fallingVertical; };
	bool GetOrientationHorizontal() { return _fallingHorizontal; };

	void SetFallingVertical(bool falling);
	void SetFallingHorizontal(bool falling);

	bool GetFallingVertical() { return _fallingVertical; };
	bool GetFallingHorizontal() { return _fallingHorizontal; };

	void SetVelocity(float y);
	float GetVelocity() { return _verticalVelocity; };
	void CheckPlatformCollisions(cocos2d::Sprite* collider);
	void CheckWallCollisions(cocos2d::Sprite* collider);
	void Flip();

	// Sprite
	Sprite* GetSprite() { return _box; };
	void SetSprite(Sprite* newSprite) { _box = newSprite; this->addChild(_box); };

	void Land(cocos2d::Sprite* collider);
	void Fall(float delta);
};

