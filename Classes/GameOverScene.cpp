#include "GameOverScene.h"
#include "MenuScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameOverScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOverScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


// on "init" you need to initialize your instance
bool GameOverScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("GameOver.csb");
	addChild(rootNode);

	auto winSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//TOUCHES
	//Set up a touch listener.
	auto touchListener = EventListenerTouchOneByOne::create();

	//Set callbacks for our touch functions.
	touchListener->onTouchBegan = CC_CALLBACK_2(GameOverScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameOverScene::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameOverScene::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(GameOverScene::onTouchCancelled, this);

	//Add our touch listener to event listener list.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//BUTTONS
	//Start button
	_retryButton = static_cast<ui::Button*>(rootNode->getChildByName("RetryButton"));
	_retryButton->addTouchEventListener(CC_CALLBACK_2(GameOverScene::RetryButtonPressed, this));
	_retryButton->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.25f));

	//Settings Button
	_mainMenuButton = static_cast<ui::Button*>(rootNode->getChildByName("MainMenuButton"));
	_mainMenuButton->addTouchEventListener(CC_CALLBACK_2(GameOverScene::MainMenuButtonPressed, this));
	_mainMenuButton->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.10f));

	//Mute Button
	_muteButton = (cocos2d::Sprite*)(rootNode->getChildByName("MuteButton"));
	//_muteButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::MuteButtonPressed, this));
	
	if (GameManager::sharedGameManager()->getIsGameMuted() == true)
	{
		_muteButton->setTexture(TextureCache::sharedTextureCache()->addImage("MutePressed.png"));
	}
	else
	{
		_muteButton->setTexture(TextureCache::sharedTextureCache()->addImage("MuteUnPressedGO.png"));
	}
	_muteButton->setPosition(Vec2(winSize.width*0.05f, winSize.height*0.95f));

	//BACKGROUND (Taken from Menu.cpp)
	//_background = (Sprite*)rootNode->getChildByName("Background");
	_background = Sprite::create("Game Over.png");
	_background->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background->setScaleX(winSize.width / _background->getContentSize().width);
	_background->setScaleY(winSize.height / _background->getContentSize().height);
	_background->setLocalZOrder(-1);

	this->addChild(_background);

	return true;
}

void GameOverScene::RetryButtonPressed(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		if (GameManager::sharedGameManager()->getCurrentLevel() <= 0)
		{
			auto scene = MenuScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.0f, scene));
		}

		else
		{
			auto scene = SceneManager::createScene(GameManager::sharedGameManager()->getCurrentLevel()); //Gets the current level from SceneManager, through the GameManager.
			Director::getInstance()->replaceScene(TransitionFade::create(1.5f, scene));
		}
	}
}

void GameOverScene::MainMenuButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		auto scene = MenuScene::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(1.5f, scene));
	}
}

void GameOverScene::MuteButtonPressed()
{
	if (GameManager::sharedGameManager()->getIsGameMuted() == false) {
		_muteButton->setTexture(TextureCache::sharedTextureCache()->addImage("MutePressed.png"));

		auEngine->PauseBackgroundMusic();
		auEngine->PauseAllEffects();
		GameManager::sharedGameManager()->setIsGameMuted(true);
	}
	else {
		_muteButton->setTexture(TextureCache::sharedTextureCache()->addImage("MuteUnPressedGO.png"));

		auEngine->ResumeBackgroundMusic();
		auEngine->ResumeAllEffects();
		GameManager::sharedGameManager()->setIsGameMuted(false);
	}
}


//Touch Functions
bool GameOverScene::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("touch began");
	return true;
}

void GameOverScene::onTouchEnded(Touch* touch, Event* event)
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


void GameOverScene::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");
}

void GameOverScene::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}
