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
}


TouchManager::~TouchManager()
{
}

void TouchManager::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event* event)
{
	/*touch1Start = touches[0]->getLocationInView();
	touch2Start = touches[1]->getLocationInView();

	touch1Start = Director::getInstance()->convertToGL(touch1Start);
	touch2Start = Director::getInstance()->convertToGL(touch2Start);*/
}


void TouchManager::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event* event)
{
	/*touch1End = touches[0]->getLocationInView();
	touch2End = touches[1]->getLocationInView();

	touch1End = Director::getInstance()->convertToGL(touch1End);
	touch2End = Director::getInstance()->convertToGL(touch2End);*/
}

void TouchManager::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event* event)
{
	if (touches.size() > 1)
	{

		Point touch1Location = touches[0]->getLocationInView();
		touch1Location = Director::getInstance()->convertToGL(touch1Location);
		Point touch2Location = touches[1]->getLocationInView();
		touch2Location = Director::getInstance()->convertToGL(touch2Location);

		float diffX = touch1Location.x - touch2Location.x;
		float diffY = touch1Location.y - touch2Location.y;
		if (diffX < 0)
			diffX = -diffX;
		if (diffY < 0)
			diffY = -diffY;
		totalDiff = (diffX + diffY);
	}
}

void TouchManager::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
}

void TouchManager::CalculateDistance()
{

}