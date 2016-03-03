#include "Box.h"


Box::Box(int boxType)
{
	_boxType = boxType;
	scaler1 = 1;
	scaler2 = 2;
	scaler3 = 3;
	scaler = 1;
	unselect = 0;
	isSelected = false;
}

Box* Box::create(float gravity, int boxType, float startingScale)
{
	Box* woodBox = new Box(boxType);
	if (!woodBox->init())
	{
		return nullptr;
	}

	woodBox->SetGravity(gravity);
	woodBox->SetStartingScale(startingScale);
	woodBox->autorelease();

	return woodBox;
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
		if (_falling)
		{
			Fall(delta);
		}
		Scaling();
	}

	// Add box movement when pushed by player
}

void Box::SetStartingScale(float startingScale)
{
	scaler2 = startingScale;
	scaler3 = startingScale*1.5f;
	scaler1 = startingScale*0.5f;
}

void Box::Scaling()
{
	if (isSelected == true)
	{
		totalDiff = rand() % 800;
		if (totalDiff > 10 && totalDiff < 200)
		{
			_box->setScale(scaler1);
			scaler = scaler1;
		}
		else if (totalDiff > 200 && totalDiff < 500)
		{
			_box->setScale(scaler2);
			scaler = scaler2;
		}
		else if (totalDiff > 500)
		{
			_box->setScale(scaler3);
			scaler = scaler3;
		}
		Selected();
	}
	else
	{
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
		unselect = 0;
		isSelected = true;
	}
	else if (isSelected == true)
	{
		unselect = unselect + 1;
		if (unselect > 2)
		{
			isSelected = false;
			unselect = 0;
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

	if (GetBoxSprite()->getPositionX() - (GetBoxSprite()->getContentSize().width / 2) *scaler < collider->getPositionX() + (scaledWidth / 2)
		&& GetBoxSprite()->getPositionX() + (GetBoxSprite()->getContentSize().width / 2) * scaler > collider->getPositionX() - (scaledWidth / 2)
		&& GetBoxSprite()->getPositionY() - (GetBoxSprite()->getContentSize().height / 2) * scaler < collider->getPositionY() + (scaledHeight / 2)
		&& GetBoxSprite()->getPositionY() + (GetBoxSprite()->getContentSize().height / 2) * scaler > collider->getPositionY() - (scaledHeight / 2))
	{
		Land(collider);
	}
	else {
		_falling = true;
	}
}

void Box::CheckWallCollisions(cocos2d::Sprite* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
	float scaledHeight = collider->getContentSize().height * collider->getScaleY();
	float scaledPlayerWidth = _box->getContentSize().width * _box->getScaleX();

	if (GetBoxSprite()->getPositionX() - (GetBoxSprite()->getContentSize().width / 2)*scaler < collider->getPositionX() + (scaledWidth / 2)
		&& GetBoxSprite()->getPositionX() + (GetBoxSprite()->getContentSize().width / 2)*scaler > collider->getPositionX() - (scaledWidth / 2)
		&& GetBoxSprite()->getPositionY() - (GetBoxSprite()->getContentSize().height / 2)*scaler < collider->getPositionY() + (scaledHeight / 2)
		&& GetBoxSprite()->getPositionY() + (GetBoxSprite()->getContentSize().height / 2)*scaler > collider->getPositionY() - (scaledHeight / 2))
	{
		if (_box->getPositionX() < collider->getPositionX()) {
			_box->setPositionX(collider->getPositionX() - (scaledWidth / 2) - (scaledPlayerWidth / 2));
		}
		else {
			_box->setPositionX(collider->getPositionX() + (scaledWidth / 2) + (scaledPlayerWidth / 2));
		}
	}
}

void Box::Land(cocos2d::Sprite* collider)
{
	if (_falling) {
		_falling = false;

		// Cancel velocity
		_verticalVelocity = 0.0f;
		_lastVelocity = 0.0f;
		_timeFalling = 0.0f;

		// Make sure the position is set so not inside platform
		float newY;
		float scaledColliderHeight = collider->getContentSize().height * collider->getScaleY();
		float scaledBoxHeight = _box->getContentSize().height * _box->getScaleY();

		if (_gravity < 0.0f) {
			newY = (collider->getPositionY() + (scaledColliderHeight / 2)) + (scaledBoxHeight / 2);
		}
		else {
			newY = (collider->getPositionY() - (scaledColliderHeight / 2)) - (scaledBoxHeight / 2);
		}

		GetBoxSprite()->setPosition(Vec2(GetBoxSprite()->getPositionX(), newY));
	}
}

void Box::Fall(float delta)
{
	if (_falling) {
		// Update falling time
		_timeFalling += delta;

		// Calculate and set new velocity
		if (_verticalVelocity > -12.0f) {
			_verticalVelocity = _lastVelocity + ((_gravity / 2) * _timeFalling);
			_box->setPosition(Vec2(_box->getPosition().x, _box->getPosition().y + _verticalVelocity));
		}
		else if (_verticalVelocity < 12.0f) {
			_verticalVelocity = _lastVelocity + ((_gravity / 2) * _timeFalling);
			_box->setPosition(Vec2(_box->getPosition().x, _box->getPosition().y + _verticalVelocity));
		}
		else {
			_verticalVelocity = -9.0f;
			_box->setPosition(Vec2(_box->getPosition().x, _box->getPosition().y + _verticalVelocity));
		}

		// Update last velocity
		_lastVelocity = _verticalVelocity;
	}
	else {
		_falling = true;
		_lastVelocity = 0.0f;
		_verticalVelocity = 0.0f;
		_timeFalling = 0.0f;
	}
}

void Box::SetFalling(bool falling)
{
	_falling = falling;
}


void Box::FlipPlayer()
{
	if (_gravity < 0)
	{
		auto rotateTo = RotateTo::create(0.5f, 0.0f);
		_box->runAction(rotateTo);
	}
	else if (_gravity > 0)
	{
		auto rotateTo = RotateTo::create(0.5f, 180.0f);
		_box->runAction(rotateTo);
	}
}