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

	this->scheduleUpdate();

	// Init member level variables
	_speed = 200;

	return true;
}

void Player::update(float delta) 
{
	//Debug
	//_falling = true;
	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		if (_falling) {
			Fall(delta);
		}

		if (_targetX != _playerSprite->getPositionX()) {
			MoveToTarget(delta);
		}
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

void Player::CheckPlatformCollisions(cocos2d::Sprite* collider)
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

void Player::CheckWallCollisions(cocos2d::Sprite* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
	float scaledHeight = collider->getContentSize().height * collider->getScaleY();
	float scaledPlayerWidth = _playerSprite->getContentSize().width * _playerSprite->getScaleX();

	if (GetSprite()->getPositionX() - (GetSprite()->getContentSize().width / 2) < collider->getPositionX() + (scaledWidth / 2)
		&& GetSprite()->getPositionX() + (GetSprite()->getContentSize().width / 2) > collider->getPositionX() - (scaledWidth / 2)
		&& GetSprite()->getPositionY() - (GetSprite()->getContentSize().height / 2) < collider->getPositionY() + (scaledHeight / 2)
		&& GetSprite()->getPositionY() + (GetSprite()->getContentSize().height / 2) > collider->getPositionY() - (scaledHeight / 2))
	{
		if (_playerSprite->getPositionX() < collider->getPositionX()) {
			_playerSprite->setPositionX(collider->getPositionX() - (scaledWidth / 2) - (scaledPlayerWidth / 2));
		}
		else {
			_playerSprite->setPositionX(collider->getPositionX() + (scaledWidth / 2) + (scaledPlayerWidth / 2));
		}
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
		float newY;
		float scaledColliderHeight = collider->getContentSize().height * collider->getScaleY();
		float scaledPlayerHeight = _playerSprite->getContentSize().height * _playerSprite->getScaleY();

		if (_gravity < 0.0f) {
			newY = (collider->getPositionY() + (scaledColliderHeight / 2)) + (scaledPlayerHeight / 2);
		}
		else {
			newY = (collider->getPositionY() - (scaledColliderHeight / 2)) - (scaledPlayerHeight / 2);
		}

		GetSprite()->setPosition(Vec2(GetSprite()->getPositionX(), newY));
	}
}

void Player::Fall(float delta)
{
	if (_falling) {
		// Update falling time
		_timeFalling += delta;

		// Calculate and set new velocity
		if (_verticalVelocity > -12.0f) {
			_verticalVelocity = _lastVelocity + ((_gravity / 2) * _timeFalling);
			_playerSprite->setPosition(Vec2(_playerSprite->getPosition().x, _playerSprite->getPosition().y + _verticalVelocity));
		}
		else if (_verticalVelocity < 12.0f) {
			_verticalVelocity = _lastVelocity + ((_gravity / 2) * _timeFalling);
			_playerSprite->setPosition(Vec2(_playerSprite->getPosition().x, _playerSprite->getPosition().y + _verticalVelocity));
		}
		else {
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

void Player::SetFalling(bool falling)
{
	_falling = falling;
}

void Player::FlipPlayer()
{
	if (_gravity < 0)
	{
		auto rotateTo = RotateTo::create(0.5f, 0.0f);
		_playerSprite->runAction(rotateTo);
	}
	else if (_gravity > 0)
	{
		auto rotateTo = RotateTo::create(0.5f, 180.0f);
		_playerSprite->runAction(rotateTo);
	}
}

void Player::SetSprite(Sprite* newSprite) 
{ 
	_playerSprite = newSprite;
	SetTarget(newSprite->getPositionX());
	
	this->addChild(_playerSprite); 
}