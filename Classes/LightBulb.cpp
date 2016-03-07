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
	Vec3 lightPos1(50.0f, 50.0f, 10.0f);
	auto light = PointLight::create(lightPos1, Color3B::RED, 1000.0f);
	addChild(light);
}

void LightBulb::update(float delta)
{
	// TO DO
}
