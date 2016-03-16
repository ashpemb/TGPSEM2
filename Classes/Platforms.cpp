#include "Platforms.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Platforms::Platforms(Player* ref, std::vector<Box*> wood, std::vector<Box*> metal)
{
	_playerRef = ref;
	_woodenRef = wood;
	_metalRef = metal;
}

Platforms* Platforms::create(Player* ref, std::vector<Box*> wood, std::vector<Box*> metal)
{
	Platforms* platform = new Platforms(ref, wood, metal);

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

void Platforms::MovePlatformHorizontal(cocos2d::Vec2 T)
{
	_touchZone->setPositionX(T.x);
	_movingPlat->setPositionX(T.x);

	float scaledWidth = getSprite()->getContentSize().width * getSprite()->getScaleX();
	float scaledHeight = getSprite()->getContentSize().height * getSprite()->getScaleY();
	float scaledPlayerWidth = _playerRef->GetSprite()->getContentSize().width * _playerRef->GetSprite()->getScaleX();
	float scaledPlayerHeight = _playerRef->GetSprite()->getContentSize().height * _playerRef->GetSprite()->getScaleY();

	if (_playerRef->GetOrientationVertical()) {
		if (_playerRef->GetSprite()->getPositionX() - (scaledPlayerWidth / 2) < getSprite()->getPositionX() + (scaledWidth / 2)
			&& _playerRef->GetSprite()->getPositionX() + (scaledPlayerWidth / 2) > getSprite()->getPositionX() - (scaledWidth / 2)
			&& _playerRef->GetSprite()->getPositionY() - (scaledPlayerHeight / 2) < getSprite()->getPositionY() + (scaledHeight / 2)
			&& _playerRef->GetSprite()->getPositionY() + (scaledPlayerHeight / 2) > getSprite()->getPositionY() - (scaledHeight / 2))
		{
			_playerRef->GetSprite()->setPositionX(T.x);
			_playerRef->SetTarget(T);
		}
	}

	for (int i = 0; i < _woodenRef.size(); i++) {
		float scaledWidth = getSprite()->getContentSize().width * getSprite()->getScaleX();
		float scaledHeight = getSprite()->getContentSize().height * getSprite()->getScaleY();
		float scaledBoxWidth = _woodenRef.at(i)->GetSprite()->getContentSize().width * _woodenRef.at(i)->GetSprite()->getScaleX();
		float scaledBoxHeight = _woodenRef.at(i)->GetSprite()->getContentSize().height *_woodenRef.at(i)->GetSprite()->getScaleY();

		if (_woodenRef.at(i)->GetSprite()->getPositionX() - (scaledBoxWidth / 2) < getSprite()->getPositionX() + (scaledWidth / 2)
			&& _woodenRef.at(i)->GetSprite()->getPositionX() + (scaledBoxWidth / 2) > getSprite()->getPositionX() - (scaledWidth / 2)
			&& _woodenRef.at(i)->GetSprite()->getPositionY() - (scaledBoxHeight / 2) < getSprite()->getPositionY() + (scaledHeight / 2)
			&& _woodenRef.at(i)->GetSprite()->getPositionY() + (scaledBoxHeight / 2) > getSprite()->getPositionY() - (scaledHeight / 2))
		{
			_woodenRef.at(i)->GetSprite()->setPositionX(T.x);
		}
	}

	for (int i = 0; i < _metalRef.size(); i++) {
		float scaledWidth = getSprite()->getContentSize().width * getSprite()->getScaleX();
		float scaledHeight = getSprite()->getContentSize().height * getSprite()->getScaleY();
		float scaledBoxWidth = _metalRef.at(i)->GetSprite()->getContentSize().width * _metalRef.at(i)->GetSprite()->getScaleX();
		float scaledBoxHeight = _metalRef.at(i)->GetSprite()->getContentSize().height *_metalRef.at(i)->GetSprite()->getScaleY();

		if (_metalRef.at(i)->GetSprite()->getPositionX() - (scaledBoxWidth / 2) < getSprite()->getPositionX() + (scaledWidth / 2)
			&& _metalRef.at(i)->GetSprite()->getPositionX() + (scaledBoxWidth / 2) > getSprite()->getPositionX() - (scaledWidth / 2)
			&& _metalRef.at(i)->GetSprite()->getPositionY() - (scaledBoxHeight / 2) < getSprite()->getPositionY() + (scaledHeight / 2)
			&& _metalRef.at(i)->GetSprite()->getPositionY() + (scaledBoxHeight / 2) > getSprite()->getPositionY() - (scaledHeight / 2))
		{
			_metalRef.at(i)->GetSprite()->setPositionX(T.x);
		}
	}
}

void Platforms::MovePlatformVertical(cocos2d::Vec2 T)
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

	if (platform.containsPoint(T) && platformType == 1)
	{
		MovePlatformHorizontal(T);
	}
	else if (platform.containsPoint(T) && platformType == 2)
	{
		MovePlatformVertical(T);
	}
}

void Platforms::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}
