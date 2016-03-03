#include "Exit.h"


Exit::Exit(cocos2d::ui::CheckBox* checkBox)
{
	_exit = checkBox;

	int woof = 229;
}

Exit* Exit::create(cocos2d::ui::CheckBox* checkBox)
{
	Exit* exit = new Exit(checkBox);
	if (!exit->init()) {
		return nullptr;
	}

	exit->autorelease();


	return exit;
}


Exit::~Exit()
{
}

bool Exit::init()
{
	if (!Node::init())
	{
		return false;
	}

	// Get window size and origin
	auto winSize = Director::getInstance()->getVisibleSize(); //Gets the size of the screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin(); //Gets the origin of the screen

	this->scheduleUpdate();

	return true;
}

void Exit::CheckNear(cocos2d::Sprite* player)
{
	// Player needs to be near the switch to press
	float scaledWidth = _exit->getContentSize().width * _exit->getScaleX();
	float scaledHeight = _exit->getContentSize().height * _exit->getScaleY();

	if (player->getPositionX() - (player->getContentSize().width / 2) < _exit->getPositionX() + (scaledWidth / 2) + (player->getContentSize().width / 2) + 20
		&& player->getPositionX() + (player->getContentSize().width / 2) > _exit->getPositionX() - (scaledWidth / 2) - (player->getContentSize().width / 2) - 20
		&& player->getPositionY() - (player->getContentSize().height / 2) < _exit->getPositionY() + (scaledHeight / 2)
		&& player->getPositionY() + (player->getContentSize().height / 2) > _exit->getPositionY() - (scaledHeight / 2))
	{
		_exit->setEnabled(true);
	}
	else {
		_exit->setEnabled(false);
	}
}
