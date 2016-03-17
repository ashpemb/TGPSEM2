#include "SwitchTimer.h"

SwitchTimer::SwitchTimer()
{
}

SwitchTimer* SwitchTimer::create()
{
	SwitchTimer* gravSwitchTimer = new SwitchTimer();
	if (!gravSwitchTimer->init()) {
		return nullptr;
	}
	gravSwitchTimer->_enabled = false;
	gravSwitchTimer->_playerNearBy = false;
	gravSwitchTimer->autorelease();

	//player->setPosition(100, 100);

	return gravSwitchTimer;
}


SwitchTimer::~SwitchTimer()
{
}

bool SwitchTimer::init()
{
	if (!Node::init())
	{
		return false;
	}

	// Get window size and origin
	auto winSize = Director::getInstance()->getVisibleSize(); //Gets the size of the screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin(); //Gets the origin of the screen
	this->schedule(schedule_selector(SwitchTimer::UpdateTimer), 15.0f);

	this->scheduleUpdate();
	return true;
}

void SwitchTimer::UpdateTimer(float dt)
{
	if (_playerNearBy == true)
	{ 
		_enabled = !_enabled;
	}
	_gravSwitchTimer->setEnabled(_enabled);
	ReverseOrientation(GetOrientation());
}

void SwitchTimer::ReverseOrientation(int _orientation)
{
	//Switch from down to up
	if (_orientation == 0)
	{
		_orientation = 2;
	}
	//Switch from up to down
	if (_orientation == 2)
	{
		_orientation = 0;
	}
	//Switch from left to right
	if (_orientation == 1)
	{
		_orientation = 3;
	}
	//Switch from right to left
	if (_orientation == 3)
	{
		_orientation = 1;
	}
}

void SwitchTimer::CheckNear(cocos2d::Sprite* player)
{
	// Player needs to be near the switch to press
	float scaledWidth = _gravSwitchTimer->getContentSize().width * _gravSwitchTimer->getScaleX();
	float scaledHeight = _gravSwitchTimer->getContentSize().height * _gravSwitchTimer->getScaleY();

	if (player->getPositionX() - (player->getContentSize().width / 2) < _gravSwitchTimer->getPositionX() + (scaledWidth / 2) + (player->getContentSize().width / 2) + 20
		&& player->getPositionX() + (player->getContentSize().width / 2) > _gravSwitchTimer->getPositionX() - (scaledWidth / 2) - (player->getContentSize().width / 2) - 20
		&& player->getPositionY() - (player->getContentSize().height / 2) < _gravSwitchTimer->getPositionY() + (scaledHeight / 2)
		&& player->getPositionY() + (player->getContentSize().height / 2) > _gravSwitchTimer->getPositionY() - (scaledHeight / 2))
	{
		//_gravSwitches[i]->setEnabled(true);
		_playerNearBy = true;
	}
	else {
		_playerNearBy = false;
	}
}

void SwitchTimer::SetSprite(cocos2d::ui::CheckBox* newSprite)
{
	_gravSwitchTimer = newSprite;

	this->addChild(_gravSwitchTimer);
}
