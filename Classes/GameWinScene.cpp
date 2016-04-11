#include "GameWinScene.h"
#include "MenuScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameWinScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	if (!scene->init()) {
		return nullptr;
	}

	// 'layer' is an autorelease object
	auto layer = GameWinScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


// on "init" you need to initialize your instance
bool GameWinScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("GameWin.csb");
	addChild(rootNode);

	auto winSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//TOUCHES
	//Set up a touch listener.
	auto touchListener = EventListenerTouchOneByOne::create();

	//Set callbacks for our touch functions.
	touchListener->onTouchBegan = CC_CALLBACK_2(GameWinScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameWinScene::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameWinScene::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(GameWinScene::onTouchCancelled, this);

	//Add our touch listener to event listener list.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//BUTTONS

	//Next Level button
	_nextLevelButton = static_cast<ui::Button*>(rootNode->getChildByName("NextLevelButton"));
	_nextLevelButton->addTouchEventListener(CC_CALLBACK_2(GameWinScene::NextLevelButtonPressed, this));
	_nextLevelButton->setPosition(Vec2(winSize.width*0.8f, winSize.height*0.10f));

	//Retry button
	_retryButton = static_cast<ui::Button*>(rootNode->getChildByName("RetryButton"));
	_retryButton->addTouchEventListener(CC_CALLBACK_2(GameWinScene::RetryButtonPressed, this));
	_retryButton->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.10f));

	//MainMenu Button
	_mainMenuButton = static_cast<ui::Button*>(rootNode->getChildByName("MainMenuButton"));
	_mainMenuButton->addTouchEventListener(CC_CALLBACK_2(GameWinScene::MainMenuButtonPressed, this));
	_mainMenuButton->setPosition(Vec2(winSize.width*0.2f, winSize.height*0.10f));

	//Mute Button
	_muteButton = (cocos2d::Sprite*)(rootNode->getChildByName("MuteButton"));
	//_muteButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::MuteButtonPressed, this));
	
	if (GameManager::sharedGameManager()->getIsGameMuted() == true)
	{
		_muteButton->setTexture(Director::getInstance()->getTextureCache()->addImage("MutePressed.png"));
	}
	else
	{
		_muteButton->setTexture(Director::getInstance()->getTextureCache()->addImage("MuteUnPressedGO.png"));
	}
	_muteButton->setPosition(Vec2(winSize.width*0.05f, winSize.height*0.95f));

	//BACKGROUND (Taken from Menu.cpp)
	//_background = (Sprite*)rootNode->getChildByName("Background");
	_background = Sprite::create("Test.png");
	_background->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background->setScaleX(winSize.width / _background->getContentSize().width);
	_background->setScaleY(winSize.height / _background->getContentSize().height);
	_background->setLocalZOrder(-1);

	for (int i = 0; i < 100; i++) {
		_stars.push_back(Sprite::create("SpaceStar.png"));

		int randomWidth = cocos2d::RandomHelper::random_real(0.0f, winSize.width);
		int randomHeight = cocos2d::RandomHelper::random_real(0.0f, winSize.height);

		_stars.at(i)->setPosition(Vec2(randomWidth, randomHeight));
		_stars.at(i)->setGlobalZOrder(-2);

		this->addChild(_stars.at(i));
	}

	// Rating
	_rating = Sprite::create("Rating.png");
	_rating->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.95f));

	// Rating
	_time = Sprite::create("Time.png");
	_time->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.45f));

	int mil = GameManager::sharedGameManager()->getMil();
	int sec = GameManager::sharedGameManager()->getSec();
	int min = GameManager::sharedGameManager()->getMin();

	_timeLabel = ui::Text::create();
	_timeLabel->setString(StringUtils::format("%d:%d:%d", min, sec, mil));
	_timeLabel->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.3));
	_timeLabel->setFontSize(100.0f);

	// Star
	int awardedStars = ScoreManager::sharedScoreManager()->getDefaultStarRating(GameManager::sharedGameManager()->getCurrentLevel());

	if (awardedStars == 3) {
		_star1 = Sprite::create("Star.png");
		_star1->setPosition(Vec2(winSize.width*0.15f, winSize.height*0.70f));

		_star2 = Sprite::create("Star.png");
		_star2->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.70f));

		_star3 = Sprite::create("Star.png");
		_star3->setPosition(Vec2(winSize.width*0.85f, winSize.height*0.70f));
	}
	else if (awardedStars == 2) {
		_star1 = Sprite::create("Star.png");
		_star1->setPosition(Vec2(winSize.width*0.15f, winSize.height*0.70f));

		_star2 = Sprite::create("Star.png");
		_star2->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.70f));

		_star3 = Sprite::create("Star Dis Trans.png");
		_star3->setPosition(Vec2(winSize.width*0.85f, winSize.height*0.70f));
	}
	else if (awardedStars == 1) {
		_star1 = Sprite::create("Star.png");
		_star1->setPosition(Vec2(winSize.width*0.15f, winSize.height*0.70f));

		_star2 = Sprite::create("Star Dis Trans.png");
		_star2->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.70f));

		_star3 = Sprite::create("Star Dis Trans.png");
		_star3->setPosition(Vec2(winSize.width*0.85f, winSize.height*0.70f));
	}
	else if (awardedStars == 0) {
		_star1 = Sprite::create("Star Dis Trans.png");
		_star1->setPosition(Vec2(winSize.width*0.15f, winSize.height*0.70f));

		_star2 = Sprite::create("Star Dis Trans.png");
		_star2->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.70f));

		_star3 = Sprite::create("Star Dis Trans.png");
		_star3->setPosition(Vec2(winSize.width*0.85f, winSize.height*0.70f));
	}

	this->addChild(_rating);
	this->addChild(_time);
	this->addChild(_timeLabel);
	this->addChild(_star1);
	this->addChild(_star2);
	this->addChild(_star3);
	//this->addChild(_starDis);
	//this->addChild(_background);

	return true;
}

void GameWinScene::NextLevelButtonPressed(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
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
			GameManager::sharedGameManager()->setCurrentLevel(GameManager::sharedGameManager()->getCurrentLevel() + 1);
			auto scene = SceneManager::createScene(GameManager::sharedGameManager()->getCurrentLevel());

			if (scene == nullptr) 
			{
				auto scene = MenuScene::createScene();
				Director::getInstance()->replaceScene(TransitionFade::create(1.5f, scene));
			}
			else 
			{	
				GameManager::sharedGameManager()->setCurrentLevel(GameManager::sharedGameManager()->getCurrentLevel());
				auto scene = SceneManager::createScene(GameManager::sharedGameManager()->getCurrentLevel());
				Director::getInstance()->replaceScene(TransitionFade::create(1.5f, scene));
			}
		}
	}
}

void GameWinScene::RetryButtonPressed(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
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

void GameWinScene::MainMenuButtonPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		auto scene = MenuScene::createScene();

		Director::getInstance()->replaceScene(TransitionFade::create(1.5f, scene));
	}
}

void GameWinScene::MuteButtonPressed()
{
	if (GameManager::sharedGameManager()->getIsGameMuted() == false) {
		_muteButton->setTexture(Director::getInstance()->getTextureCache()->addImage("MutePressed.png"));

		auEngine->PauseBackgroundMusic();
		auEngine->PauseAllEffects();
		GameManager::sharedGameManager()->setIsGameMuted(true);
	}
	else {
		_muteButton->setTexture(Director::getInstance()->getTextureCache()->addImage("MuteUnPressedGO.png"));

		auEngine->ResumeBackgroundMusic();
		auEngine->ResumeAllEffects();
		GameManager::sharedGameManager()->setIsGameMuted(false);
	}
}


//Touch Functions
bool GameWinScene::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("touch began");
	return true;
}

void GameWinScene::onTouchEnded(Touch* touch, Event* event)
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


void GameWinScene::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");
}

void GameWinScene::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}
