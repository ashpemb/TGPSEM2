#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
#include "AudioEngine.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics(); //Lets cocos2d know we want to scene to be created with physics enabled
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);// Change to Debugdraw_None to remove red borders , Change to Debugdraw_ALL to add red borders
	//scene->getPhysicsWorld()->setGravity(Vec2(0, 0)); // May want to change gravity from default at a later date

    // 'layer' is an autorelease object
	auto layer = HelloWorld::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
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

	auto rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode);

	AudioTesting();
    return true;
}

void HelloWorld::AudioTesting()
{
	AudioEngine* auEngine = new AudioEngine();

	//Background music fuctions do not need an ID as there can only be one BGM playing at one time

	//auEngine->PlayBackgroundMusic("Conquer_Divide_-_Nightmares_Pro_.wav", true);
	auEngine->PauseBackgroundMusic();
	auEngine->ResumeBackgroundMusic();
	auEngine->StopBackgroundMusic();
	
	//Sound effects need an ID to be specifically stopped or paused, this ID is given when the function 'PlaySoundEffect' is called and can be used to track a specific sound effect

	int soundID = auEngine->PlaySoundEffect("Conquer_Divide_-_Nightmares_Pro_.wav", true);
	auEngine->PauseEffect(soundID);
	auEngine->ResumeEffect(soundID);
	//auEngine->StopSoundEffect(soundID);

	//Calling the 'AllEffects' variants of these functions will allow it to control all sound effects rather then one
	//auEngine->PauseAllEffects();

	//To mute the audio call mute audio, this will stop sounds from playing in the future and will also stop current sounds from playing.
	//Although they are not destroyed, the audio is paused until the user calls the 'stop' functions
	
	//auEngine->MuteAllAudio(true);
	//auEngine->StopBackgroundMusic();
}