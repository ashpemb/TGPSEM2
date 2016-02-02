#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
#include "SimpleAudioEngine.h"
#include "LevelSelect.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

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
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);

	cocos2d::Size frameSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
	int screenSizeY = frameSize.height;
	int screenSizeX = frameSize.width;

	auto playItem = MenuItemImage::create("Play Button.png", "Play Button Clicked.png", CC_CALLBACK_1(HelloWorld::GoToGameScene, this));
	auto menu = Menu::create(playItem, NULL);
	menu->setPosition(Vec2(screenSizeX / 2, screenSizeY / 2));

	addChild(menu);
	AudioTesting();
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