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
	gravSwitchTimer->autorelease();
	gravSwitchTimer->_isTimerStarted = false;
		
	return gravSwitchTimer;
}


SwitchTimer::~SwitchTimer()
{

}

bool SwitchTimer::init()
{
	if (!Node::init()) {
		return false;
	}
	
	// Get window size and origin
	auto winSize = Director::getInstance()->getVisibleSize(); //Gets the size of the screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin(); //Gets the origin of the screen

	this->scheduleUpdate();
	return true;
}

void SwitchTimer::update(float dt)
{
	if (_timer > 0.0f) {
		_timer -= dt;
	}
	else {
		if (IsTimerRunning()) {
			_isTimerStarted = false;
			_revertGravity = true;
		}
	}
}

int SwitchTimer::SwitchGravity()
{
	_revertGravity = false;

	if (GetOrientation() == 0)
	{
		return 2;
	}	
	if (GetOrientation() == 2)
	{
		return 0;
	}
	if (GetOrientation() == 1)
	{
		return 3;
	}
	if (GetOrientation() == 3)
	{
		return 1;
	}

	return -1;
}

void SwitchTimer::SetSprite(cocos2d::ui::CheckBox* newSprite)
{
	_gravSwitchTimer = newSprite;

	cocostudio::ComExtensionData* data = dynamic_cast<cocostudio::ComExtensionData*>(_gravSwitchTimer->getComponent("ComExtensionData"));
	std::string userdata = data->getCustomProperty();

	_timerDefault = strtod(userdata.c_str(), nullptr);

	this->addChild(_gravSwitchTimer);
}

bool SwitchTimer::GetRevertGravity() 
{ 
	return _revertGravity;
}

void SwitchTimer::ResetTimer()
{ 
	_isTimerStarted = true;
	_timer = _timerDefault;
}

bool SwitchTimer::IsTimerRunning()
{
	return (_isTimerStarted == true);
}

void SwitchTimer::DisableSwitchTimer()
{
	_isTimerStarted = false;
	_revertGravity = false;
	_timer = 0.0f;
}