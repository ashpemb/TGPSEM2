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

	this->schedule(schedule_selector(Scene1::ScheduleScore), 0.001f);
	_timeLabel = (ui::Text*)rootNode->getChildByName("Time");
	_timeLabel->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.95));

	// PLAYER
	_player = Player::create(_gravity);
	_player->setName("Player");

	addChild(_player);

	auEngine = new AudioEngine();

	auEngine->PlayBackgroundMusic("testing.mp3", true);

	cocos2d::Sprite* platform;
	int i = 1;
	platform = (Sprite*)rootNode->getChildByName("Platform_" + std::to_string(i));

	while (platform != nullptr) {
		// Store platform in list
		_platforms.push_back(platform);
		_flipped.push_back(false);

		i++;
		platform = (Sprite*)rootNode->getChildByName("Platform_" + std::to_string(i));
	}

	cocos2d::ui::CheckBox* gravSwitch;
	i = 1;
	gravSwitch = static_cast<ui::CheckBox*>(rootNode->getChildByName("Switch_" + std::to_string(i)));

	while (gravSwitch != nullptr) {
		gravSwitch->addTouchEventListener(CC_CALLBACK_2(Scene1::SwitchPressed, this));
		_gravSwitches.push_back(gravSwitch);

		i++;
		gravSwitch = static_cast<ui::CheckBox*>(rootNode->getChildByName("Switch_" + std::to_string(i)));
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

void Scene1::ScheduleScore(float delta)
{
	GameManager::sharedGameManager()->updateLevelTimer();
}

void Scene1::update(float delta)
{
	if (_flipGravityCooldown > 0.0f) {
		_flipGravityCooldown -= delta;

		if (_flipGravityCooldown < 0.0f) {
			_flipGravityCooldown = 0.0f;
		}
	}

	_score = GameManager::sharedGameManager()->getTimer();

	int mil = GameManager::sharedGameManager()->getMil();
	int sec = GameManager::sharedGameManager()->getSec();
	int min = GameManager::sharedGameManager()->getMin();

	_timeLabel->setString(StringUtils::format("%d:%d:%d", min, sec, mil));

	CheckCollisions();
	CheckNear();
}

void Scene1::CheckCollisions()
{
	for (int i = 0; i < _platforms.size(); i++) {
		_player->CheckCollisions(_platforms[i]);
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

		_initialTouchPos = touchPos;
		_inTouch = true;
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
			_inTouch = false;

			// TODO
			// Add checks to ensure no object is clicked
			// If an object is clicked, DO NOT let the player move to it, instead:
			// call the appropiate methods specific to that object

			_player->SetTarget(_initialTouchPos.x);
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

void Scene1::SwitchPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	// Find what switch has been clicked
	cocos2d::ui::CheckBox* findCheckBox = (cocos2d::ui::CheckBox*)sender;

	for (int i = 0; i < _gravSwitches.size(); i++) {
		if (findCheckBox->getName() == _gravSwitches[i]->getName()) {
			_flipped[i] = !_flipped[i];
			_gravSwitches[i]->setFlippedX(_flipped[i]);

			// Flip Gravity
			if (_flipGravityCooldown == 0.0f) {
				FlipGravity();

				_flipGravityCooldown = 1.0f;
			}
		}
	}
}

void Scene1::CheckNear()
{
	for (int i = 0; i < _gravSwitches.size(); i++) {
		// Player needs to be near the switch to press
		float scaledWidth = _gravSwitches[i]->getContentSize().width * _gravSwitches[i]->getScaleX();
		float scaledHeight = _gravSwitches[i]->getContentSize().height * _gravSwitches[i]->getScaleY();

		if (_player->GetSprite()->getPositionX() - (_player->GetSprite()->getContentSize().width / 2) < _gravSwitches[i]->getPositionX() + (scaledWidth / 2) + (_player->GetSprite()->getContentSize().width / 2) + 20
			&& _player->GetSprite()->getPositionX() + (_player->GetSprite()->getContentSize().width / 2) > _gravSwitches[i]->getPositionX() - (scaledWidth / 2) - (_player->GetSprite()->getContentSize().width / 2) - 20
			&& _player->GetSprite()->getPositionY() - (_player->GetSprite()->getContentSize().height / 2) < _gravSwitches[i]->getPositionY() + (scaledHeight / 2)
			&& _player->GetSprite()->getPositionY() + (_player->GetSprite()->getContentSize().height / 2) > _gravSwitches[i]->getPositionY() - (scaledHeight / 2))
		{
			//_gravSwitches[i]->setEnabled(true);
			_gravSwitches[i]->setEnabled(true);
		}
		else {
			_gravSwitches[i]->setEnabled(false);
		}
	}
}

void Scene1::FlipGravity()
{
	if (_gravity < 0.0f) {
		_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
	}
	else {
		_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);
	}

	_gravity *= -1;

	_player->SetGravity(_gravity);
	_player->SetFalling(true);
	_player->FlipPlayer();
}