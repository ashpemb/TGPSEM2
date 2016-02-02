#include "LevelSelect.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cstdlib>
#include <ctime>
#include "SimpleAudioEngine.h"  

USING_NS_CC;

using namespace cocostudio::timeline;
Label* labelTouchInfo;
Sprite3D* sprite;
Sprite3D* sprite2;
Sprite3D* sprite3;
Sprite* bckSprite;
float bouncing;
int bounceCount;
int screenSizeY;
int screenSizeX;
Scene* LevelSelect::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LevelSelect::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LevelSelect::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	srand(time(NULL));


	//auto rootNode = CSLoader::createNode("MainScene.csb");

	//addChild(rootNode);

	this->scheduleUpdate();
	cocos2d::Size frameSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
	screenSizeY = frameSize.height;
	screenSizeX = frameSize.width;
	Device::setAccelerometerEnabled(true);




	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(LevelSelect::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(LevelSelect::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(LevelSelect::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(LevelSelect::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(LevelSelect::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(LevelSelect::onKeyReleased, this);

	auto AccelListener = EventListenerAcceleration::create(CC_CALLBACK_2(LevelSelect::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(AccelListener, this);


	this->schedule(schedule_selector(LevelSelect::UpdateTimer), 1.0f);

	sprite = Sprite3D::create("cube.obj", "World1.png");
	sprite->setPosition(Vec2(screenSizeX /2 , screenSizeY /2));
	auto rotation = RotateBy::create(60.0f, Vec3(360, 360, 0));
	sprite->runAction(RepeatForever::create(rotation));
	sprite->setScale(100.0f);
	sprite->setColor(Color3B(255, 255, 255));
	labelTouchInfo = Label::create();

	labelTouchInfo->setPosition3D(Vec3(screenSizeX/2, screenSizeY/2, 250));
	addChild(sprite);
	addChild(labelTouchInfo);



	auto sprite2 = Sprite3D::create("cube.obj", "World2.png");
	sprite2->setPosition3D(Vec3((screenSizeX / 2) - 530, (screenSizeY / 2), -220));
	auto rotation2 = RotateBy::create(40.0f, Vec3(360, 360, 0));
	sprite2->runAction(RepeatForever::create(rotation2));
	sprite2->setScale(80.0f);
	sprite2->setColor(Color3B(255, 255, 255));
	addChild(sprite2);

	auto sprite3 = Sprite3D::create("cube.obj", "World3.png");
	sprite3->setPosition3D(Vec3((screenSizeX / 2) + 530, (screenSizeY / 2), -220));
	auto rotation3 = RotateBy::create(55.0f, Vec3(360, 360, 0));
	sprite3->runAction(RepeatForever::create(rotation3));
	sprite3->setScale(80.0f);
	sprite3->setColor(Color3B(255, 255, 255));
	addChild(sprite3);

	auto mainMenuItem = MenuItemImage::create("Play Button.png", "Play Button Pressed.png", CC_CALLBACK_1(LevelSelect::GoToMainMenuScene, this));
	mainMenuItem->setPosition(Vec2(screenSizeX / 2, screenSizeY / 6));

	auto menu = Menu::create(mainMenuItem, NULL);
	menu->setPosition(Point::ZERO);
	addChild(menu);
	bouncing = 0;
	bounceCount = 0;

	return true;
}

void LevelSelect::GoToMainMenuScene(cocos2d::Ref *sender)
{

}

void LevelSelect::UpdateTimer(float dt)
{

}
void LevelSelect::update(float delta)
{
	if (bounceCount >= 5)
	{
	//bouncing -= 0.2f;
		bounceCount--;
	}
	if (bounceCount <= 0)
	{
		//bouncing += 0.2f;
		bounceCount++;
	}
	//sprite->setPosition(Vec2(sprite->getPositionX(), sprite->getPositionY() + bouncing));
}

bool LevelSelect::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::Vec2 p = touch->getLocation();
	cocos2d::Rect rect = sprite->getBoundingBox();

	if (rect.containsPoint(p))
	{
		labelTouchInfo->setString("Touched");
		sprite->setTexture("World1-Highlighted.png");
	}
	else
	{
		labelTouchInfo->setString("Missed Touch");
		sprite->setTexture("World1.png");
	}

	//cocos2d::Rect rect2 = sprite2->getBoundingBox();

	//if (rect2.containsPoint(p))
	//{
	//	sprite2->setTexture("World2-Highlighted.png");
	//}
	//else
	//{
	//	sprite2->setTexture("World2.png");
	//}

	//cocos2d::Rect rect3 = sprite3->getBoundingBox();

	//if (rect3.containsPoint(p))
	//{
	//	sprite3->setTexture("World3-Highlighted.png");
	//}
	//else
	//{
	//	sprite3->setTexture("World3.png");
	//}
	return true;

}

void LevelSelect::onAcceleration(cocos2d::Acceleration* accel, cocos2d::Event* event)
{

	int i=0;
		i += accel->y;
	i += accel->x;
}

void LevelSelect::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void LevelSelect::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Vec2 i = touch->getLocation();

}

void LevelSelect::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
