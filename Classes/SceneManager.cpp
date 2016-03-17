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

	Node* rootNode = CSLoader::createNode("Levels/Scene" + StringUtils::format("%d", _level) + ".csb");

	addChild(rootNode);

	this->scheduleUpdate();

	// GAMEMANAGER
	GameManager::sharedGameManager()->setIsGameLive(true);
	GameManager::sharedGameManager()->setIsGamePaused(true);
	GameManager::sharedGameManager()->setIsObjectTouched(false);
	GameManager::sharedGameManager()->setCurrentLevel(_level);

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
	SetupClasses(rootNode);


	return true;
		
}

void SceneManager::SetupTouches()
{
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
	touchMGR = new TouchManager;
	auto touchesListener = EventListenerTouchAllAtOnce::create();

	touchesListener->onTouchesBegan = CC_CALLBACK_2(TouchManager::onTouchesBegan, touchMGR);
	touchesListener->onTouchesEnded = CC_CALLBACK_2(TouchManager::onTouchesEnded, touchMGR);
	touchesListener->onTouchesMoved = CC_CALLBACK_2(TouchManager::onTouchesMoved, touchMGR);
	touchesListener->onTouchesCancelled = CC_CALLBACK_2(TouchManager::onTouchesCancelled, touchMGR);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchesListener, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchesListener, this);
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
	// REMINDER: Josh needs to follow our in-house comment conventions. 
	// how about no
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

	//Horizontal Moving Platforms
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("MovingPlatformY_" + StringUtils::format("%d", i))) != nullptr)
	{
		_movingPlatformVertSprites.push_back(tempSprite);
		i++;
	}

	//Vertical Moving Platforms
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("MovingPlatformX_" + StringUtils::format("%d", i))) != nullptr)
	{
		_movingPlatformHorizSprites.push_back(tempSprite);
		i++;
	}

	//Horizontal Moving Walls
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("MovingWallY_" + StringUtils::format("%d", i))) != nullptr)
	{
		_movingWallVertSprites.push_back(tempSprite);
		i++;
	}

	//Vertical Moving Walls
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("MovingWallX_" + StringUtils::format("%d", i))) != nullptr)
	{
		_movingWallHorizSprites.push_back(tempSprite);
		i++;
	}

	// WALLS
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Wall_" + StringUtils::format("%d", i))) != nullptr)
	{
		_walls.push_back(tempSprite);
		i++;
	}

	// DOORS
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Door_" + StringUtils::format("%d", i))) != nullptr)
	{
		_doorSprites.push_back(tempSprite);
		i++;
	}

	// HATCHES
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Hatch_" + StringUtils::format("%d", i))) != nullptr)
	{
		_hatchSprites.push_back(tempSprite);
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
	// Down
	cocos2d::ui::CheckBox* tempCheck;
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Down_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_gravSwitchesDown.push_back(tempCheck);
		i++;
	}

	// Left
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Left_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_gravSwitchesLeft.push_back(tempCheck);
		i++;
	}

	// Up
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Up_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_gravSwitchesUp.push_back(tempCheck);
		i++;
	}

	// Right
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Right_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_gravSwitchesRight.push_back(tempCheck);
		i++;
	}

	//
	// Timer Switches
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Down_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));
		_timerSwitchesDown.push_back(tempCheck);
		i++;
	}

	// Left
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Left_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));
		_timerSwitchesLeft.push_back(tempCheck);
		i++;
	}

	// Up
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Up_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));
		_timerSwitchesUp.push_back(tempCheck);
		i++;
	}

	// Right
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Right_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));
		_timerSwitchesRight.push_back(tempCheck);
		i++;
	}

	// EXIT DOOR
	cocos2d::ui::CheckBox* exit;
	i = 1;
	while ((exit = static_cast<ui::CheckBox*>(root->getChildByName("Exit_" + StringUtils::format("%d", i)))) != nullptr) {

		exit->addTouchEventListener(CC_CALLBACK_2(SceneManager::DoorPressed, this));
		_exit.push_back(exit);

		i++;
		exit = static_cast<ui::CheckBox*>(root->getChildByName("Exit_" + StringUtils::format("%d", i)));
	}

	// DOWN BUTTONS
	cocos2d::Sprite* tempButton;
	i = 1;
	while ((tempButton = (Sprite*)(root->getChildByName("Button_Down_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_downButtons.push_back(tempButton);
		i++;
	}

	// LEFT BUTTONS
	i = 1;
	while ((tempButton = (Sprite*)(root->getChildByName("Button_Left_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_leftButtons.push_back(tempButton);
		i++;
	}

	// UP BUTTONS
	i = 1;
	while ((tempButton = (Sprite*)(root->getChildByName("Button_Up_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_upButtons.push_back(tempButton);
		i++;
	}

	// RIGHT BUTTONS
	i = 1;
	while ((tempButton = (Sprite*)(root->getChildByName("Button_Right_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_rightButtons.push_back(tempButton);
		i++;
	}

	// Rail Start
	cocos2d::Sprite* tempRailStart;
	i = 1;
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

	// Crate Spawn zones
	cocos2d::Sprite* tempWoodSpawn;
	i = 1;
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
}

void SceneManager::SetupClasses(Node* root)
{
	// PLAYER
	_player = Player::create();
	_player->SetSprite(_playerSprite);
	_player->setName("Player");
	addChild(_player);

	// WOODEN CRATES
	for (unsigned int i = 0; i < _woodenSprites.size(); i++) {
		Box* box = Box::create(1, 1.0f);
		box->setName("Crate_Wooden_" + StringUtils::format("%d", i + 1));
		box->SetSprite(_woodenSprites[i]);

		_woodBoxes.push_back(box);

		addChild(box);
	}

	// METAL CRATES
	for (unsigned int i = 0; i < _metalSprites.size(); i++) {
		Box* box = Box::create(2, 2.0f);
		box->setName("Crate_Metal_" + StringUtils::format("%d", i + 1));
		box->SetSprite(_metalSprites[i]);

		_metalBoxes.push_back(box);

		addChild(box);
	}

	// SWITCHES
	// Down
	for (unsigned int i = 0; i < _gravSwitchesDown.size(); i++) {
		Switch* gravSwitch = Switch::create();
		gravSwitch->setName("Switch_Down_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_gravSwitchesDown[i]);
		gravSwitch->SetOrientation(0);

		_switches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	// Left
	for (unsigned int i = 0; i < _gravSwitchesLeft.size(); i++) {
		Switch* gravSwitch = Switch::create();
		gravSwitch->setName("Switch_Left_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_gravSwitchesLeft[i]);
		gravSwitch->SetOrientation(1);

		_switches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	// Up
	for (unsigned int i = 0; i < _gravSwitchesUp.size(); i++) {
		Switch* gravSwitch = Switch::create();
		gravSwitch->setName("Switch_Up_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_gravSwitchesUp[i]);
		gravSwitch->SetOrientation(2);

		_switches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	// Right
	for (unsigned int i = 0; i < _gravSwitchesRight.size(); i++) {
		Switch* gravSwitch = Switch::create();
		gravSwitch->setName("Switch_Right_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_gravSwitchesRight[i]);
		gravSwitch->SetOrientation(3);

		_switches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	// MOVING PLATFORMS - HORIZONTAL
	for (int i = 0; i < _movingPlatformHorizSprites.size(); i++)
	{
		Platforms* movingPlats = Platforms::create(_player, _woodBoxes, _metalBoxes);
		movingPlats->setName("MovingPlatform_" + StringUtils::format("%d", i + 1));
		movingPlats->setSprite(_movingPlatformHorizSprites[i]);
		movingPlats->setZoneSprite();

		_movingPlatformsHoriz.push_back(movingPlats);

		addChild(movingPlats);
	}

	// MOVING PLATFORMS - VERTICAL
	for (int i = 0; i < _movingPlatformVertSprites.size(); i++)
	{
		Platforms* movingPlats = Platforms::create(_player, _woodBoxes, _metalBoxes);
		movingPlats->setName("MovingPlatform_" + StringUtils::format("%d", i + 1));
		movingPlats->setSprite(_movingPlatformVertSprites[i]);
		movingPlats->setZoneSprite();

		_movingPlatformsVert.push_back(movingPlats);

		addChild(movingPlats);
	}

	// MOVING WALLS - HORIZONTAL
	for (int i = 0; i < _movingWallHorizSprites.size(); i++)
	{
		Wall* movingWall = Wall::create(_player, _woodBoxes, _metalBoxes);
		movingWall->setName("MovingWall_" + StringUtils::format("%d", i + 1));
		movingWall->setSprite(_movingWallHorizSprites[i]);
		movingWall->setZoneSprite();

		_movingWallsHoriz.push_back(movingWall);

		addChild(movingWall);
	}

	// MOVING WALLS - VERTICAL
	for (int i = 0; i < _movingWallVertSprites.size(); i++)
	{
		Wall* movingWall = Wall::create(_player, _woodBoxes, _metalBoxes);
		movingWall->setName("MovingWall_" + StringUtils::format("%d", i + 1));
		movingWall->setSprite(_movingWallVertSprites[i]);
		movingWall->setZoneSprite();

		_movingWallsVert.push_back(movingWall);

		addChild(movingWall);
	}

	// FLOOR BUTTONS - DOWN
	for (int i = 0; i < _downButtons.size(); i++) {
		FloorButton* button = FloorButton::create(0);
		button->setName("Button_Down_" + StringUtils::format("%d", i + 1));
		button->SetSprite(_downButtons[i]);
		button->SetActive(false);

		_buttons.push_back(button);

		addChild(button);
	}

	// FLOOR BUTTONS - LEFT
	for (int i = 0; i < _leftButtons.size(); i++) {
		FloorButton* button = FloorButton::create(1);
		button->setName("Button_Left_" + StringUtils::format("%d", i + 1));
		button->SetSprite(_leftButtons[i]);
		button->SetActive(false);

		_buttons.push_back(button);

		addChild(button);
	}

	// FLOOR BUTTONS - UP
	for (int i = 0; i < _upButtons.size(); i++) {
		FloorButton* button = FloorButton::create(2);
		button->setName("Button_Up_" + StringUtils::format("%d", i + 1));
		button->SetSprite(_upButtons[i]);
		button->SetActive(false);

		_buttons.push_back(button);

		addChild(button);
	}

	// FLOOR BUTTONS - RIGHT
	for (int i = 0; i < _rightButtons.size(); i++) {
		FloorButton* button = FloorButton::create(3);
		button->setName("Button_Right_" + StringUtils::format("%d", i + 1));
		button->SetSprite(_rightButtons[i]);
		button->SetActive(false);

		_buttons.push_back(button);

		addChild(button);
	}

	// DOORS
	for (unsigned int i = 0; i < _doorSprites.size(); i++) {
		Door* door = Door::create();
		door->setName("Door_" + StringUtils::format("%d", i + 1));
		door->SetSprite(_buttons, _doorSprites[i]);

		_doors.push_back(door);

		addChild(door);
	}

	// HATCHES
	for (unsigned int i = 0; i < _hatchSprites.size(); i++) {
		Door* hatch = Door::create();
		hatch->setName("Hatch_" + StringUtils::format("%d", i + 1));
		hatch->SetSprite(_buttons, _hatchSprites[i]);

		_hatches.push_back(hatch);

		addChild(hatch);
	}

	//Timer Switches Down
	for (unsigned int i = 0; i < _timerSwitchesDown.size(); i++) {
		SwitchTimer* gravSwitch = SwitchTimer::create();
		gravSwitch->setName("SwitchTimer_Down_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_timerSwitchesDown[i]);
		gravSwitch->SetOrientation(0);

		_tSwitches.push_back(gravSwitch);

		addChild(gravSwitch);
	}
	//Timer Switches Left
	for (unsigned int i = 0; i < _timerSwitchesLeft.size(); i++) {
		SwitchTimer* gravSwitch = SwitchTimer::create();
		gravSwitch->setName("SwitchTimer_Left_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_timerSwitchesLeft[i]);
		gravSwitch->SetOrientation(1);

		_tSwitches.push_back(gravSwitch);

		addChild(gravSwitch);
	}
	//Timer Switches Up
	for (unsigned int i = 0; i < _timerSwitchesUp.size(); i++) {
		SwitchTimer* gravSwitch = SwitchTimer::create();
		gravSwitch->setName("SwitchTimer_Up_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_timerSwitchesUp[i]);
		gravSwitch->SetOrientation(2);

		_tSwitches.push_back(gravSwitch);

		addChild(gravSwitch);
	}
	//Timer Switches Right
	for (unsigned int i = 0; i < _timerSwitchesRight.size(); i++) {
		SwitchTimer* gravSwitch = SwitchTimer::create();
		gravSwitch->setName("SwitchTimer_Right_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_timerSwitchesRight[i]);
		gravSwitch->SetOrientation(3);

		_tSwitches.push_back(gravSwitch);

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

			std::string X = "X";
			std::string Y = "Y";
			int playerPosXFl = _player->GetSprite()->getPositionX();
			int playerPosYFl = _player->GetSprite()->getPositionY();

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

void SceneManager::RevertGravity()
{
	for (int i = 0; i < _tSwitches.size(); i++)
	{
		if (_tSwitches[i]->GetRevertGravity() == true)
		{
			FlipGravity(_tSwitches[i]->SwitchGravity());
		}
	}
}

void SceneManager::CheckCollisions()
{
	// PLATFORM COLLISIONS
	for (unsigned int i = 0; i < _platforms.size(); i++) {
		_player->CheckPlatformCollisions(_platforms[i]);

		for (unsigned int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckPlatformCollisions(_platforms[i]);
		}

		for (unsigned int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckPlatformCollisions(_platforms[i]);
		}
	}

	// WALL COLLISIONS
	for (unsigned int i = 0; i < _walls.size(); i++) {
		_player->CheckWallCollisions(_walls[i]);

		for (unsigned int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckWallCollisions(_walls[i]);
		}

		for (unsigned int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckWallCollisions(_walls[i]);
		}

		for (unsigned int i2 = 0; i2 < _movingPlatformsHoriz.size(); i2++)
		{
			_movingPlatformsHoriz[i2]->CheckWallCollisions(_walls[i]);
		}

		for (unsigned int i2 = 0; i2 < _movingPlatformsVert.size(); i2++)
		{
			_movingPlatformsVert[i2]->CheckWallCollisions(_walls[i]);
		}
	}

	// DOOR COLLISIONS
	for (unsigned int i = 0; i < _doors.size(); i++) {
		if (!_doors[i]->GetOpen()) {

			_player->CheckWallCollisions(_doors[i]->GetSprite());

			for (unsigned int i2 = 0; i2 < _woodBoxes.size(); i2++) {
				_woodBoxes[i2]->CheckWallCollisions(_doors[i]->GetSprite());
			}

			for (unsigned int i2 = 0; i2 < _metalBoxes.size(); i2++) {
				_metalBoxes[i2]->CheckWallCollisions(_doors[i]->GetSprite());
			}
		}

	}

	// Hatch Collisions
	for (unsigned int i = 0; i < _hatches.size(); i++) {
		if (!_hatches[i]->GetOpen()) {
			if (getName().find("Hatch"))
			{
				_player->CheckPlatformCollisions(_hatches[i]->GetSprite());

				for (unsigned int i2 = 0; i2 < _woodBoxes.size(); i2++) {
					_woodBoxes[i2]->CheckPlatformCollisions(_hatches[i]->GetSprite());
				}

				for (unsigned int i2 = 0; i2 < _metalBoxes.size(); i2++) {
					_metalBoxes[i2]->CheckPlatformCollisions(_hatches[i]->GetSprite());
				}
			}
		}
	}

	// MOVING PLATFORM COLLISIONS
	for (int i = 0; i < _movingPlatformsHoriz.size(); i++) {
		_player->CheckPlatformCollisions(_movingPlatformsHoriz[i]->getSprite());

		for (int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckPlatformCollisions(_movingPlatformsHoriz[i]->getSprite());
		}

		for (int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckPlatformCollisions(_movingPlatformsHoriz[i]->getSprite());
		}
	}

	for (int i = 0; i < _movingPlatformsVert.size(); i++) {
		_player->CheckPlatformCollisions(_movingPlatformsVert[i]->getSprite());

		for (int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckPlatformCollisions(_movingPlatformsVert[i]->getSprite());
		}

		for (int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckPlatformCollisions(_movingPlatformsVert[i]->getSprite());
		}
	}

	// MOVING WALL COLLISIONS
	for (int i = 0; i < _movingWallsHoriz.size(); i++) {
		_player->CheckWallCollisions(_movingWallsHoriz[i]->getSprite());

		for (int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckWallCollisions(_movingWallsHoriz[i]->getSprite());
		}

		for (int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckWallCollisions(_movingWallsHoriz[i]->getSprite());
		}
	}

	for (int i = 0; i < _movingWallsVert.size(); i++) {
		_player->CheckWallCollisions(_movingWallsVert[i]->getSprite());

		for (int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_woodBoxes[i2]->CheckWallCollisions(_movingWallsVert[i]->getSprite());
		}

		for (int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_metalBoxes[i2]->CheckWallCollisions(_movingWallsVert[i]->getSprite());
		}
	}

	// BUTTON COLLISIONS
	for (unsigned int i = 0; i < _buttons.size(); i++) {
		_buttons.at(i)->SetActive(false);

		_buttons.at(i)->CheckPlayerCollisions(_player);

		for (unsigned int i2 = 0; i2 < _woodBoxes.size(); i2++) {
			_buttons.at(i)->CheckBoxCollisions(_woodBoxes.at(i2));
		}

		for (unsigned int i2 = 0; i2 < _metalBoxes.size(); i2++) {
			_buttons.at(i)->CheckBoxCollisions(_metalBoxes.at(i2));
		}
	}

	// Rail Start
	for (unsigned int i = 0; i < _railStart.size(); i++)
	{
		for (unsigned int i2 = 0; i2 < _movingPlatformsHoriz.size(); i2++)
		{
			_movingPlatformsHoriz[i2]->CheckPlatformCollisions(_railStart[i]);
		}

		for (unsigned int i2 = 0; i2 < _movingPlatformsVert.size(); i2++)
		{
			_movingPlatformsVert[i2]->CheckPlatformCollisions(_railStart[i]);
		}
	}

	// Rail End
	for (unsigned int i = 0; i < _railEnd.size(); i++)
	{
		for (unsigned int i2 = 0; i2 < _movingPlatformsHoriz.size(); i2++)
		{
			_movingPlatformsHoriz[i2]->CheckPlatformCollisions(_railEnd[i]);
		}

		for (unsigned int i2 = 0; i2 < _movingPlatformsVert.size(); i2++)
		{
			_movingPlatformsVert[i2]->CheckPlatformCollisions(_railEnd[i]);
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
		Rect currPlatform;
		Rect currTouchZone;
		_initialTouchPos = touchPos;
		_inTouch = true;

		if (_woodBoxes.size() > 0)
		{
			for (int i = 0; i < _woodBoxes.size(); i++)
			{
				_woodBoxes[i]->Collision(touch);
				_woodBoxes[i]->SetTotalDiff(touchMGR->totalDiff);
			}
		}
		if (_metalBoxes.size() > 0)
		{
			for (int i = 0; i < _metalBoxes.size(); i++)
			{
				_metalBoxes[i]->Collision(touch);
				_metalBoxes[i]->SetTotalDiff(touchMGR->totalDiff);
			}
		}

		// Touch detection for horizontal moving platforms
		for (int i = 0; i < _movingPlatformsHoriz.size(); i++)
		{
			currPlatform = _movingPlatformsHoriz[i]->getSprite()->getBoundingBox();
			currTouchZone = _movingPlatformsHoriz[i]->getTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				for (int i = 0; i < _movingPlatformsHoriz.size(); i++)
				{
					_movingPlatformsHoriz[i]->PlatformType(1);
					_movingPlatformsHoriz[i]->onTouchBegan(touch, event);
				}
			}
		}

		// Touch detection for vertical moving platforms
		for (int i = 0; i < _movingPlatformsVert.size(); i++)
		{
			currPlatform = _movingPlatformsVert[i]->getSprite()->getBoundingBox();
			currTouchZone = _movingPlatformsVert[i]->getTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				for (int i = 0; i < _movingPlatformsVert.size(); i++)
				{
					_movingPlatformsVert[i]->PlatformType(2);
					_movingPlatformsVert[i]->onTouchBegan(touch, event);
				}
			}
		}

		// Touch detection for horizontal moving walls
		for (int i = 0; i < _movingWallsHoriz.size(); i++)
		{
			currPlatform = _movingWallsHoriz[i]->getSprite()->getBoundingBox();
			currTouchZone = _movingWallsHoriz[i]->getTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				for (int i = 0; i < _movingWallsHoriz.size(); i++)
				{
					_movingWallsHoriz[i]->PlatformType(1);
					_movingWallsHoriz[i]->onTouchBegan(touch, event);
				}
			}
		}

		// Touch detection for vertical moving walls
		for (int i = 0; i < _movingWallsVert.size(); i++)
		{
			currPlatform = _movingWallsVert[i]->getSprite()->getBoundingBox();
			currTouchZone = _movingWallsVert[i]->getTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				for (int i = 0; i < _movingWallsVert.size(); i++)
				{
					_movingWallsVert[i]->PlatformType(2);
					_movingWallsVert[i]->onTouchBegan(touch, event);
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

			for (int i = 0; i < _movingPlatformsHoriz.size(); i++)
			{
				_movingPlatformsHoriz[i]->onTouchEnded(touch, event);
			}

			for (int i = 0; i < _movingPlatformsVert.size(); i++)
			{
				_movingPlatformsVert[i]->onTouchEnded(touch, event);
			}

			for (int i = 0; i < _movingWallsHoriz.size(); i++)
			{
				_movingWallsHoriz[i]->onTouchEnded(touch, event);
			}

			for (int i = 0; i < _movingWallsVert.size(); i++)
			{
				_movingWallsVert[i]->onTouchEnded(touch, event);
			}
		}
	}
}

void SceneManager::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");


	for (int i = 0; i < _movingPlatformsHoriz.size(); i++)
	{
		_movingPlatformsHoriz[i]->onTouchMoved(touch, event);
	}

	for (int i = 0; i < _movingPlatformsVert.size(); i++)
	{
		_movingPlatformsVert[i]->onTouchMoved(touch, event);
	}

	for (int i = 0; i < _movingWallsHoriz.size(); i++)
	{
		_movingWallsHoriz[i]->onTouchMoved(touch, event);
	}

	for (int i = 0; i < _movingWallsVert.size(); i++)
	{
		_movingWallsVert[i]->onTouchMoved(touch, event);
	}
}

void SceneManager::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}

void SceneManager::SwitchPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	// Find what switch has been clicked
	cocos2d::ui::CheckBox* findCheckBox = (cocos2d::ui::CheckBox*)sender;

	for (unsigned int i = 0; i < _switches.size(); i++) {
		if (findCheckBox->getName() == _switches.at(i)->GetSprite()->getName()) {
			// Flip Gravity
			if (_flipGravityCooldown == 0.0f) {
				FlipGravity(_switches.at(i)->GetOrientation());
				_flipGravityCooldown = 1.0f;
			}
		}
	}
}

void SceneManager::SwitchTimerPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	// Find what switch has been clicked
	cocos2d::ui::CheckBox* findCheckBox = (cocos2d::ui::CheckBox*)sender;

	for (unsigned int i = 0; i < _tSwitches.size(); i++) {
		if (findCheckBox->getName() == _tSwitches.at(i)->GetSprite()->getName()) {
			// Flip Gravity
			if (_flipGravityCooldown == 0.0f) {
				FlipGravity(_tSwitches.at(i)->GetOrientation());
				//_tSwitches.at(i)->SwitchGravity();
				_tSwitches.at(i)->ResetTimer();
				_flipGravityCooldown = 1.0f;
			}
		}
	}
}

void SceneManager::DoorPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	cocos2d::ui::CheckBox* findCheckBox = (cocos2d::ui::CheckBox*)sender;

	for (unsigned int i = 0; i < _exit.size(); i++) {
		if (findCheckBox->getName() == _exit.at(i)->getName()) {
			auto scene = GameWinScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
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

void SceneManager::CheckNear(float delta)
{
	bool inProximty = false;

	for (unsigned int i = 0; i < _switches.size(); i++) {
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

	for (unsigned int i = 0; i < _tSwitches.size(); i++) {
		// Player needs to be near the switch to press
		float scaledWidth = _tSwitches.at(i)->GetSprite()->getContentSize().width * _tSwitches.at(i)->GetSprite()->getScaleX();
		float scaledHeight = _tSwitches.at(i)->GetSprite()->getContentSize().height * _tSwitches.at(i)->GetSprite()->getScaleY();

		if (_player->GetSprite()->getPositionX() - (_player->GetSprite()->getContentSize().width / 2) < _tSwitches.at(i)->GetSprite()->getPositionX() + (scaledWidth / 2) + (_player->GetSprite()->getContentSize().width / 2) + 20
			&& _player->GetSprite()->getPositionX() + (_player->GetSprite()->getContentSize().width / 2) > _tSwitches.at(i)->GetSprite()->getPositionX() - (scaledWidth / 2) - (_player->GetSprite()->getContentSize().width / 2) - 20
			&& _player->GetSprite()->getPositionY() - (_player->GetSprite()->getContentSize().height / 2) < _tSwitches.at(i)->GetSprite()->getPositionY() + (scaledHeight / 2) + (_player->GetSprite()->getContentSize().height / 2) + 20
			&& _player->GetSprite()->getPositionY() + (_player->GetSprite()->getContentSize().height / 2) > _tSwitches.at(i)->GetSprite()->getPositionY() - (scaledHeight / 2) - (_player->GetSprite()->getContentSize().height / 2) - 20)
		{
			//_tSwitches.at(i)->GetSprite()->setEnabled(true);
			_tSwitches.at(i)->GetSprite()->setEnabled(true);

			inProximty = true;

			if (_tSwitches.at(i)->GetOrientation() == 0) {	// Down
				if (!_bottomHighlightEnabled) {
					_bottomHighlightTime = 1.0f;
					_bottomHighlight->runAction(FadeIn::create(_bottomHighlightTime));
					_bottomHighlightEnabled = true;
				}
			}
			else if (_tSwitches.at(i)->GetOrientation() == 1) {	// Left
				if (!_leftHighlightEnabled) {
					_leftHighlightTime = 1.0f;
					_leftHighlight->runAction(FadeIn::create(_leftHighlightTime));
					_leftHighlightEnabled = true;
				}
			}
			else if (_tSwitches.at(i)->GetOrientation() == 2) {	// Up
				if (!_topHighlightEnabled) {
					_topHighlightTime = 1.0f;
					_topHighlight->runAction(FadeIn::create(_topHighlightTime));
					_topHighlightEnabled = true;
				}
			}
			else if (_tSwitches.at(i)->GetOrientation() == 3) {	// Right
				if (!_rightHighlightEnabled) {
					_rightHighlightTime = 1.0f;
					_rightHighlight->runAction(FadeIn::create(_rightHighlightTime));
					_rightHighlightEnabled = true;
				}
			}
		}
		else {
			_tSwitches.at(i)->GetSprite()->setEnabled(false);
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
	for (unsigned int i = 0; i < _exit.size(); i++) {
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

void SceneManager::FlipGravity(int direction)
{
	_previousDirection = _gravityOrientation;
	_gravityOrientation = direction;

	// Handle DOWN gravity
	if (_previousDirection == 0) {
		if (direction == 1) { // Left
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(false);
				_metalBoxes[i]->SetFallingHorizontal(true);
			}
		}
		else if (direction == 2) { // Up
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 3) { // Right
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
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

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 2) { // Up
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 3) { // Right
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 0.5f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
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

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(false);
				_metalBoxes[i]->SetFallingHorizontal(true);
			}
		}
		else if (direction == 0) { // Down
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 3) { // Right
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 1.0f);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 1.0f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 1.0f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
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

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 2) { // Up
			_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 0.5f);
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			_player->SetGravity(3.81f);
			_player->SetFallingVertical(true);
			_player->SetFallingHorizontal(false);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(3.81f);
				_woodBoxes[i]->SetFallingVertical(true);
				_woodBoxes[i]->SetFallingHorizontal(false);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(3.81f);
				_metalBoxes[i]->SetFallingVertical(true);
				_metalBoxes[i]->SetFallingHorizontal(false);
			}
		}
		else if (direction == 3) { // Left
			_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 0.5f);
			}

			_player->SetGravity(-3.81f);
			_player->SetFallingVertical(false);
			_player->SetFallingHorizontal(true);

			for (unsigned int i = 0; i < _woodBoxes.size(); i++) {
				_woodBoxes[i]->SetGravity(-3.81f);
				_woodBoxes[i]->SetFallingVertical(false);
				_woodBoxes[i]->SetFallingHorizontal(true);
			}

			for (unsigned int i = 0; i < _metalBoxes.size(); i++) {
				_metalBoxes[i]->SetGravity(-3.81f);
				_metalBoxes[i]->SetFallingVertical(false);
				_metalBoxes[i]->SetFallingHorizontal(true);
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
	else if (_player->GetSprite()->getPosition().y > (winSize.height + _player->GetSprite()->getContentSize().height)
		|| _player->GetSprite()->getPosition().x > (winSize.width + _player->GetSprite()->getContentSize().width))
	{
		//_player->GetSprite()->setPosition(_woodCrateSpawn[0]->getPosition());
		GameManager::sharedGameManager()->setIsGameLive(false);
	}
}

void SceneManager::IsCrateInBounds()
{
	auto winSize = Director::getInstance()->getVisibleSize();

	// Moves Wood crates to spawn area if off screen
	for (int i = 0; i < _woodBoxes.size(); i++)
	{
		if (_woodBoxes[i]->GetSprite()->getPosition().y < (0.0f -_woodBoxes[i]->GetSprite()->getContentSize().height)
			|| _woodBoxes[i]->GetSprite()->getPosition().x < (0.0f - _woodBoxes[i]->GetSprite()->getContentSize().width))
		{
			_woodBoxes[i]->GetSprite()->setPosition(_woodCrateSpawn[i]->getPosition());
		}
		else if (_woodBoxes[i]->GetSprite()->getPosition().y > (winSize.height + _woodBoxes[i]->GetSprite()->getContentSize().height)
			|| _woodBoxes[i]->GetSprite()->getPosition().x >(winSize.width + _woodBoxes[i]->GetSprite()->getContentSize().width))
		{
			_woodBoxes[i]->GetSprite()->setPosition(_woodCrateSpawn[i]->getPosition());
		}
	}

	// Moves Metal crates to spawn area if off screen
	for (int i = 0; i < _metalBoxes.size(); i++)
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

	for (unsigned int i = 0; i < _platforms.size(); i++) {
		delete _platforms[i];
	}

	_platforms.clear();

	for (unsigned int i = 0; i < _walls.size(); i++) {
		delete _walls[i];
	}

	_walls.clear();

	for (unsigned int i = 0; i < _gravSwitchesDown.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (unsigned int i = 0; i < _gravSwitchesLeft.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (unsigned int i = 0; i < _gravSwitchesUp.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (unsigned int i = 0; i < _gravSwitchesRight.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	_gravSwitchesDown.clear();
	_gravSwitchesLeft.clear();
	_gravSwitchesUp.clear();
	_gravSwitchesRight.clear();

	for (unsigned int i = 0; i < _timerSwitchesDown.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (unsigned int i = 0; i < _timerSwitchesLeft.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (unsigned int i = 0; i < _timerSwitchesUp.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (unsigned int i = 0; i < _timerSwitchesRight.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	_timerSwitchesDown.clear();
	_timerSwitchesLeft.clear();
	_timerSwitchesUp.clear();
	_timerSwitchesRight.clear();
}
