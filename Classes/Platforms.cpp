#include "Platforms.h"

USING_NS_CC;

Platforms* Platforms::instance = NULL;

Platforms* Platforms::sharedPlatform()
{
	if (instance == NULL)
	{
		instance = new Platforms();
	}

	return instance;
}

Platforms::Platforms()
{

}

Platforms::~Platforms()
{

}