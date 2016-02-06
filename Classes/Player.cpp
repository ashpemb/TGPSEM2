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

	player->SetGravity(gravity);
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

	_playerSprite = Sprite::create("Husky.png");
	_playerSprite->setPosition(Vec2(500, (winSize.height / 2) + 400));

	this->addChild(_playerSprite);

	this->scheduleUpdate();

	// Init member level variables
	_targetX = _playerSprite->getPositionX();
	_speed = 50;

	return true;
}

void Player::update(float delta) 
{
	//Debug
	//_falling = true;

	if (_falling) {
		Fall(delta);
	}

	if (_targetX != _playerSprite->getPositionX()) {
		MoveToTarget(delta);
	}
}

void Player::SetGravity(float gravity)
{
	_gravity = gravity;
}

void Player::SetVelocity(float y)
{
	_verticalVelocity = y;
}

void Player::CheckCollisions(cocos2d::Sprite* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
	float scaledHeight = collider->getContentSize().height * collider->getScaleY();

	if (GetSprite()->getPositionX() - (GetSprite()->getContentSize().width / 2) < collider->getPositionX() + (scaledWidth / 2)
		&& GetSprite()->getPositionX() + (GetSprite()->getContentSize().width / 2) > collider->getPositionX() - (scaledWidth / 2)
		&& GetSprite()->getPositionY() - (GetSprite()->getContentSize().height / 2) < collider->getPositionY() + (scaledHeight / 2)
		&& GetSprite()->getPositionY() + (GetSprite()->getContentSize().height / 2) > collider->getPositionY() - (scaledHeight / 2))
	{
		Land(collider);
	}
	else {
		_falling = true;
	}
}

void Player::Land(cocos2d::Sprite* collider)
{
	if (_falling) {
		_falling = false;

		// Cancel velocity
		_verticalVelocity = 0.0f;
		_lastVelocity = 0.0f;
		_timeFalling = 0.0f;

		// Make sure the position is set so not inside platform
		float newY = (collider->getPositionY() + (collider->getContentSize().height / 2)) + (GetSprite()->getContentSize().height / 2);
		GetSprite()->setPosition(Vec2(GetSprite()->getPositionX(), newY));
	}
}

void Player::Fall(float delta)
{
	if (_falling) {
		// Update falling time
		_timeFalling += delta;

		// Calculate and set new velocity
		if (_verticalVelocity > -9.0f)
		{
			_verticalVelocity = _lastVelocity + ((_gravity / 2) * _timeFalling);
			_playerSprite->setPosition(Vec2(_playerSprite->getPosition().x, _playerSprite->getPosition().y + _verticalVelocity));
		}
		else
		{
			_verticalVelocity = -9.0f;
			_playerSprite->setPosition(Vec2(_playerSprite->getPosition().x, _playerSprite->getPosition().y + _verticalVelocity));
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

void Player::SetTarget(float target)
{
	_targetX = target;
}

void Player::MoveToTarget(float deltaTime)
{
	if (_playerSprite->getPositionX() > _targetX) {
		_playerSprite->setPositionX(_playerSprite->getPositionX() - (_speed * deltaTime));

		if (_playerSprite->getPositionX() < _targetX) {
			_playerSprite->setPositionX(_targetX);
		}
	}
	else if (_playerSprite->getPositionX() < _targetX) {
		_playerSprite->setPositionX(_playerSprite->getPositionX() + (_speed * deltaTime));

		if (_playerSprite->getPositionX() > _targetX) {
			_playerSprite->setPositionX(_targetX);
		}
	}
	else {
		// Player is already in target position, idiot
	}
}