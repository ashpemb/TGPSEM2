#include "MenuScene.h"
#include "SimpleAudioEngine.h"

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
	_startButton->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.43f));

	// AUDIO
	AudioTesting();

	return true;
}

void MenuScene::StartButtonPressed(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CCLOG("In touch! %d", type);

	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("touch ended.");
		//this->StartGame();
	}
}

void MenuScene::StartGame()
{
	auto gameScene = new Scene1();
	CCDirector::getInstance()->replaceScene(gameScene->createScene());
	auto winSize = Director::getInstance()->getVisibleSize();
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
}

void MenuScene::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");
}

void MenuScene::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}

void MenuScene::AudioTesting()
{
	AudioEngine* auEngine = new AudioEngine();

	//Background music fuctions do not need an ID as there can only be one BGM playing at one time

	//auEngine->PlayBackgroundMusic("Conquer_Divide_-_Nightmares_Pro_.wav", true);
	auEngine->PauseBackgroundMusic();
	auEngine->ResumeBackgroundMusic();
	auEngine->StopBackgroundMusic();

	//Sound effects need an ID to be specifically stopped or paused, this ID is given when the function 'PlaySoundEffect' is called and can be used to track a specific sound effect

	int soundID = auEngine->PlaySoundEffect("Conquer_Divide_-_Nightmares_Pro_.wav", true);
	auEngine->PauseEffect(soundID);
	auEngine->ResumeEffect(soundID);
	//auEngine->StopSoundEffect(soundID);

	//Calling the 'AllEffects' variants of these functions will allow it to control all sound effects rather then one
	//auEngine->PauseAllEffects();

	//To mute the audio call mute audio, this will stop sounds from playing in the future and will also stop current sounds from playing.
	//Although they are not destroyed, the audio is paused until the user calls the 'stop' functions

	//auEngine->MuteAllAudio(true);
	//auEngine->StopBackgroundMusic();
}
