#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
<<<<<<< HEAD
#include "AudioEngine.h"
#include "SimpleAudioEngine.h"
#include "LevelSelect.h"
=======
#include "Define.h"

>>>>>>> refs/remotes/origin/master
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

<<<<<<< HEAD
	cocos2d::Size frameSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
	int screenSizeY = frameSize.height;
	int screenSizeX = frameSize.width;

	auto playItem = MenuItemImage::create("Play Button.png", "Play Button Clicked.png", CC_CALLBACK_1(HelloWorld::GoToGameScene, this));
	auto menu = Menu::create(playItem, NULL);
	menu->setPosition(Vec2(screenSizeX / 2, screenSizeY / 2));

	addChild(menu);
	AudioTesting();
=======
	/////////////////////////////////////////////
	/////// Examples and physics testing/////////
	/////////////////////////////////////////////
	auto husky1 = Sprite::create("Husky.png");
	auto husky2 = Sprite::create("Husky.png");
	auto husky3 = Sprite::create("Husky.png");
	auto husky4 = Sprite::create("Husky.png");

	husky1->setPosition(Vec2(200, winSize.height / 2 + origin.y));;
	husky2->setPosition(Vec2(500, winSize.height / 2 + origin.y));
	husky3->setPosition(Vec2(800, winSize.height / 2 + origin.y));
	husky4->setPosition(Vec2(800,0));

	auto husky1Bounding = PhysicsBody::createCircle(husky1->getContentSize().width / 2,
	PhysicsMaterial(0.0f, 1.0f, 0.5f));
	husky1Bounding->applyTorque(200);
	husky1->setPhysicsBody(husky1Bounding);

	auto husky2Bounding = PhysicsBody::createBox(husky2->getContentSize(),
	PhysicsMaterial(0.0f, 0.5f, 0.5f));
	husky2->setPhysicsBody(husky2Bounding);

	auto husky3Bounding = PhysicsBody::createBox(husky3->getContentSize(),
	PhysicsMaterial(0.0f, 0.5f, 0.5f));
	husky3->setPhysicsBody(husky3Bounding);

	auto husky4Bounding = PhysicsBody::createBox(husky3->getContentSize(),
	PhysicsMaterial(0.0f, 0.5f, 0.5f));
	husky4->setPhysicsBody(husky4Bounding);

	this->addChild(husky1);
	this->addChild(husky2);
	this->addChild(husky3);
	this->addChild(husky4);

>>>>>>> refs/remotes/origin/master
    return true;
}

void HelloWorld::GoToGameScene(cocos2d::Ref *sender)
{
	auto scene = LevelSelect::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
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