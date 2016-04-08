#include "FloorButton.h"


FloorButton::FloorButton(int direction)
{
	_orientation = direction;
}


FloorButton::~FloorButton()
{
}

FloorButton* FloorButton::create(int direction)
{
	FloorButton* button = new FloorButton(direction);
	if (!button->init()) {
		return nullptr;
	}

	button->autorelease();

	return button;
}

bool FloorButton::init()
{
	if (!Node::init())
	{
		return false;
	}

	// Get window size and origin
	auto winSize = Director::getInstance()->getVisibleSize(); //Gets the size of the screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin(); //Gets the origin of the screen

	this->scheduleUpdate();

	return true;
}

void FloorButton::SetActive(bool activeState)
{
	_activated = activeState;

	if (_activated) {
		if (GetOrientation() == 0) {	// Down
			_buttonSprite->setTexture(Director::getInstance()->getTextureCache()->addImage("Button_Down_Pressed.png"));
		}
		else if (GetOrientation() == 1) {	// Left
			_buttonSprite->setTexture(Director::getInstance()->getTextureCache()->addImage("Button_Left_Pressed.png"));
		}
		else if (GetOrientation() == 2) {	// Up
			_buttonSprite->setTexture(Director::getInstance()->getTextureCache()->addImage("Button_Up_Pressed.png"));
		}
		else if (GetOrientation() == 3) {	// Right
			_buttonSprite->setTexture(Director::getInstance()->getTextureCache()->addImage("Button_Right_Pressed.png"));
		}
	}
	else if (!_activated) {
		if (GetOrientation() == 0) {	// Down
			_buttonSprite->setTexture(Director::getInstance()->getTextureCache()->addImage("Button_Down_Unpressed.png"));
		}
		else if (GetOrientation() == 1) {	// Left
			_buttonSprite->setTexture(Director::getInstance()->getTextureCache()->addImage("Button_Left_Unpressed.png"));
		}
		else if (GetOrientation() == 2) {	// Up
			_buttonSprite->setTexture(Director::getInstance()->getTextureCache()->addImage("Button_Up_Unpressed.png"));
		}
		else if (GetOrientation() == 3) {	// Right
			_buttonSprite->setTexture(Director::getInstance()->getTextureCache()->addImage("Button_Right_Unpressed.png"));
		}
	}
}

void FloorButton::CheckPlayerCollisions(Player* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledPlayerWidth = collider->GetSprite()->getContentSize().width * collider->GetSprite()->getScaleX();
	float scaledPlayerHeight = collider->GetSprite()->getContentSize().height * collider->GetSprite()->getScaleY();
	float scaledButtonWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
	float scaledButtonHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();

	// Deal with down and up buttons - If player gravity is horizontal, ignore collisions
	if (GetOrientation() == 0 || GetOrientation() == 2) {
		if (!collider->GetOrientationHorizontal()) {
			// First collision box, This will set the button into its active state
			if (GetSprite()->getPositionX() - (scaledButtonWidth / 2) < collider->GetSprite()->getPositionX() + (scaledPlayerWidth / 2)
				&& GetSprite()->getPositionX() + (scaledButtonWidth / 2) > collider->GetSprite()->getPositionX() - (scaledPlayerWidth / 2)
				&& GetSprite()->getPositionY() - (scaledButtonHeight / 2) < collider->GetSprite()->getPositionY() + (scaledPlayerHeight / 2)
				&& GetSprite()->getPositionY() + (scaledButtonHeight / 2) > collider->GetSprite()->getPositionY() - (scaledPlayerHeight / 2))
			{
				SetActive(true);

				// Second collision box, this decides how the player will sit on top of the button
				if (GetOrientation() == 0) {
					float adjustedBottom = GetSprite()->getPositionY() - (scaledButtonHeight / 2);
					float adjustedTop = adjustedBottom + (scaledButtonHeight * 0.73);
					if (adjustedBottom < collider->GetSprite()->getPositionY() + (scaledPlayerHeight / 2)
						&& adjustedTop > collider->GetSprite()->getPositionY() - (scaledPlayerHeight / 2))
					{
						collider->Land(GetSprite());
						collider->GetSprite()->setPositionY(adjustedTop + (scaledPlayerHeight / 2));
					}
				}
				else if (GetOrientation() == 2) {
					float adjustedTop = GetSprite()->getPositionY() + (scaledButtonHeight / 2);
					float adjustedBottom = adjustedTop - (scaledButtonHeight * 0.73);
					if (adjustedBottom < collider->GetSprite()->getPositionY() + (scaledPlayerHeight / 2)
						&& adjustedTop > collider->GetSprite()->getPositionY() - (scaledPlayerHeight / 2))
					{
						collider->Land(GetSprite());
						collider->GetSprite()->setPositionY(adjustedBottom - (scaledPlayerHeight / 2));
					}
				}
			}
		}
	}
	// Deal with left and right buttons - If player gravity is vertical, ignore collisions
	else if (GetOrientation() == 1 || GetOrientation() == 3) {
		if (!collider->GetOrientationVertical()) {
			// First collision box, This will set the button into its active state
			if (GetSprite()->getPositionX() - (scaledButtonWidth / 2) < collider->GetSprite()->getPositionX() + (scaledPlayerWidth / 2)
				&& GetSprite()->getPositionX() + (scaledButtonWidth / 2) > collider->GetSprite()->getPositionX() - (scaledPlayerWidth / 2)
				&& GetSprite()->getPositionY() - (scaledButtonHeight / 2) < collider->GetSprite()->getPositionY() + (scaledPlayerHeight / 2)
				&& GetSprite()->getPositionY() + (scaledButtonHeight / 2) > collider->GetSprite()->getPositionY() - (scaledPlayerHeight / 2))
			{
				SetActive(true);

				// Second collision box, this decides how the player will sit on top of the button
				if (GetOrientation() == 1) {
					float adjustedLeft = GetSprite()->getPositionX() - (scaledButtonWidth / 2);
					float adjustedRight = adjustedLeft + (scaledButtonHeight * 0.73);
					if (adjustedLeft < collider->GetSprite()->getPositionX() + (scaledPlayerWidth / 2)
						&& adjustedRight > collider->GetSprite()->getPositionX() - (scaledPlayerWidth / 2))
					{
						collider->Land(GetSprite());
						collider->GetSprite()->setPositionX(adjustedLeft + (scaledPlayerWidth / 2));
					}
				}
				else if (GetOrientation() == 3) {
					float adjustedRight = GetSprite()->getPositionX() + (scaledButtonWidth / 2);
					float adjustedLeft = adjustedRight - (scaledButtonWidth * 0.73);
					if (adjustedLeft < collider->GetSprite()->getPositionX() + (scaledPlayerWidth / 2)
						&& adjustedRight > collider->GetSprite()->getPositionX() - (scaledPlayerWidth / 2))
					{
						collider->Land(GetSprite());
						collider->GetSprite()->setPositionX(adjustedRight - (scaledPlayerWidth /2));
					}
				}
			}
		}
	}
}

void FloorButton::CheckBoxCollisions(Box* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledBoxWidth = collider->GetSprite()->getContentSize().width * collider->GetSprite()->getScaleX();
	float scaledBoxHeight = collider->GetSprite()->getContentSize().height * collider->GetSprite()->getScaleY();
	float scaledButtonWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
	float scaledButtonHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();
	float boxWeight = collider->GetBoxWeight();

	// Deal with down and up buttons - If player gravity is horizontal, ignore collisions
	if (GetOrientation() == 0 || GetOrientation() == 2) {
		if (!collider->GetOrientationHorizontal()) {
			// First collision box, This will set the button into its active state
			if (GetSprite()->getPositionX() - (scaledButtonWidth / 2) < collider->GetSprite()->getPositionX() + (scaledBoxWidth / 2)
				&& GetSprite()->getPositionX() + (scaledButtonWidth / 2) > collider->GetSprite()->getPositionX() - (scaledBoxWidth / 2)
				&& GetSprite()->getPositionY() - (scaledButtonHeight / 2) < collider->GetSprite()->getPositionY() + (scaledBoxHeight / 2)
				&& GetSprite()->getPositionY() + (scaledButtonHeight / 2) > collider->GetSprite()->getPositionY() - (scaledBoxHeight / 2))
			{
				if (boxWeight >= 2.0f)
				{
					SetActive(true);

					// Second collision box, this decides how the player will sit on top of the button
					if (GetOrientation() == 0) {
						float adjustedBottom = GetSprite()->getPositionY() - (scaledButtonHeight / 2);
						float adjustedTop = adjustedBottom + (scaledButtonHeight * 0.73);
						if (adjustedBottom < collider->GetSprite()->getPositionY() + (scaledBoxHeight / 2)
							&& adjustedTop > collider->GetSprite()->getPositionY() - (scaledBoxHeight / 2))
						{
							collider->Land(GetSprite());
							collider->GetSprite()->setPositionY(adjustedTop + (scaledBoxHeight / 2));
						}
					}
					else if (GetOrientation() == 2) {
						float adjustedTop = GetSprite()->getPositionY() + (scaledButtonHeight / 2);
						float adjustedBottom = adjustedTop - (scaledButtonHeight * 0.73);
						if (adjustedBottom < collider->GetSprite()->getPositionY() + (scaledBoxHeight / 2)
							&& adjustedTop > collider->GetSprite()->getPositionY() - (scaledBoxHeight / 2))
						{
							collider->Land(GetSprite());
							collider->GetSprite()->setPositionY(adjustedBottom - (scaledBoxHeight / 2));
						}
					}
				}
				else {
					if (GetOrientation() == 0) {
						collider->Land(GetSprite());
						collider->GetSprite()->setPositionY(GetSprite()->getPositionY() + (scaledBoxHeight / 2) + (scaledButtonHeight / 2));
					}
					else if (GetOrientation() == 2) {
						collider->Land(GetSprite());
						collider->GetSprite()->setPositionY(GetSprite()->getPositionY() - (scaledBoxHeight / 2) - (scaledButtonHeight / 2));
					}
				}
			}
		}
	}
	// Deal with left and right buttons - If player gravity is vertical, ignore collisions
	else if (GetOrientation() == 1 || GetOrientation() == 3) {
		if (!collider->GetOrientationVertical()) {
			// First collision box, This will set the button into its active state
			if (GetSprite()->getPositionX() - (scaledButtonWidth / 2) < collider->GetSprite()->getPositionX() + (scaledBoxWidth / 2)
				&& GetSprite()->getPositionX() + (scaledButtonWidth / 2) > collider->GetSprite()->getPositionX() - (scaledBoxWidth / 2)
				&& GetSprite()->getPositionY() - (scaledButtonHeight / 2) < collider->GetSprite()->getPositionY() + (scaledBoxHeight / 2)
				&& GetSprite()->getPositionY() + (scaledButtonHeight / 2) > collider->GetSprite()->getPositionY() - (scaledBoxHeight / 2))
			{
				if (boxWeight >= 2.0f)
				{
					SetActive(true);
					// Second collision box, this decides how the player will sit on top of the button
					if (GetOrientation() == 1) {
						float adjustedLeft = GetSprite()->getPositionX() - (scaledButtonWidth / 2);
						float adjustedRight = adjustedLeft + (scaledButtonHeight * 0.73);
						if (adjustedLeft < collider->GetSprite()->getPositionX() + (scaledBoxWidth / 2)
							&& adjustedRight > collider->GetSprite()->getPositionX() - (scaledBoxWidth / 2))
						{
							collider->Land(GetSprite());
							collider->GetSprite()->setPositionX(adjustedLeft + (scaledBoxWidth / 2));
						}
					}
					else if (GetOrientation() == 3) {
						float adjustedRight = GetSprite()->getPositionX() + (scaledButtonWidth / 2);
						float adjustedLeft = adjustedRight - (scaledButtonWidth * 0.73);
						if (adjustedLeft < collider->GetSprite()->getPositionX() + (scaledBoxWidth / 2)
							&& adjustedRight > collider->GetSprite()->getPositionX() - (scaledBoxWidth / 2))
						{
							collider->Land(GetSprite());
							collider->GetSprite()->setPositionX(adjustedRight - (scaledBoxWidth / 2));
						}
					}
				}
				else {
					if (GetOrientation() == 1) {
						collider->Land(GetSprite());
						collider->GetSprite()->setPositionX(GetSprite()->getPositionX() + (scaledBoxWidth / 2) + (scaledButtonWidth / 2));
					}
					else if (GetOrientation() == 3) {
						collider->Land(GetSprite());
						collider->GetSprite()->setPositionX(GetSprite()->getPositionX() - (scaledBoxWidth / 2) - (scaledButtonWidth / 2));
					}
				}
			}
		}
	}
}

void FloorButton::SetSprite(Sprite* newSprite)
{
	_buttonSprite = newSprite;

	cocostudio::ComExtensionData* data = dynamic_cast<cocostudio::ComExtensionData*>(_buttonSprite->getComponent("ComExtensionData"));
	std::string userdata = data->getCustomProperty();

	_timer = strtod(userdata.c_str(), nullptr);

	this->addChild(_buttonSprite);
}