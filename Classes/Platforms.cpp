#include "Platforms.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Platforms::Platforms()
{

}

Platforms* Platforms::create()
{
	Platforms* platform = new Platforms();

	if (!platform->init()) {
		return nullptr;
	}

	platform->autorelease();

	return platform;
}

bool Platforms::init()
{
	if (!Node::init())
	{
		return false;
	}

	this->scheduleUpdate();

	return true;
}

Platforms::~Platforms()
{

}

void Platforms::update(float delta)
{
	// TO DO
}

void Platforms::Selected()
{
	NScale = (OScale*1.2);
	_movingPlat->setScale(NScale);
}

void Platforms::UnSelected()
{
	_movingPlat->setScale(OScale);
}

void Platforms::MovePlatform(cocos2d::Vec2 T)
{
	_movingPlat->setPositionX(T.x);
}

//Touch Functions
void Platforms::onTouchBegan(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(true);
	Vec2 T = touch->getLocation();
	Rect platform = _movingPlat->getBoundingBox();

	if (platform.containsPoint(T))
	{
		Selected();
	}
}

void Platforms::onTouchEnded(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(false);
	UnSelected();
}

void Platforms::onTouchMoved(Touch* touch, Event* event)
{

	Vec2 T = touch->getLocation();
	Rect platform = _movingPlat->getBoundingBox();

	if (platform.containsPoint(T))
	{
		MovePlatform(T);
	}
}

void Platforms::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}
