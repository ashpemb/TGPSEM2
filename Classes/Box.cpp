#include "Box.h"


Box::Box(int boxType)
{
	_boxType = boxType;
}

Box* Box::create(float gravity, int boxType)
{
	Box* woodBox = new Box(boxType);
	if (!woodBox->init())
	{
		return nullptr;
	}

	woodBox->SetGravity(gravity);
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
	}

	float TotalDiff = TouchManager().totalDiff;

	ScalingObject().update(delta, TotalDiff);

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

void Box::CheckCollisions(cocos2d::Sprite* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
	float scaledHeight = collider->getContentSize().height * collider->getScaleY();

	if (GetBoxSprite()->getPositionX() - (GetBoxSprite()->getContentSize().width / 2) < collider->getPositionX() + (scaledWidth / 2)
		&& GetBoxSprite()->getPositionX() + (GetBoxSprite()->getContentSize().width / 2) > collider->getPositionX() - (scaledWidth / 2)
		&& GetBoxSprite()->getPositionY() - (GetBoxSprite()->getContentSize().height / 2) < collider->getPositionY() + (scaledHeight / 2)
		&& GetBoxSprite()->getPositionY() + (GetBoxSprite()->getContentSize().height / 2) > collider->getPositionY() - (scaledHeight / 2))
	{
		Land(collider);
	}
	else {
		_falling = true;
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