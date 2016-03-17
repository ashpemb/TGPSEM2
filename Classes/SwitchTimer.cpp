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
	this->schedule(schedule_selector(SwitchTimer::UpdateTimer), 1.0f);

	this->scheduleUpdate();
	return true;
}

void SwitchTimer::UpdateTimer(float dt)
{
	_timerLength = dt;

	// Player needs to be near the SwitchTimer to press
	float scaledWidth = _gravSwitchTimer->getContentSize().width * _gravSwitchTimer->getScaleX();
	float scaledHeight = _gravSwitchTimer->getContentSize().height * _gravSwitchTimer->getScaleY();

	if (_timerLength < 15)
	{
		//_gravSwitchTimeres[i]->setEnabled(true);
		_gravSwitchTimer->setEnabled(true);
	}
	else {
		_gravSwitchTimer->setEnabled(false);
	}

	//if (_timerLength)
}

void SwitchTimer::SetSprite(cocos2d::ui::CheckBox* newSprite)
{
	_gravSwitchTimer = newSprite;

	this->addChild(_gravSwitchTimer);
}
