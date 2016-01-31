#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Platforms
{
public:
	static Platforms* sharedPlatform();

	Platforms();
	~Platforms();

	virtual bool Init();

private:
	static Platforms* instance;
};