#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"
USING_NS_CC;

class Player : public Node
{
private:
	// Sprite
	Sprite* _playerSprite;
	int _frame;
	bool _running;
	float _spriteTimer;
	float _spriteTimerDefault = 1.0f / 12.0f;

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

	// Move to
	Vec2	_targetPos;
	float	_speed;

	void MoveToTarget(float deltaTime);
public:
	Player();
	static Player* create();
	~Player();

	virtual bool init() override;
	
	void update(float delta);

	// Physics
	void SetGravity(float gravity);

	void SetOrientationVertical(bool orientation);
	void SetOrientationHorizontal(bool orientation);

	bool GetOrientationVertical() { return _orientationVertical; };
	bool GetOrientationHorizontal() { return _orientationHorizontal; };

	void SetFallingVertical(bool falling);
	void SetFallingHorizontal(bool falling);

	bool GetFallingVertical() { return _fallingVertical; };
	bool GetFallingHorizontal() { return _fallingHorizontal; };

	void FlipPlayer();

	void SetVelocity(float y);
	float GetVelocity() { return _verticalVelocity; };

	void CheckPlatformCollisions(cocos2d::Sprite* collider);
	void CheckWallCollisions(cocos2d::Sprite* collider);

	//Sprite
	Sprite* GetSprite() { return _playerSprite; };
	void SetSprite(Sprite* newSprite, Sprite* spawnPoint);

	void Land(cocos2d::Sprite* collider);
	void BreakFall() { _fallingHorizontal, _fallingVertical = false; };
	void Fall(float delta);

	// Movement
	void SetTarget(Vec2 target);
};

#endif