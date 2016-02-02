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

//void MenuScene::StartGame()
//{
//	auto gameScene = new Level1();
//	CCDirector::getInstance()->replaceScene(gameScene->createScene());
//	auto winSize = Director::getInstance()->getVisibleSize();
//}

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
