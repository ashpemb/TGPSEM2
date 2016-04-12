#include "LevelSelect.h"

USING_NS_CC;

using namespace cocostudio::timeline;

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

	auto winSize = Director::getInstance()->getVisibleSize();
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


	_planet = Sprite::create("Planet.png");
	_planet->setPosition(Vec2(0.0f, 0.0f - (_planet->getContentSize().height / 8)));
	//_planet->setScale(8.0f);
	this->addChild(_planet);

	_ship = Sprite::create("HuskySpaceShipDamage.png");
	_ship->setPosition(Vec2(winSize.width*0.3f, winSize.height*0.55f));
	_ship->setScale(0.25f);
	_ship->setRotation(30);

	this->addChild(_ship);

	for (int i = 0; i < 100; i++) {
		_stars.push_back(Sprite::create("SpaceStar.png"));

		int randomWidth = cocos2d::RandomHelper::random_real(0.0f, winSize.width);
		int randomHeight = cocos2d::RandomHelper::random_real(0.0f, winSize.height);

		_stars.at(i)->setPosition(Vec2(randomWidth, randomHeight));
		_stars.at(i)->setGlobalZOrder(-2);

		this->addChild(_stars.at(i));
	}

	_rotatePlanetTimerDefault = 240.0f;
	_rotatePlanetTimer = 0.0f;

	_rotateShipTimerDefault = 10.0f;
	_rotateShipTimer = 0.0f;
	_rotateShipLeft = true;

	// Back Button
	_backButton = ui::Button::create();
	_backButton->loadTextureNormal("BackButtonUnPressed.png");
	_backButton->loadTexturePressed("BackButtonPressed.png");
	_backButton->loadTextureDisabled("BackButtonUnPressed.png");
	_backButton->addTouchEventListener(CC_CALLBACK_2(LevelSelect::BackButtonPressed, this));
	_backButton->setPosition(Vec2(0.0f + (_backButton->getSize().width / 2), winSize.height - (_backButton->getSize().height / 2)));

	addChild(_backButton);

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
		temp._Rotation = 0.0f;
		temp._Destination = _LevelPositions[i];
		temp._RotationAngles = Vec3(0, 0, 0);
		temp._RotationAngles.z = rand() % 1000 - 500;
		temp._StarRating = ScoreManager::sharedScoreManager()->getStarFromFile(temp._ID);
		temp._BestTimeMinutes = ScoreManager::sharedScoreManager()->getMinutesFromFile(temp._ID);
		temp._BestTimeSeconds = ScoreManager::sharedScoreManager()->getSecondsFromFile(temp._ID);
		_AllLevels.push_back(temp);
	}

	_FocusedDestination = Vec3(screenSizeX / 2, screenSizeY / 2 + 125, 600);

	_AllLevels[0]._CustomLevelName = "Learning to Walk";
	_AllLevels[1]._CustomLevelName = "Topsy-Turvy";
	_AllLevels[2]._CustomLevelName = "Open Sesame";
	_AllLevels[3]._CustomLevelName = "The Depths Above";

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
	LevelSelected = 1;
	timer = 0;
	_LevelMovementSpeed = 10.25f;

	return true;
}

void LevelSelect::LevelRight(cocos2d::Ref *sender)
{
	if (LevelSelected >= LEVELCOUNT)
	{
		LevelSelected = 1;
	}
	else
	{
		++LevelSelected;
	}
}

void LevelSelect::LevelLeft(cocos2d::Ref *sender)
{
	if (LevelSelected <= 1)
	{
		LevelSelected = LEVELCOUNT;
	}
	else
	{
		--LevelSelected;
	}
}


void LevelSelect::UpdateTimer(float dt)
{

	std::stringstream ss;
	ss << _AllLevels[LevelSelected - 1]._ID;
	std::string str = ss.str();

	std::stringstream _StringStream;
	_StringStream << _AllLevels[LevelSelected - 1]._StarRating;
	std::string _FinalString = _StringStream.str();
	for (int i = 0; i < _AllLevels.size(); i++)
	{

		if (_AllLevels[i]._IsFocused)
		{
			if (_AllLevels[i]._CustomLevelName.length() != 1)
			{
				labelTouchInfo->setString("Level " + str + ": " + _AllLevels[i]._CustomLevelName + "\nHighest Star Rating : " + _FinalString + "\nFastest Time : " + _AllLevels[i]._BestTimeMinutes + "." + _AllLevels[i]._BestTimeSeconds);
			}
			else
			{
				labelTouchInfo->setString("Level: " + str + "\nHighest Star Rating : " + _FinalString + "\nFastest Time : " + _AllLevels[LevelSelected]._BestTimeMinutes + "." + _AllLevels[LevelSelected]._BestTimeSeconds);
			}
		}
	}
}

void LevelSelect::LevelScaling()
{
	for (int i = 0; i < _AllLevels.size(); i++)
	{
		int _Scale = _AllLevels[i]._Sprite->getScale() + 0.5f;
		int _NScale = _AllLevels[i]._Sprite->getScale() - 0.5f;

		if (LevelSelected - 1 == i)
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
		if (LevelSelected - 1 == i)
		{
			_AllLevels[i]._IsFocused = true;
			if (i == 0)
			{
				_AllLevels[i]._Sprite->setTexture("LevelSelect/Level1Preview.png");
			}
			else if (i == 1)
			{
				_AllLevels[i]._Sprite->setTexture("LevelSelect/Level2Preview.png");
			}
			else if (i == 2)
			{
				_AllLevels[i]._Sprite->setTexture("LevelSelect/Level3Preview.png");
			}
			else if (i == 3)
			{
				_AllLevels[i]._Sprite->setTexture("LevelSelect/Level4Preview.png");
			}
			
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
			float newX = _Position.x;
			float newY = _Position.y;
			float newZ = _Position.z;

			if (_Position.x - 5 > _AllLevels[i]._Destination.x + 5)
			{
				newX -= _LevelMovementSpeed;
			}
			if (_Position.x + 5  < _AllLevels[i]._Destination.x - 5)
			{
				newX += _LevelMovementSpeed;
			}
			if (_Position.y - 5  > _AllLevels[i]._Destination.y + 5)
			{
				newY -= _LevelMovementSpeed;
			}
			if (_Position.y + 5 < _AllLevels[i]._Destination.y - 5)
			{
				newY += _LevelMovementSpeed;
			}
			if (_Position.z - 5  > _AllLevels[i]._Destination.z + 5)
			{
				newZ -= _LevelMovementSpeed;
			}
			if (_Position.z + 5 < _AllLevels[i]._Destination.z - 5)
			{
				newZ += _LevelMovementSpeed;
			}

			_AllLevels[i]._Sprite->setPosition3D(Vec3(newX, newY, newZ));
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


	// Background updates
	_rotateShipTimer -= delta;

	_planet->setRotation(_planet->getRotation() + (M_PI / _rotatePlanetTimerDefault));

	if (_rotateShipTimer <= 0.0f) {
		_rotateShipTimer = _rotateShipTimerDefault;

		if (_rotateShipLeft) {
			_rotateShipLeft = false;

			auto rotateTo = RotateTo::create(_rotateShipTimerDefault, 25.0f);
			_ship->runAction(rotateTo);
		}
		else {
			_rotateShipLeft = true;

			auto rotateTo = RotateTo::create(_rotateShipTimerDefault, 35.0f);
			_ship->runAction(rotateTo);
		}
	}
}

void LevelSelect::GoToGameScene(cocos2d::Ref *sender)
{
	//auto scene = SceneManager::createScene(SceneSelected);
	auto scene = SceneManager::createScene((LevelSelected));

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

void LevelSelect::BackButtonPressed(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		auto scene = MenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.5f, scene));
	}
}