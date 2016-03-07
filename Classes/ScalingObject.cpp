#include "ScalingObject.h"
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include "SimpleAudioEngine.h"  

USING_NS_CC;

using namespace cocostudio::timeline;

Sprite3D* sprite1;
int screenSize1Y;
int screenSize1X;
float scaler1;
float scaler2;
float scaler3;
float totalDiff;
int unselect;

// on "init" you need to initialize your instance
bool ScalingObject::init()
{

	srand(time(NULL));


	auto rootNode = CSLoader::createNode("MainScene.csb");

	cocos2d::Size frameSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
	screenSize1Y = frameSize.height;
	screenSize1X = frameSize.width;

	scaler1 = 100;
	scaler2 = 50;
	scaler3 = 20;
	unselect = 0;
	isSelected = false; 
	return true;
}


void ScalingObject::update(float delta,float totalDiff)
{
	if (isSelected == true)
	{
		if (totalDiff > 10 && totalDiff < 200)
		{
			sprite->setScale(scaler1);
		}
		else if (totalDiff > 200 && totalDiff < 500)
		{
			sprite->setScale(scaler2);
		}
		else if (totalDiff > 500)
		{
			sprite->setScale(scaler3);
		}
		Selected(); 
	}
	else
	{
		Deselected();
	}
}


void ScalingObject::CreateSprite(std::string tex, float scale)
{
	sprite = Sprite::create(tex);
	scaler2 = scale;
	sprite->setScale(scaler2);
}

void ScalingObject::Selected()
{
	sprite->setOpacity(100);
}

void ScalingObject::Deselected()
{
	sprite->setOpacity(255);
}

void ScalingObject::SetStartingScale(float startingScale)
{
	scaler2 = startingScale;
	scaler3 = startingScale*1.5f;
	scaler1 = startingScale*0.5f;
}


void ScalingObject::Collision(cocos2d::Touch* touch)
{
	cocos2d::Vec2 p = touch->getLocation();
	cocos2d::Rect rect = sprite->getBoundingBox();

	if (rect.containsPoint(p))
	{
		unselect = 0;
		isSelected = true;
	}
	else if (isSelected == true)
	{
		unselect = unselect+1;
		if (unselect > 4)
		{
			isSelected = false;
			unselect = 0;
		}
	}
}
