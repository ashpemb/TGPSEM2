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

void Platforms::setZoneSprite()
{
	_touchZone = Sprite::create("Platform.jpg");
	this->addChild(_touchZone);
	_touchZone->setPosition(_movingPlat->getPosition());
	_touchZone->setScaleX(_movingPlat->getScaleX() * 1.2);
	_touchZone->setScaleY(_movingPlat->getScaleY() * 3.5);
	_touchZone->setOpacity(75);

	OTouchZoneYScale = _touchZone->getScaleY();

}
void Platforms::update(float delta)
{
	// TO DO
}

void Platforms::Selected()
{
	NScale = (OScale*1.2);
	NTouchZoneYScale = (OTouchZoneYScale * 6);

	_movingPlat->setScale(NScale);
	_touchZone->setScaleY(NTouchZoneYScale);
}

void Platforms::UnSelected()
{
	_movingPlat->setScale(OScale);
	_touchZone->setScaleY(OTouchZoneYScale);
}

void Platforms::MovePlatform(cocos2d::Vec2 T)
{
	_touchZone->setPositionY(T.y);
	_movingPlat->setPositionY(T.y);
}

//Touch Functions
void Platforms::onTouchBegan(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(true);
	Vec2 T = touch->getLocation();
	Rect platform = _touchZone->getBoundingBox();

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
	Rect platform = _touchZone->getBoundingBox();

	if (platform.containsPoint(T))
	{
		MovePlatform(T);
	}
}

void Platforms::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}
