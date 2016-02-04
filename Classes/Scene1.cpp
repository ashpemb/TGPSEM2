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
	this->schedule(schedule_selector(Scene1::ScheduleScore), 1.0f);

	// PLAYER
	player = Player::create(gravity);
	player->setName("Player");

	addChild(player);
	return true;
}

void Scene1::ScheduleScore()
{
	GameManager::sharedGameManager()->updateLevelTimer();
}

void Scene1::update(float delta)
{
	score = GameManager::sharedGameManager()->getTimer();
	
}