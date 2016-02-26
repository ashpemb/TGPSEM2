#include "SceneManager.h"
USING_NS_CC;

using namespace cocostudio::timeline;

SceneManager::SceneManager(int level)
{
	_level = level;
	_gravityOrientation = 0;
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

	Node* rootNode = CSLoader::createNode("Scene" + StringUtils::format("%d", _level) + ".csb");

	addChild(rootNode);

	this->scheduleUpdate();

	// GAMEMANAGER
	GameManager::sharedGameManager()->setIsGameLive(true);
	GameManager::sharedGameManager()->setIsGamePaused(true);

	// TOUCH SETUP
	SetupTouches();

	// TIMER SETUP
	SetupTimer(rootNode);

	// AUDIO SETUP
	SetupAudio(rootNode);

	// SPRITE SETUP
	SetupSprites(rootNode);

	// BACKGROUND SETUP
	SetupBackground(rootNode);

	// HIGHLIGHT SETUP
	SetupHighlights(rootNode);

	// BUTTON SETUP
	SetupButtons(rootNode);

	// CLASS SETUP
	SetupClasses();

	return true;
		
}

void SceneManager::SetupTouches()
{
	//Set up a touch listener.
	auto touchListener = EventListenerTouchOneByOne::create();

	//Set callbacks for our touch functions.
	touchListener->onTouchBegan = CC_CALLBACK_2(SceneManager::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SceneManager::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(SceneManager::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(SceneManager::onTouchCancelled, this);

	//Add our touch listener to event listener list.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void SceneManager::SetupTimer(Node* root)
{
	// Get screen size
	auto winSize = Director::getInstance()->getVisibleSize();
	GameManager::sharedGameManager()->startLevelTimer();

	this->schedule(schedule_selector(SceneManager::ScheduleScore), 0.001f);
	int mil = GameManager::sharedGameManager()->getMil();
	int sec = GameManager::sharedGameManager()->getSec();
	int min = GameManager::sharedGameManager()->getMin();

	_timeLabel = (ui::Text*)root->getChildByName("Time");
	_timeLabel->setString(StringUtils::format("%d:%d:%d", min, sec, mil));
	_timeLabel->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.98));
}

void SceneManager::SetupAudio(Node* root)
{
	// Check if the game is muted before playing background music
	if (GameManager::sharedGameManager()->getIsGameMuted() == false) {
		auEngine->PlayBackgroundMusic("testing.mp3", true);
	}
}

void SceneManager::SetupButtons(Node* root)
{
	// Get screen size
	auto winSize = Director::getInstance()->getVisibleSize();
	GameManager::sharedGameManager()->startLevelTimer();

	// START BUTTON
	// Note: This is a debug button that may need removing for the final version
	_startGame = static_cast<ui::Button*>(root->getChildByName("StartGame"));
	_startGame->addTouchEventListener(CC_CALLBACK_2(SceneManager::StartButtonPressed, this));
	_startGame->setPosition(Vec2(winSize.width*0.5, winSize.height*0.5));
}

void SceneManager::SetupSprites(Node* root)
{
	// PLAYER
	_playerSprite = (Sprite*)root->getChildByName("Player");
	int i = 1;
	Sprite* tempSprite;

	// PLATFORMS
	while ((tempSprite = (Sprite*)root->getChildByName("Platform_" + StringUtils::format("%d", i))) != nullptr)
	{
		_platforms.push_back(tempSprite);
		i++;
	}

	// WALLS
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Wall_" + StringUtils::format("%d", i))) != nullptr)
	{
		_walls.push_back(tempSprite);
		i++;
	}

	// WOODEN CRATES
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Crate_Wooden_" + StringUtils::format("%d", i))) != nullptr)
	{
		_woodenSprites.push_back(tempSprite);
		i++;
	}

	// METAL CRATES
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Crate_Metal_" + StringUtils::format("%d", i))) != nullptr)
	{
		_metalSprites.push_back(tempSprite);
		i++;
	}

	// SWITCHES
	cocos2d::ui::CheckBox* tempCheck;
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_gravSwitches.push_back(tempCheck);
		_flipped.push_back(false);
		i++;
	}

	// EXIT DOOR
	cocos2d::ui::CheckBox* exit;
	i = 1;
	while ((exit = static_cast<ui::CheckBox*>(root->getChildByName("Exit_" + StringUtils::format("%d", i)))) != nullptr) {

		exit->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_exit.push_back(exit);

		i++;
		exit = static_cast<ui::CheckBox*>(root->getChildByName("Exit_" + StringUtils::format("%d", i)));
	}
}

void SceneManager::SetupBackground(Node* root)
{
	// Get screen size
	auto winSize = Director::getInstance()->getVisibleSize();

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
}

void SceneManager::SetupHighlights(Node* root)
{
	// Get screen size
	auto winSize = Director::getInstance()->getVisibleSize();

	_topHighlight = Sprite::create("highlight.png");
	_topHighlight->setPosition(Vec2(winSize.width*0.5f, winSize.height - (_topHighlight->getContentSize().height / 2)));
	_topHighlight->setScaleX(winSize.width);
	_topHighlight->setOpacity(0);
	_topHighlight->setLocalZOrder(3);

	_rightHighlight = Sprite::create("highlight.png");
	_rightHighlight->setPosition(Vec2(0.0f - (_rightHighlight->getContentSize().height / 2), winSize.height * 0.5f));
	_rightHighlight->setRotation(90.0f);
	_rightHighlight->setScaleX(winSize.height);
	_rightHighlight->setOpacity(0);
	_rightHighlight->setLocalZOrder(3);

	_bottomHighlight = Sprite::create("highlight.png");
	_bottomHighlight->setPosition(Vec2(winSize.width*0.5f, 0.0f + (_topHighlight->getContentSize().height / 2)));
	_bottomHighlight->setRotation(180.0f);
	_bottomHighlight->setScaleX(winSize.width);
	_bottomHighlight->setOpacity(0);
	_bottomHighlight->setLocalZOrder(3);

	_leftHighlight = Sprite::create("highlight.png");
	_leftHighlight->setPosition(Vec2(winSize.width + (_rightHighlight->getContentSize().height / 2), winSize.height * 0.5f));
	_leftHighlight->setRotation(270.0f);
	_leftHighlight->setScaleX(winSize.height);
	_leftHighlight->setOpacity(0);
	_leftHighlight->setLocalZOrder(3);

	this->addChild(_topHighlight);
	this->addChild(_leftHighlight);
	this->addChild(_bottomHighlight);
	this->addChild(_rightHighlight);
}

void SceneManager::SetupClasses()
{
	// PLAYER
	_player = Player::create();
	_player->SetSprite(_playerSprite);
	_player->setName("Player");
	addChild(_player);

	// WOODEN CRATES
	for (int i = 0; i < _woodenSprites.size(); i++) {
		Box* box = Box::create(1);
		box->setName("Crate_Wooden_" + StringUtils::format("%d", i + 1));
		box->SetSprite(_woodenSprites[i]);

		_woodBoxes.push_back(box);

		addChild(box);
	}

	// METAL CRATES
	for (int i = 0; i < _metalSprites.size(); i++) {
		Box* box = Box::create(2);
		box->setName("Crate_Metal_" + StringUtils::format("%d", i + 1));
		box->SetSprite(_metalSprites[i]);

		_metalBoxes.push_back(box);

		addChild(box);
	}

	// SWITCHES
	for (int i = 0; i < _gravSwitches.size(); i++) {
		Switch* gravSwitch = Switch::create();
		gravSwitch->setName("Switch_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_gravSwitches[i]);
		gravSwitch->SetOrientation((i + 1) % 4);

		_switches.push_back(gravSwitch);

		addChild(gravSwitch);
	}
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

			//"Gameover shit added". Ya taffer.
			//GameManager::sharedGameManager()->setIsGamePaused(false);
			GameManager::sharedGameManager()->setIsGameLive(false);
			GameManager::sharedGameManager()->setCurrentLevel(_level);
			auto scene = GameOverScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.0f, scene));
		}
	}
}

void SceneManager::CheckCollisions()
{
	for (int i = 0; i < _platforms.size(); i++) {
		_player->CheckPlatformCollisions(_platforms[i]);

		for (int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckPlatformCollisions(_platforms[i]);
		}

		for (int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckPlatformCollisions(_platforms[i]);
		}
	}

	for (int i = 0; i < _walls.size(); i++) {
		_player->CheckWallCollisions(_walls[i]);

		for (int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckWallCollisions(_walls[i]);
		}

		for (int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckWallCollisions(_walls[i]);
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

			_player->SetTarget(_initialTouchPos);
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

	for (int i = 0; i < _switches.size(); i++) {
		if (findCheckBox->getName() == _switches.at(i)->GetSprite()->getName()) {
			_flipped[i] = !_flipped[i];
			_switches.at(i)->GetSprite()->setFlippedX(_flipped[i]);

			// Flip Gravity
			if (_flipGravityCooldown == 0.0f) {
				FlipGravity(_switches.at(i)->GetOrientation());
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
	bool highLightActive = false;

	for (int i = 0; i < _switches.size(); i++) {
		// Player needs to be near the switch to press
		float scaledWidth = _switches.at(i)->GetSprite()->getContentSize().width * _switches.at(i)->GetSprite()->getScaleX();
		float scaledHeight = _switches.at(i)->GetSprite()->getContentSize().height * _switches.at(i)->GetSprite()->getScaleY();

		if (_player->GetSprite()->getPositionX() - (_player->GetSprite()->getContentSize().width / 2) < _switches.at(i)->GetSprite()->getPositionX() + (scaledWidth / 2) + (_player->GetSprite()->getContentSize().width / 2) + 20
			&& _player->GetSprite()->getPositionX() + (_player->GetSprite()->getContentSize().width / 2) > _switches.at(i)->GetSprite()->getPositionX() - (scaledWidth / 2) - (_player->GetSprite()->getContentSize().width / 2) - 20
			&& _player->GetSprite()->getPositionY() - (_player->GetSprite()->getContentSize().height / 2) < _switches.at(i)->GetSprite()->getPositionY() + (scaledHeight / 2)
			&& _player->GetSprite()->getPositionY() + (_player->GetSprite()->getContentSize().height / 2) > _switches.at(i)->GetSprite()->getPositionY() - (scaledHeight / 2))
		{
			//_switches.at(i)->GetSprite()->setEnabled(true);
			_switches.at(i)->GetSprite()->setEnabled(true);

			if (!highLightActive) {

				highLightActive = true;

				if (_switches.at(i)->GetOrientation() == 0) {
					if (_topHighlight->getOpacity() <= 0) {
						_topHighlight->runAction(FadeIn::create(1.0f));
					}
				}
				else if (_switches.at(i)->GetOrientation() == 1) {
					if (!_leftHighlight->getOpacity() <= 0) {
						_leftHighlight->runAction(FadeIn::create(1.0f));
					}
				}
				else if (_switches.at(i)->GetOrientation() == 2) {
					if (!_bottomHighlight->getOpacity() <= 0) {
						_bottomHighlight->runAction(FadeIn::create(1.0f));
					}
				}
				else if (_switches.at(i)->GetOrientation() == 3) {
					if (!_rightHighlight->getOpacity() <= 0) {
						_rightHighlight->runAction(FadeIn::create(1.0f));
					}
				}
			}
		}
		else {
			_switches.at(i)->GetSprite()->setEnabled(false);
		}
	}

	if (!highLightActive) {
		// Top
		if (_topHighlight->getOpacity() == 255) {
			_topHighlight->runAction(FadeOut::create(1.0f));
		}

		// Right
		if (_rightHighlight->getOpacity() == 255) {
			_rightHighlight->runAction(FadeOut::create(1.0f));
		}

		// Bottom
		if (_bottomHighlight->getOpacity() == 255) {
			_bottomHighlight->runAction(FadeOut::create(1.0f));
		}

		// Left
		if (_leftHighlight->getOpacity() == 255) {
			_leftHighlight->runAction(FadeOut::create(1.0f));
		}
	}
}

void SceneManager::CheckNearDoor()
{
	for (int i = 0; i < _exit.size(); i++) {
		float scaledWidth = _exit[i]->getContentSize().width * _exit[i]->getScaleX();
		float scaledHeight = _exit[i]->getContentSize().height * _exit[i]->getScaleY();

		if (_player->GetSprite()->getPositionX() - (_player->GetSprite()->getContentSize().width / 2) < _exit[i]->getPositionX() + (scaledWidth / 2) + (_player->GetSprite()->getContentSize().width / 2) + 20
			&& _player->GetSprite()->getPositionX() + (_player->GetSprite()->getContentSize().width / 2) > _exit[i]->getPositionX() - (scaledWidth / 2) - (_player->GetSprite()->getContentSize().width / 2) - 20
			&& _player->GetSprite()->getPositionY() - (_player->GetSprite()->getContentSize().height / 2) < _exit[i]->getPositionY() + (scaledHeight / 2)
			&& _player->GetSprite()->getPositionY() + (_player->GetSprite()->getContentSize().height / 2) > _exit[i]->getPositionY() - (scaledHeight / 2))
		{
			//_switches.at(i)->GetSprite()->setEnabled(true);
			_exit[i]->setEnabled(true);
		}
		else {
			_exit[i]->setEnabled(false);
		}
	}
}

void SceneManager::FlipGravity(int direction)
{
	_gravityOrientation = direction;

	// Handle DOWN gravity
	if (direction == 0) {
		_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);

		for (int i = 0; i < _woodBoxes.size(); i++) {
			_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
		}

		for (int i = 0; i < _metalBoxes.size(); i++) {
			_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
		}

		_player->SetGravity(-3.81f);
		_player->SetFallingVertical(true);
		_player->SetFallingHorizontal(false);

		for (int i = 0; i < _woodBoxes.size(); i++) {
			_woodBoxes[i]->SetGravity(-3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);
		}
	}
	// Handle LEFT gravity
	else if (direction == 1) {
		_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

		for (int i = 0; i < _woodBoxes.size(); i++) {
			_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 0.5f);
		}

		for (int i = 0; i < _metalBoxes.size(); i++) {
			_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 0.5f);
		}

		_player->SetGravity(-3.81f);
		_player->SetFallingVertical(false);
		_player->SetFallingHorizontal(true);

		for (int i = 0; i < _woodBoxes.size(); i++) {
			_woodBoxes[i]->SetGravity(-3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);
		}
	}
	// Handle UP gravity
	else if (direction == 2) {
		_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);

		for (int i = 0; i < _woodBoxes.size(); i++) {
			_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
		}

		for (int i = 0; i < _metalBoxes.size(); i++) {
			_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
		}

		_player->SetGravity(3.81f);
		_player->SetFallingVertical(true);
		_player->SetFallingHorizontal(false);

		for (int i = 0; i < _woodBoxes.size(); i++) {
			_woodBoxes[i]->SetGravity(3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);
		}
	}
	// Handle RIGHT gravity
	else if (direction == 3) {
		_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

		for (int i = 0; i < _woodBoxes.size(); i++) {
			_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
		}

		for (int i = 0; i < _metalBoxes.size(); i++) {
			_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
		}

		_player->SetGravity(3.81f);
		_player->SetFallingVertical(false);
		_player->SetFallingHorizontal(true);

		for (int i = 0; i < _woodBoxes.size(); i++) {
			_woodBoxes[i]->SetGravity(3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);
		}
	}
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
		delete _switches.at(i)->GetSprite();
	}

	_gravSwitches.clear();
}

