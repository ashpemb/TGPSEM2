#include "Switch.h"


Switch::Switch(cocos2d::ui::CheckBox* checkBox)
{
	_gravSwitch = checkBox;

	int woof = 229;
}

Switch* Switch::create(cocos2d::ui::CheckBox* checkBox)
{
	Switch* gravSwitch = new Switch(checkBox);
	if (!gravSwitch->init()) {
		return nullptr;
	}

	gravSwitch->autorelease();

	//player->setPosition(100, 100);

	return gravSwitch;
}


Switch::~Switch()
{
}

bool Switch::init()
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

void Switch::CheckNear(cocos2d::Sprite* player)
{
	// Player needs to be near the switch to press
	float scaledWidth = _gravSwitch->getContentSize().width * _gravSwitch->getScaleX();
	float scaledHeight = _gravSwitch->getContentSize().height * _gravSwitch->getScaleY();

	if (player->getPositionX() - (player->getContentSize().width / 2) < _gravSwitch->getPositionX() + (scaledWidth / 2) + (player->getContentSize().width / 2) + 20
		&& player->getPositionX() + (player->getContentSize().width / 2) > _gravSwitch->getPositionX() - (scaledWidth / 2) - (player->getContentSize().width / 2) - 20
		&& player->getPositionY() - (player->getContentSize().height / 2) < _gravSwitch->getPositionY() + (scaledHeight / 2)
		&& player->getPositionY() + (player->getContentSize().height / 2) > _gravSwitch->getPositionY() - (scaledHeight / 2))
	{
		//_gravSwitches[i]->setEnabled(true);
		_gravSwitch->setEnabled(true);
	}
	else {
		_gravSwitch->setEnabled(false);
	}
}
