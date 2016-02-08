#include "Platforms.h"

USING_NS_CC;

using namespace cocostudio::timeline;

float OScale;
float NScale;

bool Platforms::init()
{

	srand(time(NULL));


	auto rootNode = CSLoader::createNode("MainScene.csb");

	cocos2d::Size frameSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();

	CreateTouch();
	
	return true;
}

void Platforms::CreateTouch()
{
	//Set up a touch listener.
	auto touchListener = EventListenerTouchOneByOne::create();

	//Set callbacks for our touch functions.
	touchListener->onTouchBegan = CC_CALLBACK_2(Platforms::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Platforms::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Platforms::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(Platforms::onTouchCancelled, this);

	//Add our touch listener to event listener list.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void Platforms::CreateSprite(std::string tex, int x, int y)
{
	sprite = Sprite::create(tex);
	sprite->setPosition(Vec2(x, y));
	OScale = sprite->getScale();
}

void Platforms::Selected()
{
	NScale = (OScale*1.2);
	sprite->setScale(NScale);
}

void Platforms::UnSelected()
{
	sprite->setScale(OScale);
}

void Platforms::MovePlatform(Vec2 T)
{
	sprite->setPositionX(T.x);
}

//Touch Functions
void Platforms::onTouchBegan(Touch* touch, Event* event)
{
	Vec2 T = touch->getLocation();
	Rect platform = sprite->getBoundingBox();

	if (platform.containsPoint(T))
	{
		Selected();
	}
}

void Platforms::onTouchEnded(Touch* touch, Event* event)
{
	UnSelected();
}

void Platforms::onTouchMoved(Touch* touch, Event* event)
{

	Vec2 T = touch->getLocation();
	Rect platform = sprite->getBoundingBox();

	if (platform.containsPoint(T))
	{
		MovePlatform(T);
	}
}

