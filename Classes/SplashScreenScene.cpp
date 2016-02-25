#include "SplashScreenScene.h"
#include "MenuScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* SplashScreenScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SplashScreenScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SplashScreenScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::createNode("SplashScreen.csb");

	addChild(rootNode);

	this->scheduleOnce(schedule_selector(SplashScreenScene::TimeMainMenu), 2.5f);

	//BACKGROUND (Taken from Menu.cpp)
	//_background = (Sprite*)rootNode->getChildByName("Background");
	_background = Sprite::create("Test.png");
	_background->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background->setScaleX(winSize.width / _background->getContentSize().width);
	_background->setScaleY(winSize.height / _background->getContentSize().height);
	_background->setLocalZOrder(-1);

	this->addChild(_background);

	return true;
}

void SplashScreenScene::TimeMainMenu(float time)
{
	auto scene = MenuScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
}