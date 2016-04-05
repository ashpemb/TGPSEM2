#ifndef __Box_H__
#define __Box_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/CCComExtensionData.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"
#include "ScalingObject.h"
USING_NS_CC;


class Box : public Node
{
private:
	// Sprite
	Sprite* _box;

	// Gravity variables
	float	_defaultTouchTimer = 0.5f;
	float	_touchTimer;
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
	float _weight;

	// Scaler
	float scaler1;
	float scaler2;
	float scaler3;
	float scaler;
	float totalDiff;
	int unselect;
	bool isSelected;
public:
	Box(int, float, float);
	static Box* create(int boxType, float boxStartingWeight, float boxGravity);
	~Box();

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

	void SetVelocity(float y);
	float GetVelocity() { return _verticalVelocity; };
	void CheckPlatformCollisions(cocos2d::Sprite* collider);
	void CheckWallCollisions(cocos2d::Sprite* collider);

	void Flip();

	void Land(cocos2d::Sprite* collider);
	void Fall(float delta);

	float GetBoxWeight() { return _weight; }

	// Sprite
	Sprite* GetSprite() { return _box; };
	void SetSprite(Sprite* newSprite);

	// Scaling
	void SetTotalDiff(float totalDiffNew);
	void Selected();
	void Deselected();
	void SetStartingScale();
	void Collision(cocos2d::Touch* touch);
	void Scaling();
	//void UpdateBoxScale(){ _box->setScale(scaler); }
};

#endif