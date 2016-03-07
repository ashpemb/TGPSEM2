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
	if (!LayerColor::initWithColor(Color4B(200,200,200,255)))
	{
		return false;
	}

	// Times
	_complete = false;
	_vikingTime = 0.5f;
	_davidTime = 0.5f;
	_ashTime = 0.5f;
	_alexTime = 0.5f;
	_joshTime = 0.5f;
	_jamesTime = 0.5f;
	_mattTime = 0.5f;
	_holdTime = 3.0f;

	auto winSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::createNode("SplashScreen.csb");

	addChild(rootNode);

	this->scheduleUpdate();

	// SPRITE SETUP
	_vikingLogo = Sprite::create("SplashScreen/VikingLogo.png");
	_vikingLogo->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_vikingLogo->setScaleX((winSize.width / _vikingLogo->getContentSize().width) * 0.9);
	_vikingLogo->setScaleY(_vikingLogo->getScaleX());
	_vikingLogo->setOpacity(0);

	// DAVE SETUP
	_david = Sprite::create("SplashScreen/David.png");
	_david->setPosition(Vec2(winSize.width*0.35f, winSize.height*0.45f));
	_david->setScaleX((winSize.width / _vikingLogo->getContentSize().width));
	_david->setScaleY(_vikingLogo->getScaleX());
	_david->setOpacity(0);

	// ASH SETUP
	_ash = Sprite::create("SplashScreen/Ash.png");
	_ash->setPosition(Vec2(winSize.width*0.45f, winSize.height*0.45f));
	_ash->setScaleX((winSize.width / _vikingLogo->getContentSize().width));
	_ash->setScaleY(_vikingLogo->getScaleX());
	_ash->setOpacity(0);

	// ALEX SETUP
	_alex = Sprite::create("SplashScreen/Alex.png");
	_alex->setPosition(Vec2(winSize.width*0.55f, winSize.height*0.45f));
	_alex->setScaleX((winSize.width / _vikingLogo->getContentSize().width));
	_alex->setScaleY(_vikingLogo->getScaleX());
	_alex->setOpacity(0);

	// JOSH SETUP
	_josh = Sprite::create("SplashScreen/Josh.png");
	_josh->setPosition(Vec2(winSize.width*0.65f, winSize.height*0.45f));
	_josh->setScaleX((winSize.width / _vikingLogo->getContentSize().width));
	_josh->setScaleY(_vikingLogo->getScaleX());
	_josh->setOpacity(0);

	// JAMES SETUP
	_james = Sprite::create("SplashScreen/James.png");
	_james->setPosition(Vec2(winSize.width*0.75f, winSize.height*0.45f));
	_james->setScaleX((winSize.width / _vikingLogo->getContentSize().width));
	_james->setScaleY(_vikingLogo->getScaleX());
	_james->setOpacity(0);

	// MATT SETUP
	_matt = Sprite::create("SplashScreen/Matt.png");
	_matt->setPosition(Vec2(winSize.width*0.85f, winSize.height*0.45f));
	_matt->setScaleX((winSize.width / _vikingLogo->getContentSize().width));
	_matt->setScaleY(_vikingLogo->getScaleX());
	_matt->setOpacity(0);

	this->addChild(_vikingLogo);
	this->addChild(_david);
	this->addChild(_ash);
	this->addChild(_alex);
	this->addChild(_josh);
	this->addChild(_james);
	this->addChild(_matt);

	return true;
}

void SplashScreenScene::update(float delta)
{
	if (!_complete) {
		if (_vikingTime > 0.0f) {
			if (_vikingTime == 0.5f) {
				_vikingLogo->runAction(FadeIn::create(2.0f));
			}

			_vikingTime -= delta;
		}
		else if (_davidTime > 0.25f) {
			if (_davidTime == 0.5f) {
				_david->runAction(FadeIn::create(1.0f));
			}

			_davidTime -= delta;
		}
		else if (_ashTime > 0.25f) {
			if (_ashTime == 0.5f) {
				_ash->runAction(FadeIn::create(1.0f));
			}

			_ashTime -= delta;
		}
		else if (_alexTime > 0.25f) {
			if (_alexTime == 0.5f) {
				_alex->runAction(FadeIn::create(1.0f));
			}

			_alexTime -= delta;
		}
		else if (_joshTime > 0.25f) {
			if (_joshTime == 0.5f) {
				_josh->runAction(FadeIn::create(1.0f));
			}

			_joshTime -= delta;
		}
		else if (_jamesTime > 0.25f) {
			if (_jamesTime == 0.5f) {
				_james->runAction(FadeIn::create(1.0f));
			}

			_jamesTime -= delta;
		}
		else if (_mattTime > 0.25f) {
			if (_mattTime == 0.5f) {
				_matt->runAction(FadeIn::create(1.0f));
			}

			_mattTime -= delta;
		}
		else if (_holdTime > 0.0f) {
			_holdTime -= delta;
		}
		else {
			_complete = true;

			auto scene = MenuScene::createScene();

			Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
		}
	}
}

void SplashScreenScene::TimeMainMenu(float time)
{
	auto scene = MenuScene::createScene();

	_vikingLogo->runAction(FadeIn::create(0.0f));
	_david->runAction(FadeIn::create(1.2f));
	_ash->runAction(FadeIn::create(1.4f));
	_alex->runAction(FadeIn::create(1.6f));
	_josh->runAction(FadeIn::create(1.8f));
	_james->runAction(FadeIn::create(2.0f));
	_matt->runAction(FadeIn::create(2.2f));

	Director::getInstance()->replaceScene(TransitionFade::create(3.0f, scene));
}