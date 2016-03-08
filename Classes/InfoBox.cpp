#include "InfoBox.h"


InfoBox::InfoBox()
{
	infoBoxBackground = new ScalingObject;
	infoBoxBackground->init();
	infoBoxBackground->sprite = Sprite::create();
	label = Label::create();
}


InfoBox::~InfoBox()
{
}

void InfoBox::SetPosition(Vec2 infoboxPos)
{
	infoBoxBackground->sprite->setPosition(infoboxPos);
	label->setPosition(infoboxPos);

}

void InfoBox::SetInfoBoxBackground(std::string imageLoc)
{
	infoBoxBackground->sprite->setTexture(imageLoc);
}

void InfoBox::SetText(std::string infoboxText)
{
	label->setString(infoboxText);
}