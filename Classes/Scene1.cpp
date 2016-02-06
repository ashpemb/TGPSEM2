#include "Scene1.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* Scene1::createScene()
{
	// 'scene' is an autorelease object
	Scene* scene = new Scene();

	if (!scene->init()) {
		return nullptr;
	}

	// 'layer' is an autorelease object
	auto layer = Scene1::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Scene1::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize(); //Gets the size of the screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin(); //Gets the origin of the screen

	
	auto rootNode = CSLoader::createNode("Scene1.csb");

	addChild(rootNode);

	GameManager::sharedGameManager()->startLevelTimer();

	this->scheduleUpdate();
	//this->schedule(schedule_selector(Scene1::ScheduleScore), 1.0f);

	// PLAYER
	player = Player::create(gravity);
	player->setName("Player");

	addChild(player);

	auEngine = new AudioEngine();

	auEngine->PlayBackgroundMusic("testing.mp3", true);

	cocos2d::Sprite* platform;
	int i = 1;
	platform = (Sprite*)rootNode->getChildByName("Platform_" + std::to_string(i));

	while (platform != nullptr) {
		// Store platform in list
		platforms.push_back(platform);

		i++;
		platform = (Sprite*)rootNode->getChildByName("Platform_" + std::to_string(i));
	}

	//platform->setPosition(Vec2(429, 1504));
	int woof = 229;
	return true;
}

void Scene1::ScheduleScore()
{
	GameManager::sharedGameManager()->updateLevelTimer();
}

void Scene1::update(float delta)
{
	score = GameManager::sharedGameManager()->getTimer();

	CheckCollisions();
}

void Scene1::CheckCollisions()
{
	for (int i = 0; i < platforms.size(); i++) {
		player->checkCollisions(platforms[i]);
	}
}

//Touch Functions
bool Scene1::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("touch began");

	// record position


	return true;
}

void Scene1::onTouchEnded(Touch* touch, Event* event)
{
	cocos2d::log("touch ended");
}

void Scene1::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");
}

void Scene1::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}