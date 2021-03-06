#include "TouchManager.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include "SimpleAudioEngine.h"  

USING_NS_CC;

using namespace cocostudio::timeline;

TouchManager::TouchManager()
{
	auto touchesListener = EventListenerTouchAllAtOnce::create();

	touchesListener->onTouchesBegan = CC_CALLBACK_2(TouchManager::onTouchesBegan, this);
	touchesListener->onTouchesEnded = CC_CALLBACK_2(TouchManager::onTouchesEnded, this);
	touchesListener->onTouchesMoved = CC_CALLBACK_2(TouchManager::onTouchesMoved, this);
	touchesListener->onTouchesCancelled = CC_CALLBACK_2(TouchManager::onTouchesCancelled, this);
	totalDiff = 0; 
}


TouchManager::~TouchManager()
{
}

void TouchManager::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event* event)
{

}


void TouchManager::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event* event)
{

}

void TouchManager::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event* event)
{
	if (touches[0] != nullptr && touches[1] != nullptr)
	{
		Vec2 touch1Location = touches[0]->getLocation();
		Vec2 touch2Location = touches[1]->getLocation();
		float diffX = touch1Location.x - touch2Location.x;
		float diffY = touch1Location.y - touch2Location.y;
		totalDiff = (diffX + diffY);
	}
}

void TouchManager::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
}