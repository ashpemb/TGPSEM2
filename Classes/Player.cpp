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

	Sprite* sprite = Sprite::create("Husky.png");

	/////////////////////////////////////////////
	/////// Examples and physics testing/////////
	/////////////////////////////////////////////
	auto husky1 = Sprite::create("Husky.png");

	husky1->setPosition(Vec2(200, winSize.height / 2 + origin.y));;

	auto husky1Bounding = PhysicsBody::createCircle(husky1->getContentSize().width / 2,
		PhysicsMaterial(0.0f, 1.0f, 0.5f));
	husky1Bounding->applyTorque(200);
	husky1->setPhysicsBody(husky1Bounding);

	this->addChild(husky1);

	this->scheduleUpdate();

	//// Other Husky implementations... Might need to use these over above husky. ASK JAMES
	//auto husky2 = Sprite::create("Husky.png");
	//auto husky3 = Sprite::create("Husky.png");
	//auto husky4 = Sprite::create("Husky.png");
	//husky2->setPosition(Vec2(500, winSize.height / 2 + origin.y));
	//husky3->setPosition(Vec2(800, winSize.height / 2 + origin.y));
	//husky4->setPosition(Vec2(800, 0));

	//auto husky2Bounding = PhysicsBody::createBox(husky2->getContentSize(),
	//	PhysicsMaterial(0.0f, 0.5f, 0.5f));
	//husky2->setPhysicsBody(husky2Bounding);

	//auto husky3Bounding = PhysicsBody::createBox(husky3->getContentSize(),
	//	PhysicsMaterial(0.0f, 0.5f, 0.5f));
	//husky3->setPhysicsBody(husky3Bounding);

	//auto husky4Bounding = PhysicsBody::createBox(husky3->getContentSize(),
	//	PhysicsMaterial(0.0f, 0.5f, 0.5f));
	//husky4->setPhysicsBody(husky4Bounding);

	//this->addChild(husky2);
	//this->addChild(husky3);
	//this->addChild(husky4);
}

void Player::Update(float delta) 
{
	// TO DO
}