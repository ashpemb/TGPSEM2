
#include "Box.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Box::Box(int boxType, float boxStartingWeight, float gravity)
{
	_boxType = boxType;
	scaler1 = 0;
	scaler2 = 0;
	scaler3 = 0;
	scaler = 0;
	unselect = 0;
	isSelected = false;
	_touchTimer = 0.0f;
	_weight = boxStartingWeight;
	SetStartingScale();
	SetGravity(gravity);
	SetOrientationVertical(true);
	//SetFallingVertical(true);
	SetOrientationHorizontal(false);
	//SetFallingHorizontal(false);

	// Default node stuff
	//setPosition(Vec2(0.0f, 0.0f));
}

Box* Box::create(int boxType, float boxStartingWeight, float boxGravity)
{
	Box* box = new Box(boxType, boxStartingWeight, boxGravity);
	if (!box->init())
	{
		return nullptr;
	}

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
		if (_touchTimer > 0.0f) {
			_touchTimer -= delta;
		}

		if (_fallingHorizontal || _fallingVertical) {
			Fall(delta);
		}

		//_box->setScale(scaler2);
		//_weight = 2.0f;
		Scaling();
		//UpdateBoxScale();
	}
	// Add box movement when pushed by player
}

void Box::SetStartingScale()
{
	scaler1 = 1.0f;
	scaler2 = 1.5f;
	scaler3 = 2.0f;

	scaler = scaler1;
}

void Box::Scaling()
{
	if (isSelected)
	{
		if (totalDiff > 10 && totalDiff < 200)
		{
			//_box->setScale(scaler1);
			scaler = scaler1;
			_weight = 1.0f * _boxType;
		}
		else if (totalDiff > 200 && totalDiff < 500)
		{
			//_box->setScale(scaler2);
			scaler = scaler2;
			_weight = 2.0f * _boxType;
		}
		else if (totalDiff > 500)
		{
			//_box->setScale(scaler3);
			scaler = scaler3;
			_weight = 3.0f * _boxType;
		}
		_box->setScale(scaler);
		Selected();
	}
	else
	{
		//_box->setScale(scaler);
		Deselected();
	}
}

void Box::SetTotalDiff(float totalDiffNew)
{
	totalDiff = totalDiffNew;
}

void Box::Selected()
{
	_box->setOpacity(75);
}

void Box::Deselected()
{
	_box->setOpacity(255);
}

void Box::Collision(cocos2d::Touch* touch)
{
	cocos2d::Vec2 p = touch->getLocation();
	cocos2d::Rect rect = _box->getBoundingBox();

	if (rect.containsPoint(p))
	{
		if (_touchTimer <= 0.0f) {
			_touchTimer = _defaultTouchTimer;
			isSelected = !isSelected;
			//isSelected = true;
		}

	}
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

	float boxScaledWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
	float boxScaledHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();

	if (_orientationVertical) {
		if (GetSprite()->getPositionX() - (boxScaledWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
			&& GetSprite()->getPositionX() + (boxScaledWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
			&& GetSprite()->getPositionY() - (boxScaledHeight / 2) < collider->getPositionY() + (scaledHeight / 2)
			&& GetSprite()->getPositionY() + (boxScaledHeight / 2) > collider->getPositionY() - (scaledHeight / 2))
		{
			Land(collider);
		}
		else {
			_fallingVertical = true;
		}
	}
	else if (_orientationHorizontal) {
		if (GetSprite()->getPositionX() - (boxScaledHeight / 2) < collider->getPositionX() + (scaledWidth / 2)
			&& GetSprite()->getPositionX() + (boxScaledHeight / 2) > collider->getPositionX() - (scaledWidth / 2)
			&& GetSprite()->getPositionY() - (boxScaledWidth / 2) < collider->getPositionY() + (scaledHeight / 2)
			&& GetSprite()->getPositionY() + (boxScaledWidth / 2) > collider->getPositionY() - (scaledHeight / 2))
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

	float boxScaledWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
	float boxScaledHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();


	if (_orientationVertical) {
		if (GetSprite()->getPositionX() - (boxScaledWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
			&& GetSprite()->getPositionX() + (boxScaledWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
			&& GetSprite()->getPositionY() - (boxScaledHeight / 2) < collider->getPositionY() + (scaledHeight / 2)
			&& GetSprite()->getPositionY() + (boxScaledHeight / 2) > collider->getPositionY() - (scaledHeight / 2))
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
		if (GetSprite()->getPositionX() - (boxScaledHeight / 2) < collider->getPositionX() + (scaledWidth / 2)
			&& GetSprite()->getPositionX() + (boxScaledHeight / 2) > collider->getPositionX() - (scaledWidth / 2)
			&& GetSprite()->getPositionY() - (boxScaledWidth / 2) < collider->getPositionY() + (scaledHeight / 2)
			&& GetSprite()->getPositionY() + (boxScaledWidth / 2) > collider->getPositionY() - (scaledHeight / 2))
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
			if (_verticalVelocity > -48.0f) {
				_verticalVelocity = _verticalVelocityLast + ((_gravity / 2) * _timeFalling / 2);
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x, GetSprite()->getPosition().y + _verticalVelocity));
			}
			else if (_verticalVelocity < 48.0f) {
				_verticalVelocity = _verticalVelocityLast + ((_gravity / 2) * _timeFalling / 2);
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
			if (_horizontalVelocity > -48.0f) {
				_horizontalVelocity = _horizontalVelocityLast + ((_gravity / 2) * _timeFalling / 2);
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x + _horizontalVelocity, GetSprite()->getPosition().y));
			}
			else if (_horizontalVelocity < 48.0f) {
				_horizontalVelocity = _horizontalVelocityLast + ((_gravity / 2) * _timeFalling / 2);
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

void Box::SetSprite(Sprite* newSprite) {
	_box = newSprite;

	if (_box->getPosition() != newSprite->getPosition())
	{
		_box->setPosition(newSprite->getPosition());
	}

	cocostudio::ComExtensionData* data = dynamic_cast<cocostudio::ComExtensionData*>(GetSprite()->getComponent("ComExtensionData"));
	std::string userdata = data->getCustomProperty();

	if (userdata == "Large" || userdata == "large") {
		scaler = scaler3;
	}
	else if (userdata == "Medium" || userdata == "medium") {
		scaler = scaler2;
	}
	else if (userdata == "Small" || userdata == "small") {
		scaler = scaler1;
	}

	_box->setScale(scaler);
	this->addChild(_box);
}