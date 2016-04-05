#include "SceneManager.h"

Scene* SceneManager::createScene(int level)
{
	auto scene = Scene::create();
	auto layer = SceneManager::create();
	layer->SetLevel(level);
	layer->SetGravityOrientation(0);
	layer->SetupNode();

	scene->addChild(layer);

	return scene;
}

bool SceneManager::init()
{
	if (!Layer::init()) {
		return false;
	}

	return true;
}

bool SceneManager::SetupNode()
{
	try {
		// GAMEMANAGER
		GameManager::sharedGameManager()->setIsGameLive(true);
		GameManager::sharedGameManager()->setIsGamePaused(true);
		GameManager::sharedGameManager()->setIsObjectTouched(false);
		GameManager::sharedGameManager()->setCurrentLevel(_level);

		// NODE
		Node* rootNode = CSLoader::createNode("Levels/Scene" + StringUtils::format("%d", _level) + ".csb");
		addChild(rootNode);

		this->scheduleUpdate();

		// SETUP CLASSES
		SetupTouches();

		SetupTimer(rootNode);
		SetupAudio(rootNode);
		SetupButtons(rootNode);
		SetupBackground(rootNode);
		SetupHighlights(rootNode);

		SetupPlayer(rootNode);
		SetupWalls(rootNode);
		SetupBoxes(rootNode);
		SetupFloorButtons(rootNode);
		SetupSwitches(rootNode);
		SetupExits(rootNode);
		SetupRails(rootNode);
		SetupSpawns(rootNode);

		// Door and Platform setup MUST be done after all other setups, as it needs to see other scene elements.
		SetupDoors(rootNode);
		SetupPlatforms(rootNode);

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupTouches()
{
	try {
		// SINGLE TOUCHES
		//Set up a touch listener.
		auto touchListener = EventListenerTouchOneByOne::create();

		//Set callbacks for our touch functions.
		touchListener->onTouchBegan = CC_CALLBACK_2(SceneManager::onTouchBegan, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(SceneManager::onTouchEnded, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(SceneManager::onTouchMoved, this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(SceneManager::onTouchCancelled, this);

		//Add our touch listener to event listener list.
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

		//Setting up Multi Touch Listener
		_touchMGR = new TouchManager;
		auto touchesListener = EventListenerTouchAllAtOnce::create();

		touchesListener->onTouchesBegan = CC_CALLBACK_2(TouchManager::onTouchesBegan, _touchMGR);
		touchesListener->onTouchesEnded = CC_CALLBACK_2(TouchManager::onTouchesEnded, _touchMGR);
		touchesListener->onTouchesMoved = CC_CALLBACK_2(TouchManager::onTouchesMoved, _touchMGR);
		touchesListener->onTouchesCancelled = CC_CALLBACK_2(TouchManager::onTouchesCancelled, _touchMGR);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchesListener, this);
		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchesListener, this);

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupTimer(Node* root)
{
	try {
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

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupAudio(Node* root)
{
	try {
		// Check if the game is muted before playing background music
		if (GameManager::sharedGameManager()->getIsGameMuted() == false) {
			_auEngine->PlayBackgroundMusic("testing.mp3", true);
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupButtons(Node* root)
{
	try {
		// Get screen size
		auto winSize = Director::getInstance()->getVisibleSize();
		GameManager::sharedGameManager()->startLevelTimer();

		// START BUTTON
		// Note: This is a debug button that may need removing for the final version
		_startGame = static_cast<ui::Button*>(root->getChildByName("StartGame"));
		_startGame->addTouchEventListener(CC_CALLBACK_2(SceneManager::StartButtonPressed, this));
		_startGame->setPosition(Vec2(winSize.width*0.5, winSize.height*0.5));

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupPlayer(Node* root)
{
	try {
		Sprite* playerSprite = (Sprite*)root->getChildByName("Player");

		_player = Player::create();
		_player->SetSprite(playerSprite);
		_player->setName("Player");
		_player->SetGravity(-3.81f);

		addChild(_player);

		root->removeChildByName("Player");

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupPlatforms(Node* root)
{
	try {
		int i = 1;
		Sprite* tempSprite;

		// Normal Platforms
		while ((tempSprite = (Sprite*)root->getChildByName("Platform_" + StringUtils::format("%d", i))) != nullptr)
		{
			_platforms.push_back(tempSprite);
			i++;
		}

		// Horizontal Moving Platforms
		i = 1;
		while ((tempSprite = (Sprite*)root->getChildByName("MovingPlatformX_" + StringUtils::format("%d", i))) != nullptr)
		{
			//_movingPlatformVertSprites.push_back(tempSprite);
			Platforms* movingPlat = Platforms::create(_player, _woodBoxes, _metalBoxes);
			movingPlat->setName("MovingPlatform_" + StringUtils::format("%d", i));
			movingPlat->SetSprite(tempSprite);
			movingPlat->setZoneSprite();

			_movingPlatformsHorizontal.push_back(movingPlat);
			addChild(movingPlat);
			root->removeChildByName("MovingPlatformX_" + StringUtils::format("%d", i));
			i++;
		}

		// Vertical Moving Platforms
		i = 1;
		while ((tempSprite = (Sprite*)root->getChildByName("MovingPlatformY_" + StringUtils::format("%d", i))) != nullptr)
		{
			Platforms* movingPlat = Platforms::create(_player, _woodBoxes, _metalBoxes);
			movingPlat->setName("MovingPlatform_" + StringUtils::format("%d", i));
			movingPlat->SetSprite(tempSprite);
			movingPlat->setZoneSprite();

			_movingPlatformsVertical.push_back(movingPlat);
			addChild(movingPlat);
			root->removeChildByName("MovingPlatformY_" + StringUtils::format("%d", i));
			i++;
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupWalls(Node* root)
{
	try {
		// Normal Walls
		int i = 1;
		Sprite* tempSprite;

		while ((tempSprite = (Sprite*)root->getChildByName("Wall_" + StringUtils::format("%d", i))) != nullptr)
		{
			_walls.push_back(tempSprite);
			i++;
		}

		// Horizontal Moving Walls
		i = 1;
		while ((tempSprite = (Sprite*)root->getChildByName("MovingWallX_" + StringUtils::format("%d", i))) != nullptr)
		{
			Wall* movingWall = Wall::create(_player, _woodBoxes, _metalBoxes);
			movingWall->setName("MovingWall_" + StringUtils::format("%d", i + 1));
			movingWall->setSprite(tempSprite);
			movingWall->setZoneSprite();

			_movingWallsHorizontal.push_back(movingWall);
			addChild(movingWall);
			i++;
		}

		// Vertical Moving Walls
		i = 1;
		while ((tempSprite = (Sprite*)root->getChildByName("MovingWallY_" + StringUtils::format("%d", i))) != nullptr)
		{
			Wall* movingWall = Wall::create(_player, _woodBoxes, _metalBoxes);
			movingWall->setName("MovingWall_" + StringUtils::format("%d", i + 1));
			movingWall->setSprite(tempSprite);
			movingWall->setZoneSprite();

			_movingWallsVertical.push_back(movingWall);
			addChild(movingWall);
			i++;
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupBoxes(Node* root)
{
	try {
		// Wooden Crates
		int i = 1;
		Sprite* tempSprite;

		while ((tempSprite = (Sprite*)root->getChildByName("Crate_Wooden_" + StringUtils::format("%d", i))) != nullptr)
		{
			Box* box = Box::create(1, 1.0f);
			box->setName("Crate_Wooden_" + StringUtils::format("%d", i));
			box->SetSprite(tempSprite);
			box->SetGravity(-3.81f);

			_woodBoxes.push_back(box);
			addChild(box);
			root->removeChildByName("Crate_Wooden_" + StringUtils::format("%d", i));
			i++;
		}

		// Metal Crates
		i = 1;

		while ((tempSprite = (Sprite*)root->getChildByName("Crate_Metal_" + StringUtils::format("%d", i))) != nullptr)
		{
			Box* box = Box::create(2, 2.0f);
			box->setName("Crate_Metal_" + StringUtils::format("%d", i));
			box->SetSprite(tempSprite);
			box->SetGravity(-3.81f);

			_metalBoxes.push_back(box);
			addChild(box);
			root->removeChildByName("Crate_Metal_" + StringUtils::format("%d", i));
			i++;
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupFloorButtons(Node* root)
{
	try {
		// DOWN BUTTONS
		int i = 1;
		cocos2d::Sprite* tempButton;

		while ((tempButton = (Sprite*)(root->getChildByName("Button_Down_" + StringUtils::format("%d", i)))) != nullptr)
		{
			FloorButton* button = FloorButton::create(0);
			button->setName("Button_Down_" + StringUtils::format("%d", i));
			button->SetSprite(tempButton);
			button->SetActive(false);

			_buttons.push_back(button);
			addChild(button);
			i++;
		}

		// LEFT BUTTONS
		i = 1;
		while ((tempButton = (Sprite*)(root->getChildByName("Button_Left_" + StringUtils::format("%d", i)))) != nullptr)
		{
			FloorButton* button = FloorButton::create(1);
			button->setName("Button_Left_" + StringUtils::format("%d", i));
			button->SetSprite(tempButton);
			button->SetActive(false);

			_buttons.push_back(button);
			addChild(button);
			i++;
		}

		// UP BUTTONS
		i = 1;
		while ((tempButton = (Sprite*)(root->getChildByName("Button_Up_" + StringUtils::format("%d", i)))) != nullptr)
		{
			FloorButton* button = FloorButton::create(2);
			button->setName("Button_Up_" + StringUtils::format("%d", i));
			button->SetSprite(tempButton);
			button->SetActive(false);

			_buttons.push_back(button);

			addChild(button);
			i++;
		}

		// RIGHT BUTTONS
		i = 1;
		while ((tempButton = (Sprite*)(root->getChildByName("Button_Right_" + StringUtils::format("%d", i)))) != nullptr)
		{
			FloorButton* button = FloorButton::create(3);
			button->setName("Button_Right_" + StringUtils::format("%d", i));
			button->SetSprite(tempButton);
			button->SetActive(false);

			_buttons.push_back(button);

			addChild(button);
			i++;
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupDoors(Node* root)
{
	try {
		int i = 1;
		Sprite* tempSprite;

		// Normal Door
		while ((tempSprite = (Sprite*)root->getChildByName("Door_" + StringUtils::format("%d", i))) != nullptr)
		{
			Door* door = Door::create();
			door->setName("Door_" + StringUtils::format("%d", i + 1));
			door->SetSprite(_buttons, tempSprite);

			_doors.push_back(door);
			addChild(door);
			i++;
		}

		// Solid Door
		i = 1;
		while ((tempSprite = (Sprite*)root->getChildByName("Solid_Door_" + StringUtils::format("%d", i))) != nullptr)
		{
			_solidDoorSprites.push_back(tempSprite);
			i++;
		}

		// Hatch
		i = 1;
		while ((tempSprite = (Sprite*)root->getChildByName("Hatch_" + StringUtils::format("%d", i))) != nullptr)
		{
			Door* hatch = Door::create();
			hatch->setName("Hatch_" + StringUtils::format("%d", i + 1));
			hatch->SetSprite(_buttons, tempSprite);

			_hatches.push_back(hatch);
			addChild(hatch);
			i++;
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupSwitches(Node* root)
{
	try {
		// NORMAL SWITCHES
		// Normal Switch Down
		cocos2d::ui::CheckBox* tempCheck;
		int i = 1;

		while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Down_" + StringUtils::format("%d", i)))) != nullptr)
		{
			tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));

			Switch* gravSwitch = Switch::create();
			gravSwitch->setName("Switch_Down_" + StringUtils::format("%d", i));
			gravSwitch->SetSprite(tempCheck);
			gravSwitch->SetOrientation(0);

			_switches.push_back(gravSwitch);
			addChild(gravSwitch);
			i++;
		}

		// Normal Switch Left
		i = 1;
		while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Left_" + StringUtils::format("%d", i)))) != nullptr)
		{
			tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));

			Switch* gravSwitch = Switch::create();
			gravSwitch->setName("Switch_Left_" + StringUtils::format("%d", i));
			gravSwitch->SetSprite(tempCheck);
			gravSwitch->SetOrientation(1);

			_switches.push_back(gravSwitch);
			addChild(gravSwitch);
			i++;
		}

		// Normal Switch Up
		i = 1;
		while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Up_" + StringUtils::format("%d", i)))) != nullptr)
		{
			tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));

			Switch* gravSwitch = Switch::create();
			gravSwitch->setName("Switch_Up_" + StringUtils::format("%d", i));
			gravSwitch->SetSprite(tempCheck);
			gravSwitch->SetOrientation(2);

			_switches.push_back(gravSwitch);
			addChild(gravSwitch);
			i++;
		}

		// Normal Switch Right
		i = 1;
		while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Right_" + StringUtils::format("%d", i)))) != nullptr)
		{
			tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));

			Switch* gravSwitch = Switch::create();
			gravSwitch->setName("Switch_Right_" + StringUtils::format("%d", i));
			gravSwitch->SetSprite(tempCheck);
			gravSwitch->SetOrientation(3);

			_switches.push_back(gravSwitch);
			addChild(gravSwitch);
			i++;
		}

		// TIMER SWITCHES
		// Timed Switch Down
		i = 1;
		while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Down_" + StringUtils::format("%d", i)))) != nullptr)
		{
			tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));

			SwitchTimer* gravSwitch = SwitchTimer::create();
			gravSwitch->setName("SwitchTimer_Down_" + StringUtils::format("%d", i));
			gravSwitch->SetSprite(tempCheck);
			gravSwitch->SetOrientation(0);

			_timedSwitches.push_back(gravSwitch);
			addChild(gravSwitch);
			i++;
		}

		// Timed Switch Left
		i = 1;
		while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Left_" + StringUtils::format("%d", i)))) != nullptr)
		{
			tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));

			SwitchTimer* gravSwitch = SwitchTimer::create();
			gravSwitch->setName("SwitchTimer_Left_" + StringUtils::format("%d", i));
			gravSwitch->SetSprite(tempCheck);
			gravSwitch->SetOrientation(1);

			_timedSwitches.push_back(gravSwitch);
			addChild(gravSwitch);
			i++;
		}

		// Timed Switch Up
		i = 1;
		while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Up_" + StringUtils::format("%d", i)))) != nullptr)
		{
			tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));

			SwitchTimer* gravSwitch = SwitchTimer::create();
			gravSwitch->setName("SwitchTimer_Up_" + StringUtils::format("%d", i));
			gravSwitch->SetSprite(tempCheck);
			gravSwitch->SetOrientation(2);

			_timedSwitches.push_back(gravSwitch);
			addChild(gravSwitch);
			i++;
		}

		// Timed Switch Right
		i = 1;
		while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Right_" + StringUtils::format("%d", i)))) != nullptr)
		{
			tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));

			SwitchTimer* gravSwitch = SwitchTimer::create();
			gravSwitch->setName("SwitchTimer_Right_" + StringUtils::format("%d", i));
			gravSwitch->SetSprite(tempCheck);
			gravSwitch->SetOrientation(3);

			_timedSwitches.push_back(gravSwitch);
			addChild(gravSwitch);
			i++;
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupExits(Node* root)
{
	try {
		cocos2d::ui::CheckBox* exit;
		int i = 1;
		while ((exit = static_cast<ui::CheckBox*>(root->getChildByName("Exit_" + StringUtils::format("%d", i)))) != nullptr) {
			exit->addTouchEventListener(CC_CALLBACK_2(SceneManager::DoorPressed, this));
			_exit.push_back(exit);

			i++;
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupRails(Node* root)
{
	try {
		// Rail Start
		cocos2d::Sprite* tempRailStart;
		int i = 1;
		while ((tempRailStart = (Sprite*)(root->getChildByName("RailStart_" + StringUtils::format("%d", i)))) != nullptr)
		{
			_railStart.push_back(tempRailStart);
			i++;
		}

		//Rail end
		cocos2d::Sprite* tempRailEnd;
		i = 1;
		while ((tempRailEnd = (Sprite*)(root->getChildByName("RailEnd_" + StringUtils::format("%d", i)))) != nullptr)
		{
			_railEnd.push_back(tempRailEnd);
			i++;
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupSpawns(Node* root)
{
	try {
		// Crate Spawn zones
		cocos2d::Sprite* tempWoodSpawn;
		int i = 1;
		while ((tempWoodSpawn = (Sprite*)(root->getChildByName("WoodBoxSpawn_" + StringUtils::format("%d", i)))) != nullptr)
		{
			_woodCrateSpawn.push_back(tempWoodSpawn);
			i++;
		}

		cocos2d::Sprite* tempMetalSpawn;
		i = 1;
		while ((tempMetalSpawn = (Sprite*)(root->getChildByName("MetalBoxSpawn_" + StringUtils::format("%d", i)))) != nullptr)
		{
			_metalCrateSpawn.push_back(tempMetalSpawn);
			i++;
		}

		// Player Spawn Point
		_playerSpawn = (Sprite*)root->getChildByName("PlayerSpawn");
		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupBackground(Node* root)
{
	try {
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

		return true;
	}
	catch (...) {
		return false;
	}
}

bool SceneManager::SetupHighlights(Node* root)
{
	try {
		// Get screen size
		auto winSize = Director::getInstance()->getVisibleSize();

		// SETUP SPRITES
		_topHighlight = Sprite::create("highlight.png");
		_topHighlight->setPosition(Vec2(winSize.width*0.5f, winSize.height - (_topHighlight->getContentSize().height / 2)));
		_topHighlight->setScaleX(winSize.width);
		_topHighlight->setOpacity(0);
		_topHighlight->setLocalZOrder(3);

		_rightHighlight = Sprite::create("highlight.png");
		_rightHighlight->setPosition(Vec2(winSize.width - (_rightHighlight->getContentSize().height / 2), winSize.height * 0.5f));
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
		_leftHighlight->setPosition(Vec2(0.0f + (_rightHighlight->getContentSize().height / 2), winSize.height * 0.5f));
		_leftHighlight->setRotation(270.0f);
		_leftHighlight->setScaleX(winSize.height);
		_leftHighlight->setOpacity(0);
		_leftHighlight->setLocalZOrder(3);

		// SETUP TIMERS
		_topHighlightTime = 1.0f;
		_rightHighlightTime = 1.0f;
		_bottomHighlightTime = 1.0f;
		_leftHighlightTime = 1.0f;

		// SETUP ENABLES
		_topHighlightEnabled = false;
		_rightHighlightEnabled = false;
		_bottomHighlightEnabled = false;
		_leftHighlightEnabled = false;

		// ADD TO THIS
		this->addChild(_topHighlight);
		this->addChild(_leftHighlight);
		this->addChild(_bottomHighlight);
		this->addChild(_rightHighlight);

		return true;
	}
	catch (...) {
		return false;
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

			std::string X = "X";
			std::string Y = "Y";
			/*int playerPosXFl = _player->GetSprite()->getPositionX();
			int playerPosYFl = _player->GetSprite()->getPositionY();*/

			_timeLabel->setString(StringUtils::format("%d:%d:%d", min, sec, mil));

			CheckCollisions();
			CheckNearTimer(delta);
			RevertGravity();
			CheckNear(delta);
			CheckNearDoor(delta);
			IsPlayerInBounds();
		}
		else
		{
			//GameManager::sharedGameManager()->setIsGamePaused(false);
			GameManager::sharedGameManager()->setIsGameLive(false);
			auto scene = GameOverScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.0f, scene));
		}
	}
}

bool SceneManager::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (GameManager::sharedGameManager()->getIsGameLive() == true) {
		//Store the coordinates of where this touch began.
		Point touchPos = touch->getLocationInView();
		touchPos = Director::getInstance()->convertToGL(touchPos);
		touchPos = convertToNodeSpace(touchPos);
		Rect currPlatform;
		Rect currTouchZone;
		_initialTouchPos = touchPos;
		_inTouch = true;

		// Touch detection for horizontal moving platforms
		for (int i = 0; i < (int)_movingPlatformsHorizontal.size(); i++)
		{
			currPlatform = _movingPlatformsHorizontal[i]->getSprite()->getBoundingBox();
			currTouchZone = _movingPlatformsHorizontal[i]->getTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				for (int i = 0; i < (int)_movingPlatformsHorizontal.size(); i++)
				{
					_movingPlatformsHorizontal[i]->PlatformType(1);
					_movingPlatformsHorizontal[i]->onTouchBegan(touch, event);
				}
			}
		}

		// Touch detection for vertical moving platforms
		for (int i = 0; i < (int)_movingPlatformsVertical.size(); i++)
		{
			currPlatform = _movingPlatformsVertical[i]->getSprite()->getBoundingBox();
			currTouchZone = _movingPlatformsVertical[i]->getTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				for (int i = 0; i < (int)_movingPlatformsVertical.size(); i++)
				{
					_movingPlatformsVertical[i]->PlatformType(2);
					_movingPlatformsVertical[i]->onTouchBegan(touch, event);
				}
			}
		}

		// Touch detection for horizontal moving walls
		for (int i = 0; i < (int)_movingWallsHorizontal.size(); i++)
		{
			currPlatform = _movingWallsHorizontal[i]->getSprite()->getBoundingBox();
			currTouchZone = _movingWallsHorizontal[i]->getTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				for (int i = 0; i < (int)_movingWallsHorizontal.size(); i++)
				{
					_movingWallsHorizontal[i]->PlatformType(1);
					_movingWallsHorizontal[i]->onTouchBegan(touch, event);
				}
			}
		}

		// Touch detection for vertical moving walls
		for (int i = 0; i < (int)_movingWallsVertical.size(); i++)
		{
			currPlatform = _movingWallsVertical[i]->getSprite()->getBoundingBox();
			currTouchZone = _movingWallsVertical[i]->getTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				for (int i = 0; i < (int)_movingWallsVertical.size(); i++)
				{
					_movingWallsVertical[i]->PlatformType(2);
					_movingWallsVertical[i]->onTouchBegan(touch, event);
				}
			}
		}

		if (!GameManager::sharedGameManager()->getIsObjectTouched())
		{
			_player->SetTarget(_initialTouchPos);
		}

		return true;
	}
	else {
		return false;
	}

	return true;
}

void SceneManager::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (GameManager::sharedGameManager()->getIsGameLive() == true) {
		if (!GameManager::sharedGameManager()->getIsGamePaused()) {
			_inTouch = false;

			if (_woodBoxes.size() > 0)
			{
				for (int i = 0; i < (int)_woodBoxes.size(); i++)
				{
					_woodBoxes[i]->Collision(touch);
					_woodBoxes[i]->SetTotalDiff(_touchMGR->totalDiff);
				}
			}
			if (_metalBoxes.size() > 0)
			{
				for (int i = 0; i < (int)_metalBoxes.size(); i++)
				{
					_metalBoxes[i]->Collision(touch);
					_metalBoxes[i]->SetTotalDiff(_touchMGR->totalDiff);
				}
			}

			for (int i = 0; i < (int)_movingPlatformsHorizontal.size(); i++)
			{
				_movingPlatformsHorizontal[i]->onTouchEnded(touch, event);
			}

			for (int i = 0; i < (int)_movingPlatformsVertical.size(); i++)
			{
				_movingPlatformsVertical[i]->onTouchEnded(touch, event);
			}

			for (int i = 0; i < (int)_movingWallsHorizontal.size(); i++)
			{
				_movingWallsHorizontal[i]->onTouchEnded(touch, event);
			}

			for (int i = 0; i < (int)_movingWallsVertical.size(); i++)
			{
				_movingWallsVertical[i]->onTouchEnded(touch, event);
			}
		}
	}
}

void SceneManager::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	for (int i = 0; i < (int)_movingPlatformsHorizontal.size(); i++)
	{
		_movingPlatformsHorizontal[i]->onTouchMoved(touch, event);
	}

	for (int i = 0; i < (int)_movingPlatformsVertical.size(); i++)
	{
		_movingPlatformsVertical[i]->onTouchMoved(touch, event);
	}

	for (int i = 0; i < (int)_movingWallsHorizontal.size(); i++)
	{
		_movingWallsHorizontal[i]->onTouchMoved(touch, event);
	}

	for (int i = 0; i < (int)_movingWallsVertical.size(); i++)
	{
		_movingWallsVertical[i]->onTouchMoved(touch, event);
	}
}

void SceneManager::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void SceneManager::SwitchPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	// Find what switch has been clicked
	cocos2d::ui::CheckBox* findCheckBox = (cocos2d::ui::CheckBox*)sender;

	for (int i = 0; i < (int)_timedSwitches.size(); i++) {
		if (_timedSwitches.at(i)->IsTimerRunning()) {
			_timedSwitches.at(i)->DisableSwitchTimer();
		}
	}

	for (int i = 0; i < (int)_switches.size(); i++) {
		if (findCheckBox->getName() == _switches.at(i)->GetSprite()->getName()) {
			// Flip Gravity
			if (_flipGravityCooldown == 0.0f) {
				FlipGravity(_switches.at(i)->GetOrientation());
				_flipGravityCooldown = 1.0f;
			}
		}
	}
}

void SceneManager::DoorPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	cocos2d::ui::CheckBox* findCheckBox = (cocos2d::ui::CheckBox*)sender;

	for (int i = 0; i < (int)_exit.size(); i++) {
		if (findCheckBox->getName() == _exit.at(i)->getName()) {
			auto scene = GameWinScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
		}
	}
}

void SceneManager::StartButtonPressed(Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		GameManager::sharedGameManager()->setIsGamePaused(false);
		_startGame->setVisible(false);
	}
}

void SceneManager::SwitchTimerPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	// Find what switch has been clicked
	cocos2d::ui::CheckBox* findCheckBox = (cocos2d::ui::CheckBox*)sender;

	for (int i = 0; i < (int)_timedSwitches.size(); i++) {
		if (findCheckBox->getName() == _timedSwitches.at(i)->GetSprite()->getName()) {
			// Flip Gravity
			if (_flipGravityCooldown == 0.0f) {
				FlipGravity(_timedSwitches.at(i)->GetOrientation());
				_timedSwitches.at(i)->ResetTimer();
				_flipGravityCooldown = 1.0f;
			}
		}
		else {
			_timedSwitches.at(i)->DisableSwitchTimer();
		}
	}
}

void SceneManager::FlipGravity(int direction)
{
	_previousDirection = _gravityOrientation;
	_gravityOrientation = direction;

	// Handle DOWN gravity
	if (_previousDirection == 0) {
		if (direction == 1) { // Left
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(false);
				_metalBoxes[i]->SetFallingHorizontal(true);
			}
		}
		else if (direction == 2) { // Up
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 3) { // Right
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(3.81f);
				_metalBoxes[i]->SetFallingVertical(false);
				_metalBoxes[i]->SetFallingHorizontal(true);
			}
		}
	}
	// Handle LEFT gravity
	else if (_previousDirection == 1) {
		if (direction == 0) { // Down
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 2) { // Up
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 3) { // Right
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(3.81f);
				_metalBoxes[i]->SetFallingVertical(false);
				_metalBoxes[i]->SetFallingHorizontal(true);
			}
		}
	}
	// Handle UP gravity
	else if (_previousDirection == 2) {
		if (direction == 1) { // Left
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(false);
				_metalBoxes[i]->SetFallingHorizontal(true);
			}
		}
		else if (direction == 0) { // Down
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 3) { // Right
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 1.0f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 1.0f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 1.0f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(3.81f);
				_metalBoxes[i]->SetFallingVertical(false);
				_metalBoxes[i]->SetFallingHorizontal(true);
			}
		}
	}
	// Handle RIGHT gravity
	else if (_previousDirection == 3) {
		if (direction == 0) { // Down
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 2) { // Up
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 1) { // Left
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (int i = 0; i < (int)_woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (int i = 0; i < (int)_metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(false);
				_metalBoxes[i]->SetFallingHorizontal(true);
			}
		}
	}
}

void SceneManager::RevertGravity()
{
	for (int i = 0; i < (int)_timedSwitches.size(); i++)
	{
		if (_timedSwitches[i]->GetRevertGravity() == true)
		{
			FlipGravity(_timedSwitches[i]->SwitchGravity());
		}
	}
}

void SceneManager::CheckCollisions()
{
	// PLATFORM COLLISIONS
	for (int i = 0; i < (int)_platforms.size(); i++) {
		_player->CheckPlatformCollisions(_platforms[i]);

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckPlatformCollisions(_platforms[i]);
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckPlatformCollisions(_platforms[i]);
		}
	}

	// WALL COLLISIONS
	for (int i = 0; i < (int)_walls.size(); i++) {
		_player->CheckWallCollisions(_walls[i]);

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckWallCollisions(_walls[i]);
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckWallCollisions(_walls[i]);
		}

		for (int i2 = 0; i2 < (int)_movingPlatformsHorizontal.size(); i2++)
		{
			_movingPlatformsHorizontal[i2]->CheckWallCollisions(_walls[i]);
		}

		for (int i2 = 0; i2 < (int)_movingPlatformsVertical.size(); i2++)
		{
			_movingPlatformsVertical[i2]->CheckWallCollisions(_walls[i]);
		}
	}

	// DOOR COLLISIONS
	for (int i = 0; i < (int)_doors.size(); i++) {
		if (!_doors[i]->GetOpen()) {

			_player->CheckWallCollisions(_doors[i]->GetSprite());

			for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) {
				_woodBoxes[i2]->CheckWallCollisions(_doors[i]->GetSprite());
			}

			for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) {
				_metalBoxes[i2]->CheckWallCollisions(_doors[i]->GetSprite());
			}
		}

	}

	for (int i = 0; i < (int)_solidDoorSprites.size(); i++) {
		if (!_doors[i]->GetOpen()) {

			_player->CheckWallCollisions(_solidDoorSprites[i]);

			for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) {
				_woodBoxes[i2]->CheckWallCollisions(_solidDoorSprites[i]);
			}

			for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) {
				_metalBoxes[i2]->CheckWallCollisions(_solidDoorSprites[i]);
			}
		}

	}

	// Hatch Collisions
	for (int i = 0; i < (int)_hatches.size(); i++) {
		if (!_hatches[i]->GetOpen()) {
			if (getName().find("Hatch"))
			{
				_player->CheckPlatformCollisions(_hatches[i]->GetSprite());

				for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) {
					_woodBoxes[i2]->CheckPlatformCollisions(_hatches[i]->GetSprite());
				}

				for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) {
					_metalBoxes[i2]->CheckPlatformCollisions(_hatches[i]->GetSprite());
				}
			}
		}
	}

	// MOVING PLATFORM COLLISIONS
	for (int i = 0; i < (int)_movingPlatformsHorizontal.size(); i++) {
		_player->CheckPlatformCollisions(_movingPlatformsHorizontal[i]->getSprite());

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckPlatformCollisions(_movingPlatformsHorizontal[i]->getSprite());
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckPlatformCollisions(_movingPlatformsHorizontal[i]->getSprite());
		}
	}

	for (int i = 0; i < (int)_movingPlatformsVertical.size(); i++) {
		_player->CheckPlatformCollisions(_movingPlatformsVertical[i]->getSprite());

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckPlatformCollisions(_movingPlatformsVertical[i]->getSprite());
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckPlatformCollisions(_movingPlatformsVertical[i]->getSprite());
		}
	}

	// MOVING WALL COLLISIONS
	for (int i = 0; i < (int)_movingWallsHorizontal.size(); i++) {
		_player->CheckWallCollisions(_movingWallsHorizontal[i]->getSprite());

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckWallCollisions(_movingWallsHorizontal[i]->getSprite());
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckWallCollisions(_movingWallsHorizontal[i]->getSprite());
		}
	}

	for (int i = 0; i < (int)_movingWallsVertical.size(); i++) {
		_player->CheckWallCollisions(_movingWallsVertical[i]->getSprite());

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckWallCollisions(_movingWallsVertical[i]->getSprite());
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckWallCollisions(_movingWallsVertical[i]->getSprite());
		}
	}

	// BUTTON COLLISIONS
	for (int i = 0; i < (int)_buttons.size(); i++) {
		_buttons.at(i)->SetActive(false);

		_buttons.at(i)->CheckPlayerCollisions(_player);

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) {
			_buttons.at(i)->CheckBoxCollisions(_woodBoxes.at(i2));
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) {
			_buttons.at(i)->CheckBoxCollisions(_metalBoxes.at(i2));
		}
	}

	// Rail Start
	for (int i = 0; i < (int)_railStart.size(); i++)
	{
		for (int i2 = 0; i2 < (int)_movingPlatformsHorizontal.size(); i2++)
		{
			_movingPlatformsHorizontal[i2]->CheckPlatformCollisions(_railStart[i]);
		}

		for (int i2 = 0; i2 < (int)_movingPlatformsVertical.size(); i2++)
		{
			_movingPlatformsVertical[i2]->CheckPlatformCollisions(_railStart[i]);
		}
	}

	// Rail End
	for (int i = 0; i < (int)_railEnd.size(); i++)
	{
		for (int i2 = 0; i2 < (int)_movingPlatformsHorizontal.size(); i2++)
		{
			_movingPlatformsHorizontal[i2]->CheckPlatformCollisions(_railEnd[i]);
		}

		for (int i2 = 0; i2 < (int)_movingPlatformsVertical.size(); i2++)
		{
			_movingPlatformsVertical[i2]->CheckPlatformCollisions(_railEnd[i]);
		}
	}
}

void SceneManager::CheckNear(float delta)
{
	bool inProximty = false;

	for (int i = 0; i < (int)_switches.size(); i++) {
		// Player needs to be near the switch to press
		float scaledWidth = _switches.at(i)->GetSprite()->getContentSize().width * _switches.at(i)->GetSprite()->getScaleX();
		float scaledHeight = _switches.at(i)->GetSprite()->getContentSize().height * _switches.at(i)->GetSprite()->getScaleY();

		if (_player->GetSprite()->getPositionX() - (_player->GetSprite()->getContentSize().width / 2) < _switches.at(i)->GetSprite()->getPositionX() + (scaledWidth / 2) + (_player->GetSprite()->getContentSize().width / 2) + 20
			&& _player->GetSprite()->getPositionX() + (_player->GetSprite()->getContentSize().width / 2) > _switches.at(i)->GetSprite()->getPositionX() - (scaledWidth / 2) - (_player->GetSprite()->getContentSize().width / 2) - 20
			&& _player->GetSprite()->getPositionY() - (_player->GetSprite()->getContentSize().height / 2) < _switches.at(i)->GetSprite()->getPositionY() + (scaledHeight / 2) + (_player->GetSprite()->getContentSize().height / 2) + 20
			&& _player->GetSprite()->getPositionY() + (_player->GetSprite()->getContentSize().height / 2) > _switches.at(i)->GetSprite()->getPositionY() - (scaledHeight / 2) - (_player->GetSprite()->getContentSize().height / 2) - 20)
		{
			//_switches.at(i)->GetSprite()->setEnabled(true);
			_switches.at(i)->GetSprite()->setEnabled(true);

			inProximty = true;

			if (_switches.at(i)->GetOrientation() == 0) {	// Down
				if (!_bottomHighlightEnabled) {
					_bottomHighlightTime = 1.0f;
					_bottomHighlight->runAction(FadeIn::create(_bottomHighlightTime));
					_bottomHighlightEnabled = true;
				}
			}
			else if (_switches.at(i)->GetOrientation() == 1) {	// Left
				if (!_leftHighlightEnabled) {
					_leftHighlightTime = 1.0f;
					_leftHighlight->runAction(FadeIn::create(_leftHighlightTime));
					_leftHighlightEnabled = true;
				}
			}
			else if (_switches.at(i)->GetOrientation() == 2) {	// Up
				if (!_topHighlightEnabled) {
					_topHighlightTime = 1.0f;
					_topHighlight->runAction(FadeIn::create(_topHighlightTime));
					_topHighlightEnabled = true;
				}
			}
			else if (_switches.at(i)->GetOrientation() == 3) {	// Right
				if (!_rightHighlightEnabled) {
					_rightHighlightTime = 1.0f;
					_rightHighlight->runAction(FadeIn::create(_rightHighlightTime));
					_rightHighlightEnabled = true;
				}
			}
		}
		else {
			_switches.at(i)->GetSprite()->setEnabled(false);
		}
	}

	if (_topHighlightEnabled) {
		if (_topHighlightTime > 0.0f) {
			_topHighlightTime -= delta;
		}
		else if (!inProximty) {
			_topHighlight->runAction(FadeOut::create(1.0f));
			_topHighlightEnabled = false;
		}
	}
	else if (_rightHighlightEnabled) {
		if (_rightHighlightTime > 0.0f) {
			_rightHighlightTime -= delta;
		}
		else if (!inProximty) {
			_rightHighlight->runAction(FadeOut::create(1.0f));
			_rightHighlightEnabled = false;
		}
	}
	else if (_bottomHighlightEnabled) {
		if (_bottomHighlightTime > 0.0f) {
			_bottomHighlightTime -= delta;
		}
		else if (!inProximty) {
			_bottomHighlight->runAction(FadeOut::create(1.0f));
			_bottomHighlightEnabled = false;
		}
	}
	else if (_leftHighlightEnabled) {
		if (_leftHighlightTime > 0.0f) {
			_leftHighlightTime -= delta;
		}
		else if (!inProximty) {
			_leftHighlight->runAction(FadeOut::create(1.0f));
			_leftHighlightEnabled = false;
		}
	}
}

void SceneManager::CheckNearTimer(float delta)
{
	bool inProximty = false;

	for (int i = 0; i < (int)_timedSwitches.size(); i++) {
		// Player needs to be near the switch to press
		float scaledWidth = _timedSwitches.at(i)->GetSprite()->getContentSize().width * _timedSwitches.at(i)->GetSprite()->getScaleX();
		float scaledHeight = _timedSwitches.at(i)->GetSprite()->getContentSize().height * _timedSwitches.at(i)->GetSprite()->getScaleY();

		if (_player->GetSprite()->getPositionX() - (_player->GetSprite()->getContentSize().width / 2) < _timedSwitches.at(i)->GetSprite()->getPositionX() + (scaledWidth / 2) + (_player->GetSprite()->getContentSize().width / 2) + 20
			&& _player->GetSprite()->getPositionX() + (_player->GetSprite()->getContentSize().width / 2) > _timedSwitches.at(i)->GetSprite()->getPositionX() - (scaledWidth / 2) - (_player->GetSprite()->getContentSize().width / 2) - 20
			&& _player->GetSprite()->getPositionY() - (_player->GetSprite()->getContentSize().height / 2) < _timedSwitches.at(i)->GetSprite()->getPositionY() + (scaledHeight / 2) + (_player->GetSprite()->getContentSize().height / 2) + 20
			&& _player->GetSprite()->getPositionY() + (_player->GetSprite()->getContentSize().height / 2) > _timedSwitches.at(i)->GetSprite()->getPositionY() - (scaledHeight / 2) - (_player->GetSprite()->getContentSize().height / 2) - 20)
		{
			//_timedSwitches.at(i)->GetSprite()->setEnabled(true);
			_timedSwitches.at(i)->GetSprite()->setEnabled(true);

			inProximty = true;

			if (_timedSwitches.at(i)->GetOrientation() == 0) {	// Down
				if (!_bottomHighlightEnabled) {
					_bottomHighlightTime = 1.0f;
					_bottomHighlight->runAction(FadeIn::create(_bottomHighlightTime));
					_bottomHighlightEnabled = true;
				}
			}
			else if (_timedSwitches.at(i)->GetOrientation() == 1) {	// Left
				if (!_leftHighlightEnabled) {
					_leftHighlightTime = 1.0f;
					_leftHighlight->runAction(FadeIn::create(_leftHighlightTime));
					_leftHighlightEnabled = true;
				}
			}
			else if (_timedSwitches.at(i)->GetOrientation() == 2) {	// Up
				if (!_topHighlightEnabled) {
					_topHighlightTime = 1.0f;
					_topHighlight->runAction(FadeIn::create(_topHighlightTime));
					_topHighlightEnabled = true;
				}
			}
			else if (_timedSwitches.at(i)->GetOrientation() == 3) {	// Right
				if (!_rightHighlightEnabled) {
					_rightHighlightTime = 1.0f;
					_rightHighlight->runAction(FadeIn::create(_rightHighlightTime));
					_rightHighlightEnabled = true;
				}
			}
		}
		else {
			_timedSwitches.at(i)->GetSprite()->setEnabled(false);
		}
	}

	if (_topHighlightEnabled) {
		if (_topHighlightTime > 0.0f) {
			_topHighlightTime -= delta;
		}
		else if (!inProximty) {
			_topHighlight->runAction(FadeOut::create(1.0f));
			_topHighlightEnabled = false;
		}
	}
	else if (_rightHighlightEnabled) {
		if (_rightHighlightTime > 0.0f) {
			_rightHighlightTime -= delta;
		}
		else if (!inProximty) {
			_rightHighlight->runAction(FadeOut::create(1.0f));
			_rightHighlightEnabled = false;
		}
	}
	else if (_bottomHighlightEnabled) {
		if (_bottomHighlightTime > 0.0f) {
			_bottomHighlightTime -= delta;
		}
		else if (!inProximty) {
			_bottomHighlight->runAction(FadeOut::create(1.0f));
			_bottomHighlightEnabled = false;
		}
	}
	else if (_leftHighlightEnabled) {
		if (_leftHighlightTime > 0.0f) {
			_leftHighlightTime -= delta;
		}
		else if (!inProximty) {
			_leftHighlight->runAction(FadeOut::create(1.0f));
			_leftHighlightEnabled = false;
		}
	}
}

void SceneManager::CheckNearDoor(float delta)
{
	for (int i = 0; i < (int)_exit.size(); i++) {
		float scaledWidth = _exit[i]->getContentSize().width * _exit[i]->getScaleX();
		float scaledHeight = _exit[i]->getContentSize().height * _exit[i]->getScaleY();

		if (_player->GetSprite()->getPositionX() - (_player->GetSprite()->getContentSize().width / 2) < _exit[i]->getPositionX() + (scaledWidth / 2) + (_player->GetSprite()->getContentSize().width / 2) + 20
			&& _player->GetSprite()->getPositionX() + (_player->GetSprite()->getContentSize().width / 2) > _exit[i]->getPositionX() - (scaledWidth / 2) - (_player->GetSprite()->getContentSize().width / 2) - 20
			&& _player->GetSprite()->getPositionY() - (_player->GetSprite()->getContentSize().height / 2) < _exit[i]->getPositionY() + (scaledHeight / 2)
			&& _player->GetSprite()->getPositionY() + (_player->GetSprite()->getContentSize().height / 2) > _exit[i]->getPositionY() - (scaledHeight / 2))
		{
			if (!_exit[i]->isEnabled()) {
				_exit[i]->setEnabled(true);
			}
		}
		else {
			if (_exit[i]->isEnabled()) {
				_exit[i]->setEnabled(false);
			}
		}
	}
}

void SceneManager::IsPlayerInBounds()
{
	auto winSize = Director::getInstance()->getVisibleSize();
	// Checks if the player has gone off the bottom of the screen
	if (_player->GetSprite()->getPosition().y < (0.0f - _player->GetSprite()->getContentSize().height)
		|| _player->GetSprite()->getPosition().x < (0.0f - _player->GetSprite()->getContentSize().width))
	{
		GameManager::sharedGameManager()->setIsGameLive(false);
		//_player->GetSprite()->setPosition(_woodCrateSpawn[0]->getPosition());

	}
	else if (_player->GetSprite()->getPosition().y >(winSize.height + _player->GetSprite()->getContentSize().height)
		|| _player->GetSprite()->getPosition().x >(winSize.width + _player->GetSprite()->getContentSize().width))
	{
		//_player->GetSprite()->setPosition(_woodCrateSpawn[0]->getPosition());
		GameManager::sharedGameManager()->setIsGameLive(false);
	}
}

void SceneManager::IsCrateInBounds()
{
	auto winSize = Director::getInstance()->getVisibleSize();

	// Moves Wood crates to spawn area if off screen
	for (int i = 0; i < (int)_woodBoxes.size(); i++)
	{
		if (_woodBoxes[i]->GetSprite()->getPosition().y < (0.0f - _woodBoxes[i]->GetSprite()->getContentSize().height)
			|| _woodBoxes[i]->GetSprite()->getPosition().x < (0.0f - _woodBoxes[i]->GetSprite()->getContentSize().width))
		{
			_woodBoxes[i]->GetSprite()->setPosition(_woodCrateSpawn[i]->getPosition());
		}
		else if (_woodBoxes[i]->GetSprite()->getPosition().y >(winSize.height + _woodBoxes[i]->GetSprite()->getContentSize().height)
			|| _woodBoxes[i]->GetSprite()->getPosition().x >(winSize.width + _woodBoxes[i]->GetSprite()->getContentSize().width))
		{
			_woodBoxes[i]->GetSprite()->setPosition(_woodCrateSpawn[i]->getPosition());
		}
	}

	// Moves Metal crates to spawn area if off screen
	for (int i = 0; i < (int)_metalBoxes.size(); i++)
	{
		if (_metalBoxes[i]->GetSprite()->getPosition().y < (0.0f - _metalBoxes[i]->GetSprite()->getContentSize().height)
			|| _metalBoxes[i]->GetSprite()->getPosition().x < (0.0f - _metalBoxes[i]->GetSprite()->getContentSize().width))
		{
			_metalBoxes[i]->GetSprite()->setPosition(_metalCrateSpawn[i]->getPosition());
		}
		else if (_metalBoxes[i]->GetSprite()->getPosition().y >(winSize.height + _metalBoxes[i]->GetSprite()->getContentSize().height)
			|| _metalBoxes[i]->GetSprite()->getPosition().x >(winSize.width + _metalBoxes[i]->GetSprite()->getContentSize().width))
		{
			_metalBoxes[i]->GetSprite()->setPosition(_metalCrateSpawn[i]->getPosition());
		}
	}
}