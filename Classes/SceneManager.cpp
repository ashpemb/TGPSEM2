#include "SceneManager.h"
USING_NS_CC;

using namespace cocostudio::timeline;

SceneManager::SceneManager(int level)
{
	_level = level;
}

SceneManager* SceneManager::createScene(int level)
{
	// 'scene' is an autorelease object
	SceneManager* scene = new SceneManager(level);

	if (!scene->init()) {
		return nullptr;
	}

	// 'layer' is an autorelease object
	auto layer = SceneManager::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SceneManager::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize(); //Gets the size of the screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin(); //Gets the origin of the screen

	auto rootNode = CSLoader::createNode("Scene" + StringUtils::format("%d", _level) + ".csb");

	addChild(rootNode);

	GameManager::sharedGameManager()->startLevelTimer();


	this->scheduleUpdate();

	this->schedule(schedule_selector(SceneManager::ScheduleScore), 0.001f);
	_timeLabel = (ui::Text*)rootNode->getChildByName("Time");
	_timeLabel->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.98));

	// PLAYER
	_player = Player::create(_gravity);
	_player->SetSprite((Sprite*)rootNode->getChildByName("Player"));
	_player->setName("Player");

	addChild(_player);

	// AUDIO
	auEngine = new AudioEngine();

	auEngine->PlayBackgroundMusic("testing.mp3", true);

	// PLATFORMS
	cocos2d::Sprite* platform;
	int i = 1;
	platform = (Sprite*)rootNode->getChildByName("Platform_" + StringUtils::format("%d", i));

	while (platform != nullptr) {
		// Store platform in list
		_platforms.push_back(platform);
		_flipped.push_back(false);

		i++;
		platform = (Sprite*)rootNode->getChildByName("Platform_" + StringUtils::format("%d", i));
	}

	// SWITCHES
	cocos2d::ui::CheckBox* gravSwitch;
	i = 1;
	gravSwitch = static_cast<ui::CheckBox*>(rootNode->getChildByName("Switch_" + StringUtils::format("%d", i)));

	while (gravSwitch != nullptr) {
		gravSwitch->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_gravSwitches.push_back(gravSwitch);

		i++;
		gravSwitch = static_cast<ui::CheckBox*>(rootNode->getChildByName("Switch_" + StringUtils::format("%d", i)));
	}

	// WALLS
	cocos2d::Sprite* wall;
	i = 1;
	wall = (Sprite*)(rootNode->getChildByName("Wall_" + StringUtils::format("%d", i)));

	while (wall != nullptr) {
		_walls.push_back(wall);

		i++;
		wall = (Sprite*)(rootNode->getChildByName("Wall_" + StringUtils::format("%d", i)));
	}

	// GAMEMANAGER
	GameManager::sharedGameManager()->setIsGameLive(true);
	//GameManager::sharedGameManager()->setIsGamePaused(false);

	// TOUCH ME
	//Set up a touch listener.
	auto touchListener = EventListenerTouchOneByOne::create();

	//Set callbacks for our touch functions.
	touchListener->onTouchBegan = CC_CALLBACK_2(SceneManager::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SceneManager::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(SceneManager::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(SceneManager::onTouchCancelled, this);

	//Add our touch listener to event listener list.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// BACKGROUND
	_background1 = Sprite::create("BG1.png");
	_background1->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background1->setScaleX(winSize.width / _background1->getContentSize().width);
	_background1->setScaleY(winSize.height / _background1->getContentSize().height);
	_background1->setLocalZOrder(-3);

	_background2 = Sprite::create("BG2.png");
	_background2->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background2->setScaleX(winSize.width / _background2->getContentSize().width);
	_background2->setScaleY(winSize.height / _background2->getContentSize().height);
	_background2->setLocalZOrder(-1);

	_background3 = Sprite::create("BG3.png");
	_background3->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background3->setScaleX(winSize.width / _background3->getContentSize().width);
	_background3->setScaleY(winSize.height / _background3->getContentSize().height);
	_background3->setLocalZOrder(-2);

	_background4 = Sprite::create("BG4.png");
	_background4->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background4->setScaleX(winSize.width / _background4->getContentSize().width);
	_background4->setScaleY(winSize.height / _background4->getContentSize().height);
	_background4->setLocalZOrder(-2);

	_blackTransparency = Sprite::create("Black Screen.png");
	_blackTransparency->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_blackTransparency->setScaleX(winSize.width / _blackTransparency->getContentSize().width);
	_blackTransparency->setScaleY(winSize.height / _blackTransparency->getContentSize().height);
	_blackTransparency->setOpacity(123);
	_blackTransparency->setVisible(false);

	this->addChild(_background1);
	this->addChild(_background2);
	this->addChild(_background3);
	this->addChild(_background4);
	this->addChild(_blackTransparency);

	// WOODEN CRATES
	cocos2d::Sprite* crateWooden;
	i = 1;
	crateWooden = (Sprite*)(rootNode->getChildByName("Crate_Wooden_" + StringUtils::format("%d", i)));

	while (crateWooden != nullptr) {
		Box* box = Box::create(_gravity, 1);
		box->setName("Crate_Wooden_" + StringUtils::format("%d", i));
		box->SetSprite(crateWooden);

		_woodBoxes.push_back(box);

		addChild(_woodBoxes[i - 1]);

		i++;
		crateWooden = (Sprite*)(rootNode->getChildByName("Crate_Wooden_" + StringUtils::format("%d", i)));
	}

	// METAL CRATES
	cocos2d::Sprite* crateMetal;
	i = 1;
	crateMetal = (Sprite*)(rootNode->getChildByName("Crate_Metal_" + StringUtils::format("%d", i)));

	while (crateMetal != nullptr) {
		Box* box = Box::create(_gravity, 2);
		box->setName("Crate_Metal_" + StringUtils::format("%d", i));
		box->SetSprite(crateMetal);

		_metalBoxes.push_back(box);

		addChild(_metalBoxes[i - 1]);

		i++;
		crateMetal = (Sprite*)(rootNode->getChildByName("Crate_Metal_" + StringUtils::format("%d", i)));
	}

	_startGame = static_cast<ui::Button*>(rootNode->getChildByName("StartGame"));
	_startGame->addTouchEventListener(CC_CALLBACK_2(SceneManager::StartButtonPressed, this));
	_startGame->setPosition(Vec2(winSize.width*0.5, winSize.height*0.5));

	return true;
}

void SceneManager::ScheduleScore(float delta)
{
	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		GameManager::sharedGameManager()->updateLevelTimer();
	}
}

void SceneManager::update(float delta)
{
	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		if (GameManager::sharedGameManager()->getIsGameLive())
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
			IsPlayerInBounds();
		}
		else
		{
			_blackTransparency->setVisible(true);
			// Add gameover shit
		}
	}
}

void SceneManager::CheckCollisions()
{
	for (int i = 0; i < _platforms.size(); i++) {
		_player->CheckPlatformCollisions(_platforms[i]);

		for (int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckCollisions(_platforms[i]);
		}

		for (int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckCollisions(_platforms[i]);
		}
	}

	for (int i = 0; i < _walls.size(); i++) {
		_player->CheckWallCollisions(_walls[i]);

		for (int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckCollisions(_walls[i]);
		}

		for (int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckCollisions(_walls[i]);
		}
	}
}

//Touch Functions
bool SceneManager::onTouchBegan(Touch* touch, Event* event)
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

void SceneManager::onTouchEnded(Touch* touch, Event* event)
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

void SceneManager::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");
}

void SceneManager::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}

void SceneManager::SwitchPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
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

void SceneManager::StartButtonPressed(Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	CCLOG("In Touch! %d", type);

	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("touch ended");
		GameManager::sharedGameManager()->setIsGamePaused(false);
		_startGame->setVisible(false);
	}
}

void SceneManager::CheckNear()
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

void SceneManager::FlipGravity()
{
	if (_gravity < 0.0f) {
		_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);

		for (int i = 0; i < _woodBoxes.size(); i++) {
			_woodBoxes[i]->GetBoxSprite()->setPositionY(_woodBoxes[i]->GetBoxSprite()->getPositionY() + 0.5f);
		}


		for (int i = 0; i < _metalBoxes.size(); i++) {
			_metalBoxes[i]->GetBoxSprite()->setPositionY(_metalBoxes[i]->GetBoxSprite()->getPositionY() + 0.5f);
		}
	}
	else {
		_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);

		for (int i = 0; i < _woodBoxes.size(); i++) {
			_woodBoxes[i]->GetBoxSprite()->setPositionY(_woodBoxes[i]->GetBoxSprite()->getPositionY() - 0.5f);
		}

		for (int i = 0; i < _metalBoxes.size(); i++) {
			_metalBoxes[i]->GetBoxSprite()->setPositionY(_metalBoxes[i]->GetBoxSprite()->getPositionY() - 0.5f);
		}
	}

	_gravity *= -1;

	_player->SetGravity(_gravity);

	for (int i = 0; i < _woodBoxes.size(); i++) {
		_woodBoxes[i]->SetGravity(_gravity);
		_woodBoxes[i]->SetFalling(true);
	}

	for (int i = 0; i < _metalBoxes.size(); i++) {
		_metalBoxes[i]->SetGravity(_gravity);
		_metalBoxes[i]->SetFalling(true);
	}

	_player->SetFalling(true);
	_player->FlipPlayer();
}

void SceneManager::IsPlayerInBounds()
{
	auto winSize = Director::getInstance()->getVisibleSize();
	if (_player->GetSprite()->getPosition().y < (0.0f - _player->GetSprite()->getContentSize().height))
	{
		GameManager::sharedGameManager()->setIsGameLive(false);
		
	}
	else if (_player->GetSprite()->getPosition().y > (winSize.height + _player->GetSprite()->getContentSize().height))
	{
		GameManager::sharedGameManager()->setIsGameLive(false);
	}
}

SceneManager::~SceneManager()
{
	// I have no idea if this is ever called
	delete _player;
	delete _timeLabel;
	delete _background1;
	delete _background2;
	delete _background3;
	delete _background4;
	delete _blackTransparency;
	delete auEngine;

	for (int i = 0; i < _platforms.size(); i++) {
		delete _platforms[i];
	}

	_platforms.clear();

	for (int i = 0; i < _walls.size(); i++) {
		delete _walls[i];
	}

	_walls.clear();

	for (int i = 0; i < _gravSwitches.size(); i++) {
		delete _gravSwitches[i];
	}

	_gravSwitches.clear();
}