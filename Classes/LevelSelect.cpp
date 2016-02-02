#include "LevelSelect.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cstdlib>
#include <ctime>
#include "SimpleAudioEngine.h"  

USING_NS_CC;

using namespace cocostudio::timeline;
Label* labelTouchInfo;
Sprite* Player;
Sprite* Planet;
Sprite* Stars[50];
Sprite* spaceObjects[10];
ParticleSystemQuad* spaceShipThruster;
float movementSpeed;
float spaceObjectSpeed;
float starSpeed;
Vec2 pos;
Vec2 playerPos;
Vec2 temp;
bool hasBegun;
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


	auto rootNode = CSLoader::createNode("MainScene.csb");

	addChild(rootNode);

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

	sprite = Sprite3D::create("cube.obj", "HelloWorld.png");
	sprite->setPosition(Vec2(500, 350));
	auto rotation = RotateBy::create(15, Vec3(360, 360, 0));
	sprite->runAction(RepeatForever::create(rotation));
	sprite->setScale(50.0f);
	sprite->setColor(Color3B(255, 255, 255));
	labelTouchInfo = Label::create();
	labelTouchInfo->getPosition(200, 200);

	addChild(sprite);
	addChild(labelTouchInfo);



	auto sprite2 = Sprite3D::create("cube.obj", "HelloWorld.png");
	sprite2->setPosition(Vec2(200, 350));
	sprite2->runAction(RepeatForever::create(rotation));
	sprite2->setScale(50.0f);
	sprite2->setColor(Color3B(255, 255, 255));
	addChild(sprite2);

	return true;
}
void LevelSelect::UpdateTimer(float dt)
{

}
void LevelSelect::update(float delta)
{

}

bool LevelSelect::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::Vec2 p = touch->getLocation();
	cocos2d::Rect rect = sprite->getBoundingBox();

	if (rect.containsPoint(p))
	{
		labelTouchInfo->setString("Touched");
	}
	return true;

}

void LevelSelect::onAcceleration(cocos2d::Acceleration* accel, cocos2d::Event* event)
{

	pos.x += accel->y;
	pos.y += accel->x;
}

void LevelSelect::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void LevelSelect::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	pos = touch->getLocation();

}

void LevelSelect::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
