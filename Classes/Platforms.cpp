#include "Platforms.h"

USING_NS_CC;

using namespace cocostudio::timeline;

bool Platforms::init()
{

	srand(time(NULL));


	auto rootNode = CSLoader::createNode("MainScene.csb");

	cocos2d::Size frameSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();

	//Set up a touch listener.
	auto touchListener = EventListenerTouchOneByOne::create();

	//Set callbacks for our touch functions.
	touchListener->onTouchBegan = CC_CALLBACK_2(Platforms::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Platforms::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Platforms::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(Platforms::onTouchCancelled, this);

	//Add our touch listener to event listener list.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

void Platforms::CreateSprite(std::string tex, int x, int y)
{
	sprite = Sprite::create(tex);
	sprite->setPosition(Vec2(x, y));
}

//Touch Functions
void Platforms::onTouchBegan(Touch* touch, Event* event)
{

}

void Platforms::onTouchEnded(Touch* touch, Event* event)
{
	
}

void Platforms::onTouchMoved(Touch* touch, Event* event)
{
	
}

void Platforms::onTouchCancelled(Touch* touch, Event* event)
{
	
}
