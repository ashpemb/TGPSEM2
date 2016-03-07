#include "LightBulb.h"

USING_NS_CC;

using namespace cocostudio::timeline;

float OScale;
float NScale;

LightBulb::LightBulb()
{

}

LightBulb* LightBulb::create(Vec3 lightPos)
{
	LightBulb* lightBulb = new LightBulb();

	if (!lightBulb->init()) {
		return nullptr;
	}

	lightBulb->MakeLight(lightPos);
	lightBulb->autorelease();

	return lightBulb;
}

bool LightBulb::init()
{
	if (!Node::init())
	{
		return false;
	}

	this->scheduleUpdate();

	return true;
}

LightBulb::~LightBulb()
{

}

void LightBulb::MakeLight(Vec3 lightPos)
{
	auto light = PointLight::create(lightPos, Color3B::WHITE, 100.0f);
}

void LightBulb::update(float delta)
{
	// TO DO
}
