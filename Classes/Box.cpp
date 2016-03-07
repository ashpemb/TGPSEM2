#include "Box.h"


Box::Box(int boxType)
{
	_boxType = boxType;
}

Box* Box::create(int boxType)
{
	Box* box = new Box(boxType);
	if (!box->init())
	{
		return nullptr;
	}

	box->SetGravity(-3.81f);
	box->SetOrientationVertical(true);
	box->SetOrientationHorizontal(false);
	box->autorelease();

	return box;
}

Box::~Box()
{
}

bool Box::init()
{
	if (!Node::init()) {
		return false;
	}

	this->scheduleUpdate();

	return true;
}

void Box::update(float delta)
{
	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		if (_fallingHorizontal || _fallingVertical) {
			Fall(delta);
		}
	}
	// Add box movement when pushed by player
}

void Box::SetGravity(float gravity)
{
	_gravity = gravity;
}

void Box::SetVelocity(float y)
{
	_verticalVelocity = y;
}

void Box::CheckPlatformCollisions(cocos2d::Sprite* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
	float scaledHeight = collider->getContentSize().height * collider->getScaleY();
	float scaledPlayerWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();

	if (_orientationVertical) {
		if (GetSprite()->getPositionX() - (GetSprite()->getContentSize().width / 2) < collider->getPositionX() + (scaledWidth / 2)
			&& GetSprite()->getPositionX() + (GetSprite()->getContentSize().width / 2) > collider->getPositionX() - (scaledWidth / 2)
			&& GetSprite()->getPositionY() - (GetSprite()->getContentSize().height / 2) < collider->getPositionY() + (scaledHeight / 2)
			&& GetSprite()->getPositionY() + (GetSprite()->getContentSize().height / 2) > collider->getPositionY() - (scaledHeight / 2))
		{
			Land(collider);
		}
		else {
			_fallingVertical = true;
		}
	}
	else if (_orientationHorizontal) {
		if (GetSprite()->getPositionX() - (GetSprite()->getContentSize().height / 2) < collider->getPositionX() + (scaledWidth / 2)
			&& GetSprite()->getPositionX() + (GetSprite()->getContentSize().height / 2) > collider->getPositionX() - (scaledWidth / 2)
			&& GetSprite()->getPositionY() - (GetSprite()->getContentSize().width / 2) < collider->getPositionY() + (scaledHeight / 2)
			&& GetSprite()->getPositionY() + (GetSprite()->getContentSize().width / 2) > collider->getPositionY() - (scaledHeight / 2))
		{
			if (GetSprite()->getPositionY() < collider->getPositionY()) {
				GetSprite()->setPositionY(collider->getPositionY() - (scaledHeight / 2) - (scaledPlayerWidth / 2));
			}
			else {
				GetSprite()->setPositionY(collider->getPositionY() + (scaledHeight / 2) + (scaledPlayerWidth / 2));
			}
		}
	}
}

void Box::CheckWallCollisions(cocos2d::Sprite* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
	float scaledHeight = collider->getContentSize().height * collider->getScaleY();
	float scaledPlayerWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();

	if (_orientationVertical) {
		if (GetSprite()->getPositionX() - (GetSprite()->getContentSize().width / 2) < collider->getPositionX() + (scaledWidth / 2)
			&& GetSprite()->getPositionX() + (GetSprite()->getContentSize().width / 2) > collider->getPositionX() - (scaledWidth / 2)
			&& GetSprite()->getPositionY() - (GetSprite()->getContentSize().height / 2) < collider->getPositionY() + (scaledHeight / 2)
			&& GetSprite()->getPositionY() + (GetSprite()->getContentSize().height / 2) > collider->getPositionY() - (scaledHeight / 2))
		{
			if (GetSprite()->getPositionX() < collider->getPositionX()) {
				GetSprite()->setPositionX(collider->getPositionX() - (scaledWidth / 2) - (scaledPlayerWidth / 2));
			}
			else {
				GetSprite()->setPositionX(collider->getPositionX() + (scaledWidth / 2) + (scaledPlayerWidth / 2));
			}
		}
	}
	else if (_orientationHorizontal) {
		if (GetSprite()->getPositionX() - (GetSprite()->getContentSize().height / 2) < collider->getPositionX() + (scaledWidth / 2)
			&& GetSprite()->getPositionX() + (GetSprite()->getContentSize().height / 2) > collider->getPositionX() - (scaledWidth / 2)
			&& GetSprite()->getPositionY() - (GetSprite()->getContentSize().width / 2) < collider->getPositionY() + (scaledHeight / 2)
			&& GetSprite()->getPositionY() + (GetSprite()->getContentSize().width / 2) > collider->getPositionY() - (scaledHeight / 2))
		{
			Land(collider);
		}
		else {
			_fallingHorizontal = true;
		}
	}
}

void Box::Land(cocos2d::Sprite* collider)
{
	if (_fallingVertical) {
		_fallingVertical = false;

		// Cancel velocity
		_verticalVelocity = 0.0f;
		_verticalVelocityLast = 0.0f;
		_timeFalling = 0.0f;

		// Make sure the position is set so not inside platform
		float newY;
		float scaledColliderHeight = collider->getContentSize().height * collider->getScaleY();
		float scaledPlayerHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();

		if (_gravity < 0.0f) {
			newY = (collider->getPositionY() + (scaledColliderHeight / 2)) + (scaledPlayerHeight / 2);
		}
		else {
			newY = (collider->getPositionY() - (scaledColliderHeight / 2)) - (scaledPlayerHeight / 2);
		}

		GetSprite()->setPosition(Vec2(GetSprite()->getPositionX(), newY));
	}
	else if (_fallingHorizontal) {
		_fallingHorizontal = false;

		// Cancel velocity
		_horizontalVelocity = 0.0f;
		_horizontalVelocityLast = 0.0f;
		_timeFalling = 0.0f;

		// Make sure the position is set so not inside platform
		float newX;
		float scaledColliderWidth = collider->getContentSize().width * collider->getScaleX();
		float scaledPlayerHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();

		if (_gravity > 0.0f) {
			newX = (collider->getPositionX() - (scaledColliderWidth / 2)) - (scaledPlayerHeight / 2);
		}
		else {
			newX = (collider->getPositionX() + (scaledColliderWidth / 2)) + (scaledPlayerHeight / 2);
		}

		GetSprite()->setPosition(Vec2(newX, GetSprite()->getPositionY()));
	}
}

void Box::Fall(float delta)
{
	if (_orientationVertical) {
		if (_fallingVertical) {
			// Update falling time
			_timeFalling += delta;

			// Calculate and set new velocity
			if (_verticalVelocity > -12.0f) {
				_verticalVelocity = _verticalVelocityLast + ((_gravity / 2) * _timeFalling);
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x, GetSprite()->getPosition().y + _verticalVelocity));
			}
			else if (_verticalVelocity < 12.0f) {
				_verticalVelocity = _verticalVelocityLast + ((_gravity / 2) * _timeFalling);
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x, GetSprite()->getPosition().y + _verticalVelocity));
			}
			else {
				_verticalVelocity = -9.0f;
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x, GetSprite()->getPosition().y + _verticalVelocity));
			}

			// Update last velocity
			_verticalVelocityLast = _verticalVelocity;
		}
		else {
			_fallingVertical = true;
			_verticalVelocityLast = 0.0f;
			_verticalVelocity = 0.0f;
			_timeFalling = 0.0f;
		}
	}
	else if (_orientationHorizontal) {
		if (_fallingHorizontal) {
			// Update falling time
			_timeFalling += delta;

			// Calculate and set new velocity
			if (_horizontalVelocity > -12.0f) {
				_horizontalVelocity = _horizontalVelocityLast + ((_gravity / 2) * _timeFalling);
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x + _horizontalVelocity, GetSprite()->getPosition().y));
			}
			else if (_horizontalVelocity < 12.0f) {
				_horizontalVelocity = _horizontalVelocityLast + ((_gravity / 2) * _timeFalling);
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x + _horizontalVelocity, GetSprite()->getPosition().y));
			}
			else {
				_horizontalVelocity = -9.0f;
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x + _horizontalVelocity, GetSprite()->getPosition().y));
			}

			// Update last velocity
			_horizontalVelocityLast = _horizontalVelocity;
		}
		else {
			_fallingHorizontal = true;
			_verticalVelocityLast = 0.0f;
			_verticalVelocity = 0.0f;
			_timeFalling = 0.0f;
		}
	}
}

void Box::SetOrientationVertical(bool orientation)
{
	_orientationVertical = orientation;
}

void Box::SetOrientationHorizontal(bool orientation)
{
	_orientationHorizontal = orientation;
}

void Box::SetFallingVertical(bool falling)
{
	_fallingVertical = falling;
	_orientationVertical = falling;

	_fallingHorizontal = !falling;
	_orientationHorizontal = !falling;

	Flip();
}

void Box::SetFallingHorizontal(bool falling)
{
	_fallingHorizontal = falling;
	_orientationHorizontal = falling;

	_fallingVertical = !falling;
	_orientationVertical = !falling;

	Flip();
}

void Box::Flip()
{
	if (_fallingVertical) {
		if (_gravity < 0)
		{
			auto rotateTo = RotateTo::create(0.5f, 0.0f);
			GetSprite()->runAction(rotateTo);
		}
		else if (_gravity > 0)
		{
			auto rotateTo = RotateTo::create(0.5f, 180.0f);
			GetSprite()->runAction(rotateTo);
		}
	}
	else if (_fallingHorizontal) {
		if (_gravity < 0)
		{
			auto rotateTo = RotateTo::create(0.5f, 90.0f);
			GetSprite()->runAction(rotateTo);
		}
		else if (_gravity > 0)
		{
			auto rotateTo = RotateTo::create(0.5f, 270.0f);
			GetSprite()->runAction(rotateTo);
		}
	}
}