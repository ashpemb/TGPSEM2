#include "LevelSelect.h"

USING_NS_CC;

using namespace cocostudio::timeline;
Label* labelTouchInfo;
Sprite3D* sprite;
Sprite3D* sprite2;
Sprite3D* sprite3;
ScalingObject* stars[25];
int screenSizeY;
int screenSizeX;
int LevelSelected;
Vec3 SelectedLevelPos;
Vec3 SecondLevelPos;
Vec3 ThirdLevelPos;
float rotation1;
float rotation2;
float rotation3;
float scale1;
float scale2;
float scale3;
float levelSelectRotation;
int SceneSelected;
int fallingSpeed;
int timer;

Scene* LevelSelect::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LevelSelect::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LevelSelect::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	srand(time(NULL));


	auto rootNode = CSLoader::createNode("MainScene.csb");

	addChild(rootNode);

	this->scheduleUpdate();
	cocos2d::Size frameSize = Director::getInstance()->getVisibleSize();
	screenSizeY = frameSize.height;
	screenSizeX = frameSize.width;
	Device::setAccelerometerEnabled(true);

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(LevelSelect::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(LevelSelect::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(LevelSelect::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(LevelSelect::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(LevelSelect::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(LevelSelect::onKeyReleased, this);

	auto AccelListener = EventListenerAcceleration::create(CC_CALLBACK_2(LevelSelect::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(AccelListener, this);


	this->schedule(schedule_selector(LevelSelect::UpdateTimer), 1.0f);

	SelectedLevelPos = Vec3(screenSizeX / 2, screenSizeY / 2, 100);
	SecondLevelPos = Vec3(screenSizeX - 60, screenSizeY / 2, -200);
	ThirdLevelPos = Vec3(100, screenSizeY / 2, -200);

	for (int i = 0; i < 25; i++)
	{
		stars[i] = new ScalingObject;
		stars[i]->init();
		stars[i]->sprite = new Sprite;
		//stars[i]->CreateSO("cube.obj", "World1.png");
		stars[i]->CreateSprite("World1.png", 2);
		
		stars[i]->sprite->setPosition3D(Vec3(rand() % screenSizeX, rand() % screenSizeY, rand() % 300));
		stars[i]->sprite->setScale(0.1f);
		stars[i]->SetStartingScale(0.1f);
		addChild(stars[i]->sprite);
	}
	touchMGR = new TouchManager;
	auto touchesListener = EventListenerTouchAllAtOnce::create();

	touchesListener->onTouchesBegan = CC_CALLBACK_2(TouchManager::onTouchesBegan, touchMGR);
	touchesListener->onTouchesEnded = CC_CALLBACK_2(TouchManager::onTouchesEnded, touchMGR);
	touchesListener->onTouchesMoved = CC_CALLBACK_2(TouchManager::onTouchesMoved, touchMGR);
	touchesListener->onTouchesCancelled = CC_CALLBACK_2(TouchManager::onTouchesCancelled, touchMGR);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchesListener, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchesListener, this);


	sprite = Sprite3D::create("cube.obj", "World1.png");
	sprite->setPosition3D(SelectedLevelPos);
	sprite->setScale(100.0f);
	sprite->setColor(Color3B(255, 255, 255));
	labelTouchInfo = Label::create();
	labelTouchInfo->setPosition(Vec2(screenSizeX/2, screenSizeY -100));
	labelTouchInfo->setSystemFontName("Times New Roman");
	labelTouchInfo->setSystemFontSize(64);
	//auto _textureCube = TextureCube::create("bluecloud_lf.jpg", "bluecloud_rt.jpg",
	//	"bluecloud_dn.png", "bluecloud_up.jpg", "bluecloud_ft.jpg", "bluecloud_bk.jpg");

	//Texture2D::TexParams tRepeatParams;
	//tRepeatParams.magFilter = GL_NEAREST;
	//tRepeatParams.minFilter = GL_NEAREST;
	//tRepeatParams.wrapS = GL_MIRRORED_REPEAT;
	//tRepeatParams.wrapT = GL_MIRRORED_REPEAT;
	//_textureCube->setTexParameters(tRepeatParams);

	//auto shader = GLProgram::createWithFilenames("cube_map.vert", "cube_map.frag");
	//auto _state = GLProgramState::create(shader);

	//_state->setUniformTexture("u_cubeTex", _textureCube);
	//
	//sprite->setTexture(_textureCube);
	addChild(sprite);
	addChild(labelTouchInfo);


    sprite2 = Sprite3D::create("cube.obj", "World2.png");
	sprite2->setPosition3D(SecondLevelPos);
	sprite2->setScale(80.0f);
	sprite2->setColor(Color3B(255, 255, 255));
	addChild(sprite2);

	sprite3 = Sprite3D::create("cube.obj", "World3.png");
	sprite3->setPosition3D(ThirdLevelPos);
	sprite3->setScale(80.0f);
	sprite3->setColor(Color3B(255, 255, 255));
	addChild(sprite3);

	auto playItem = MenuItemImage::create("Play Button.png", "Play Button Clicked.png", CC_CALLBACK_1(LevelSelect::GoToGameScene, this));
	auto leftItem = MenuItemImage::create("Left.png", "Left2.png", CC_CALLBACK_1(LevelSelect::LevelLeft, this));
	leftItem->setPosition(Vec2(-150, 0));

	auto rightItem = MenuItemImage::create("Right.png", "Right2.png", CC_CALLBACK_1(LevelSelect::LevelRight, this));
	rightItem->setPosition(Vec2(150, 0));

	auto menu = Menu::create(playItem, leftItem,rightItem,NULL);
	menu->setPosition(Vec2(screenSizeX / 2, screenSizeY / 8));
	addChild(menu);
	LevelSelected = 1;
	rotation1 = 0;
	rotation2 = 0;
	rotation3 = 0;
	scale1 = 100;
	scale2 = 80;
	scale3 = 80;
	levelSelectRotation = 0;
	SceneSelected = 1;
	timer = 0;

	return true;
}

void LevelSelect::LevelRight(cocos2d::Ref *sender)
{
	if (LevelSelected == 1)
	{
		SceneSelected += 1;
		std::stringstream ss;
		ss << SceneSelected-1;
		std::string str = ss.str();
		labelTouchInfo->setString("Scene " + str);
	}
	if (LevelSelected == 2)
	{
		SceneSelected += 1;
		std::stringstream ss;
		ss << SceneSelected - 1;
		std::string str = ss.str();
		labelTouchInfo->setString("Scene " + str);
	}
	if (LevelSelected == 3)
	{
		SceneSelected += 1;
		std::stringstream ss;
		ss << SceneSelected - 1;
		std::string str = ss.str();
		labelTouchInfo->setString("Scene " + str);
	}
}

void LevelSelect::LevelLeft(cocos2d::Ref *sender)
{
	if (LevelSelected == 1)
	{
		SceneSelected -= 1;
		std::stringstream ss;
		ss << SceneSelected+1;
		std::string str = ss.str();
		labelTouchInfo->setString("Scene " + str);
	}
	if (LevelSelected == 2)
	{
		SceneSelected -= 1;
		std::stringstream ss;
		ss << SceneSelected+1;
		std::string str = ss.str();
		labelTouchInfo->setString("Scene " + str);
	}
	if (LevelSelected == 3)
	{
		SceneSelected -= 1;
		std::stringstream ss;
		ss << SceneSelected + 1;
		std::string str = ss.str();
		labelTouchInfo->setString("Scene " + str);
	}
}


void LevelSelect::UpdateTimer(float dt)
{

}

void LevelSelect::LevelScaling()
{
	if (LevelSelected == 1)
	{
		if (scale1 != 100.0f)
		{
			if (scale1 > 100.0f)
			{
				scale1 -= 0.5f;
			}
			if (scale1 < 100.f)
			{
				scale1 += 0.5f;
			}
		}
		if (scale2 != 80.0f)
		{
			if (scale2 > 80.0f)
			{
				scale2 -= 0.5f;
			}
			if (scale2 < 80.f)
			{
				scale2 += 0.5f;
			}
		}
		if (scale3 != 80.0f)
		{
			if (scale3 > 80.0f)
			{
				scale3 -= 0.5f;
			}
			if (scale3 < 80.f)
			{
				scale3 += 0.5f;
			}
		}
	}

	if (LevelSelected == 2)
	{
		if (scale2 != 100.0f)
		{
			if (scale2 > 100.0f)
			{
				scale2 -= 0.5f;
			}
			if (scale2 < 100.f)
			{
				scale2 += 0.5f;
			}
		}
		if (scale1 != 80.0f)
		{
			if (scale1 > 80.0f)
			{
				scale1 -= 0.5f;
			}
			if (scale1 < 80.f)
			{
				scale1 += 0.5f;
			}
		}
		if (scale3 != 80.0f)
		{
			if (scale3 > 80.0f)
			{
				scale3 -= 0.5f;
			}
			if (scale3 < 80.f)
			{
				scale3 += 0.5f;
			}
		}
	}
	if (LevelSelected == 3)
	{
		if (scale3 != 100.0f)
		{
			if (scale3 > 100.0f)
			{
				scale3 -= 0.5f;
			}
			if (scale3 < 100.f)
			{
				scale3 += 0.5f;
			}
		}
		if (scale1 != 80.0f)
		{
			if (scale1 > 80.0f)
			{
				scale1 -= 0.5f;
			}
			if (scale1 < 80.f)
			{
				scale1 += 0.5f;
			}
		}
		if (scale2 != 80.0f)
		{
			if (scale2 > 80.0f)
			{
				scale2 -= 0.5f;
			}
			if (scale2 < 80.f)
			{
				scale2 += 0.5f;
			}
		}
	}
	sprite->setScale(scale1);
	sprite3->setScale(scale3);
	sprite2->setScale(scale2);
}

void LevelSelect::LevelRotation()
{
	if (LevelSelected == 1)
	{
		rotation2 += 0.5f;
		rotation3 += 0.25f;
		if (rotation1 != levelSelectRotation)
		{
			if (rotation1 > levelSelectRotation)
			{
				rotation1 -= 1.5f;
			}
			if (rotation1 < levelSelectRotation)
			{
				rotation1 += 1.5f;
			}
		}
		sprite->setRotation3D(Vec3(rotation1, rotation1, accelRotation));
		sprite2->setRotation3D(Vec3(rotation2, rotation2, 0));
		sprite3->setRotation3D(Vec3(rotation3, rotation3, 0));
	}
	if (LevelSelected == 2)
	{
		rotation1 += 0.5f;
		rotation3 += 0.25f;
		if (rotation2 != levelSelectRotation)
		{
			if (rotation2 > levelSelectRotation)
			{
				rotation2 -= 1.5f;
			}
			if (rotation2 < levelSelectRotation)
			{
				rotation2 += 1.5f;
			}
		}
		sprite->setRotation3D(Vec3(rotation1, rotation1, 0));
		sprite2->setRotation3D(Vec3(rotation2, rotation2, accelRotation));
		sprite3->setRotation3D(Vec3(rotation3, rotation3, 0));
	}
	if (LevelSelected == 3)
	{
		rotation2 += 0.5f;
		rotation1 += 0.25f;
		if (rotation3 != levelSelectRotation)
		{
			if (rotation3 > levelSelectRotation)
			{
				rotation3 -= 1.5f;
			}
			if (rotation2 < levelSelectRotation)
			{
				rotation3 += 1.5f;
			}
		}
		sprite->setRotation3D(Vec3(rotation1, rotation1, 0));
		sprite2->setRotation3D(Vec3(rotation2, rotation2, 0));
		sprite3->setRotation3D(Vec3(rotation3, rotation3, accelRotation));
	}
}

void LevelSelect::LevelMovement()
{
	if (LevelSelected == 1)
	{
		if (sprite->getPosition3D() != SelectedLevelPos)
		{
			if (sprite->getPositionX() < SelectedLevelPos.x)
			{
				sprite->setPositionX(sprite->getPositionX() + 4.0f);
			}
			if (sprite->getPositionX() > SelectedLevelPos.x)
			{
				sprite->setPositionX(sprite->getPositionX() - 4.0f);
			}

			if (sprite->getPositionY() < SelectedLevelPos.y)
			{
				sprite->setPositionY(sprite->getPositionY() + 4.0f);
			}
			if (sprite->getPositionY() > SelectedLevelPos.y)
			{
				sprite->setPositionY(sprite->getPositionY() - 4.0f);
			}

			if (sprite->getPositionZ() < SelectedLevelPos.z)
			{
				sprite->setPositionZ(sprite->getPositionZ() + 4.0f);
			}
			if (sprite->getPositionZ() > SelectedLevelPos.z)
			{
				sprite->setPositionZ(sprite->getPositionZ() - 4.0f);
			}
		}

		if (sprite2->getPosition3D() != SecondLevelPos)
		{
			if (sprite2->getPositionX() < SecondLevelPos.x)
			{
				sprite2->setPositionX(sprite2->getPositionX() + 4.0f);
			}
			if (sprite2->getPositionX() > SecondLevelPos.x)
			{
				sprite2->setPositionX(sprite2->getPositionX() - 4.0f);
			}

			if (sprite2->getPositionY() < SecondLevelPos.y)
			{
				sprite2->setPositionY(sprite2->getPositionY() + 4.0f);
			}
			if (sprite2->getPositionY() > SecondLevelPos.y)
			{
				sprite2->setPositionY(sprite2->getPositionY() - 4.0f);
			}

			if (sprite2->getPositionZ() < SecondLevelPos.z)
			{
				sprite2->setPositionZ(sprite2->getPositionZ() + 4.0f);
			}
			if (sprite2->getPositionZ() > SecondLevelPos.z)
			{
				sprite2->setPositionZ(sprite2->getPositionZ() - 4.0f);
			}
		}

		if (sprite3->getPosition3D() != ThirdLevelPos)
		{
			if (sprite3->getPositionX() < ThirdLevelPos.x)
			{
				sprite3->setPositionX(sprite3->getPositionX() + 4.0f);
			}
			if (sprite3->getPositionX() > ThirdLevelPos.x)
			{
				sprite3->setPositionX(sprite3->getPositionX() - 4.0f);
			}

			if (sprite3->getPositionY() < ThirdLevelPos.y)
			{
				sprite3->setPositionY(sprite3->getPositionY() + 4.0f);
			}
			if (sprite3->getPositionY() > ThirdLevelPos.y)
			{
				sprite3->setPositionY(sprite3->getPositionY() - 4.0f);
			}

			if (sprite3->getPositionZ() < ThirdLevelPos.z)
			{
				sprite3->setPositionZ(sprite3->getPositionZ() + 4.0f);
			}
			if (sprite3->getPositionZ() > ThirdLevelPos.z)
			{
				sprite3->setPositionZ(sprite3->getPositionZ() - 4.0f);
			}
		}
	}

	if (LevelSelected == 2)
	{
		if (sprite2->getPosition3D() != SelectedLevelPos)
		{
			if (sprite2->getPositionX() < SelectedLevelPos.x)
			{
				sprite2->setPositionX(sprite2->getPositionX() + 4.0f);
			}
			if (sprite2->getPositionX() > SelectedLevelPos.x)
			{
				sprite2->setPositionX(sprite2->getPositionX() - 4.0f);
			}

			if (sprite2->getPositionY() < SelectedLevelPos.y)
			{
				sprite2->setPositionY(sprite2->getPositionY() + 4.0f);
			}
			if (sprite2->getPositionY() > SelectedLevelPos.y)
			{
				sprite2->setPositionY(sprite2->getPositionY() - 4.0f);
			}

			if (sprite2->getPositionZ() < SelectedLevelPos.z)
			{
				sprite2->setPositionZ(sprite2->getPositionZ() + 4.0f);
			}
			if (sprite2->getPositionZ() > SelectedLevelPos.z)
			{
				sprite2->setPositionZ(sprite2->getPositionZ() - 4.0f);
			}
		}

		if (sprite3->getPosition3D() != SecondLevelPos)
		{
			if (sprite3->getPositionX() < SecondLevelPos.x)
			{
				sprite3->setPositionX(sprite3->getPositionX() + 4.0f);
			}
			if (sprite3->getPositionX() > SecondLevelPos.x)
			{
				sprite3->setPositionX(sprite3->getPositionX() - 4.0f);
			}

			if (sprite3->getPositionY() < SecondLevelPos.y)
			{
				sprite3->setPositionY(sprite3->getPositionY() + 4.0f);
			}
			if (sprite3->getPositionY() > SecondLevelPos.y)
			{
				sprite3->setPositionY(sprite3->getPositionY() - 4.0f);
			}

			if (sprite3->getPositionZ() < SecondLevelPos.z)
			{
				sprite3->setPositionZ(sprite3->getPositionZ() + 4.0f);
			}
			if (sprite3->getPositionZ() > SecondLevelPos.z)
			{
				sprite3->setPositionZ(sprite3->getPositionZ() - 4.0f);
			}
		}

		if (sprite->getPosition3D() != ThirdLevelPos)
		{
			if (sprite->getPositionX() < ThirdLevelPos.x)
			{
				sprite->setPositionX(sprite->getPositionX() + 4.0f);
			}
			if (sprite->getPositionX() > ThirdLevelPos.x)
			{
				sprite->setPositionX(sprite->getPositionX() - 4.0f);
			}

			if (sprite->getPositionY() < ThirdLevelPos.y)
			{
				sprite->setPositionY(sprite->getPositionY() + 4.0f);
			}
			if (sprite->getPositionY() > ThirdLevelPos.y)
			{
				sprite->setPositionY(sprite->getPositionY() - 4.0f);
			}

			if (sprite->getPositionZ() < ThirdLevelPos.z)
			{
				sprite->setPositionZ(sprite->getPositionZ() + 4.0f);
			}
			if (sprite->getPositionZ() > ThirdLevelPos.z)
			{
				sprite->setPositionZ(sprite->getPositionZ() - 4.0f);
			}
		}
	}
	if (LevelSelected == 3)
	{
		if (sprite3->getPosition3D() != SelectedLevelPos)
		{
			if (sprite3->getPositionX() < SelectedLevelPos.x)
			{
				sprite3->setPositionX(sprite3->getPositionX() + 4.0f);
			}
			if (sprite3->getPositionX() > SelectedLevelPos.x)
			{
				sprite3->setPositionX(sprite3->getPositionX() - 4.0f);
			}

			if (sprite3->getPositionY() < SelectedLevelPos.y)
			{
				sprite3->setPositionY(sprite3->getPositionY() + 4.0f);
			}
			if (sprite3->getPositionY() > SelectedLevelPos.y)
			{
				sprite3->setPositionY(sprite3->getPositionY() - 4.0f);
			}

			if (sprite3->getPositionZ() < SelectedLevelPos.z)
			{
				sprite3->setPositionZ(sprite3->getPositionZ() + 4.0f);
			}
			if (sprite3->getPositionZ() > SelectedLevelPos.z)
			{
				sprite3->setPositionZ(sprite3->getPositionZ() - 4.0f);
			}
		}

		if (sprite->getPosition3D() != SecondLevelPos)
		{
			if (sprite->getPositionX() < SecondLevelPos.x)
			{
				sprite->setPositionX(sprite->getPositionX() + 4.0f);
			}
			if (sprite->getPositionX() > SecondLevelPos.x)
			{
				sprite->setPositionX(sprite->getPositionX() - 4.0f);
			}

			if (sprite->getPositionY() < SecondLevelPos.y)
			{
				sprite->setPositionY(sprite->getPositionY() + 4.0f);
			}
			if (sprite->getPositionY() > SecondLevelPos.y)
			{
				sprite->setPositionY(sprite->getPositionY() - 4.0f);
			}

			if (sprite->getPositionZ() < SecondLevelPos.z)
			{
				sprite->setPositionZ(sprite->getPositionZ() + 4.0f);
			}
			if (sprite->getPositionZ() > SecondLevelPos.z)
			{
				sprite->setPositionZ(sprite->getPositionZ() - 4.0f);
			}
		}

		if (sprite2->getPosition3D() != ThirdLevelPos)
		{
			if (sprite2->getPositionX() < ThirdLevelPos.x)
			{
				sprite2->setPositionX(sprite2->getPositionX() + 4.0f);
			}
			if (sprite2->getPositionX() > ThirdLevelPos.x)
			{
				sprite2->setPositionX(sprite2->getPositionX() - 4.0f);
			}

			if (sprite2->getPositionY() < ThirdLevelPos.y)
			{
				sprite2->setPositionY(sprite2->getPositionY() + 4.0f);
			}
			if (sprite2->getPositionY() > ThirdLevelPos.y)
			{
				sprite2->setPositionY(sprite2->getPositionY() - 4.0f);
			}

			if (sprite2->getPositionZ() < ThirdLevelPos.z)
			{
				sprite2->setPositionZ(sprite2->getPositionZ() + 4.0f);
			}
			if (sprite2->getPositionZ() > ThirdLevelPos.z)
			{
				sprite2->setPositionZ(sprite2->getPositionZ() - 4.0f);
			}
		}
	}
}

void LevelSelect::update(float delta)
{
	if (rotation1 > 360)
	{
		rotation1 = 0;
	}	
	if (rotation2 > 360)
	{
		rotation2 = 0;
	}	
	if (rotation3 > 360)
	{
		rotation3 = 0;
	}	
	if (levelSelectRotation > 360)
	{
		levelSelectRotation = 0;
	}

	if (SceneSelected == 1)
	{
		levelSelectRotation = 0;
	}
	if (SceneSelected == 2)
	{
		levelSelectRotation = 90;
	}
	if (SceneSelected == 3)
	{
		levelSelectRotation = 180;
	}

	if (SceneSelected > 3)
	{
		SceneSelected = 1;
	}
	if (SceneSelected < 1)
	{
		SceneSelected = 3;
	}


	for (int i = 0; i < 25; i++)
	{
		stars[i]->update(delta, touchMGR->totalDiff);
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		stars[i]->sprite->setPositionX(stars[i]->sprite->getPositionX() - r);
		//stars[i]->sprite->setRotation3D(Vec3(rotation1, rotation2, rotation3));
		if (stars[i]->sprite->getPositionX() < 0 - 100)
		{
			stars[i]->sprite->setPositionX(screenSizeX + 200);
			stars[i]->sprite->setPositionY(rand() % screenSizeY);
			stars[i]->sprite->setPositionZ(rand() % 300);
		}
	}
	LevelScaling();
	LevelRotation();
	LevelMovement();
}

void LevelSelect::GoToGameScene(cocos2d::Ref *sender)
{
	//auto scene = SceneManager::createScene(SceneSelected);
	auto scene = SceneManager::createScene(5);

	Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

bool LevelSelect::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::Vec2 p = touch->getLocation();
	cocos2d::Rect rect = sprite->getBoundingBox();

	if (rect.containsPoint(p))
	{
		sprite->setTexture("World1-Highlighted.png");
		LevelSelected = 1;
		sprite->setTexture("World1-Highlighted.png");
		sprite3->setTexture("World3.png");
		sprite2->setTexture("World2.png");
	}
	else
	{
		sprite->setTexture("World1.png");
	}
	cocos2d::Rect rect2 = sprite2->getBoundingBox();

	if (rect2.containsPoint(p))
	{
		sprite2->setTexture("World2-Highlighted.png");
		LevelSelected = 2;
		sprite2->setTexture("World2-Highlighted.png");
		sprite->setTexture("World1.png");
		sprite3->setTexture("World3.png");
	}
	else
	{
		sprite2->setTexture("World2.png");
	}

	cocos2d::Rect rect3 = sprite3->getBoundingBox();

	if (rect3.containsPoint(p))
	{
		sprite3->setTexture("World3-Highlighted.png");
		LevelSelected = 3;
		sprite->setTexture("World1.png");
		sprite2->setTexture("World2.png");
	}
	else
	{
		sprite3->setTexture("World3.png");
	}

	for (int i = 0; i < 25; i++)
	{
		stars[i]->Collision(touch);
	}
	return true;

}

void LevelSelect::onAcceleration(cocos2d::Acceleration* accel, cocos2d::Event* event)
{
	accelRotation = accel->z;
}

void LevelSelect::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void LevelSelect::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void LevelSelect::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
