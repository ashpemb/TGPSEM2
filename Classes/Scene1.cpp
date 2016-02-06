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

	// GAMEMANAGER
	GameManager::sharedGameManager()->setIsGameLive(true);
	GameManager::sharedGameManager()->setIsGamePaused(false);

	// TOUCH ME
	//Set up a touch listener.
	auto touchListener = EventListenerTouchOneByOne::create();

	//Set callbacks for our touch functions.
	touchListener->onTouchBegan = CC_CALLBACK_2(Scene1::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Scene1::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Scene1::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(Scene1::onTouchCancelled, this);

	//Add our touch listener to event listener list.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

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
		player->CheckCollisions(platforms[i]);
	}
}

//Touch Functions
bool Scene1::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("touch began");

	if (GameManager::sharedGameManager()->getIsGameLive() == true) {
		//Store the coordinates of where this touch began.
		Point touchPos = touch->getLocationInView();
		touchPos = Director::getInstance()->convertToGL(touchPos);
		touchPos = convertToNodeSpace(touchPos);

		initialTouchPos = touchPos;
		inTouch = true;
		return true;
	}
	else {
		return false;
	}

	return true;
}

void Scene1::onTouchEnded(Touch* touch, Event* event)
{
	cocos2d::log("touch ended");

	if (GameManager::sharedGameManager()->getIsGameLive() == true) {
		if (!GameManager::sharedGameManager()->getIsGamePaused()) {
			inTouch = false;

			// TODO
			// Add checks to ensure no object is clicked
			// If an object is clicked, DO NOT let the player move to it, instead:
			// call the appropiate methods specific to that object

			player->SetTarget(initialTouchPos.x);
		}
	}
}

void Scene1::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");
}

void Scene1::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}