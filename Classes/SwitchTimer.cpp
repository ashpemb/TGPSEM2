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
	gravSwitchTimer->_isEnabled = false;
	gravSwitchTimer->_isPlayerNearBy = false;
	gravSwitchTimer->StartTimer();
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
	this->schedule(schedule_selector(SwitchTimer::Update), 15.0f);
	this->schedule(schedule_selector(SwitchTimer::UpdateTimer), 0.001f);

		this->scheduleUpdate();
	return true;
	}

void SwitchTimer::Update(float dt)
 {
	if (_isPlayerNearBy == true)
		 {
		_isEnabled = !_isEnabled;
		}
	_gravSwitchTimer->setEnabled(_isEnabled);
}

void SwitchTimer::StartTimer()
{
	_mil = 0;
	_min = 0;
	_sec = 0;
}

void SwitchTimer::UpdateTimer(float dt)
{
	_mil++;

	if (_mil > 1000)
	{
		_sec++;
		_mil = 0;
	}

	if (_sec >= 15)
	{
		_min++;
		_sec = _sec - 15;
		_isEnabled = !_isEnabled;
	}
}

void SwitchTimer::CheckNear(cocos2d::Sprite* player)
 {
		// Player needs to be near the switch to press
		float scaledWidth = _gravSwitchTimer->getContentSize().width * _gravSwitchTimer->getScaleX();
	float scaledHeight = _gravSwitchTimer->getContentSize().height * _gravSwitchTimer->getScaleY();
	
	if (player->getPositionX() - (player->getContentSize().width / 2) < _gravSwitchTimer->getPositionX() + (scaledWidth / 2) + (player->getContentSize().width / 2) + 20
		 && player->getPositionX() +  (player->getContentSize().width / 2) > _gravSwitchTimer->getPositionX() - (scaledWidth / 2) - (player->getContentSize().width / 2) - 20
		 && player->getPositionY() - (player->getContentSize().height / 2) < _gravSwitchTimer->getPositionY() + (scaledHeight / 2)
		 && player->getPositionY() + (player->getContentSize().height / 2) > _gravSwitchTimer->getPositionY() - (scaledHeight / 2))
	{
				//_gravSwitches[i]->setEnabled(true);
		_isPlayerNearBy = true;
	}
	else 
	{
		_isPlayerNearBy = false;
	}
}