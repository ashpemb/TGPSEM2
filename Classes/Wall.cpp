#include "Wall.h"


Wall::Wall(Player* ref, std::vector<Box*> wood, std::vector<Box*> metal)
{
	_playerRef = ref;
	_woodenRef = wood;
	_metalRef = metal;

	CollidingWithPlatform = false;
	CollidingWithWall = false;
}

Wall* Wall::create(Player* ref, std::vector<Box*> wood, std::vector<Box*> metal)
{
	Wall* wall = new Wall(ref, wood, metal);

	if (!wall->init()) {
		return nullptr;
	}

	wall->autorelease();

	return wall;
}

bool Wall::init()
{
	if (!Node::init())
	{
		return false;
	}

	this->scheduleUpdate();

	return true;
}

Wall::~Wall()
{
}

void Wall::setZoneSprite()
{
	_touchZone = Sprite::create("Wall.jpg");
	this->addChild(_touchZone);
	_touchZone->setPosition(_movingPlat->getPosition());
	_touchZone->setScaleX(_movingPlat->getScaleX() * 3.5);
	_touchZone->setScaleY(_movingPlat->getScaleY() * 1.2);
	_touchZone->setOpacity(75);

	OTouchZoneXScale = _touchZone->getScaleX();
}

void Wall::update(float delta)
{
	// TO DO
}

void Wall::Selected()
{
	NScale = (OScale*1.2);
	NTouchZoneXScale = (OTouchZoneXScale * 6);

	_movingPlat->setScale(NScale);
	_touchZone->setScaleX(NTouchZoneXScale);
}

void Wall::UnSelected()
{
	_movingPlat->setScale(OScale);
	_touchZone->setScaleX(OTouchZoneXScale);
}

void Wall::MovePlatformHorizontal(cocos2d::Vec2 T)
{
	if (CollidingWithPlatform)
	{
		_touchZone->setPositionX(T.x);
		_movingPlat->setPositionX(T.x);
	}
	else
	{
		_touchZone->setPositionX(_movingPlat->getPositionX());
		_movingPlat->setPositionX(T.x);
	}
	
}

void Wall::MovePlatformVertical(cocos2d::Vec2 T)
{
	_touchZone->setPositionY(T.y);
	_movingPlat->setPositionY(T.y);

	float scaledWidth = getSprite()->getContentSize().width * getSprite()->getScaleX();
	float scaledHeight = getSprite()->getContentSize().height * getSprite()->getScaleY();
	float scaledPlayerWidth = _playerRef->GetSprite()->getContentSize().width * _playerRef->GetSprite()->getScaleX();
	float scaledPlayerHeight = _playerRef->GetSprite()->getContentSize().height * _playerRef->GetSprite()->getScaleY();

	if (_playerRef->GetOrientationHorizontal()) {
		if (_playerRef->GetSprite()->getPositionX() - (scaledPlayerWidth / 2) < getSprite()->getPositionX() + (scaledWidth / 2)
			&& _playerRef->GetSprite()->getPositionX() + (scaledPlayerWidth / 2) > getSprite()->getPositionX() - (scaledWidth / 2)
			&& _playerRef->GetSprite()->getPositionY() - (scaledPlayerHeight / 2) < getSprite()->getPositionY() + (scaledHeight / 2)
			&& _playerRef->GetSprite()->getPositionY() + (scaledPlayerHeight / 2) > getSprite()->getPositionY() - (scaledHeight / 2))
		{
			_playerRef->GetSprite()->setPositionY(T.y);
			_playerRef->SetTarget(T);
		}
	}

	for (int i = 0; i < (int)_woodenRef.size(); i++) {
		float scaledWidth = getSprite()->getContentSize().width * getSprite()->getScaleX();
		float scaledHeight = getSprite()->getContentSize().height * getSprite()->getScaleY();
		float scaledBoxWidth = _woodenRef.at(i)->GetSprite()->getContentSize().width * _woodenRef.at(i)->GetSprite()->getScaleX();
		float scaledBoxHeight = _woodenRef.at(i)->GetSprite()->getContentSize().height *_woodenRef.at(i)->GetSprite()->getScaleY();

		if (_woodenRef.at(i)->GetSprite()->getPositionX() - (scaledBoxWidth / 2) < getSprite()->getPositionX() + (scaledWidth / 2)
			&& _woodenRef.at(i)->GetSprite()->getPositionX() + (scaledBoxWidth / 2) > getSprite()->getPositionX() - (scaledWidth / 2)
			&& _woodenRef.at(i)->GetSprite()->getPositionY() - (scaledBoxHeight / 2) < getSprite()->getPositionY() + (scaledHeight / 2)
			&& _woodenRef.at(i)->GetSprite()->getPositionY() + (scaledBoxHeight / 2) > getSprite()->getPositionY() - (scaledHeight / 2))
		{
			_woodenRef.at(i)->GetSprite()->setPositionY(T.y);
		}
	}

	for (int i = 0; i < (int)_metalRef.size(); i++) {
		float scaledWidth = getSprite()->getContentSize().width * getSprite()->getScaleX();
		float scaledHeight = getSprite()->getContentSize().height * getSprite()->getScaleY();
		float scaledBoxWidth = _metalRef.at(i)->GetSprite()->getContentSize().width * _metalRef.at(i)->GetSprite()->getScaleX();
		float scaledBoxHeight = _metalRef.at(i)->GetSprite()->getContentSize().height *_metalRef.at(i)->GetSprite()->getScaleY();

		if (_metalRef.at(i)->GetSprite()->getPositionX() - (scaledBoxWidth / 2) < getSprite()->getPositionX() + (scaledWidth / 2)
			&& _metalRef.at(i)->GetSprite()->getPositionX() + (scaledBoxWidth / 2) > getSprite()->getPositionX() - (scaledWidth / 2)
			&& _metalRef.at(i)->GetSprite()->getPositionY() - (scaledBoxHeight / 2) < getSprite()->getPositionY() + (scaledHeight / 2)
			&& _metalRef.at(i)->GetSprite()->getPositionY() + (scaledBoxHeight / 2) > getSprite()->getPositionY() - (scaledHeight / 2))
		{
			_metalRef.at(i)->GetSprite()->setPositionY(T.y);
		}
	}
}

//Touch Functions
void Wall::onTouchBegan(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(true);
	Vec2 T = touch->getLocation();
	Rect platform = _touchZone->getBoundingBox();

	if (platform.containsPoint(T))
	{
		Selected();
	}
}

void Wall::onTouchEnded(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(false);
	UnSelected();
}

void Wall::onTouchMoved(Touch* touch, Event* event)
{

	Vec2 T = touch->getLocation();
	Rect platform = _touchZone->getBoundingBox();

	if (!CollidingWithPlatform || !CollidingWithWall)
	{
		if (platform.containsPoint(T) && platformType == 1)
		{
			MovePlatformHorizontal(T);
		}
		else if (platform.containsPoint(T) && platformType == 2)
		{
			MovePlatformVertical(T);
		}
	}
}

void Wall::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}

void Wall::CheckPlatformCollisions(cocos2d::Sprite* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
	float scaledHeight = collider->getContentSize().height * collider->getScaleY();

	float scaledPlatformHeight = getSprite()->getContentSize().height * getSprite()->getScaleY();
	float scaledPlatformWidth = getSprite()->getContentSize().width * getSprite()->getScaleX();

	if (getSprite()->getPositionX() - (scaledPlatformWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
		&& getSprite()->getPositionX() + (scaledPlatformWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
		&& getSprite()->getPositionY() - (scaledPlatformHeight / 2) < collider->getPositionY() + (scaledHeight / 2)
		&& getSprite()->getPositionY() + (scaledPlatformHeight / 2) > collider->getPositionY() - (scaledHeight / 2))
	{
		// checks if the moving platform is colliding with the top of a static platform
		CollidingWithPlatform = true;
		MovePlatformVertical(Vec2(collider->getPositionX() + (scaledPlatformWidth / 2), collider->getPositionY() + (scaledPlatformHeight)));

	}
	else
	{
		//CollidingWithPlatform = false;
	}
}

void Wall::CheckWallCollisions(cocos2d::Sprite* collider)
{

}