#include "Scene1.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* Scene1::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics(); //Lets cocos2d know we want to scene to be created with physics enabled
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);// Change to Debugdraw_None to remove red borders , Change to Debugdraw_ALL to add red borders
	//scene->getPhysicsWorld()->setGravity(Vec2(0, 0)); // May want to change gravity from default at a later date

	// 'layer' is an autorelease object
	auto layer = Scene1::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Scene1::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize(); //Gets the size of the screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin(); //Gets the origin of the screen

	auto edgeBody = PhysicsBody::createEdgeBox(winSize, PhysicsMaterial(World_Density, World_Restitution, World_Friction), 5);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(winSize.width / 2 + origin.x, winSize.height / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);
	this->addChild(edgeNode);

	auto rootNode = CSLoader::createNode("Scene1.csb");
	addChild(rootNode);

	// PLAYER
	player = Player::create();
	player->setName("Player");
	//player->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 80));

	addChild(player);
	return true;
}