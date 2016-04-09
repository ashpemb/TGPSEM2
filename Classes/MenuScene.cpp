#include "MenuScene.h"
#include "LevelSelect.h"
#include "SimpleAudioEngine.h"
#include "ScalingObject.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scenemenu = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuScene::create();

	// add layer as a child to scene
	scenemenu->addChild(layer);

	// return the scene
	return scenemenu;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("MenuScene.csb");
	addChild(rootNode);

	this->scheduleUpdate();
	auto winSize = Director::getInstance()->getVisibleSize();

	//TOUCHES
	//Set up a touch listener.
	auto touchListener = EventListenerTouchOneByOne::create();

	//Set callbacks for our touch functions.
	touchListener->onTouchBegan = CC_CALLBACK_2(MenuScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MenuScene::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(MenuScene::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(MenuScene::onTouchCancelled, this);

	//Add our touch listener to event listener list.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//BUTTONS
	//Start button
	_startButton = static_cast<ui::Button*>(rootNode->getChildByName("StartButton"));
	_startButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::StartButtonPressed, this));
	_startButton->setPosition(Vec2(winSize.width*0.75f, winSize.height*0.45f));
	_startButton->setGlobalZOrder(9);
	_startButton->setScale(1.5f);
	_startButton->setCascadeOpacityEnabled(true);

	_startButtonFrame = Sprite::create("ButtonFrame.png");
	_startButtonFrame->setPosition(Vec2(winSize.width*0.75f, winSize.height*0.45f));
	_startButtonFrame->setGlobalZOrder(10);
	_startButtonFrame->setScale(1.5f);
	this->addChild(_startButtonFrame);

	//Credits Button

	_creditsButton = static_cast<ui::Button*>(rootNode->getChildByName("CreditsButton"));
	_creditsButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::CreditsButtonPressed, this));
	_creditsButton->setPosition(Vec2(winSize.width*0.75f, winSize.height*0.30f));
	_creditsButton->setGlobalZOrder(9);
	_creditsButton->setScale(1.5f);
	_creditsButton->setCascadeOpacityEnabled(true);

	_creditsButtonFrame = Sprite::create("ButtonFrame.png");
	_creditsButtonFrame->setPosition(Vec2(winSize.width*0.75f, winSize.height*0.30f));
	_creditsButtonFrame->setGlobalZOrder(10);
	_creditsButtonFrame->setScale(1.5f);
	this->addChild(_creditsButtonFrame);

	//Exit Button
	_exitButton = static_cast<ui::Button*>(rootNode->getChildByName("ExitButton"));
	_exitButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::ExitButtonPressed, this));
	_exitButton->setPosition(Vec2(winSize.width*0.75f, winSize.height*0.15f));
	_exitButton->setGlobalZOrder(9);
	_exitButton->setScale(1.5f);
	_exitButton->setCascadeOpacityEnabled(true);

	_exitButtonFrame = Sprite::create("ButtonFrame.png");
	_exitButtonFrame->setPosition(Vec2(winSize.width*0.75f, winSize.height*0.15f));
	_exitButtonFrame->setGlobalZOrder(10);
	_exitButtonFrame->setScale(1.5f);
	this->addChild(_exitButtonFrame);

	//Mute Button
	_muteButton = (cocos2d::Sprite*)(rootNode->getChildByName("MuteButton"));
	//_muteButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::MuteButtonPressed, this));
	
	if (GameManager::sharedGameManager()->getIsGameMuted() == true)
	{
		_muteButton->setTexture(Director::getInstance()->getTextureCache()->addImage("MutePressed.png"));
	}
	else
	{
		_muteButton->setTexture(Director::getInstance()->getTextureCache()->addImage("MuteUnPressed.png"));
	}

	_muteButton->setPosition(Vec2(winSize.width*0.05f, winSize.height*0.95f));

	//BACKGROUND
	//_background = (Sprite*)rootNode->getChildByName("Background");
	/*_background = Sprite::create("MainMenuBackground.png");
	_background->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background->setScaleX(winSize.width / _background->getContentSize().width);
	_background->setScaleY(winSize.height / _background->getContentSize().height);
	_background->setLocalZOrder(-1);*/

	_logo = Sprite::create("Logo.png");
	_logo->setPosition(Vec2(winSize.width*0.60f, winSize.height*0.75f));
	_logo->setScale(0.75f);

	this->addChild(_logo);

	_planet = Sprite::create("Planet.png");
	_planet->setPosition(Vec2(0.0f, 0.0f - (_planet->getContentSize().height / 8)));
	//_planet->setScale(8.0f);
	this->addChild(_planet);

	_ship = Sprite::create("HuskySpaceShipDamage.png");
	_ship->setPosition(Vec2(winSize.width*0.3f, winSize.height*0.55f));
	_ship->setScale(0.25f);
	_ship->setRotation(30);

	this->addChild(_ship);

	for (int i = 0; i < 100; i++) {
		_stars.push_back(Sprite::create("SpaceStar.png"));

		int randomWidth = cocos2d::RandomHelper::random_real(0.0f, winSize.width);
		int randomHeight = cocos2d::RandomHelper::random_real(0.0f, winSize.height);

		_stars.at(i)->setPosition(Vec2(randomWidth, randomHeight));
		_stars.at(i)->setGlobalZOrder(-2);

		this->addChild(_stars.at(i));
	}

	_rotatePlanetTimerDefault = 240.0f;
	_rotatePlanetTimer = 0.0f;

	_rotateShipTimerDefault = 10.0f;
	_rotateShipTimer = 0.0f;
	_rotateShipLeft = true;

	_moveButtonTimerDefault = 5.0f;
	_moveButtonTimer = 0.0f;
	_moveButtonUp = true;
	_opacityButtonUp = true;
	_opacityButton = _startButton->getOpacity();

	// AUDIO
	auEngine = new AudioEngine();

	if (GameManager::sharedGameManager()->getIsGameMuted() == false)

	{
		if (!auEngine->isAudioPlaying())
		{
			auEngine->PlayBackgroundMusic("menu.mp3", true);
		}
	}

	return true;
}

void MenuScene::update(float delta)
{
	_rotateShipTimer -= delta;
	_moveButtonTimer -= delta;

	_planet->setRotation(_planet->getRotation() + (M_PI / _rotatePlanetTimerDefault));

	if (_rotateShipTimer <= 0.0f) {
		_rotateShipTimer = _rotateShipTimerDefault;

		if (_rotateShipLeft) {
			_rotateShipLeft = false;

			auto rotateTo = RotateTo::create(_rotateShipTimerDefault, 25.0f);
			_ship->runAction(rotateTo);
		}
		else {
			_rotateShipLeft = true;

			auto rotateTo = RotateTo::create(_rotateShipTimerDefault, 35.0f);
			_ship->runAction(rotateTo);
		}
	}

	if (_moveButtonTimer <= 0.0f) {
		_moveButtonTimer = _moveButtonTimerDefault;

		if (_moveButtonUp) {
			_moveButtonUp = false;

			auto startMoveTo = MoveTo::create(_moveButtonTimerDefault, _startButton->getPosition() - Vec2(0.0f, 20.0f));
			auto creditMoveTo = MoveTo::create(_moveButtonTimerDefault, _creditsButton->getPosition() - Vec2(0.0f, 20.0f));
			auto exitMoveTo = MoveTo::create(_moveButtonTimerDefault, _exitButton->getPosition() - Vec2(0.0f, 20.0f));

			auto startFrameMoveTo = MoveTo::create(_moveButtonTimerDefault, _startButton->getPosition() - Vec2(0.0f, 20.0f));
			auto creditFrameMoveTo = MoveTo::create(_moveButtonTimerDefault, _creditsButton->getPosition() - Vec2(0.0f, 20.0f));
			auto exitFrameMoveTo = MoveTo::create(_moveButtonTimerDefault, _exitButton->getPosition() - Vec2(0.0f, 20.0f));

			_startButton->runAction(startMoveTo);
			_creditsButton->runAction(creditMoveTo);
			_exitButton->runAction(exitMoveTo);

			_startButtonFrame->runAction(startFrameMoveTo);
			_creditsButtonFrame->runAction(creditFrameMoveTo);
			_exitButtonFrame->runAction(exitFrameMoveTo);
		}
		else {
			_moveButtonUp = true;

			auto startMoveTo = MoveTo::create(_moveButtonTimerDefault, _startButton->getPosition() + Vec2(0.0f, 20.0f));
			auto creditMoveTo = MoveTo::create(_moveButtonTimerDefault, _creditsButton->getPosition() + Vec2(0.0f, 20.0f));
			auto exitMoveTo = MoveTo::create(_moveButtonTimerDefault, _exitButton->getPosition() + Vec2(0.0f, 20.0f));

			auto startFrameMoveTo = MoveTo::create(_moveButtonTimerDefault, _startButton->getPosition() + Vec2(0.0f, 20.0f));
			auto creditFrameMoveTo = MoveTo::create(_moveButtonTimerDefault, _creditsButton->getPosition() + Vec2(0.0f, 20.0f));
			auto exitFrameMoveTo = MoveTo::create(_moveButtonTimerDefault, _exitButton->getPosition() + Vec2(0.0f, 20.0f));

			_startButton->runAction(startMoveTo);
			_creditsButton->runAction(creditMoveTo);
			_exitButton->runAction(exitMoveTo);

			_startButtonFrame->runAction(startFrameMoveTo);
			_creditsButtonFrame->runAction(creditFrameMoveTo);
			_exitButtonFrame->runAction(exitFrameMoveTo);
		}
	}

	if (_startButton->getOpacity() >= 254 && _opacityButtonUp) {
		_opacityButtonUp = false;
	}
	else if (_startButton->getOpacity() <= 100 && !_opacityButtonUp) {
		_opacityButtonUp = true;
	}

	if (_opacityButtonUp) {
		_opacityButton += 70 * delta;
	}
	else {
		_opacityButton -= 70 * delta;
	}

	_startButton->setOpacity(_opacityButton);
	_creditsButton->setOpacity(_opacityButton);
	_exitButton->setOpacity(_opacityButton);
}

void MenuScene::StartButtonPressed(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CCLOG("In touch! %d", type);

	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("touch ended.");
		this->StartGame();
	}
}

void MenuScene::MuteButtonPressed()
{

	if (GameManager::sharedGameManager()->getIsGameMuted() == false) {
		_muteButton->setTexture(Director::getInstance()->getTextureCache()->addImage("MutePressed.png"));

		auEngine->PauseBackgroundMusic();
		auEngine->PauseAllEffects();
		GameManager::sharedGameManager()->setIsGameMuted(true);
	}
	else {
		_muteButton->setTexture(Director::getInstance()->getTextureCache()->addImage("MuteUnPressed.png"));

		auEngine->ResumeBackgroundMusic();
		auEngine->ResumeAllEffects();
		GameManager::sharedGameManager()->setIsGameMuted(false);
	}
}

void MenuScene::CreditsButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		//TODO
		this->Credits();
	}
}

void MenuScene::ExitButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->end();
	}
}

void MenuScene::StartGame()
{
	GameManager::sharedGameManager()->setIsGamePaused(true);
	auEngine->StopBackgroundMusic();
	Scene* scene = LevelSelect::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void MenuScene::Credits()
{
	//auEngine->PauseBackgroundMusic();
	Scene* scene = CreditsScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

//Touch Functions
bool MenuScene::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("touch began");
	return true;
}

void MenuScene::onTouchEnded(Touch* touch, Event* event)
{
	cocos2d::log("touch ended");

	Vec2 touchPosition = touch->getLocation();

	int realMuteWidth = _muteButton->getContentSize().width * _muteButton->getScaleX();
	int realMuteHeight = _muteButton->getContentSize().height * _muteButton->getScaleY();

	if (touchPosition.x < _muteButton->getPositionX() + (realMuteWidth / 2)
		&& touchPosition.x > _muteButton->getPositionX() - (realMuteWidth / 2)
		&& touchPosition.y < _muteButton->getPositionY() + (realMuteHeight / 2)
		&& touchPosition.y > _muteButton->getPositionY() - (realMuteHeight / 2))
	{
		MuteButtonPressed();
	}
}

void MenuScene::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");
}

void MenuScene::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}

void MenuScene::RotatePlanet()
{
	auto rotateTo = RotateTo::create(2.0f, 320.0f);
	_planet->runAction(rotateTo);
}