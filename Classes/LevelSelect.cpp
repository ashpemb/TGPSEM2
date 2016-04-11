#include "LevelSelect.h"

USING_NS_CC;

using namespace cocostudio::timeline;

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

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(LevelSelect::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(LevelSelect::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(LevelSelect::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(LevelSelect::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	this->schedule(schedule_selector(LevelSelect::UpdateTimer), 1.0f);

	_Background = Sprite::create("LevelSelect/Background.png");
	_Background->setGlobalZOrder(0);
	_Background->setScale(4);
	_Background->setPosition3D(Vec3(screenSizeX, screenSizeY, -750));
	addChild(_Background);

	float segdeg = 2 * Pi / LEVELCOUNT;
	for (int i = 0; i < LEVELCOUNT; i++)
	{
		_LevelPositions.push_back(Vec3(sin(segdeg*i) * 640 + screenSizeX / 2, ((screenSizeY / 2) - 250), cos(segdeg*i) * 640));
	}
	for (int i = 0; i < LEVELCOUNT; i++)
	{
		Level temp;
		temp._ID = i+1;
		temp._CustomLevelName = "-";
		temp._Scale = Vec3(100, 80, 60);
		temp._Sprite = Sprite3D::create("cube.obj", "Level1Preview.png");
		temp._Sprite->setGlobalZOrder(i+40);
		temp._Sprite->setScale(temp._Scale.z);
		temp._Sprite->setPosition3D(_LevelPositions[i]);
		temp._IsFocused = false;
		temp._RotationAngles = Vec3(0, 0, 0);
		temp._RotationAngles.z = rand() % 1000 - 500;
		temp._StarRating = _ScoreManager->getStarFromFile(temp._ID);
		temp._BestTimeMinutes = _ScoreManager->getMinutesFromFile(temp._ID);
		temp._BestTimeSeconds = _ScoreManager->getSecondsFromFile(temp._ID);
		_AllLevels.push_back(temp);
	}
	for (int i = 0; i < LEVELCOUNT; i++)
	{
		addChild(_AllLevels[i]._Sprite);
	}

	_InfoBox = Sprite::create("TempInfoBox.png");
	_InfoBox->setPosition(Vec2(screenSizeX / 2, screenSizeY - 100));
	_InfoBox->setScale(0.75f,0.5f);
	_InfoBox->setGlobalZOrder(1);
	addChild(_InfoBox);
	touchMGR = new TouchManager;
	auto touchesListener = EventListenerTouchAllAtOnce::create();

	touchesListener->onTouchesBegan = CC_CALLBACK_2(TouchManager::onTouchesBegan, touchMGR);
	touchesListener->onTouchesEnded = CC_CALLBACK_2(TouchManager::onTouchesEnded, touchMGR);
	touchesListener->onTouchesMoved = CC_CALLBACK_2(TouchManager::onTouchesMoved, touchMGR);
	touchesListener->onTouchesCancelled = CC_CALLBACK_2(TouchManager::onTouchesCancelled, touchMGR);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchesListener, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchesListener, this);

	labelTouchInfo = Label::create();
	labelTouchInfo->setPosition(Vec2(screenSizeX/2, screenSizeY -100));
	labelTouchInfo->setSystemFontName("Franklin Gothic");
	labelTouchInfo->setSystemFontSize(36);
	labelTouchInfo->setGlobalZOrder(3);
	addChild(labelTouchInfo);

	auto leftItem = MenuItemImage::create("Left.png", "Left2.png", CC_CALLBACK_1(LevelSelect::LevelLeft, this));
	leftItem->setScaleY(2);
	leftItem->setPosition(Vec2(-750, 0));

	auto rightItem = MenuItemImage::create("Right.png", "Right2.png", CC_CALLBACK_1(LevelSelect::LevelRight, this));
	rightItem->setPosition(Vec2(750, 0));
	rightItem->setScaleY(2);
	auto menu = Menu::create(leftItem,rightItem,NULL);
	menu->setPosition3D(Vec3(screenSizeX / 2, screenSizeY/2, 250));
	menu->setGlobalZOrder(100);
	menu->setLocalZOrder(100);
	menu->setZOrder(100);
	addChild(menu);
	// Var init
	LevelSelected = 0;
	timer = 0;
	_LevelMovementSpeed = 5.25f;
	return true;
}

void LevelSelect::LevelRight(cocos2d::Ref *sender)
{
	if (LevelSelected >= LEVELCOUNT-1)
	{
		LevelSelected = 0;
	}
	else
	{
		++LevelSelected;
	}
}

void LevelSelect::LevelLeft(cocos2d::Ref *sender)
{
	if (LevelSelected <= 0)
	{
		LevelSelected = LEVELCOUNT-1;
	}
	else
	{
		--LevelSelected;
	}
}


void LevelSelect::UpdateTimer(float dt)
{

	std::stringstream ss;
	ss << _AllLevels[LevelSelected]._ID;
	std::string str = ss.str();

	std::stringstream _StringStream;
	_StringStream << _AllLevels[LevelSelected]._StarRating;
	std::string _FinalString = _StringStream.str();
	for (int i = 0; i < _AllLevels.size(); i++)
	{
		if (_AllLevels[i]._CustomLevelName.length() != 1)
		{
			labelTouchInfo->setString("Level " + _AllLevels[i]._CustomLevelName + "\nHighest Star Rating : " + _FinalString + "\nFastest Time : " + _AllLevels[LevelSelected]._BestTimeMinutes + "." + _AllLevels[LevelSelected]._BestTimeSeconds);
		}
		else
		{
			labelTouchInfo->setString("Level " + str + "\nHighest Star Rating : " + _FinalString + "\nFastest Time : " + _AllLevels[LevelSelected]._BestTimeMinutes + "." + _AllLevels[LevelSelected]._BestTimeSeconds);
		}
	}
}

void LevelSelect::LevelScaling()
{
	for (int i = 0; i < _AllLevels.size(); i++)
	{
		int _Scale = _AllLevels[i]._Sprite->getScale() + 0.5f;
		int _NScale = _AllLevels[i]._Sprite->getScale() - 0.5f;

		if (LevelSelected == i)
		{
			if (_AllLevels[i]._Sprite->getScale() < _AllLevels[i]._Scale.x)
			{
				_AllLevels[i]._Sprite->setScale(_Scale);
			}
			else if (_AllLevels[i]._Sprite->getScale() > _AllLevels[i]._Scale.x)
			{
				_AllLevels[i]._Sprite->setScale(_NScale);
			}
		}
	}
}

void LevelSelect::LevelMovement()
{

	for (int i = 0; i < _AllLevels.size(); i++)
	{
		if (LevelSelected == i)
		{
			_AllLevels[i]._IsFocused = true;
			_AllLevels[i]._Sprite->setTexture("LevelSelect/Level1Preview.png");
		}
		else
		{
			_AllLevels[i]._IsFocused = false;
			_AllLevels[i]._Sprite->setTexture("World1.png");
		}
	}
	
	for (int i = 0; i < _AllLevels.size(); i++)
	{
		if (_AllLevels[i]._IsFocused == true)
		{
			_FocusedDestination = Vec3(screenSizeX / 2, screenSizeY / 2 + 125, 600);
			_AllLevels[i]._Sprite->setScale(_AllLevels[i]._Scale.x);
			_AllLevels[i]._Destination = _FocusedDestination;
		}
		else
		{
			_AllLevels[i]._Sprite->setScale(_AllLevels[i]._Scale.y);
			_AllLevels[i]._Destination = _LevelPositions[i];
		}
	}
	for (int i = 0; i < _AllLevels.size(); i++)
	{
		Vec3 _Position = _AllLevels[i]._Sprite->getPosition3D();
		if (_Position != _AllLevels[i]._Destination)
		{
			if (_Position.x - 5 > _AllLevels[i]._Destination.x + 5)
			{
				_AllLevels[i]._Sprite->setPosition3D(Vec3((_Position.x - _LevelMovementSpeed), _Position.y, _Position.z));
			}
			if (_Position.x + 5  < _AllLevels[i]._Destination.x - 5)
			{
				_AllLevels[i]._Sprite->setPosition3D(Vec3((_Position.x + _LevelMovementSpeed), _Position.y, _Position.z));
			}
			if (_Position.y - 5  > _AllLevels[i]._Destination.y + 5)
			{
				_AllLevels[i]._Sprite->setPosition3D(Vec3(_Position.x, (_Position.y - _LevelMovementSpeed), _Position.z));
			}
			if (_Position.y + 5 < _AllLevels[i]._Destination.y - 5)
			{
				_AllLevels[i]._Sprite->setPosition3D(Vec3(_Position.x, (_Position.y + _LevelMovementSpeed), _Position.z));
			}
			if (_Position.z - 5  > _AllLevels[i]._Destination.z + 5)
			{
				_AllLevels[i]._Sprite->setPosition3D(Vec3(_Position.x, _Position.y, (_Position.z - _LevelMovementSpeed)));
			}
			if (_Position.z + 5 < _AllLevels[i]._Destination.z - 5)
			{
				_AllLevels[i]._Sprite->setPosition3D(Vec3(_Position.x, _Position.y, (_Position.z + _LevelMovementSpeed)));
			}
		}
	}
}

void LevelSelect::update(float delta)
{
	for (int i = 0; i < _AllLevels.size(); i++)
	{
		if (_AllLevels[i]._Rotation > 360)
		{
			_AllLevels[i]._Rotation = 0;
		}
		_AllLevels[i]._RotationAngles.x += 0.5f;
		_AllLevels[i]._RotationAngles.z += 0.2f;
		_AllLevels[i]._Sprite->setRotation3D(Vec3(_AllLevels[i]._RotationAngles.x, 0, _AllLevels[i]._RotationAngles.z * 0.24f));
	}
	_AllLevels[0]._Rotation = _AllLevels[0]._Rotation + 1;

	LevelScaling();
	//LevelRotation();
	LevelMovement();
}

void LevelSelect::GoToGameScene(cocos2d::Ref *sender)
{
	//auto scene = SceneManager::createScene(SceneSelected);
	auto scene = SceneManager::createScene(LevelSelected + 1);

	Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

bool LevelSelect::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::Vec2 p = touch->getLocation();
	
	Ref* sender =0;
	for (int i = 0; i < LEVELCOUNT; i++)
	{
		cocos2d::Rect rect = _AllLevels[i]._Sprite->getBoundingBox();
		if (rect.containsPoint(p) && _AllLevels[i]._IsFocused == true)
		{
			GoToGameScene(sender);
		}
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
