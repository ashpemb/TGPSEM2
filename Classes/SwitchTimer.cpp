#include "SwitchTimer.h"

USING_NS_CC;

using namespace cocostudio::timeline;

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
	gravSwitchTimer->auManager = new AudioEngine();
	gravSwitchTimer->_tickerSoundID = 0;
	return gravSwitchTimer;
}


SwitchTimer::~SwitchTimer()
{
	delete auManager;
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
	this->schedule(schedule_selector(SwitchTimer::UpdateAudio), 0.5f);

	return true;
}

void SwitchTimer::update(float dt)
{
	if (_timer > 0.0f) 
	{
		_timer -= dt;
		_isTimerStarted = true;
	}
	else 
	{
		if (IsTimerRunning()) 
		{
			_isTimerStarted = false;
			_revertGravity = true;
		}
	}	
}

void SwitchTimer::UpdateAudio(float dt)
{
	if (_isTimerStarted == true)
	{
		_tickerSoundID = auManager->PlaySoundEffect("TickingClock.wav", false);
	}
	else
	{
		auManager->StopSoundEffect(_tickerSoundID);
	}
	//setColor(Color3B(255, 255, 255));
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