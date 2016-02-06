#include "Player.h"


Player::Player()
{
}

Player* Player::create(float gravity)
{
	Player* player = new Player();
	if (!player->init()) {
		return nullptr;
	}

	player->setGravity(gravity);
	player->autorelease();

	//player->setPosition(100, 100);

	return player;
}

Player::~Player()
{
}

bool Player::init()
{
	if (!Node::init())
	{
		return false;
	}

	// Get window size and origin
	auto winSize = Director::getInstance()->getVisibleSize(); //Gets the size of the screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin(); //Gets the origin of the screen

	_husky2 = Sprite::create("Husky.png");
	_husky2->setPosition(Vec2(500, winSize.height / 2));

	this->addChild(_husky2);

	this->scheduleUpdate();

	return true;
}

void Player::update(float delta) 
{
	//Debug
	//_falling = true;

	if (_falling) {
		fall(delta);
	}
}

void Player::setGravity(float gravity)
{
	_gravity = gravity;
}

void Player::setVelocity(float y)
{
	_verticalVelocity = y;
}

void Player::checkCollisions(cocos2d::Sprite* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	if (getSprite()->getPositionX() - (getSprite()->getContentSize().width / 2) < collider->getPositionX() + (collider->getContentSize().width / 2)
		&& getSprite()->getPositionX() + (getSprite()->getContentSize().width / 2) > collider->getPositionX() - (collider->getContentSize().width / 2)
		&& getSprite()->getPositionY() - (getSprite()->getContentSize().height / 2) < collider->getPositionY() + (collider->getContentSize().height / 2)
		&& getSprite()->getPositionY() + (getSprite()->getContentSize().height / 2) > collider->getPositionY() - (collider->getContentSize().height / 2))
	{
		land(collider);
	}
}

void Player::land(cocos2d::Sprite* collider)
{
	if (_falling) {
		_falling = false;

		// Cancel velocity
		_verticalVelocity = 0.0f;
		_lastVelocity = 0.0f;
		_timeFalling = 0.0f;

		// Make sure the position is set so not inside platform
		float newY = (collider->getPositionY() + (collider->getContentSize().height / 2)) + (getSprite()->getContentSize().height / 2);
		getSprite()->setPosition(Vec2(getSprite()->getPositionX(), newY));
	}
}

void Player::fall(float delta)
{
	if (_falling) {
		// Update falling time
		_timeFalling += delta;

		// Calculate and set new velocity
		if (_verticalVelocity > -9.0f)
		{
			_verticalVelocity = _lastVelocity + ((_gravity / 2) * _timeFalling);
			_husky2->setPosition(Vec2(_husky2->getPosition().x, _husky2->getPosition().y + _verticalVelocity));
		}
		else
		{
			_verticalVelocity = -9.0f;
			_husky2->setPosition(Vec2(_husky2->getPosition().x, _husky2->getPosition().y + _verticalVelocity));
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