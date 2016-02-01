#include "Scene1.h"


Scene1::Scene1()
{
}


Scene1::~Scene1()
{
}

Scene* Scene1::createScene()
{
	auto scene = Scene::createWithPhysics();
	auto layer = Scene1::create();

	scene->addChild(layer);

	return scene;
}

bool Scene1::init()
{
	if (!Layer::init()) {
		return false;
	}

	auto rootNode = CSLoader::createNode("Level1.csb");
	addChild(rootNode);

	player = Player::create();
	rootNode->addChild(player);

	return true;
}

void Scene1::Update(float delta)
{

}