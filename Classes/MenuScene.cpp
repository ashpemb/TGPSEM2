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
	_startButton->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.45f));

	//Settings Button
	_settingsButton = static_cast<ui::Button*>(rootNode->getChildByName("SettingsButton"));
	_settingsButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::SettingsButtonPressed, this));
	_settingsButton->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.30f));

	//Exit Button
	_exitButton = static_cast<ui::Button*>(rootNode->getChildByName("ExitButton"));
	_exitButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::ExitButtonPressed, this));
	_exitButton->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.15f));

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
	_background = Sprite::create("MainMenuBackground.png");
	_background->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background->setScaleX(winSize.width / _background->getContentSize().width);
	_background->setScaleY(winSize.height / _background->getContentSize().height);
	_background->setLocalZOrder(-1);

	this->addChild(_background);

	// AUDIO
	auEngine = new AudioEngine();

	if (GameManager::sharedGameManager()->getIsGameMuted() == false)

	{
		auEngine->PlayBackgroundMusic("menu.mp3", true);
	}

	return true;
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

void MenuScene::SettingsButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		//TODO
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
