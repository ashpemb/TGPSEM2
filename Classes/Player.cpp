#include "Player.h"


Player::Player()
{
}

Player* Player::create()
{
	Player* player = new Player();
	if (!player->init()) {
		return nullptr;
	}
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

	auto husky2 = Sprite::create("Husky.png");
	husky2->setPosition(Vec2(500, winSize.height / 2 + origin.y));

	this->addChild(husky2);
}

void Player::Update(float delta) 
{
	// TO DO
}